#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define _PaintShopCategoryOption_vtbl 0x79B6E4
#define _PSPurchaseSetup_vtbl 0x79B6EC

void __fastcall ChoosePaintShopCategory_AddPaintShopCategoryOption(DWORD* ChoosePaintShopCategory, void* EDX_Unused, DWORD icon, DWORD text, DWORD field_0x48, DWORD field_0x4C, BYTE locked)
{
    DWORD* PSCP = (DWORD*)j__malloc(0x50);

    if (PSCP)
    {
        PSCP[3] = icon;
        PSCP[4] = 0;
        PSCP[5] = 0;
        PSCP[6] = 0;
        *((BYTE*)PSCP + 32) = 0;
        *((BYTE*)PSCP + 33) = 1;
        *((BYTE*)PSCP + 34) = locked;
        *((BYTE*)PSCP + 35) = 0;
        PSCP[11] = text;
        PSCP[12] = 0;
        PSCP[13] = 1065353216;
        PSCP[14] = 1065353216;
        PSCP[15] = 1065353216;
        PSCP[16] = 1065353216;
        *((BYTE*)PSCP + 68) = 1;
        PSCP[18] = field_0x48;
        PSCP[19] = field_0x4C;
        *PSCP = (DWORD)_PaintShopCategoryOption_vtbl;
        *((BYTE*)PSCP + 69) = 0;
    }
    else
    {
        PSCP = 0;
    }
    (*(void(__thiscall**)(DWORD*, DWORD*))(*ChoosePaintShopCategory + 24))(ChoosePaintShopCategory, PSCP);
}

void __fastcall ChoosePaintShopCategory_AddPSPurchaseSetupOption(DWORD* ChoosePaintShopCategory, void* EDX_Unused, DWORD icon, DWORD text, DWORD field_0x40)
{
    DWORD* PSCP = (DWORD*)j__malloc(0x48);

    if (PSCP)
    {
        PSCP[3] = icon;
        PSCP[4] = 0;
        PSCP[5] = 0;
        PSCP[6] = 0;
        *((BYTE*)PSCP + 32) = 0;
        *((BYTE*)PSCP + 33) = 1;
        *((BYTE*)PSCP + 34) = 0;
        *((BYTE*)PSCP + 35) = 0;
        PSCP[11] = text;
        PSCP[12] = field_0x40;
        PSCP[13] = 1065353216;
        PSCP[14] = 1065353216;
        PSCP[15] = 1065353216;
        PSCP[16] = 1065353216;
        *((BYTE*)PSCP + 68) = 1;
        *PSCP = (DWORD)_PSPurchaseSetup_vtbl;
        *((BYTE*)PSCP + 69) = 1;
    }
    else
    {
        PSCP = 0;
    }
    (*(void(__thiscall**)(DWORD*, DWORD*))(*ChoosePaintShopCategory + 24))(ChoosePaintShopCategory, PSCP);
}

void __fastcall ChoosePaintShopCategory_Setup(DWORD* ChoosePaintShopCategory, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    
    if (CarConfigs[CarTypeID].Paint.Paint)
        ChoosePaintShopCategory_AddPaintShopCategoryOption(
        ChoosePaintShopCategory,
        EDX_Unused,
        CarConfigs[CarTypeID].Icons.PaintPaint, // VISUAL_PART_PAINT
        CarConfigs[CarTypeID].Names.PaintPaint, // PAINT_CATEGORY_PAINTS
        2,
        0,
        0); 

    if (CarConfigs[CarTypeID].Paint.Vinyls)
        ChoosePaintShopCategory_AddPaintShopCategoryOption(
        ChoosePaintShopCategory,
        EDX_Unused,
        CarConfigs[CarTypeID].Icons.PaintVinyls, // VISUAL_PART_VINYL
        CarConfigs[CarTypeID].Names.PaintVinyls, // PAINT_CATEGORY_VINYLS
        0,
        1,
        0);
    
    bool IsDecalsLocked = *(int*)_FEDatabase == 1
        ? !(*(bool*)_UnlockAllThings || UnlockSystem_AreAnyDecalsUnlocked(2)) 
        : 0;

    if (CarConfigs[CarTypeID].Paint.Decals)
    ChoosePaintShopCategory_AddPaintShopCategoryOption(
        ChoosePaintShopCategory,
        EDX_Unused,
        CarConfigs[CarTypeID].Icons.PaintDecals, // VISUAL_PART_DECALS
        CarConfigs[CarTypeID].Names.PaintDecals, // PAINT_CATEGORY_DECALS
        1,
        2,
        IsDecalsLocked);
    
    ChoosePaintShopCategory_AddPSPurchaseSetupOption(
        ChoosePaintShopCategory,
        EDX_Unused,
        *(int*)_FEDatabase == 1 ? CarConfigs[CarTypeID].Icons.PaintPurchaseSetup : CarConfigs[CarTypeID].Icons.PaintInstallSetup, // GENERIC_ICON_PURCHASE
        *(int*)_FEDatabase == 1 ? CarConfigs[CarTypeID].Names.PaintPurchaseSetup : CarConfigs[CarTypeID].Names.PaintInstallSetup, // PAINT_CATEGORY_PURCHASE_SETUP / PAINT_CATEGORY_INSTALL_SETUP
        0);
    
    unsigned int LastButton = FEngGetLastButton((char const*)ChoosePaintShopCategory[1]);

    (*(void(__thiscall**)(DWORD*, DWORD))(ChoosePaintShopCategory[19] + 32))(ChoosePaintShopCategory + 19, LastButton); // IconScroller::SetInitialPos
    (*(int(__thiscall**)(DWORD*))(*ChoosePaintShopCategory + 16))(ChoosePaintShopCategory); // IconScrollerMenu::RefreshHeader

    if (*(int*)_FEDatabase == 1)
    {
        ChooseDecalCategory_ShowTotalPaintCost(ChoosePaintShopCategory);
        int PaintSetupPrice = CarCustomizeManager_GetPaintSetupPrice((DWORD*)gCarCustomizeManager);
        FEPrintf((char const*)ChoosePaintShopCategory[1], CT_bStringHash("TRADE_IN_CASH"), "%$d", PaintSetupPrice);
    }
    else
    {
        ChooseDecalCategory_HideTotalPaintCost(ChoosePaintShopCategory);
    }
}