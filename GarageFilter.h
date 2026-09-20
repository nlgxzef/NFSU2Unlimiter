#pragma once

#include "stdafx.h"
#include "stdio.h"
#include <filesystem>
#include "GlobalVariables.h"
#include "InGameFunctions.h"
#include "Specialties.h"

// Keeps the career garage to parts the player actually bought.
//
// The game already means to do this. CarCustomizeManager::GetPartUnlockFilter picks the filter by
// where customizing was entered from:
//
//   0050F0DA  mov  ecx, dword_83898C     ; eCustomizeEntryPoint
//   0050F0E2  cmp  ecx, 1                ; 1 is the career garage
//   0050F0E5  setnz al
//   0050F0E8  lea  eax, ds:2[eax*4]      ; garage 2, shop 6
//
// So the garage asks for bit 2 alone, "owned", while a shop also gets bit 4, "buyable". The leak is
// in what bit 2 accepts. CareerUnlocker::IsCarPartUnlocked reads it as owned OR untiered:
//
//   00515515  mov  bl, [esi+5]
//   0051551D  and  bl, 0E0h              ; the upgrade tier
//   00515520  neg  bl
//   00515522  sbb  bl, bl
//   0051552A  inc  bl                    ; bl = tier is zero
//   0051553D  call OwnsCarPart
//   00515542  or   bl, al
//
// Most visual parts carry no tier, which is the same hole that leaves gauges free, so the garage
// lists neons, purges, tints and the rest whether or not they were ever bought. Ask for ownership
// outright instead.
//
// Whatever is on the car right now is always kept. Ownership is recorded per slot and part index by
// OwnedPartInventory::GetWordToSetBitFor, and a save made before this existed can hold a part that
// was installed without a purchase ever being recorded. Hiding that would take a part off the car
// with no way to put it back.

bool GarageShowsOwnedPartsOnly = false;

// Says, per part, which of the three things this rests on actually held: the entry point really
// being the garage, the career car being found, and the inventory bit being set.
bool GarageFilterTrace = false;
int GarageFilterTraceUsed = 0;
constexpr int GarageFilterTraceLimit = 300;

void GarageFilterTraceLine(const char* fmt, ...)
{
	if (GarageFilterTraceUsed >= GarageFilterTraceLimit) return;

	GarageFilterTraceUsed++;

	auto Path = CurrentWorkingDirectory / "UnlimiterData" / "_GarageFilter.txt";
	FILE* f = fopen(Path.string().c_str(), "a");

	if (!f) return;

	va_list args;
	va_start(args, fmt);
	vfprintf(f, fmt, args);
	va_end(args);

	fclose(f);
}

// GetCurrentCareerCar ends in retn 4, so it takes the career car key as a stack argument and cleans
// it up itself. Declaring it without one made every call eat four bytes of the caller's stack.
DWORD* (__thiscall* PlayerCareerState_GetCurrentCareerCar)(DWORD* This, DWORD CareerCarKey) = (DWORD * (__thiscall*)(DWORD*, DWORD))0x503680;
bool(__thiscall* OwnedPartInventory_OwnsCarPart)(DWORD* This, int CarSlotID, DWORD* CarPart) = (bool(__thiscall*)(DWORD*, int, DWORD*))0x503DB0;

#define ThePlayerCareer 0x83AD90
#define gCurrentCareerCarKey 0x863480 // what IsCarPartUnlocked passes at 0x515518

DWORD* GetCareerCar()
{
	return PlayerCareerState_GetCurrentCareerCar((DWORD*)ThePlayerCareer, *(DWORD*)gCurrentCareerCarKey);
}

bool DoesPlayerOwnCarPart(int CarSlotID, DWORD* CarPart)
{
	DWORD* CareerCar = GetCareerCar();

	// No career car means no inventory to ask, so do not hide anything.
	if (!CareerCar) return true;

	return OwnedPartInventory_OwnsCarPart(CareerCar + 0x434 / 4, CarSlotID, CarPart);
}

// Which slots the ownership requirement applies to when the Body Shop builds the list.
//
// Not all of them, because the tier 0 shortcut is not only a hole. In a body slot a tier 0 part is
// the stock one, and it has to stay reachable or there is no way back from a kit. Tiered parts are
// already judged on ownership by CareerUnlocker::IsCarPartUnlocked, so the garage was always right
// about body kits and nothing here needs to help.
//
// The specialties list has its own builder and passes IsSpecialtyList instead, because there the
// slot says nothing: split hoods come through HOOD and spinners through FRONT_WHEEL.
bool IsGarageOwnershipSlot(int CarSlotID)
{
	switch (CarSlotID)
	{
	case CARSLOTID_SPINNER:
	case CARSLOTID_LICENSE_PLATE:
	case CARSLOTID_WINDOW_TINT:
	case CARSLOTID_NEON:
	case CARSLOTID_NEON_ENGINE:
	case CARSLOTID_NEON_CABIN:
	case CARSLOTID_NEON_TRUNK:
	case CARSLOTID_CABIN_NEON_FRAME:
	case CARSLOTID_HEADLIGHT_BULB:
	case CARSLOTID_DOOR_STYLE:
	case CARSLOTID_HYDRAULICS:
	case CARSLOTID_NOS_PURGE:
	case CARSLOTID_CUSTOM_HUD:
	case CARSLOTID_HUD_BACKING_COLOUR:
	case CARSLOTID_HUD_NEEDLE_COLOUR:
	case CARSLOTID_HUD_CHARACTER_COLOUR:
	case CARSLOTID_MISC:                // Exhaust Flame
	case CARSLOTID_WHEEL_MANUFACTURER:  // Tire Smoke
		return true;
	}

	return CarSlotID >= CARSLOTID_TRUNK_AUDIO && CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_11;
}

// The first part a slot offers is the one that means nothing fitted: the stock bumper in a body
// slot, NEON_NONE in a neon slot. It is never bought, so an ownership test would hide it and leave
// no way to take a part back off. The trace caught exactly that: part 101A3314 was hidden for
// NEON_CABIN while being the part installed in NEON_ENGINE, which is the shared "none" entry.
// First is cleared as it is read, so each loop only has to declare it.
bool ShouldHideInGarage(int CarSlotID, DWORD* CarPart, bool IsSpecialtyList, bool& First)
{
	bool IsFirst = First;
	First = false;

	if (!GarageShowsOwnedPartsOnly && !GarageFilterTrace) return false;
	if (!CarPart) return false;

	bool InGarage = IsCustomizingFromGarage();
	DWORD* CareerCar = GetCareerCar();
	bool Installed = RideInfo_GetPart((DWORD*)gCarCustomizeManager + 592, CarSlotID) == CarPart;
	bool Owned = CareerCar ? OwnedPartInventory_OwnsCarPart(CareerCar + 0x434 / 4, CarSlotID, CarPart) : false;

	bool Hide = GarageShowsOwnedPartsOnly && InGarage && CareerCar
		&& !Installed && !Owned && !IsFirst
		&& (IsSpecialtyList || IsGarageOwnershipSlot(CarSlotID));

	if (GarageFilterTrace)
		GarageFilterTraceLine("slot %3d  part %08X  entrypoint %d  careercar %08X  installed %d  owned %d  first %d  spec %d  -> %s\n",
			CarSlotID, (DWORD)CarPart, *(int*)0x83898C, (DWORD)CareerCar,
			Installed, Owned, IsFirst, IsSpecialtyList, Hide ? "hidden" : "shown");

	return Hide;
}

// For the Body Shop list, which still asks the game first.
bool IsCarPartOffered(int UnlockFilter, int CarSlotID, DWORD* CarPart, int Unk, bool& First)
{
	if (!UnlockSystem_IsCarPartUnlocked(UnlockFilter, CarSlotID, CarPart, Unk)) return false;

	return !ShouldHideInGarage(CarSlotID, CarPart, false, First);
}

// For the specialties list, whose own builder applies no unlock test of its own, so this adds only
// the ownership rule and leaves everything else exactly as it was.
bool IsIcePartOffered(int CarSlotID, DWORD* CarPart, bool& First)
{
	return !ShouldHideInGarage(CarSlotID, CarPart, true, First);
}