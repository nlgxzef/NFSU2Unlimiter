#include "stdio.h"
#include "InGameFunctions.h"
#include "ChooseRimBrand.h"
#include "includes\IniReader.h"

#define _BrandNameHash 0x839BBC

void __fastcall RimsBrowser_RefreshHeader(DWORD* RimsBrowser, void* EDX_Unused)
{
    int CurrentNodeIndex; // eax
    DWORD* TheCarPart; // edi MAPDST
    const char* PartName; // ebx MAPDST
    unsigned int PartNameLanguageHash; // eax
    const char* PartNameFromHash; // eax
    const char* BrandName; // eax
    BOOL HasLanguageHash; // eax
    const char* RimsBrowserPackage; // ebx MAPDST
    int Cost; // eax
    int InnerRadius; // [esp-4h] [ebp-10h]

    CurrentNodeIndex = ThumbnailScroller_GetCurrenNodeIndex(RimsBrowser + 84);
    FEScrollBar_Update((DWORD*)RimsBrowser + 109, 1, RimsBrowser[132], CurrentNodeIndex);
    TheCarPart = *(DWORD**)(*(DWORD*)(RimsBrowser[22] + 76) + 8);
    PartName = (const char*)CarPart_GetName(TheCarPart);

    if (CarPart_HasAppliedAttribute(TheCarPart, 0x4732DA07)) // LANGUAGEHASH
    {
        PartNameLanguageHash = CarPart_GetAppliedAttributeUParam(TheCarPart, 0x4732DA07, 0); // LANGUAGEHASH
        PartNameFromHash = GetLocalizedString(PartNameLanguageHash);
        PartName = PartNameFromHash;
    }

    DWORD BrandNameHash = *(DWORD*)_BrandNameHash;
    if (IsNoBrandName(BrandNameHash))
    {
        FEngPrintf((DWORD*)RimsBrowser[77], "%s", PartName);
    }
    else
    {
        BrandName = CarCustomizeManager_GetBrandNameFromHash((DWORD*)gCarCustomizeManager_0, BrandNameHash);
        FEngPrintf((DWORD*)RimsBrowser[77], "%s %s", BrandName, PartName);
    }

    if ((DWORD*)RimsBrowser[85] != RimsBrowser + 85)
    {
        TheCarPart = *(DWORD**)(RimsBrowser[87] + 44);
        HasLanguageHash = CarPart_HasAppliedAttribute(TheCarPart, 0x4732DA07);// LANGUAGEHASH
        RimsBrowserPackage = (const char*)RimsBrowser[1];

        if (HasLanguageHash)
        {
            InnerRadius = (char)CarPart_GetAppliedAttributeUParam(TheCarPart, 0xEB0101E2, 0);// INNER_RADIUS
            PartName = (const char*)CarPart_GetAppliedAttributeUParam(TheCarPart, 0x4732DA07, 0);// LANGUAGEHASH
        }
        else
        {
            InnerRadius = (char)CarPart_GetAppliedAttributeUParam(TheCarPart, 0xEB0101E2, 0);// INNER_RADIUS
            PartName = (const char*)CarPart_GetName(TheCarPart);
        }

        FEPrintf(RimsBrowserPackage, 0xD28B9316, "%s %d\"", PartName, InnerRadius);

        if (*(int*)_FEDatabase == 1)
        {
            Cost = PlayerCareerState_GetCarPartCost((DWORD*)ThePlayerCareer, 29, TheCarPart);
            FEPrintf(RimsBrowserPackage, 0x380B632, "%d", Cost);
        }
        else
        {
            FEngSetInvisible_Pkg(RimsBrowserPackage, 0x380B632);
        }
    }
}