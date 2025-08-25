#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall ChooseCustomizeCategory_Setup(DWORD* ChooseCustomizeCategory, void* EDX_Unused)
{
    DWORD* CCT;

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    if (*(int*)_FEDatabase == 256 && *(int*)0x836338) // Online Car Lot
    {
        CCT = (DWORD*)j__malloc(0x4Cu);
        if (CCT)
            CustomizeCategoryThing_IconOption(CCT,
                5,
                0x8244F164, // SHOP_ICON_CAR_LOT
                0x7EDC9715, // CAREER_CAR_LOT_TITLE
                0);
        else
            CCT = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, CCT);
    }

    if (CarConfigs[CarTypeID].Category.BodyShop && !IsMenuEmpty_BodyShop(CarTypeID))
    {
        CCT = (DWORD*)j__malloc(0x4Cu);
        if (CCT)
            CustomizeCategoryThing_IconOption(
                CCT,
                0,
                CarConfigs[CarTypeID].Icons.BodyShop,
                CarConfigs[CarTypeID].Names.BodyShop,
                0);
        else
            CCT = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, CCT);
    }

    if (CarConfigs[CarTypeID].Category.Performance && !IsMenuEmpty_Performance(CarTypeID))
    {
        CCT = (DWORD*)j__malloc(0x4Cu);
        if (CCT)
            CustomizeCategoryThing_IconOption(
                CCT,
                1,
                CarConfigs[CarTypeID].Icons.Performance,
                CarConfigs[CarTypeID].Names.Performance,
                0);
        else
            CCT = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, CCT);
    }
    
    if ((CarConfigs[CarTypeID].Category.Paint) && (*(int*)0x83898C != 1) && !IsMenuEmpty_Paint(CarTypeID))
    {
        CCT = (DWORD*)j__malloc(0x4Cu);
        if (CCT)
            CustomizeCategoryThing_IconOption(
                CCT,
                2,
                CarConfigs[CarTypeID].Icons.Paint,
                CarConfigs[CarTypeID].Names.Paint,
                0);
        else
            CCT = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, CCT);
    }

    if (CarConfigs[CarTypeID].Category.Specialties)
    {
        CCT = (DWORD*)j__malloc(0x4Cu);
        if (CCT)
            CustomizeCategoryThing_IconOption(
                CCT,
                3,
                CarConfigs[CarTypeID].Icons.Specialties,
                CarConfigs[CarTypeID].Names.Specialties,
                0);
        else
            CCT = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, CCT);
    }

    if (((*(int*)0x83898C == 3 || *(int*)0x83898C == 2 || *(int*)0x83898C == 4) && *(int*)_FEDatabase != 512)
        && CarConfigs[CarTypeID].Performance.TestDyno)
    {
        DWORD* PMST = (DWORD*)j__malloc(0x48u);
        if (PMST)
            PerfMenuStartTuning_IconOption(
                PMST,
                CarConfigs[CarTypeID].Icons.PerformanceTestDyno,
                CarConfigs[CarTypeID].Names.PerformanceTestDyno,
                0);
        else
            PMST = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, PMST);
    }
    
    // Debug Car Customize
    CCT = (DWORD*)j__malloc(0x4Cu);
    if (CCT)
        CustomizeCategoryThing_IconOption(
            CCT,
            4,
            0x74CE8C0B, // UI_ICON_DEBUG
            0xE79A53F8, // OPT_DEBUG
            0);
    else
        CCT = 0;
    (*(void(__thiscall**)(DWORD*, DWORD*))(*ChooseCustomizeCategory + 24))(ChooseCustomizeCategory, CCT);

    unsigned int LastButton = FEngGetLastButton((char const*)ChooseCustomizeCategory[1]);

    (*(void(__thiscall**)(DWORD*, int))(ChooseCustomizeCategory[19] + 32))(ChooseCustomizeCategory + 19, LastButton); // IconScroller::SetInitialPos
    (*(int(__thiscall**)(DWORD*))(*ChooseCustomizeCategory + 16))(ChooseCustomizeCategory); // IconScrollerMenu::RefreshHeader
}