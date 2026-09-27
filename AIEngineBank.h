#pragma once

#include "stdafx.h"
#include "GlobalVariables.h"

#define CARSFX_LoadedAIEngineBanks 0x82A5A0
constexpr int CARSFX_LoadedBankCount = 6; // rep stosd of 6 dwords at 0x480FD1

// The engine data array we hand the game lives in this DLL, so the base has to come from the patch
// site rather than a constant: whichever g_ED the redirect at 0x4594DB points at right now.
DWORD EngineDataArrayBase()
{
	return injector::ReadMemory<DWORD>(0x4594DB, true);
}

// -1 for anything that is not a whole record into that array, which is how a restored entry is
// checked for being real before it is put back.
int EngineDataIndexOf(DWORD Entry)
{
	DWORD Base = EngineDataArrayBase();

	if (!Base || Entry < Base) return -1;

	DWORD Offset = Entry - Base;

	return (Offset % 0x4C) ? -1 : (int)(Offset / 0x4C);
}

// Stops an AI car from being given another AI car's engine sound.
//
// Two arrays decide what an opponent sounds like. CSTATEMGR_AICar::ResolveEngineBankLoading
// (0x463F60) picks an engine data entry per car and writes it to [car+7Ch] and [car+80h]. Then
// CARSFX_AIEngine::SetupLoadData (0x465EA0) appends that same [car+80h] to LoadedAIEngineBanks
// (0x82A5A0), a six slot list of what is in memory, walking it with CurLoadedAIBank (0x7ED928):
//
//   00465EAC  mov  eax, CurLoadedAIBank
//   00465EB1  mov  [eax], edx            ; this car's entry
//   00465EB7  add  CurLoadedAIBank, 4    ; no bounds check, no per race reset
//
// EAXAITunerCar::ConnectCar (0x4698D0) runs when the car object shows up and looks for its own
// entry in those six slots. On a miss it does not keep what Resolve chose, it takes the next
// loaded bank instead:
//
//   004698F3  mov  eax, dword_82CB3C     ; round robin cursor
//   004698F8  inc  eax
//   00469900  div  esi                   ; modulo six
//   00469902  mov  eax, LoadedAIEngineBanks[edx*4]
//   0046995D  mov  [ecx+80h], esi        ; and overwrite the car's own choice with it
//
// So a miss is not a fallback, it is a swap, and it is permanent: SetupLoadData later appends
// whatever ConnectCar left behind, so the wrong bank is the one that actually gets loaded.
//
// A miss is easy to come by. LoadedAIEngineBanks is cleared only in sub_480EB0 (0x480FC5) and
// CurLoadedAIBank is reset only there too, so the cache carries over between races while the
// cursor keeps climbing. Once it is past the sixth slot the appends land outside the array
// entirely, where the search at 0x4698E0 cannot see them, and every AI car misses. Each one then
// takes the next entry the cursor points at, which is a rotation: with three cars whose banks are
// all still sitting in the stale slots, all three sounds are present and all three are on the
// wrong car. The player is unaffected, a player car is a CARSFX_PlayerEngine and never goes
// through this path.
//
// None of this is audible in a stock game. Vanilla loads at most three engine banks for the whole
// AI field, one per engine group (0x464095, bRandom over EngGroup1..3), so opponents already share
// a handful of generic engines and being handed a neighbour's is not something an ear can catch.
// Give every car its own bank, which is the point of Car Sound Tuner, and the swap becomes obvious.
//
// ConnectCar is only the second place a car loses its engine, though. Resolve itself will not let
// a car keep its own bank unless that bank is named in a hardcoded table:
//
//   00463FF0  call IsEngineInLoadedArray   ; is my engine id listed for my engine group?
//   00463FFA  jz   loc_464039              ; no, so this car cannot have its own
//   00464095  call bRandom                 ; pick one of the group's ids at random instead
//
// The three tables at 0x7ECAA8 hold nothing but stock ids, EngGroup1 {0,1,2,3,4,5,10,11,18,20,21,
// 23,24,34}, EngGroup2 {1,2,6,15,16,18,20,21,24,25,28}, EngGroup3 {12,13,9,15}. A car whose sound
// car type is outside 1..32 falls to group 0 as well (0x458F05). So any car sitting on an id that
// EA did not ship in its group, which is most of them once Car Sound Tuner is in play, is given a
// random stock engine belonging to a car that may not even be in the race.
//
// The fix has two halves that work together. After Resolve runs, put back the entry the car's own
// mapping chose, which CarPreSetup left at [car+88h] and which Resolve never touches. Then, at
// ConnectCar, keep that entry instead of the round robin's offer. The game functions both still
// run, so anything they get right is untouched.
//
// Restoring is bounded. Vanilla loads three engine banks for the whole AI field and this allows
// six, the size of the array the game itself keeps, so a full race of opponents each get their own
// without opening the door to a sound pack loading a bank per car. Cars past the budget keep
// whatever Resolve gave them, which is what they would have had anyway.
//
// The append into LoadedAIEngineBanks is bounds checked at the same time, since writing past the
// sixth slot lands in the data that follows the array.

bool AIEngineBankFix = true;

#define CARSFX_CurLoadedAIBank 0x7ED928
// One past the sixth slot. Inline asm only folds macros, so this cannot be written in terms of
// CARSFX_LoadedBankCount; the assert keeps the two from drifting apart.
#define CARSFX_LoadedAIEngineBanksEnd (CARSFX_LoadedAIEngineBanks + 24)
static_assert(CARSFX_LoadedAIEngineBanksEnd == CARSFX_LoadedAIEngineBanks + CARSFX_LoadedBankCount * 4, "bank array size");

void(__thiscall* EAXAITunerCar_ConnectCar_Game)(DWORD* This, DWORD* Car) = (void(__thiscall*)(DWORD*, DWORD*))0x4698D0;

void __declspec(naked) AIEngineBankAppendGuardCodeCave()
{
	_asm {
		mov eax, ds: [CARSFX_CurLoadedAIBank]
		cmp eax, CARSFX_LoadedAIEngineBanksEnd
		jnb Full
		mov[eax], edx
		add dword ptr ds : [CARSFX_CurLoadedAIBank] , 4
		Full :
		mov eax, [esp + 4]
		push 0x465EBE
		retn
	}
}

// Vanilla budgets three banks for the whole field. Six is the size of LoadedAIEngineBanks, so this
// stays inside what the game already keeps track of.
constexpr int AIEngineBankBudget = CARSFX_LoadedBankCount;

void(__thiscall* CSTATEMGR_AICar_ResolveEngineBankLoading_Game)(DWORD* This) = (void(__thiscall*)(DWORD*))0x463F60;
bool(__cdecl* IsEngineInLoadedArray_Game)(int Group, int Id) = (bool(__cdecl*)(int, int))0x458E70;

int AIEngineBankRestored = 0;

void __fastcall CSTATEMGR_AICar_ResolveEngineBankLoading(DWORD* This, void* EDX_Unused)
{
	CSTATEMGR_AICar_ResolveEngineBankLoading_Game(This);

	if (!AIEngineBankFix) return;

	DWORD Base = EngineDataArrayBase();

	if (!Base) return;

	int Spent = 0;
	int Distinct[AIEngineBankBudget];

	// The manager keeps its states on a singly linked list, head at +10h, next at +4, which is the
	// same walk Resolve and ConnectCar do.
	for (DWORD* State = (DWORD*)This[0x10 / 4]; State; State = (DWORD*)State[0x04 / 4])
	{
		DWORD Mapping = State[0x88 / 4];

		if (Mapping < 0x10000 || IsBadReadPtr((void*)Mapping, 4)) continue;

		// The first field of the mapping row is the engine id, which is exactly what CarPreSetup
		// read out of it at 0x4594D1 before Resolve had its say.
		int OwnId = *(int*)Mapping;

		if (OwnId < 0 || OwnId > 255) continue;

		DWORD OwnEntry = Base + OwnId * 0x4C;

		if (IsBadReadPtr((void*)OwnEntry, 0x4C)) continue;

		DWORD Given = State[0x80 / 4];

		if (Given == OwnEntry) continue;

		// A bank already being restored for another car is free, only a new one costs a slot.
		int Seen = 0;

		while (Seen < Spent && Distinct[Seen] != OwnId) Seen++;

		if (Seen == Spent)
		{
			if (Spent == AIEngineBankBudget) continue;

			Distinct[Spent++] = OwnId;
		}

		State[0x7C / 4] = OwnId;
		State[0x80 / 4] = OwnEntry;

		AIEngineBankRestored++;
	}
}

void __fastcall EAXAITunerCar_ConnectCar(DWORD* This, void* EDX_Unused, DWORD* Car)
{
	DWORD BeforeIndex = This[0x7C / 4];
	DWORD BeforeEntry = This[0x80 / 4];

	EAXAITunerCar_ConnectCar_Game(This, Car);

	if (This[0x80 / 4] == BeforeEntry) return;

	// Only undo a swap when the entry being put back is a real one.
	if (EngineDataIndexOf(BeforeEntry) < 0) return;

	This[0x7C / 4] = BeforeIndex;
	This[0x80 / 4] = BeforeEntry;

	AIEngineBankRestored++;
}

void InitAIEngineBank()
{
	if (!AIEngineBankFix) return;

	injector::WriteMemory(0x78BF3C, &EAXAITunerCar_ConnectCar, true); // EAXAITunerCar vtable +0Ch

	injector::MakeCALL(0x476B9F, CSTATEMGR_AICar_ResolveEngineBankLoading, true); // its only caller

	injector::MakeJMP(0x465EAC, AIEngineBankAppendGuardCodeCave, true); // CARSFX_AIEngine::SetupLoadData
}