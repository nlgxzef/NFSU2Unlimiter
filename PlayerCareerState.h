#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define UnlockablePartsDataList *(int*)0x83845C

int __fastcall PlayerCareerState_GetCarPartCost(DWORD *PlayerCareerState, void *EDX_Unused, int slot, DWORD* part)
{
    int unlockable; // eax
    char UpgradeLevel; // cl
    int v5; // edx
    int v6; // eax
    int result = 0; // eax

    unlockable = MapCarPartToUnlockable(slot, part);
    UpgradeLevel = *((BYTE*)part + 5) >> 5;

    v5 = UpgradeLevel;
    if (UpgradeLevel == 4)
        goto ZeroCost;
    if (UpgradeLevel)
    {
        if (unlockable <= 0 || unlockable >= 87)
            goto ZeroCost;
    }
    else
    {
        if (unlockable != 76)
            goto ZeroCost;
        v5 = 1;
    }
    v6 = 5 * unlockable;
    if (UnlockablePartsDataList + 8 * v6 == 40)
        goto ZeroCost;
    result = *((__int16*)UnlockablePartData_GetLevel((BYTE*)(UnlockablePartsDataList + 8 * v6 - 40), v5) + 1);
    if (result == -1)
        goto ZeroCost;
    goto GetCost;

ZeroCost:
    result = 0;
    
GetCost:
	return CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("COST"), result);
}

int __fastcall PlayerCareerState_GetCarPartRep(DWORD* PlayerCareerState, void* EDX_Unused, int type, int slot, DWORD* part)
{
    int unlockable; // eax
    int result; // eax

    unlockable = MapCarPartToUnlockable(slot, part);
    result = GetUnlockablePartRep(unlockable, *((BYTE*)part + 5) >> 5);
    if (slot == CAR_SLOT_ID::BASE_PAINT && *(DWORD*)part == *(DWORD*)(0x890 * type + *(DWORD*)_CarTypeInfoArray + 0x84C) || result == -1)
		return 0; // stock paint
    return CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("REP"), result);
}