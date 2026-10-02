#pragma once
#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"

char RimBrandName[64];

#define dword_838970 *(DWORD*)0x838970
#define _RimsCategory_vtbl 0x79BE44

void __fastcall ChooseRimBrand_AddSameAsRimCategory(DWORD* ChooseRimBrand, void* EDX_Unused, DWORD brand, DWORD icon, DWORD text)
{
    DWORD* CRBP = (DWORD*)j__malloc(0x54);

    if (CRBP)
    {
        CRBP[18] = brand;
        CRBP[11] = text;
        CRBP[3] = icon;
        CRBP[4] = 0;
        CRBP[5] = 0;
        CRBP[6] = 0;
        *((BYTE*)CRBP + 32) = 0;
        *((BYTE*)CRBP + 33) = 1;
        *((BYTE*)CRBP + 34) = 0;
        *((BYTE*)CRBP + 35) = 0;
        
        CRBP[12] = 0;
        CRBP[13] = 1065353216;
        CRBP[14] = 1065353216;
        CRBP[15] = 1065353216;
        CRBP[16] = 1065353216;
        *((BYTE*)CRBP + 68) = 1;
        *((BYTE*)CRBP + 69) = 0;
        *CRBP = (DWORD)_RimsCategory_vtbl;
        CRBP[19] = (DWORD)&CRBP[19];
        CRBP[20] = (DWORD)&CRBP[19];
        
    }
    else
    {
        CRBP = 0;
    }
    
    DWORD* icn = (*(DWORD*(__thiscall**)(DWORD*, DWORD*))(*((DWORD*)ChooseRimBrand + 19) + 8))(ChooseRimBrand + 19, CRBP); // IconScroller_AddOption
    if (icn && icn[9] != icon)
    {
        icn[9] = icon;
        icn[7] |= 0x400000u;
    }
}

void __fastcall ChooseRimBrand_Setup(DWORD* ChooseRimBrand, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    
    char const* ChooseRimBrandPackage = (char const*)ChooseRimBrand[1];

    // Add stock
    ChooseRimBrand_AddRimCategory(ChooseRimBrand, CT_bStringHash("STOCK"), CT_bStringHash("VISUAL_RIMS_BRAND_STOCK"), CT_bStringHash("RIMS_BRAND_STOCK"));// STOCK

    CarPart* Part = (CarPart*)RideInfo_GetPart((DWORD*)gTheRideInfo, RimsToCustomize == -1 ? CARSLOTID_REAR_WHEEL : CARSLOTID_FRONT_WHEEL);
    CarPart* OtherPart = (CarPart*)RideInfo_GetPart((DWORD*)gTheRideInfo, RimsToCustomize == -1 ? CARSLOTID_FRONT_WHEEL : CARSLOTID_REAR_WHEEL);
    CarPart* StockPart = (CarPart*)CarCustomizeManager_GetStockCarPart_Game((DWORD*)gCarCustomizeManager, RimsToCustomize == -1 ? CARSLOTID_REAR_WHEEL : CARSLOTID_FRONT_WHEEL);

    // Add same as front/rear
    if (Part && OtherPart && StockPart)
    {
        switch (RimsToCustomize)
        {
        case -1:
            ChooseRimBrand_AddSameAsRimCategory(ChooseRimBrand, EDX_Unused, CT_bStringHash("SAME_AS_FRONT"), CT_bStringHash("VISUAL_RIMS_BRAND_STOCK"), CT_bStringHash("RIMS_BRAND_SAME_AS_FRONT")); // Same as front
            break;
        case 1:
            ChooseRimBrand_AddSameAsRimCategory(ChooseRimBrand, EDX_Unused, CT_bStringHash("SAME_AS_REAR"), CT_bStringHash("VISUAL_RIMS_BRAND_STOCK"), CT_bStringHash("RIMS_BRAND_SAME_AS_REAR")); // Same as rear
            break;
        }
    }

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

    if (Part)
    {
        unsigned int BrandNameHash = LastButton ? LastButton : CarPart_GetAppliedAttributeUParam((DWORD*)Part, CT_bStringHash("BRAND_NAME"), 0);

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

void __fastcall ChooseRimBrand_NotificationMessage(DWORD* ChooseRimBrandScreen, void* EDX_Unused, DWORD message, DWORD* fe_obj, DWORD param1, DWORD param2)
{
    DWORD* CategoryNode = (DWORD*)ChooseRimBrandScreen[22];
    DWORD BrandHash = 0;
    if (CategoryNode) BrandHash = CategoryNode[18];

    CarPart* Part = (CarPart*)RideInfo_GetPart((DWORD*)gTheRideInfo, RimsToCustomize == -1 ? CARSLOTID_REAR_WHEEL : CARSLOTID_FRONT_WHEEL);
    CarPart* OtherPart = (CarPart*)RideInfo_GetPart((DWORD*)gTheRideInfo, RimsToCustomize == -1 ? CARSLOTID_FRONT_WHEEL : CARSLOTID_REAR_WHEEL);
    CarPart* StockPart = (CarPart*)CarCustomizeManager_GetStockCarPart_Game((DWORD*)gCarCustomizeManager, RimsToCustomize == -1 ? CARSLOTID_REAR_WHEEL : CARSLOTID_FRONT_WHEEL);

    switch (message)
    {
    case CT_bStringHash("SAME_AS_FRONT"):
        IconScrollerMenu_NotificationMessage(ChooseRimBrandScreen, message, fe_obj, param1, param2);
        CarCustomizeManager_InstallPart((DWORD*)gCarCustomizeManager, CARSLOTID_REAR_WHEEL, (DWORD*)OtherPart);
        IconScrollerMenu_StopDim(ChooseRimBrandScreen);
        break;
    case CT_bStringHash("SAME_AS_REAR"):
        IconScrollerMenu_NotificationMessage(ChooseRimBrandScreen, message, fe_obj, param1, param2);
        CarCustomizeManager_InstallPart((DWORD*)gCarCustomizeManager, CARSLOTID_FRONT_WHEEL, (DWORD*)OtherPart);
        IconScrollerMenu_StopDim(ChooseRimBrandScreen);
        break;
    case CT_bStringHash("BUTTON_PRESSED"):
        IconScrollerMenu_NotificationMessage(ChooseRimBrandScreen, message, fe_obj, param1, param2);
        switch (BrandHash)
        {
        case CT_bStringHash("STOCK"): // Stock
            if (Part == StockPart)
            {
                DialogInterface_ShowOk((const char*)ChooseRimBrandScreen[1], "", CT_bStringHash("CUSTOMIZE_CONFIRM_PART_ALREADY_INSTALLED"));
                dword_838970 = 0x34DC1BEC;
            }
            else
            {
                DialogInterface_ShowTwoButtons(
                    (const char*)ChooseRimBrandScreen[1],
                    "",
                    CT_bStringHash("YES"),
                    CT_bStringHash("NO"),
                    0xD05FC3A3,
                    0x1FAB5998,
                    0x1FAB5998,
                    1,
                    CT_bStringHash("CUSTOMIZE_CONFIRM_RESET_TO_STOCK"));
                dword_838970 = 0x1FAB5998;
            }
            IconScrollerMenu_StartDim(ChooseRimBrandScreen);
            break;
        case CT_bStringHash("SAME_AS_FRONT"): // Rear -> Same as front
            if (Part == OtherPart)
            {
                DialogInterface_ShowOk((const char*)ChooseRimBrandScreen[1], "", CT_bStringHash("CUSTOMIZE_CONFIRM_PART_ALREADY_INSTALLED"));
                dword_838970 = 0x34DC1BEC;
            }
            else
            {
                DialogInterface_ShowTwoButtons(
                    (const char*)ChooseRimBrandScreen[1],
                    "",
                    CT_bStringHash("YES"),
                    CT_bStringHash("NO"),
                    CT_bStringHash("SAME_AS_FRONT"),
                    0x1FAB5998,
                    0x1FAB5998,
                    1,
                    CT_bStringHash("CUSTOMIZE_CONFIRM_SAME_AS_FRONT"));
                dword_838970 = 0x1FAB5998;
            }
            IconScrollerMenu_StartDim(ChooseRimBrandScreen);
            break;
        case CT_bStringHash("SAME_AS_REAR"): // Front -> Same as rear
            if (Part == OtherPart)
            {
                DialogInterface_ShowOk((const char*)ChooseRimBrandScreen[1], "", CT_bStringHash("CUSTOMIZE_CONFIRM_PART_ALREADY_INSTALLED"));
                dword_838970 = 0x34DC1BEC;
            }
            else
            {
                DialogInterface_ShowTwoButtons(
                    (const char*)ChooseRimBrandScreen[1],
                    "",
                    CT_bStringHash("YES"),
                    CT_bStringHash("NO"),
                    CT_bStringHash("SAME_AS_REAR"),
                    0x1FAB5998,
                    0x1FAB5998,
                    1,
                    CT_bStringHash("CUSTOMIZE_CONFIRM_SAME_AS_REAR"));
                dword_838970 = 0x1FAB5998;
            }
            IconScrollerMenu_StartDim(ChooseRimBrandScreen);
            break;
        default:
            //goto vanilla;
            if (!FEngIsScriptRunning_Pkg((const char*)ChooseRimBrandScreen[1], CT_bStringHash("_EVENT_HANDLER_"), CT_bStringHash("FORWARD"))
                && !*((BYTE*)CategoryNode + 34))
            {
                FEngSetScript_Pkg_Hsh((const char*)ChooseRimBrandScreen[1], CT_bStringHash("_EVENT_HANDLER_"), CT_bStringHash("FORWARD"), 1);
            }
            break;
        }
        break;
    default:
    vanilla:
        ChooseRimBrand_NotificationMessage_Game(ChooseRimBrandScreen, message, fe_obj, param1, param2);
        break;
    }
}