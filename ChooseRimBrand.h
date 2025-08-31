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
    ChooseRimBrand_AddRimCategory(ChooseRimBrand, CT_bStringHash("STOCK"), CT_bStringHash("VISUAL_RIMS_BRAND_STOCK"), CT_bStringHash("RIMS_BRAND_STOCK"));// STOCK

    // Read brand from ini
    bool HasNoCustomRims = CarConfigs[CarTypeID].BodyShop.RimsCustom == 0;

    int RimBrandsCount = RimBrands.size();
    DWORD RimBrandIconHash = -1;
    DWORD RimBrandNameHash = -1;

    for (int i = 0; i < RimBrandsCount; i++)
    {
        if (i == 0)
        {
            if (HasNoCustomRims) continue;

            RimBrandIconHash = CarConfigs[CarTypeID].Icons.BodyShopRimsCustom;
            RimBrandNameHash = CarConfigs[CarTypeID].Names.BodyShopRimsCustom;

            if (RimBrandIconHash == -1) RimBrandIconHash = RimBrands[i].TextureHash;
            if (RimBrandNameHash == -1) RimBrandNameHash = RimBrands[i].StringHash;
        }
        else
        {
            RimBrandIconHash = RimBrands[i].TextureHash;
            RimBrandNameHash = RimBrands[i].StringHash;
        }

        if (RemoveRimSizeRestrictions || (IsSUV(CarTypeID) && RimBrands[i].AvailableForSUVs) || (!IsSUV(CarTypeID) && RimBrands[i].AvailableForRegularCars))
        ChooseRimBrand_AddRimCategory(ChooseRimBrand, RimBrands[i].BrandNameHash, RimBrandIconHash, RimBrandNameHash);
    }

    // Check installed part
    int LastButton = FEngGetLastButton(ChooseRimBrandPackage); // returns hash

    DWORD* CarPart = RideInfo_GetPart((DWORD*)gTheRideInfo, RimsToCustomize == -1 ? CAR_SLOT_ID::REAR_WHEEL : CAR_SLOT_ID::FRONT_WHEEL);
    if (CarPart)
    {
        unsigned int BrandNameHash = LastButton ? LastButton : CarPart_GetAppliedAttributeUParam(CarPart, CT_bStringHash("BRAND_NAME"), 0);

        if (BrandNameHash)
        {
            DWORD* TS = (DWORD*)ChooseRimBrand[20];
            DWORD* TSLast = (DWORD*)ChooseRimBrand + 20;
            for (int i = 1; i < RimBrandsCount && TS != TSLast; i++, TS = (DWORD*)TS[0])
            {
                if (TS[17] == BrandNameHash)
                {
                    LastButton = i; // get number
                    break;
                }
            }
        }
    }

    (*(void(__thiscall**)(DWORD*, int))(ChooseRimBrand[19] + 32))(ChooseRimBrand + 19, LastButton); // IconScroller::SetInitialPos

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
    if (BrandNameHash == CT_bStringHash("SPINNER")) return 0;

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

    if ((*((BYTE*)CarPart + 5) & 0xE0) != 0 || (BrandNameHash != CT_bStringHash("STOCK")))
        IsStock = 0;

    if (CarPart_GetAppliedAttributeUParam(CarPart, CT_bStringHash("BRAND_NAME"), 0) == BrandNameHash) // "BRAND_NAME"
    {
        RimOuterRadius = *(BYTE*)((BYTE*)CarTypeInfo + 0xDC);
        if ((CarPart_GetAppliedAttributeUParam(CarPart, CT_bStringHash("OUTER_RADIUS"), 0) == RimOuterRadius) || (IsNoRimSize(BrandNameHash)) || RemoveRimSizeRestrictions)
        {
            UnlockFilter = CarCustomizeManager_GetPartUnlockFilter();
            if (UnlockSystem_IsCarPartUnlocked(UnlockFilter, 29, CarPart, *(int*)0x8389B0))
                return IsStock | 1;
        }
    }
    return 0;
}