#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall ChoosePerfCategory_Setup(DWORD* ChoosePerfCategory, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    if (CarConfigs[CarTypeID].Performance.Engine)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        3, 
        CarConfigs[CarTypeID].Icons.PerformanceEngine, // PERFORMANCE_ENGINE
        CarConfigs[CarTypeID].Names.PerformanceEngine);// PERF_CATEGORY_ENGINE

    if (CarConfigs[CarTypeID].Performance.ECU)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        7, 
        CarConfigs[CarTypeID].Icons.PerformanceECU, // PERFORMANCE_ECU
        CarConfigs[CarTypeID].Names.PerformanceECU);// PERF_CATEGORY_ECU

    if (CarConfigs[CarTypeID].Performance.Transmission)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        1, 
        CarConfigs[CarTypeID].Icons.PerformanceTransmission, // PERFORMANCE_TRANSMISSION
        CarConfigs[CarTypeID].Names.PerformanceTransmission);// PERF_CATEGORY_TRANSMISSION

    if (CarConfigs[CarTypeID].Performance.Chassis)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        5, 
        CarConfigs[CarTypeID].Icons.PerformanceChassis,// PERFORMANCE_SUSPENSION
        CarConfigs[CarTypeID].Names.PerformanceChassis);// PERF_CATEGORY_SUSPENSION

    if (CarConfigs[CarTypeID].Performance.Nitrous)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        2, 
        CarConfigs[CarTypeID].Icons.PerformanceNitrous, // PERFORMANCE_NITROUS
        CarConfigs[CarTypeID].Names.PerformanceNitrous);// PERF_CATEGORY_NOS

    if (CarConfigs[CarTypeID].Performance.Tires)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        8, 
        CarConfigs[CarTypeID].Icons.PerformanceTires, // PERFORMANCE_TIRES
        CarConfigs[CarTypeID].Names.PerformanceTires);// PERF_CATEGORY_TIRES

    if (CarConfigs[CarTypeID].Performance.Brakes)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        6, 
        CarConfigs[CarTypeID].Icons.PerformanceBrakes, // PERFORMANCE_BRAKES
        CarConfigs[CarTypeID].Names.PerformanceBrakes);// PERF_CATEGORY_BRAKE

    if (CarConfigs[CarTypeID].Performance.WeightReduction)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        0, 
        CarConfigs[CarTypeID].Icons.PerformanceWeightReduction, // PERFORMANCE_WEIGHT_REDUCTION
        CarConfigs[CarTypeID].Names.PerformanceWeightReduction);// PERF_CATEGORY_WEIGHT_REDUCTION

    if (CarConfigs[CarTypeID].Performance.Induction)
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory, 
        4, 
        UsesSupercharger(CarTypeID) ? CarConfigs[CarTypeID].Icons.PerformanceSupercharger : CarConfigs[CarTypeID].Icons.PerformanceTurbo, // PERFORMANCE_SUPERCHARGER / PERFORMANCE_TURBO
        UsesSupercharger(CarTypeID) ? CarConfigs[CarTypeID].Names.PerformanceSupercharger : CarConfigs[CarTypeID].Names.PerformanceTurbo); // PERF_CATEGORY_SUPERCHARGER / PERF_CATEGORY_TURBO

    if (CarConfigs[CarTypeID].Performance.Aerodynamics) // Aerodynamics
    ChoosePerfCategory_AddCategory(
        ChoosePerfCategory,
        9,
        CarConfigs[CarTypeID].Icons.PerformanceAerodynamics, // PERFORMANCE_AERODYNAMICS
        CarConfigs[CarTypeID].Names.PerformanceAerodynamics);// PERF_CATEGORY_AERODYNAMICS

    if ((*(int*)_FEDatabase == 1 && !*(int*)0x83898C) && CarConfigs[CarTypeID].Performance.TestDyno)
    {
        DWORD* PMST = (DWORD*)j__malloc(0x48u);
        if (PMST)
            PerfMenuStartTuning_IconOption(
                PMST, 
                CarConfigs[CarTypeID].Icons.PerformanceTestDyno, // CRIB_ICON_TEST_TRACK
                CarConfigs[CarTypeID].Names.PerformanceTestDyno, // UI_CAREER_LOAD_TEST_TRACK
                0);
        else
            PMST = 0;
        (*(void(__thiscall**)(DWORD*, DWORD*))(*ChoosePerfCategory + 24))(ChoosePerfCategory, PMST); // IconScrollerMenu::AddOption
    }

    unsigned int LastButton = FEngGetLastButton((char const*)ChoosePerfCategory[1]);

    (*(void(__thiscall**)(DWORD*, DWORD))(ChoosePerfCategory[19] + 32))(ChoosePerfCategory + 19, LastButton); // IconScroller::SetInitialPos
    (*(int(__thiscall**)(DWORD*))(*ChoosePerfCategory + 16))(ChoosePerfCategory); // IconScrollerMenu::RefreshHeader
}