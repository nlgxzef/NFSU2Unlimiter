#pragma once

#include "stdafx.h"
#include "stdio.h"
#include <filesystem>
#include "GlobalVariables.h"

// Records which engine sound entry a car asks for and which one it actually ends up with.
//
// The game keeps only six engine banks loaded at a time, in LoadedAIEngineBanks (0x82A5A0), and
// EAXAITunerCar::ConnectCar (0x4698D0) does this when a car's own bank is not among them:
//
//   004698E0  walk the six slots looking for this car's entry
//   004698ED  cmp ax, 6
//   004698F3  not found, so take dword_82CB3C, add one, modulo six
//   00469902  mov eax, LoadedAIEngineBanks[edx*4]   ; whatever happens to sit there
//   0046995D  mov [ecx+80h], esi                    ; and use that instead
//
// So a car whose bank did not make the cut is silently given a neighbour's. AIEngineBank.h has the
// full account of how that happens and undoes it; this file is the log that proves which of the
// two is running, and what each car asked for before anything touched it.
//
// Read it as: CarPreSetup lines say what the mapping chose, ConnectCar lines say what survived.

bool EngineBankTrace = false;

#define CARSFX_LoadedAIEngineBanks 0x82A5A0
constexpr int CARSFX_LoadedBankCount = 6; // rep stosd of 6 dwords at 0x480FD1

void(__thiscall* EAXCar_CarPreSetup_Game)(DWORD* This, DWORD* DriverInfo) = (void(__thiscall*)(DWORD*, DWORD*))0x4593B0;
void(__thiscall* EAXAITunerCar_CarPreSetup_Game)(DWORD* This, DWORD* DriverInfo) = (void(__thiscall*)(DWORD*, DWORD*))0x4698A0;

int EngineBankTraceUsed = 0;
constexpr int EngineBankTraceLimit = 600;

void EngineBankTraceLine(const char* fmt, ...)
{
	if (EngineBankTraceUsed >= EngineBankTraceLimit) return;

	EngineBankTraceUsed++;

	auto Path = CurrentWorkingDirectory / "UnlimiterData" / "_EngineBankTrace.txt";
	FILE* f = fopen(Path.string().c_str(), "a");

	if (!f) return;

	va_list args;
	va_start(args, fmt);
	vfprintf(f, fmt, args);
	va_end(args);

	fclose(f);
}

// The arrays we hand the game live in this DLL, so the base has to come from the patch site rather
// than from a constant: whichever of our g_ED the redirect at 0x4594DB is pointing at right now.
DWORD EngineBankTraceBase()
{
	return injector::ReadMemory<DWORD>(0x4594DB, true);
}

int EngineBankIndexOf(DWORD Entry)
{
	DWORD Base = EngineBankTraceBase();

	if (!Base || Entry < Base) return -1;

	DWORD Offset = Entry - Base;

	return (Offset % 0x4C) ? -1 : (int)(Offset / 0x4C);
}

const char* EngineBankNameOf(DWORD Entry)
{
	if (Entry < 0x10000 || IsBadReadPtr((void*)Entry, 4)) return "<unreadable>";

	DWORD Name = *(DWORD*)Entry; // MainRAMBankName is the first field

	if (Name < 0x10000 || IsBadReadPtr((void*)Name, 1)) return "<no name>";

	return (const char*)Name;
}

void EngineBankTraceDumpLoaded()
{
	DWORD* Loaded = (DWORD*)CARSFX_LoadedAIEngineBanks;

	EngineBankTraceLine("      loaded banks:");

	for (int i = 0; i < CARSFX_LoadedBankCount; i++)
	{
		if (!Loaded[i]) { EngineBankTraceLine(" [%d]empty", i); continue; }

		EngineBankTraceLine(" [%d]%d:%s", i, EngineBankIndexOf(Loaded[i]), EngineBankNameOf(Loaded[i]));
	}

	EngineBankTraceLine("\n");
}

// Everything in an engine data record that decides what you actually hear. Field offsets follow
// CarSoundTuner's EngineData: +4 aux bank, +0Ch ginsu accel, +10h use dual ginsu, +14h ginsu decel
// id, +24h/+28h the two volumes, +30h..+44h the three crossfade pairs between the sampled layer and
// the granular one. The game reads all of these off [car+80h], so swapping that pointer swaps the
// mix as well as the bank.
void EngineBankDescribe(const char* Label, DWORD Entry)
{
	if (Entry < 0x10000 || IsBadReadPtr((void*)Entry, 0x4C))
	{
		EngineBankTraceLine("      %s <unreadable>\n", Label);
		return;
	}

	auto Str = [](DWORD At) -> const char* {
		DWORD P = *(DWORD*)At;
		return (P < 0x10000 || IsBadReadPtr((void*)P, 1)) ? "<none>" : (const char*)P;
		};

	EngineBankTraceLine("      %s aux=%s  ginsu=%s  dual=%d/%d  vol a/g=%d/%d  mixL %.2f/%.2f  mixS %.2f/%.2f  mixN %.2f/%.2f\n",
		Label, Str(Entry + 0x04), Str(Entry + 0x0C),
		*(int*)(Entry + 0x10), *(int*)(Entry + 0x14),
		*(int*)(Entry + 0x24), *(int*)(Entry + 0x28),
		*(float*)(Entry + 0x30), *(float*)(Entry + 0x34),
		*(float*)(Entry + 0x38), *(float*)(Entry + 0x3C),
		*(float*)(Entry + 0x40), *(float*)(Entry + 0x44));
}

void EngineBankTraceCarPreSetup(DWORD* This, const char* Which)
{
	DWORD Entry = This[0x80 / 4];

	// +7Ch is the id the mapping resolved to, +8Ch the sound car type it came from, +D0h the engine
	// group, which is what decides the bank a substitution would be allowed to reach for.
	EngineBankTraceLine("CarPreSetup   car=%08X  %s  cartype %d  group %d  mapping picked id %d (%s)\n",
		(DWORD)This, Which, This[0x8C / 4], This[0xD0 / 4],
		EngineBankIndexOf(Entry), EngineBankNameOf(Entry));
}

void __fastcall EAXCar_CarPreSetup(DWORD* This, void* EDX_Unused, DWORD* DriverInfo)
{
	EAXCar_CarPreSetup_Game(This, DriverInfo);

	if (EngineBankTrace) EngineBankTraceCarPreSetup(This, "player");
}

// The AI cars come through their own override, which runs EAXCar::CarPreSetup and then fills in the
// engine group at +D0h. Hooking only EAXCar's vtable slot missed every opponent, because opponents
// are EAXAITunerCar and never reach that slot.
void __fastcall EAXAITunerCar_CarPreSetup(DWORD* This, void* EDX_Unused, DWORD* DriverInfo)
{
	EAXAITunerCar_CarPreSetup_Game(This, DriverInfo);

	if (EngineBankTrace) EngineBankTraceCarPreSetup(This, "ai    ");
}

void InitEngineBankTrace()
{
	if (!EngineBankTrace) return;

	std::error_code ErrorCode;
	std::filesystem::remove(CurrentWorkingDirectory / "UnlimiterData" / "_EngineBankTrace.txt", ErrorCode);

	EngineBankTraceLine("engine bank trace. CarPreSetup says what the mapping chose,\n"
		"ConnectCar says what survived the six slot cache at 0x82A5A0.\n\n");

	injector::WriteMemory(0x78BAD0, &EAXCar_CarPreSetup, true);           // EAXCar vtable +28h
	injector::WriteMemory(0x78BF58, &EAXAITunerCar_CarPreSetup, true);    // EAXAITunerCar vtable +28h
}