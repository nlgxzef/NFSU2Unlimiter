#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define ForceCarStars *(float*)0x7FA290

int GetRepFromNumStars(float NumStars)
{
	if (NumStars < 0.0f) NumStars = 0.0f;
	if (NumStars > (float)TheStarGazer.MaxStars) NumStars = (float)TheStarGazer.MaxStars;

	float BeforeDot;
	float AfterDot = modff(NumStars, &BeforeDot);
	int iNumStars = floorf(BeforeDot);
	int iNextNumStars = iNumStars + 1;

	// Max failsafe
	if (NumStars > TheStarGazer.MaxStars) iNextNumStars = iNumStars;

	return TheStarGazer.Rep[iNumStars]
		+ AfterDot * (TheStarGazer.Rep[iNextNumStars] - TheStarGazer.Rep[iNumStars]);
}

float GetNumStarsFromRep(int Rep)
{
	if (Rep < 0) Rep = 0;

	int i;

	for (i = 0; i < TheStarGazer.MaxStars; i++)
	{
		if (Rep >= TheStarGazer.Rep[i])
		{
			if (Rep < TheStarGazer.Rep[i + 1]) break;

			if (TheStarGazer.Rep[i + 1] <= TheStarGazer.Rep[i]) return (float)i;
		}
	}

	if (i == TheStarGazer.MaxStars) return (float)TheStarGazer.MaxStars;

	// Calculate level
	int RepForThisLevel = TheStarGazer.Rep[i + 1] - TheStarGazer.Rep[i];
	int Remainder = Rep - TheStarGazer.Rep[i];

	if (RepForThisLevel <= 0) return (float)i;

	return (float)i + (float)Remainder / (float)RepForThisLevel;
}

float __fastcall StarGazerGuide_GetNumberOfStars(DWORD* StarGazerGuide, void* EDX_Unused, DWORD* ride)
{
	int Rep = CarConfigs[ride[0]].StarGazer.StartingRep;

	if (TheStarGazer.ForceRep != -1)
	{
		ForceCarStars = GetNumStarsFromRep(TheStarGazer.ForceRep);
	}

	if (ForceCarStars != -1.0f) return ForceCarStars;

	DWORD* WideBodyPart = (DWORD*)ride[356 + CAR_SLOT_ID::WIDE_BODY];
	DWORD* Part;
	bool HasWidebody = 1;

	if (!WideBodyPart || ((*(BYTE*)WideBodyPart + 5) & 0xE0) == 0) HasWidebody = 0;

	for (int i = 0; i < CAR_SLOT_ID::__NUM; i++)
	{
		if ((i >= CAR_SLOT_ID::VINYL_COLOUR0_0 && i <= VINYL_COLOUR3_3) || (i == CAR_SLOT_ID::REAR_WHEEL)) continue; // Vinyl Colors & Rear Wheel

		Part = (DWORD*)ride[356 + i];

		if (i == CAR_SLOT_ID::FRONT_WHEEL) // Compare front and rear wheels, use the greater one
		{
			int FWRep = PlayerCareerState_GetCarPartRep((DWORD*)ThePlayerCareer, EDX_Unused, ride[0], i, Part);
			Part = (DWORD*)ride[356 + i + 1];
			int RWRep = PlayerCareerState_GetCarPartRep((DWORD*)ThePlayerCareer, EDX_Unused, ride[0], i, Part);

			Rep += (FWRep > RWRep) ? FWRep : RWRep;
			continue;
		}

		if (Part)
		{
			if (HasWidebody && !IsCustomWidebody(WideBodyPart, i)) continue;
			Rep += PlayerCareerState_GetCarPartRep((DWORD*)ThePlayerCareer, EDX_Unused, ride[0], i, Part);
		}
	}

	return GetNumStarsFromRep(Rep);
}