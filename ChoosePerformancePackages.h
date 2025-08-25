#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define _ChoosePerformancePackages_bBrowsingIndividualParts 0x83855D
#define _ChoosePerformancePackages_CurrentPackageType 0x838994

#define _PPCustomizePackage_vtbl 0x79B604
#define _PPPackage_vtbl 0x79B60C
#define _PPReturnToStock_vtbl 0x79B614

#define _PerformancePackageList 0x8383F8
#define _NumPerfPackages 0x8383FC

DWORD __fastcall ChoosePerformancePackages_GetIconForPackageType(DWORD* ChoosePerformancePackages, void* EDX_Unused, int type)
{
    DWORD result = 0; // eax

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    switch (type)
    {
    case 0: // Weight Reduction
        result = CarConfigs[CarTypeID].Icons.PerformanceWeightReduction;
        break;
    case 1: // Transmission
        result = CarConfigs[CarTypeID].Icons.PerformanceTransmission;
        break;
    case 2: // NOS
        result = CarConfigs[CarTypeID].Icons.PerformanceNitrous;
        break;
    case 3: // Engine
        result = CarConfigs[CarTypeID].Icons.PerformanceEngine;
        break;
    case 4: // Induction
        result = UsesSupercharger(CarTypeID)
            ? CarConfigs[CarTypeID].Icons.PerformanceSupercharger
            : CarConfigs[CarTypeID].Icons.PerformanceTurbo;
        break;
    case 5: // Chassis
        result = CarConfigs[CarTypeID].Icons.PerformanceChassis;
        break;
    case 6: // Brakes
        result = CarConfigs[CarTypeID].Icons.PerformanceBrakes;
        break;
    case 7: // ECU
        result = CarConfigs[CarTypeID].Icons.PerformanceECU;
        break;
    case 8: // Tires
        result = CarConfigs[CarTypeID].Icons.PerformanceTires;
        break;
    case 9: // Aerodynamics
        result = CarConfigs[CarTypeID].Icons.PerformanceAerodynamics;
        break;
    }
    return result;
}

DWORD __fastcall ChoosePerformancePackages_GetLangHashForPackageType(DWORD* ChoosePerformancePackages, void* EDX_Unused, int type)
{
    DWORD result = 0; // eax

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    switch (type)
    {
    case 0: // Weight Reduction
        result = CarConfigs[CarTypeID].Names.PerformanceWeightReduction;
        break;
    case 1: // Transmission
        result = CarConfigs[CarTypeID].Names.PerformanceTransmission;
        break;
    case 2: // NOS
        result = CarConfigs[CarTypeID].Names.PerformanceNitrous;
        break;
    case 3: // Engine
        result = CarConfigs[CarTypeID].Names.PerformanceEngine;
        break;
    case 4: // Induction
        result = UsesSupercharger(CarTypeID) 
            ? CarConfigs[CarTypeID].Names.PerformanceSupercharger
            : CarConfigs[CarTypeID].Names.PerformanceTurbo;
        break;
    case 5: // Chassis
        result = CarConfigs[CarTypeID].Names.PerformanceChassis;
        break;
    case 6: // Brakes
        result = CarConfigs[CarTypeID].Names.PerformanceBrakes;
        break;
    case 7: // ECU
        result = CarConfigs[CarTypeID].Names.PerformanceECU;
        break;
    case 8: // Tires
        result = CarConfigs[CarTypeID].Names.PerformanceTires;
        break;
    case 9: // Aerodynamics
        result = CarConfigs[CarTypeID].Names.PerformanceAerodynamics;
        break;
    }
    return result;
}

DWORD __fastcall ChoosePerformancePackages_GetIconForPackageLevel(DWORD* ChoosePerformancePackages, void* EDX_Unused, int type, int level)
{
    DWORD result = 0; // eax

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    switch (type)
    {
    case 0: // Weight Reduction
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceWeightReductionLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceWeightReductionLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceWeightReductionLevel3;
            break;
        }
        break;
    case 1: // Transmission
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceTransmissionLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceTransmissionLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceTransmissionLevel3;
            break;
        }
        break;
    case 2: // NOS
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceNitrousLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceNitrousLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceNitrousLevel3;
            break;
        }
        break;
    case 3: // Engine
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceEngineLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceEngineLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceEngineLevel3;
            break;
        }
        break;
    case 4: // Induction
        switch (level)
        {
        case 1:
            result = UsesSupercharger(CarTypeID)
                ? CarConfigs[CarTypeID].Icons.PerformanceSuperchargerLevel1
                : CarConfigs[CarTypeID].Icons.PerformanceTurboLevel1;
            break;
        case 2:
            result = UsesSupercharger(CarTypeID)
                ? CarConfigs[CarTypeID].Icons.PerformanceSuperchargerLevel2
                : CarConfigs[CarTypeID].Icons.PerformanceTurboLevel2;
            break;
        case 3:
            result = UsesSupercharger(CarTypeID)
                ? CarConfigs[CarTypeID].Icons.PerformanceSuperchargerLevel3
                : CarConfigs[CarTypeID].Icons.PerformanceTurboLevel3;
            break;
        }
        break;
    case 5: // Chassis
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceChassisLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceChassisLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceChassisLevel3;
            break;
        }
        break;
    case 6: // Brakes
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceBrakesLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceBrakesLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceBrakesLevel3;
            break;
        }
        break;
    case 7: // ECU
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceECULevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceECULevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceECULevel3;
            break;
        }
        break;
    case 8: // Tires
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceTiresLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceTiresLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceTiresLevel3;
            break;
        }
        break;
    case 9: // Aerodynamics
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Icons.PerformanceAerodynamicsLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Icons.PerformanceAerodynamicsLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Icons.PerformanceAerodynamicsLevel3;
            break;
        }
        break;
    }
    return result;
}

DWORD __fastcall ChoosePerformancePackages_GetLangHashForPackageLevel(DWORD* ChoosePerformancePackages, void* EDX_Unused, int type, int level)
{
    DWORD result = 0; // eax

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    switch (type)
    {
    case 0: // Weight Reduction
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceWeightReductionLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceWeightReductionLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceWeightReductionLevel3;
            break;
        }
        break;
    case 1: // Transmission
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceTransmissionLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceTransmissionLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceTransmissionLevel3;
            break;
        }
        break;
    case 2: // NOS
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceNitrousLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceNitrousLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceNitrousLevel3;
            break;
        }
        break;
    case 3: // Engine
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceEngineLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceEngineLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceEngineLevel3;
            break;
        }
        break;
    case 4: // Induction
        switch (level)
        {
        case 1:
            result = UsesSupercharger(CarTypeID)
                ? CarConfigs[CarTypeID].Names.PerformanceSuperchargerLevel1
                : CarConfigs[CarTypeID].Names.PerformanceTurboLevel1;
            break;
        case 2:
            result = UsesSupercharger(CarTypeID)
                ? CarConfigs[CarTypeID].Names.PerformanceSuperchargerLevel2
                : CarConfigs[CarTypeID].Names.PerformanceTurboLevel2;
            break;
        case 3:
            result = UsesSupercharger(CarTypeID)
                ? CarConfigs[CarTypeID].Names.PerformanceSuperchargerLevel3
                : CarConfigs[CarTypeID].Names.PerformanceTurboLevel3;
            break;
        }
        break;
    case 5: // Chassis
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceChassisLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceChassisLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceChassisLevel3;
            break;
        }
        break;
    case 6: // Brakes
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceBrakesLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceBrakesLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceBrakesLevel3;
            break;
        }
        break;
    case 7: // ECU
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceECULevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceECULevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceECULevel3;
            break;
        }
        break;
    case 8: // Tires
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceTiresLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceTiresLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceTiresLevel3;
            break;
        }
        break;
    case 9: // Aerodynamics
        switch (level)
        {
        case 1:
            result = CarConfigs[CarTypeID].Names.PerformanceAerodynamicsLevel1;
            break;
        case 2:
            result = CarConfigs[CarTypeID].Names.PerformanceAerodynamicsLevel2;
            break;
        case 3:
            result = CarConfigs[CarTypeID].Names.PerformanceAerodynamicsLevel3;
            break;
        }
        break;
    }
    return result;
}

int GetEndPerfPackageEnum()
{
    if (*(int*)0x863444 && *(int*)0x86344C && *(DWORD*)(*(int*)0x86344C + 56) == 0x7FC8D0D0) return 3;
    return 4;
}

void __fastcall ChoosePerformancePackages_AddIndividualPartsOption(DWORD* ChoosePerformancePackages, void* EDX_Unused, DWORD icon, DWORD text)
{
    DWORD *PPCP = (DWORD*)j__malloc(0x4Cu);

    if (PPCP)
    {
        PPCP[3] = icon;
        PPCP[4] = 0;
        PPCP[5] = 0;
        PPCP[6] = 0;
        *((BYTE*)PPCP + 32) = 0;
        *((BYTE*)PPCP + 33) = 1;
        *((BYTE*)PPCP + 34) = 0;
        *((BYTE*)PPCP + 35) = 0;
        PPCP[11] = text;
        PPCP[12] = 0;
        PPCP[13] = 1065353216;
        PPCP[14] = 1065353216;
        PPCP[15] = 1065353216;
        PPCP[16] = 1065353216;
        *((BYTE*)PPCP + 68) = 1;
        PPCP[18] = 0;
        *PPCP = (DWORD)_PPCustomizePackage_vtbl;
        *((BYTE*)PPCP + 69) = 1;
    }
    else
    {
        PPCP = 0;
    }
    (*(void(__thiscall**)(DWORD*, DWORD*))(*ChoosePerformancePackages + 24))(ChoosePerformancePackages, PPCP);
}

void __fastcall ChoosePerformancePackages_AddPackageOption(DWORD* ChoosePerformancePackages, void* EDX_Unused, DWORD icon, DWORD text, DWORD *package, int upgrade_level)
{
    DWORD* PPCP = (DWORD*)j__malloc(0x4Cu);

    if (PPCP)
    {
        PPCP[3] = icon;
        PPCP[4] = 0;
        PPCP[5] = 0;
        PPCP[6] = 0;
        *((BYTE*)PPCP + 32) = 0;
        *((BYTE*)PPCP + 33) = 1;
        *((BYTE*)PPCP + 34) = 0;
        *((BYTE*)PPCP + 35) = 0;
        PPCP[11] = text;
        PPCP[12] = 0;
        PPCP[13] = 1065353216;
        PPCP[14] = 1065353216;
        PPCP[15] = 1065353216;
        PPCP[16] = 1065353216;
        *((BYTE*)PPCP + 68) = 1;
        PPCP[18] = 1;
        *PPCP = (DWORD)_PPPackage_vtbl;
        PPCP[19] = (DWORD)package;
        *((BYTE*)PPCP + 69) = 1;

        if (package) *((BYTE*)PPCP + 34) = UnlockSystem_IsPerfPackageUnlocked(CarCustomizeManager_GetPartUnlockFilter(), *package, upgrade_level, *(int*)0x8389B0) == 0;
    }
    else
    {
        PPCP = 0;
    }
    (*(void(__thiscall**)(DWORD*, DWORD*))(*ChoosePerformancePackages + 24))(ChoosePerformancePackages, PPCP);
}

void __fastcall ChoosePerformancePackages_AddReturnToStockOption(DWORD* ChoosePerformancePackages, void* EDX_Unused, DWORD icon, DWORD text)
{
    DWORD* PPCP = (DWORD*)j__malloc(0x4Cu);

    if (PPCP)
    {
        PPCP[3] = icon;
        PPCP[4] = 0;
        PPCP[5] = 0;
        PPCP[6] = 0;
        *((BYTE*)PPCP + 32) = 0;
        *((BYTE*)PPCP + 33) = 1;
        *((BYTE*)PPCP + 34) = 0;
        *((BYTE*)PPCP + 35) = 0;
        PPCP[11] = text;
        PPCP[12] = 0;
        PPCP[13] = 1065353216;
        PPCP[14] = 1065353216;
        PPCP[15] = 1065353216;
        PPCP[16] = 1065353216;
        *((BYTE*)PPCP + 68) = 1;
        PPCP[18] = 2;
        *PPCP = (DWORD)_PPReturnToStock_vtbl;
        *((BYTE*)PPCP + 69) = 1;
    }
    else
    {
        PPCP = 0;
    }
    (*(void(__thiscall**)(DWORD*, DWORD*))(*ChoosePerformancePackages + 24))(ChoosePerformancePackages, PPCP);
}

void __fastcall ChoosePerformancePackages_Setup(DWORD* ChoosePerformancePackages, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    //char buf[64];
    //DWORD Icon, Text;

    ChoosePerformancePackages_AddIndividualPartsOption(
        ChoosePerformancePackages,
        EDX_Unused,
        ChoosePerformancePackages_GetIconForPackageType(ChoosePerformancePackages, EDX_Unused, *(int*)_ChoosePerformancePackages_CurrentPackageType), // PERFORMANCE_ICON_CHOOSE_SPECIFIC_PARTS
        CT_bStringHash("PERFORMANCE_CHOOSE_SPECIFIC"));

    bool PacksAvailable = 1;

    if (*(int*)_FEDatabase == 1 && *(int*)0x83898C == 1)
    {
        PacksAvailable = 0;
        *((BYTE*)ChoosePerformancePackages + 354) = 1;
    }
    *(int*)_ChoosePerformancePackages_bBrowsingIndividualParts = 1;

    if (PacksAvailable)
    {
        for (int i = 1; i < GetEndPerfPackageEnum(); i++)
        {
            DWORD* PerfPack = GetPerformancePackage_Type_Level(*(int*)_ChoosePerformancePackages_CurrentPackageType, i);

            //if (PerfPack)
            {
                /*snprintf(buf, sizeof(buf) - 1, "PERFORMANCE_PACKAGE_%d", i);
                Text = bStringHash(buf);
                snprintf(buf, sizeof(buf) - 1, "PERFORMANCE_ICON_PACKAGE_%d", i);
                Icon = bStringHash(buf);
                */
                ChoosePerformancePackages_AddPackageOption(
                    ChoosePerformancePackages,
                    EDX_Unused,
                    ChoosePerformancePackages_GetIconForPackageLevel(ChoosePerformancePackages, EDX_Unused, *(int*)_ChoosePerformancePackages_CurrentPackageType, i),
                    ChoosePerformancePackages_GetLangHashForPackageLevel(ChoosePerformancePackages, EDX_Unused, *(int*)_ChoosePerformancePackages_CurrentPackageType, i),
                    PerfPack,
                    i);
            }
        }
    }

    ChoosePerformancePackages_AddReturnToStockOption(
        ChoosePerformancePackages,
        EDX_Unused,
        CT_bStringHash("GENERIC_STOCK"),
        CT_bStringHash("PERFORMANCE_RETURN_TO_STOCK"));

    *((BYTE*)ChoosePerformancePackages + 352) = 1;
    *((BYTE*)ChoosePerformancePackages + 353) = 1;

    FEngShowPackage(*(char const**)_gCC_BUY_PERF_PARTS, (const char*)ChoosePerformancePackages[1], 0xFF, 0); // UI_BuyPerformanceParts.fng
    (*(void(__thiscall**)(DWORD*, int))(ChoosePerformancePackages[19] + 32))(ChoosePerformancePackages + 19, 1); // IconScroller::SetInitialPos

    DWORD* FEObject = (DWORD*)FEngFindObject((const char*)ChoosePerformancePackages[1], CT_bStringHash("NON_INTERACTIVE_GROUP_2"));
    FEngSetInvisible(FEObject);

    (*(void(__thiscall**)(DWORD*))(*ChoosePerformancePackages + 16))(ChoosePerformancePackages); // ChoosePerformancePackages::RefreshHeader
}