#pragma once
#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"

void __fastcall ChooseRimBrand_Setup(DWORD* ChooseRimBrand, void* EDX_Unused)
{
	char const* ChooseRimBrandPackage = (char const*)ChooseRimBrand[1];

	// Add stock
	ChooseRimBrand_AddRimCategory(ChooseRimBrand, bStringHash("STOCK"), bStringHash("VISUAL_RIMS_BRAND_STOCK"), bStringHash("RIMS_BRAND_STOCK"));// STOCK

	// Read brand from ini
	int RimBrandsCount = g_Config.RimBrands.size();
	for (int i = 0; i < RimBrandsCount; i++)
	{
		ChooseRimBrand_AddRimCategory(ChooseRimBrand, g_Config.RimBrands[i]->Name, g_Config.RimBrands[i]->Icon, g_Config.RimBrands[i]->String);
	}

	// Check installed part
	DWORD* CarPart = CarCustomizeManager_GetInstalledPart((DWORD*)gCarCustomizeManager, 29);
	if (CarPart)
	{
		unsigned int BrandNameHash = CarPart_GetAppliedAttributeUParam(CarPart, bStringHash("BRAND_NAME"), 0);
		if (BrandNameHash)
		{
			if (!FEngGetLastButton(ChooseRimBrandPackage))
			{
				FEngSetLastButton(ChooseRimBrandPackage, BrandNameHash);
			}
		}
	}

	if (FEngGetLastButton(ChooseRimBrandPackage))
	{
		DWORD* v13 = (DWORD*)ChooseRimBrand[20];
		for (int i = 1; i<= (RimBrandsCount + 1); ++i) // +1 = Stock and custom
		{
			DWORD* v14 = v13 ? v13 - 4 : 0;
			DWORD* v15 = ChooseRimBrand == (DWORD*)-80 ? 0 : (DWORD*)((char*)ChooseRimBrand + 76);
			if ((DWORD*)v14 == v15)
				break;

			if (*(DWORD*)(v14 + 72) == FEngGetLastButton(ChooseRimBrandPackage))
			{
				(*(void(__thiscall**)(DWORD*, int))(ChooseRimBrand[19] + 32))(ChooseRimBrand + 19, i);
				break;
			}
			v13 = (DWORD*)*(DWORD*)(v14 + 4);
		}
	}
	else
	{
		(*(void(__thiscall**)(DWORD*, int))(ChooseRimBrand[19] + 32))(ChooseRimBrand + 19, 0);
	}
	(*(void(__thiscall**)(DWORD*))(*(DWORD*)ChooseRimBrand + 16))(ChooseRimBrand);
}

bool IsNoRimSize(DWORD BrandNameHash)
{
	int RimBrandsCount = g_Config.RimBrands.size();
	for (int i = 0; i < RimBrandsCount; i++)
	{
		if (BrandNameHash == g_Config.RimBrands[i]->Name)
		{
			return g_Config.RimBrands[i]->NoRimSize;
		}
	}

	return 0;
}

bool IsNoBrandName(DWORD BrandNameHash)
{
	int RimBrandsCount = g_Config.RimBrands.size();
	for (int i = 0; i < RimBrandsCount; i++)
	{
		if (BrandNameHash == g_Config.RimBrands[i]->Name)
		{
			return g_Config.RimBrands[i]->HideBrandName;
		}
	}

	return 0;
}

bool IsRimAvailable(int CarTypeID, DWORD* CarPart, DWORD BrandNameHash)
{
	bool IsAvailable = 1; // al MAPDST
	bool IsStock = 1; // al MAPDST
	DWORD* CarTypeInfo; // esi
	int RimOuterRadius; // esi
	int UnlockFilter; // eax

	if (!CarPart)
		return 0;

	CarTypeInfo = GetCarTypeInfo(CarTypeID);

	if ((*((BYTE*)CarPart + 5) & 0xE0) != 0 || (BrandNameHash != 0x3B83203)) // "STOCK"
		IsStock = 0;

	if (CarPart_GetAppliedAttributeUParam(CarPart, 0xEBB03E66, 0) == BrandNameHash) // "BRAND_NAME"
	{
		RimOuterRadius = *(BYTE*)((BYTE*)CarTypeInfo + 0xDC);
		if ((CarPart_GetAppliedAttributeUParam(CarPart, 0xCE7D8DB5, 0) == RimOuterRadius) || (IsNoRimSize(BrandNameHash)) || RemoveRimSizeRestrictions)
		{
			UnlockFilter = CarCustomizeManager_GetPartUnlockFilter();
			if (UnlockSystem_IsCarPartUnlocked(UnlockFilter, 29, CarPart, *(int*)0x8389B0))
				return IsStock | 1;
		}
	}
	return 0;
}