#pragma once
#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"

char RimBrandID[15];
char RimBrandName[64];
char RimBrandString[64];
char RimBrandIcon[64];

void __fastcall ChooseRimBrand_Setup(DWORD* ChooseRimBrand, void* EDX_Unused)
{
    char const* ChooseRimBrandPackage = (char const*)ChooseRimBrand[1];

    // Add stock
    ChooseRimBrand_AddRimCategory(ChooseRimBrand, bStringHash("STOCK"), bStringHash("VISUAL_RIMS_BRAND_STOCK"), bStringHash("RIMS_BRAND_STOCK"));// STOCK

    // Read brand from ini
    CIniReader RimBrandsINI("UnlimiterData\\_RimBrands.ini");
    int RimBrandsCount = RimBrandsINI.ReadInteger("RimBrands", "NumberOfRimBrands", -1);
    for (int i = 0; i <= RimBrandsCount; i++)
    {
        sprintf(RimBrandID, "Brand%d", i);
        sprintf(RimBrandName, RimBrandsINI.ReadString(RimBrandID, "BrandName", ""));
        sprintf(RimBrandIcon, RimBrandsINI.ReadString(RimBrandID, "Texture", ""));
        sprintf(RimBrandString, RimBrandsINI.ReadString(RimBrandID, "String", ""));
        ChooseRimBrand_AddRimCategory(ChooseRimBrand, bStringHash(RimBrandName), bStringHash(RimBrandIcon), bStringHash(RimBrandString));
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
    CIniReader RimBrandsINI("UnlimiterData\\_RimBrands.ini");

    int RimBrandsCount = RimBrandsINI.ReadInteger("RimBrands", "NumberOfRimBrands", -1);
    if (RimBrandsCount == -1) return 0;

    for (int i = 0; i <= RimBrandsCount; i++)
    {
        sprintf(RimBrandID, "Brand%d", i);
        sprintf(RimBrandName, RimBrandsINI.ReadString(RimBrandID, "BrandName", ""));
        int hsh = bStringHash(RimBrandName);
        if (BrandNameHash == bStringHash(RimBrandName))
        {
            return RimBrandsINI.ReadInteger(RimBrandID, "NoRimSize", 0) != 0;
        }
    }

    return 0;
}

bool IsNoBrandName(DWORD BrandNameHash)
{
    CIniReader RimBrandsINI("UnlimiterData\\_RimBrands.ini");

    int RimBrandsCount = RimBrandsINI.ReadInteger("RimBrands", "NumberOfRimBrands", -1);
    if (RimBrandsCount == -1) return 0;

    for (int i = 0; i <= RimBrandsCount; i++)
    {
        sprintf(RimBrandID, "Brand%d", i);
        sprintf(RimBrandName, RimBrandsINI.ReadString(RimBrandID, "BrandName", ""));
        int hsh = bStringHash(RimBrandName);
        if (BrandNameHash == bStringHash(RimBrandName))
        {
            return RimBrandsINI.ReadInteger(RimBrandID, "HideBrandName", 0) != 0;
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