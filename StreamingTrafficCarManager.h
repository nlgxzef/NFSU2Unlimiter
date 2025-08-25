#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

BYTE TheStreamingTrafficCarManager[0x1230 + 0x920 * 64]; // 0x1230 = (0x10, DummyRideInfo = 0x8F0, other stuff = 0x330) + 0x920 * traffic car count)

void __fastcall StreamingTrafficCarManager_ApplyRandomSkins(DWORD* StreamingTrafficCarManager, void* EDX_Unused)
{
	DWORD* StreamingTrafficCarHead = StreamingTrafficCarManager + 576;
	char SkinNameBuf[64];
	
	for (DWORD* StreamingTrafficCar = (DWORD*)StreamingTrafficCarManager[576]; 
		StreamingTrafficCar != StreamingTrafficCarHead; 
		StreamingTrafficCar = (DWORD*)StreamingTrafficCar[0])
	{
		// Get skin number and ride info
		BYTE skin = ((BYTE*)StreamingTrafficCar)[23];
		DWORD* ride = StreamingTrafficCar + 4;
		sprintf(SkinNameBuf, "%s_SKIN%d", GetCarTypeName(ride[0]), skin);

		bReleasePrintf("StreamingTrafficCarManager_ApplyRandomSkins: %p = %s\n", StreamingTrafficCar, SkinNameBuf);

		// Update
		ride[353] = bStringHash(SkinNameBuf); // mCompositeSkinHash
	}
}

void __fastcall StreamingTrafficCarManager_Update(DWORD *StreamingTrafficCarManager, void *EDX_Unused)
{
	StreamingTrafficCarManager_Update_Game(StreamingTrafficCarManager);
	StreamingTrafficCarManager_ApplyRandomSkins(StreamingTrafficCarManager, EDX_Unused);
}

void __fastcall StreamingTrafficCarManager_ResetStats(DWORD* StreamingTrafficCarManager, void* EDX_Unused)
{
	StreamingTrafficCarManager_ResetStats_Game(StreamingTrafficCarManager);
	StreamingTrafficCarManager_ApplyRandomSkins(StreamingTrafficCarManager, EDX_Unused);
}