#pragma once

#include "stdafx.h"
#include "stdio.h"

#define MaxPermTex 109 // original value: 69
#define MaxTempTex 29 // original value: 69

#pragma pack(push, 1)

typedef struct // total size: 0x2FC
{
	DWORD TexturesToLoadPerm[MaxPermTex]; // offset 0x0, size 0x114
	DWORD TexturesToLoadTemp[MaxTempTex]; // offset 0x114, size 0x114
	int NumTexturesToLoadPerm; // offset 0x228, size 0x4
	int NumTexturesToLoadTemp; // offset 0x22C, size 0x4
	DWORD MappedSkinHash; // offset 0x230, size 0x4
	DWORD MappedSkinBHash; // offset 0x234, size 0x4
	DWORD MappedGlobalHash; // offset 0x238, size 0x4
	DWORD MappedWheelHash; // offset 0x23C, size 0x4
	DWORD MappedSpinnerHash; // offset 0x240, size 0x4
	DWORD MappedBadging; // offset 0x244, size 0x4
	DWORD MappedSpoilerHash; // offset 0x248, size 0x4
	DWORD MappedRoofScoopHash; // offset 0x24C, size 0x4
	DWORD MappedDashSkinHash; // offset 0x250, size 0x4
	DWORD MappedLightHash[11]; // offset 0x254, size 0x2C
	DWORD MappedTireHash; // offset 0x280, size 0x4
	DWORD MappedRimHash; // offset 0x284, size 0x4
	DWORD MappedRimBlurHash; // offset 0x288, size 0x4
	DWORD MappedLicensePlateHash; // offset 0x28C, size 0x4
	DWORD ReplaceSkinHash; // offset 0x290, size 0x4
	DWORD ReplaceSkinBHash; // offset 0x294, size 0x4
	DWORD ReplaceGlobalHash; // offset 0x298, size 0x4
	DWORD ReplaceWheelHash; // offset 0x29C, size 0x4
	DWORD ReplaceSpinnerHash; // offset 0x2A0, size 0x4
	DWORD ReplaceSpoilerHash; // offset 0x2A4, size 0x4
	DWORD ReplaceRoofScoopHash; // offset 0x2A8, size 0x4
	DWORD ReplaceDashSkinHash; // offset 0x2AC, size 0x4
	DWORD ReplaceHeadlightHash[3]; // offset 0x2B0, size 0xC
	DWORD ReplaceHeadlightGlassHash[3]; // offset 0x2BC, size 0xC
	DWORD ReplaceBrakelightHash[3]; // offset 0x2C8, size 0xC
	DWORD ReplaceBrakelightGlassHash[3]; // offset 0x2D4, size 0xC
	DWORD ReplaceCentreBrakelightHash[3]; // offset 0x2E0, size 0xC
	DWORD ReplaceReverselightHash[3]; // offset 0x2EC, size 0xC
	DWORD ShadowHash; // offset 0x2F8, size 0x4
} UsedCarTextureInfo;

#pragma pack(pop)

void GetUsedCarTextureInfo(UsedCarTextureInfo* info, DWORD* ride_info, int front_end_only, int engine); // Prototype so the compiler doesn't complain about the function being used before its definition

int UsedCarTextureReplace(DWORD* UsedCarTextureTable, int NumberOfTextures, int SourceTextureHash, int DestinationTextureHash)
{
	if (UsedCarTextureTable)
	{
		for (int i = 0; i < NumberOfTextures; i++)
		{
			if (UsedCarTextureTable[i] == SourceTextureHash) UsedCarTextureTable[i] = DestinationTextureHash;
		}
	}

	return DestinationTextureHash;
}