#pragma once

#include "stdafx.h"
#include "stdio.h"
#include <filesystem>
#include "GlobalVariables.h"

// Catches a car engine sound object that has been freed while still registered, and drops it
// instead of letting the game drive into it.
//
// CARSFX_EngineBase keeps a stack of engines that are still loading:
//
//   CARSFX_AIEngine::InitSFX      0x461060   CurEngineSlot++; g_pEngineSFXObjs[CurEngineSlot] = this
//   CARSFX_PlayerEngine::InitSFX  0x461570   the same
//   CSTATEMGR_PlayerCar::UpdateParams 0x463D80
//                                            reads the top entry, asks IsEngineFinishedLoading,
//                                            and on yes clears it and does CurEngineSlot--
//
// g_pEngineSFXObjs (0x82AF90) holds six pointers and neither side bounds checks the index. Popping
// happens only from UpdateParams and only for the entry on top, so an engine that is destroyed
// while still on the stack leaves a dangling pointer that nothing ever removes.
//
// That is what crashed at 0x46100B: CurEngineSlot was 0, the array was intact, slot 0 held
// 0x0E9D5C00, and that object's field at +0xB0 read back 0xEEEEEEEE. The pool fills memory it
// takes back with exactly that pattern (0x43FAFA, mov eax 0EEEEEEEEh + rep stosd) and its own
// verifier checks free blocks still carry it (0x43FFA0), so the object had been freed underneath
// the registration. The same dangling entry explains the pool damage found later by the verifier
// at 0x440039 and by the allocator at 0x43FE5C: anything still writing through it lands in a freed
// block and takes out the block header.
//
// Since the fill pattern is the game's own, a dead entry is directly recognisable. The scrub below
// runs just before UpdateParams walks the stack, pops anything poisoned exactly the way the game
// pops a finished engine, and writes down what it dropped. So the log names the moment and the
// game keeps running, rather than having to pick one.
//
// This does not explain who frees the object without deregistering it. The log is there to answer
// that: it records every registration with its slot, so the entry that went stale can be matched
// against the one that was never popped.

bool EngineSFXGuard = true;

#define CARSFX_CurEngineSlot 0x7ED440
#define CARSFX_EngineSFXObjs 0x82AF90
constexpr int CARSFX_EngineSlotCount = 6; // rep stosd of 6 dwords at 0x473C3A

constexpr DWORD PoolFreeFill = 0xEEEEEEEE; // 0x43FAFA

// Two budgets on purpose. Registrations are steady background noise, a few per race, and on the
// first run they spent the whole allowance before anything interesting happened. Drops and
// anomalies are the reason this file exists, so they get a budget nothing else can touch.
int EngineSFXGuardNoise = 0;
int EngineSFXGuardSignal = 0;
constexpr int EngineSFXGuardNoiseLimit = 400;
constexpr int EngineSFXGuardSignalLimit = 400;

void(__thiscall* CSTATEMGR_PlayerCar_UpdateParams_Game)(DWORD* This, float Dt) = (void(__thiscall*)(DWORD*, float))0x463D80;
void(__thiscall* CARSFX_AIEngine_InitSFX_Game)(DWORD* This) = (void(__thiscall*)(DWORD*))0x461060;
void(__thiscall* CARSFX_PlayerEngine_InitSFX_Game)(DWORD* This) = (void(__thiscall*)(DWORD*))0x461570;
bool(__thiscall* CARSFX_EngineBase_IsEngineFinishedLoading_Game)(DWORD* This) = (bool(__thiscall*)(DWORD*))0x461000;

void EngineSFXGuardLine(const char* fmt, ...)
{
	auto Path = CurrentWorkingDirectory / "UnlimiterData" / "_EngineSFXGuard.txt";
	FILE* f = fopen(Path.string().c_str(), "a");

	if (!f) return;

	va_list args;
	va_start(args, fmt);
	vfprintf(f, fmt, args);
	va_end(args);

	fclose(f);
}

bool EngineSFXReadable(DWORD Address)
{
	return Address >= 0x10000 && !IsBadReadPtr((void*)Address, 4);
}

DWORD EngineSFXRead(DWORD Address)
{
	return EngineSFXReadable(Address) ? *(DWORD*)Address : PoolFreeFill;
}

// A live object always has a readable vtable pointer. Either that or the load handle at +0xB0
// reading back as the pool's fill pattern means the storage went back to the pool.
bool EngineSFXIsDead(DWORD Object)
{
	if (!EngineSFXReadable(Object)) return true;
	if (EngineSFXRead(Object) == PoolFreeFill) return true;
	if (EngineSFXRead(Object + 0xB0) == PoolFreeFill) return true;

	return false;
}

void EngineSFXGuardReport(bool Signal, const char* What, int Slot, DWORD Object)
{
	int& Used = Signal ? EngineSFXGuardSignal : EngineSFXGuardNoise;
	const int Limit = Signal ? EngineSFXGuardSignalLimit : EngineSFXGuardNoiseLimit;

	if (Used >= Limit) return;

	Used++;

	EngineSFXGuardLine("%s%s slot %d object 0x%08X  vtable %08X  +B0 %08X  +B4 %08X\n",
		Signal ? "*** " : "", What, Slot, Object,
		EngineSFXRead(Object), EngineSFXRead(Object + 0xB0), EngineSFXRead(Object + 0xB4));

	if (Used == Limit)
		EngineSFXGuardLine("(%s budget spent, further ones are silent)\n", Signal ? "drop" : "registration");
}

// Pops dead entries off the top exactly the way 0x463DBF and 0x463DCA do for a finished one.
void EngineSFXGuardScrub()
{
	int* Slot = (int*)CARSFX_CurEngineSlot;
	DWORD* Objs = (DWORD*)CARSFX_EngineSFXObjs;

	// Neither InitSFX nor UpdateParams range checks this, so a runaway index would read and write
	// past six pointers into whatever globals follow. Never seen it happen, but it is one compare.
	if (*Slot >= CARSFX_EngineSlotCount)
	{
		EngineSFXGuardReport(true, "SLOT INDEX PAST THE END OF THE ARRAY,", *Slot, 0);
		*Slot = CARSFX_EngineSlotCount - 1;
	}

	while (*Slot >= 0 && *Slot < CARSFX_EngineSlotCount)
	{
		DWORD Object = Objs[*Slot];

		if (!Object) break;                 // empty top, nothing pending
		if (!EngineSFXIsDead(Object)) break; // still alive, leave it to the game

		EngineSFXGuardReport(true, "dropped freed engine sfx,", *Slot, Object);

		Objs[*Slot] = 0;
		(*Slot)--;
	}
}

void __fastcall CSTATEMGR_PlayerCar_UpdateParams(DWORD* This, void* EDX_Unused, float Dt)
{
	EngineSFXGuardScrub();
	CSTATEMGR_PlayerCar_UpdateParams_Game(This, Dt);
}

// Second line of defence, right at the instruction that faulted (0x46100B dereferences the +0xB0
// read from 0x461001). Saying the load has finished is what makes the caller pop the entry.
bool __fastcall CARSFX_EngineBase_IsEngineFinishedLoading(DWORD* This, void* EDX_Unused)
{
	if (EngineSFXIsDead((DWORD)This))
	{
		EngineSFXGuardReport(true, "freed engine sfx asked whether it finished loading,", *(int*)CARSFX_CurEngineSlot, (DWORD)This);
		return true;
	}

	return CARSFX_EngineBase_IsEngineFinishedLoading_Game(This);
}

void __fastcall CARSFX_AIEngine_InitSFX(DWORD* This, void* EDX_Unused)
{
	CARSFX_AIEngine_InitSFX_Game(This);

	EngineSFXGuardReport(false, "AI engine registered,", *(int*)CARSFX_CurEngineSlot, (DWORD)This);
}

void __fastcall CARSFX_PlayerEngine_InitSFX(DWORD* This, void* EDX_Unused)
{
	CARSFX_PlayerEngine_InitSFX_Game(This);

	EngineSFXGuardReport(false, "player engine registered,", *(int*)CARSFX_CurEngineSlot, (DWORD)This);
}

void InitEngineSFXGuard()
{
	if (!EngineSFXGuard) return;

	std::error_code ErrorCode;
	std::filesystem::remove(CurrentWorkingDirectory / "UnlimiterData" / "_EngineSFXGuard.txt", ErrorCode);

	injector::WriteMemory(0x78A3C8, &CSTATEMGR_PlayerCar_UpdateParams, true); // CSTATEMGR_PlayerCar vtable
	injector::WriteMemory(0x78AEAC, &CARSFX_AIEngine_InitSFX, true);          // CARSFX_AIEngine vtable
	injector::WriteMemory(0x78AEFC, &CARSFX_AIEngine_InitSFX, true);          // and its second table
	injector::WriteMemory(0x78AF4C, &CARSFX_PlayerEngine_InitSFX, true);      // CARSFX_PlayerEngine vtable

	injector::MakeCALL(0x463DB1, CARSFX_EngineBase_IsEngineFinishedLoading, true); // the only caller
}