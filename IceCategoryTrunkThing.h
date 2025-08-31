#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define IceCategoryTrunkThing_vTable 0x79BE54

void __fastcall IceCategoryTrunkThing_Create(DWORD* IceCategoryTrunkThing, void *EDX_Unused, DWORD TextureHash, DWORD StringHash)
{
	IceSelectionThing_Create(
		IceCategoryTrunkThing,
		CAR_SLOT_ID::TRUNK_AUDIO, 
		TextureHash,
		StringHash,
		0,
		0,
		0);

	IceCategoryTrunkThing[0] = IceCategoryTrunkThing_vTable;
	IceCategoryTrunkThing[23] = -1;
	((BYTE*)IceCategoryTrunkThing)[34] = IceCategoryIsLocked(CAR_SLOT_ID::TRUNK_AUDIO);
	((BYTE*)IceCategoryTrunkThing)[35] = 1;
}