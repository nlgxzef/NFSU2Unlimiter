#pragma once
#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"

char RimBrandName[64];

void __fastcall ChooseRimBrand_Setup(DWORD* ChooseRimBrand, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    
    char const* ChooseRimBrandPackage = (char const*)ChooseRimBrand[1];

    // Add stock
    ChooseRimBrand_AddRimCategory(ChooseRimBrand, CT_bStringHash("STOCK"), bStringHash("VISUAL_RIMS_BRAND_STOCK"), bStringHash("RIMS_BRAND_STOCK"));// STOCK

    // Read brand from ini
    int RimBrandsCount = RimBrands.size();
    for (int i = 0; i < RimBrandsCount; i++)
    {
        if (RemoveRimSizeRestrictions || (IsSUV(CarTypeID) && RimBrands[i].AvailableForSUVs) || (!IsSUV(CarTypeID) && RimBrands[i].AvailableForRegularCars))
        ChooseRimBrand_AddRimCategory(ChooseRimBrand, RimBrands[i].BrandNameHash, RimBrands[i].TextureHash, RimBrands[i].StringHash);
    }

    // Check installed part
    DWORD* CarPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 29);
    if (CarPart)
    {
        unsigned int BrandNameHash = CarPart_GetAppliedAttributeUParam(CarPart, CT_bStringHash("BRAND_NAME"), 0);
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

int GetRimBrandIDFromHash(DWORD BrandNameHash)
{
    int RimBrandsCount = RimBrands.size();
    if (RimBrandsCount == -1) return 0;

    for (int i = 0; i <= RimBrandsCount; i++)
    {
        if (BrandNameHash == RimBrands[i].BrandNameHash)
        {
            return i;
        }
    }

    return 0;
}

bool IsNoRimSize(DWORD BrandNameHash)
{
    return RimBrands[GetRimBrandIDFromHash(BrandNameHash)].NoRimSize != 0;
}

bool IsNoBrandName(DWORD BrandNameHash)
{
    return RimBrands[GetRimBrandIDFromHash(BrandNameHash)].HideBrandName != 0;
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