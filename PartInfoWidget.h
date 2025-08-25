#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall PartInfoWidget_Draw(DWORD* PartInfoWidget, DWORD* EDX_Unused)
{
    PerfPart *ThePart = (PerfPart*)PartInfoWidget[19];
    
    // Part name
    FEngSetLanguageHash_obj((DWORD*)PartInfoWidget[11], GetPerfPartNameHash(ThePart));
    //FEngSetLanguageHash_obj(PartInfoWidget[11], *(DWORD *)(PartInfoWidget[19] + 4));

    if (PhysicsUpgradeSpecification_IsPerfPartInstalled((BYTE*)0x8395B0, *(DWORD*)PartInfoWidget[19]))
    {
        FEngSetLanguageHash_obj((DWORD*)PartInfoWidget[12], CT_bStringHash("INSTALLED"));
        FEngSetScript_Obj((DWORD*)PartInfoWidget[12], "Purchased", 1);
        return;
    }
    if (*(int*)_FEDatabase == 1)
    {
        if (CarCustomizeManager_IsPerfPartPurchased((DWORD*)gCarCustomizeManager, PartInfoWidget[19]))
        {
            FEngSetLanguageHash_obj((DWORD*)PartInfoWidget[12], CT_bStringHash("OWNED"));
            FEngSetScript_Obj((DWORD*)PartInfoWidget[12], "Purchased", 1);
            return;
        }
        FEngSetScript_Obj((DWORD*)PartInfoWidget[12], "Init", 1);
        FEngPrintf((DWORD*)PartInfoWidget[12], "%$d", *(DWORD*)(PartInfoWidget[19] + 8)); // Cost
    }
    else
    {
        FEngSetLanguageHash_obj((DWORD*)PartInfoWidget[12], CT_bStringHash("NOT_INSTALLED"));
        FEngSetScript_Obj((DWORD*)PartInfoWidget[12], "Init", 1);
    }
}