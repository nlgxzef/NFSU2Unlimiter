#pragma once
#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"

void __stdcall ChooseRimBrand_Setup(DWORD* ChooseRimBrand, bool isSuv)
{
	char const* ChooseRimBrandPackage = (char const*)ChooseRimBrand[1];

	// Add stock
	ChooseRimBrand_AddRimCategory(ChooseRimBrand, bStringHash("STOCK"), bStringHash("VISUAL_RIMS_BRAND_STOCK"), bStringHash("RIMS_BRAND_STOCK"));// STOCK

	// Read brand from ini
	int RimBrandsCount = g_Config.RimBrands.size();
	for (int i = 0; i < RimBrandsCount; i++)
	{
		if (isSuv && !g_Config.RimBrands[i]->Suv)
		{
			continue;
		}

		if (!isSuv && !g_Config.RimBrands[i]->Car)
		{
			continue;
		}

		ChooseRimBrand_AddRimCategory(ChooseRimBrand, g_Config.RimBrands[i]->Name, g_Config.RimBrands[i]->Icon, g_Config.RimBrands[i]->String);
	}
}

void __declspec(naked) ChooseRimBrandCave()
{
	static constexpr auto cExit = 0x0054677A;

	__asm
	{
		pushad;
		push al;
		push ebx;
		call ChooseRimBrand_Setup;
		popad;

		jmp cExit;
	}
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