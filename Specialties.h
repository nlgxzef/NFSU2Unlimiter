#pragma once

#include "stdafx.h"
#include <string.h>

// The Specialties menu is built in IceSelectionScreen.h and gated again in Helpers.h, and both need
// to agree on which entries the career garage is allowed to show, so the shared part lives here.

bool IsCustomizingFromGarage()
{
	return *(int*)0x83898C == 1;
}

// One bit per entry of the Specialties menu, in the order the menu lists them.
enum eSpecialty
{
	SPEC_CustomGauges, SPEC_Neon, SPEC_WindowTint, SPEC_HeadlightColor, SPEC_NosPurge,
	SPEC_ExhaustFlame, SPEC_TireSmoke, SPEC_Hydraulics, SPEC_TrunkAudio, SPEC_Spinners,
	SPEC_SplitHoods, SPEC_Doors, SPEC_LicensePlate, SPEC_NUM
};

const char* SpecialtyNames[SPEC_NUM] =
{
	"CustomGauges", "Neon", "WindowTint", "HeadlightColor", "NosPurge",
	"ExhaustFlame", "TireSmoke", "Hydraulics", "TrunkAudio", "Spinners",
	"SplitHoods", "Doors", "LicensePlate"
};

// Hiding the whole category was too blunt. Some of these do survive a purchase and belong in the
// garage, others behave like a shop with no prices, and which is which differs per car. So the
// setting names the ones to hide rather than switching the lot.
unsigned int HiddenSpecialtiesInGarage = 0;

// Whether this entry should be offered at all. Enabled is the car's own config flag and still has
// the final say: a car with no neons never shows neons, wherever it is being customized from.
bool IsSpecialtyOffered(int Which, int Enabled)
{
	if (!Enabled) return false;

	if (IsCustomizingFromGarage() && (HiddenSpecialtiesInGarage & (1u << Which))) return false;

	return true;
}

// Accepts 0 for nothing hidden, 1 for all of them, which is what the old boolean meant, or a list of
// entry names. The settings reader hands back the raw line, trailing // comment and all, so that
// goes first: a comment mentioning Neon must not switch Neon off.
unsigned int ParseHiddenSpecialties(const char* Value)
{
	if (!Value) return 0;

	char Buf[512];
	strncpy(Buf, Value, sizeof(Buf) - 1);
	Buf[sizeof(Buf) - 1] = 0;

	char* Comment = strstr(Buf, "//");
	if (Comment) *Comment = 0;

	char* Start = Buf;
	while (*Start == ' ' || *Start == '	') Start++;

	char* End = Start + strlen(Start);
	while (End > Start && (End[-1] == ' ' || End[-1] == '	' || End[-1] == '^M')) *--End = 0;

	if (strcmp(Start, "0") == 0) return 0;
	if (strcmp(Start, "1") == 0) return (1u << SPEC_NUM) - 1;

	unsigned int Mask = 0;

	for (char* Token = strtok(Start, " ,;	"); Token; Token = strtok(nullptr, " ,;	"))
		for (int i = 0; i < SPEC_NUM; i++)
			if (_stricmp(Token, SpecialtyNames[i]) == 0) { Mask |= 1u << i; break; }

	return Mask;
}