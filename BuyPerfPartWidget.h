#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall BuyPerfPartWidget_Draw(DWORD* BuyPerfPartWidget, DWORD* EDX_Unused)
{
    PerfPart* ThePart = (PerfPart*)BuyPerfPartWidget[19];
    
    // Part name
    FEngSetLanguageHash_obj((DWORD*)BuyPerfPartWidget[11], GetPerfPartNameHash(ThePart));
    //FEngSetLanguageHash_obj((DWORD*)BuyPerfPartWidget[11], *(DWORD*)(BuyPerfPartWidget[19] + 4));

    if (PhysicsUpgradeSpecification_IsPerfPartInstalled((BYTE*)0x8395B0, *(DWORD*)BuyPerfPartWidget[19]))
    {
        FEngSetLanguageHash_obj((DWORD*)BuyPerfPartWidget[12], CT_bStringHash("INSTALLED"));
        FEngSetScript_Obj((DWORD*)BuyPerfPartWidget[12], "Purchased", 1);
        return;
    }
    if (*(int*)_FEDatabase == 1)
    {
        if (CarCustomizeManager_IsPerfPartPurchased((DWORD*)gCarCustomizeManager, BuyPerfPartWidget[19]))
        {
            FEngSetLanguageHash_obj((DWORD*)BuyPerfPartWidget[12], CT_bStringHash("OWNED"));
            FEngSetScript_Obj((DWORD*)BuyPerfPartWidget[12], "Purchased", 1);
            return;
        }
        FEngSetScript_Obj((DWORD*)BuyPerfPartWidget[12], "Unhighlight", 1);
        FEngPrintf((DWORD*)BuyPerfPartWidget[12], "%$d", *(DWORD*)(BuyPerfPartWidget[19] + 8)); // Cost
    }
    else
    {
        FEngSetLanguageHash_obj((DWORD*)BuyPerfPartWidget[12], CT_bStringHash("NOT_INSTALLED"));
        FEngSetScript_Obj((DWORD*)BuyPerfPartWidget[12], "Unhighlight", 1);
    }
}