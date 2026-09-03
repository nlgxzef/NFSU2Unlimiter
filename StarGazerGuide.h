#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define ForceCarStars *(float*)0x7FA290

int GetRepFromNumStars(float NumStars)
{
	if (TheStarGazer.MaxStars <= 0) return 0;

	if (NumStars < 0.0f) NumStars = 0.0f;
	if (NumStars > (float)TheStarGazer.MaxStars) NumStars = (float)TheStarGazer.MaxStars;

	float BeforeDot;
	float AfterDot = modff(NumStars, &BeforeDot);
	int iNumStars = (int)floorf(BeforeDot);

	// Max failsafe
	if (NumStars >= TheStarGazer.MaxStars) return TheStarGazer.Rep[TheStarGazer.MaxStars];

	return TheStarGazer.Rep[iNumStars]
		+ (int)AfterDot * (float)(TheStarGazer.Rep[iNumStars + 1] - TheStarGazer.Rep[iNumStars]);
}

float GetNumStarsFromRep(int Rep)
{
	if (TheStarGazer.MaxStars <= 0) return 0.0f;
	if (Rep < 0) Rep = 0;

	int i;

	for (i = 0; i < TheStarGazer.MaxStars; i++)
	{
		if (Rep < TheStarGazer.Rep[i + 1]) break;                            // we are inside level i
		if (TheStarGazer.Rep[i + 1] <= TheStarGazer.Rep[i]) return (float)i; // flat or broken table
	}

	if (i >= TheStarGazer.MaxStars) return (float)TheStarGazer.MaxStars;

	// Calculate level
	int RepForThisLevel = TheStarGazer.Rep[i + 1] - TheStarGazer.Rep[i];
	int Remainder = Rep - TheStarGazer.Rep[i];

	if (RepForThisLevel <= 0 || Remainder <= 0) return (float)i;

	return (float)i + (float)Remainder / (float)RepForThisLevel;
}

float __fastcall StarGazerGuide_GetNumberOfStars(DWORD* StarGazerGuide, void* EDX_Unused, DWORD* ride)
{
	if (TheStarGazer.ForceRep != -1)
	{
		float Forced = GetNumStarsFromRep(TheStarGazer.ForceRep);
		ForceCarStars = Forced;
		return Forced;
	}

	int CarType = ride[0];
	int Rep = CarConfigs[CarType].StarGazer.StartingRep;

	DWORD* WideBodyPart = (DWORD*)ride[356 + CARSLOTID_WIDE_BODY];

	bool HasWidebody = (WideBodyPart != nullptr) && ((*((BYTE*)WideBodyPart + 5) & 0xE0) != 0);

	for (int i = 0; i < CARSLOTID_NUM; i++)
	{
		// Vinyl colours are packed values, not parts
		if (i >= CARSLOTID_VINYL_COLOUR0_0 && i <= CARSLOTID_VINYL_COLOUR3_3) continue;

		// Counted together with FRONT_WHEEL below
		if (i == CARSLOTID_REAR_WHEEL) continue;

		DWORD *Part = (DWORD*)ride[356 + i];

		if (i == CARSLOTID_FRONT_WHEEL) // Compare front and rear wheels, use the greater one
		{
			DWORD* RearPart = (DWORD*)ride[356 + CARSLOTID_REAR_WHEEL];

			int FWRep = Part
				? PlayerCareerState_GetCarPartRep((DWORD*)ThePlayerCareer, EDX_Unused, CarType, i, Part) : 0;
			int RWRep = RearPart
				? PlayerCareerState_GetCarPartRep((DWORD*)ThePlayerCareer, EDX_Unused, CarType, i, RearPart) : 0;

			Rep += (FWRep > RWRep) ? FWRep : RWRep;
			continue;
		}

		if (!Part) continue;
		if (HasWidebody && !IsCustomWidebody(WideBodyPart, i)) continue;
		
		Rep += PlayerCareerState_GetCarPartRep((DWORD*)ThePlayerCareer, EDX_Unused, CarType, i, Part);
		
	}

	return GetNumStarsFromRep(Rep);
}