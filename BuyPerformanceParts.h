#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define _BuyPerfPartWidget_vtbl 0x79B620
#define _NoPerfPartsWidgy_vtbl 0x79B660

void __fastcall BuyPerformanceParts_Setup(DWORD* BuyPerformanceParts, void* EDX_Unused)
{
    
    int v4; // eax
    DWORD* v5; // ecx
    DWORD* v6; // eax
    int v7; // esi
    DWORD* v8; // ebx
    int PartUnlockFilter; // eax
    DWORD* AStatWidget; // esi MAPDST
    DWORD v12; // [esp-8h] [ebp-20h]
    int v13; // [esp-4h] [ebp-1Ch]
    int v14; // [esp+Ch] [ebp-Ch]
    signed int v16; // [esp+14h] [ebp-4h]

    for (int i = 1; i < GetEndPerfPackageEnum(); i++)
    {
        v4 = 0;
        if (*(int*)_NumPerfPackages <= 0)
        {
        LABEL_12:
            v6 = 0;
        }
        else
        {
            v5 = (DWORD*)*(DWORD*)_PerformancePackageList;
            while (*v5 != *(int*)0x838994 || v5[1] != i)
            {
                ++v4;
                v5 += 95;
                if (v4 >= *(int*)_NumPerfPackages)
                    goto LABEL_12;
            }
            v6 = (DWORD*)(*(DWORD*)_PerformancePackageList + 380 * v4);
        }
        v7 = 0;
        v16 = v6[2];
        v14 = 0;
        v8 = v6 + 3;
        do
        {
            if (v7 >= v16)
                break;
            v13 = *(int*)0x8389B0;
            v12 = *v8;
            PartUnlockFilter = CarCustomizeManager_GetPartUnlockFilter();
            if (UnlockSystem_IsPerfPartUnlocked(PartUnlockFilter, v12, v13))
            {
                AStatWidget = (DWORD*)j__malloc(0x50u);
                if (AStatWidget)
                {
                    FEStatWidget_FEStatWidget(
                        AStatWidget,
                        (float*)BuyPerformanceParts + 63,
                        (float*)BuyPerformanceParts + 67,
                        (float*)BuyPerformanceParts + 69,
                        (float*)BuyPerformanceParts + 71,
                        1);
                    *AStatWidget = (DWORD)_BuyPerfPartWidget_vtbl;
                    AStatWidget[19] = (DWORD)v8;
                    AStatWidget[7] = 0xC3878000;
                    AStatWidget[8] = 0xC1200000;
                }
                else
                {
                    AStatWidget = 0;
                }
                UIWidgetMenu_AddStatOption(BuyPerformanceParts, AStatWidget);
                v7 = v14;
            }
            ++v7;
            v8 += 23;
            v14 = v7;
        } while (v7 < 4);
    }
    if ((DWORD*)BuyPerformanceParts[19] == BuyPerformanceParts + 19)
    {
        *((BYTE*)BuyPerformanceParts + 485) = 1;
        AStatWidget = (DWORD*)j__malloc(0x4Cu);
        if (AStatWidget)
        {
            FEStatWidget_FEStatWidget(
                AStatWidget,
                (float*)BuyPerformanceParts + 63,
                (float*)BuyPerformanceParts + 67,
                (float*)BuyPerformanceParts + 69,
                (float*)BuyPerformanceParts + 71,
                1);
            *AStatWidget = (DWORD)_NoPerfPartsWidgy_vtbl;
                AStatWidget[7] = 0xC3878000;
            AStatWidget[8] = 0xC1200000;
        }
        else
        {
            AStatWidget = 0;
        }
        UIWidgetMenu_AddStatOption(BuyPerformanceParts, AStatWidget);
    }
    UIWidgetMenu_SetInitialOption(BuyPerformanceParts, 0);
    BuyPerformanceParts_RefreshHeader(BuyPerformanceParts);
}