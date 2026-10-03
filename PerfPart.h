#pragma once

#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"

#pragma pack(push, 1)

typedef struct
{
	int Index;
	DWORD NameHash;
	int Cost;
	int NumBrands;
	DWORD Brand1;
	DWORD Brand2;
	DWORD Brand3;
	DWORD Brand4;
	DWORD Brand5;
	DWORD Brand6;
	DWORD Brand7;
	DWORD Brand8;
	float AmplifierFraction;
	float RangeX;
	float RangeY;
	float RangeZ;
	int ReplacedByIndex1;
	int ReplacedByIndex2;
	BYTE pad[20];
} PerfPart;

#pragma pack(pop)

PerfPart SuperchargerParts[3];

DWORD GetPerfPartNameHash(PerfPart *part)
{
	// Read Part Options for the car
	DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
	int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

	DWORD result = part->NameHash;

	// Replace turbo with supercharger if needed
	if (UsesSupercharger(CarTypeID))
	{
		switch (part->NameHash)
		{
		case CT_bStringHash("TU_STAGE_1_TURBO_KIT"):
			result = DefaultPerformancePartHashes[PERF_PART_SC_STAGE_1_SUPERCHARGER_KIT]; // Supercharger Stage 1
			break;
		case CT_bStringHash("TU_STAGE_2_TURBO_KIT"):
			result = DefaultPerformancePartHashes[PERF_PART_SC_STAGE_2_SUPERCHARGER_KIT]; // Supercharger Stage 2
			break;
		case CT_bStringHash("TU_STAGE_3_TURBO_KIT"):
			result = DefaultPerformancePartHashes[PERF_PART_SC_STAGE_3_SUPERCHARGER_KIT]; // Supercharger Stage 3
			break;
		}
	}

	// Get replaced part name hash
	for (int p = 0; p < DefaultPerformancePartCount; p++)
	{
		if (result == DefaultPerformancePartHashes[p])
		{
			result = CarConfigs[CarTypeID].PerformanceParts.ReplacementHashes[p];
			break;
		}
	}

	return result;
}