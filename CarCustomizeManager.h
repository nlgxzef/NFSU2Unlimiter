#include "stdio.h"
#include "InGameFunctions.h"


bool __fastcall CarCustomizeManager_IsTrunkPaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
    return (unsigned __int8)(*((BYTE*)CarCustomizeManager_GetInstalledPart(CarCustomizeManager + 592, 34) + 5) >> 5) > 2u; // TRUNK_AUDIO
}

bool __fastcall CarCustomizeManager_IsTrunkAudioSlotAvailable(DWORD* CarCustomizeManager, void* EDX_Unused, int CarSlotID)
{
	DWORD* TrunkAudioPart;
    bool result; // al
    int CarTypeID; // ecx

    if (CarSlotID < 35)
        return 0;
    if (CarSlotID > 46)
        return 0;
	TrunkAudioPart = (DWORD*)CarCustomizeManager[356 + 34]; // TRUNK_AUDIO
    if (!TrunkAudioPart)
        return 0;

    int NumberOfSlots = CarPart_GetAppliedAttributeUParam(TrunkAudioPart, bStringHash("NUMSLOTS"), 0);
    if (NumberOfSlots) return CarSlotID <= 34 + NumberOfSlots;
    
    return CarCustomizeManager_IsTrunkAudioSlotAvailable_Game(CarCustomizeManager, CarSlotID);
}

DWORD* __fastcall CarCustomizeManager_GetLayoutPart(DWORD* CarCustomizeManager, void* EDX_Unused, int CarSlotID, int LayoutID)
{
    DWORD* WideBodyPart; // eax
    DWORD* QuarterPart; // eax
    int TuningCarType; // edi
    DWORD* TheCarPart; // eax
    int i; // ebx
    int CarPartID; // [esp+14h] [ebp+8h]
    char QuarterNameBuf[64];
    DWORD QuarterNamePartialHash;
    DWORD PartHash;

    if (CarSlotID >= 52 && CarSlotID <= 62)
    {
        CarPartID = GetCarPartFromSlot(CarSlotID);
        DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
        TuningCarType = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

        WideBodyPart = CarCustomizeManager_GetInstalledPart(CarCustomizeManager + 592, 6);
        QuarterPart = CarCustomizeManager_GetInstalledPart(CarCustomizeManager + 592, 24);

        if (WideBodyPart)
        {
            if (*((BYTE*)WideBodyPart + 5) >> 5)
                CarSlotID = CarCustomizeManager_TranslateToWidebodyLayoutSlotIfNecessary(CarSlotID);
        }
        if (QuarterPart)
        {
            for (int i = 1; i <= 99; i++)
            {
                sprintf(QuarterNameBuf, "%s_KIT%02d_", GetCarTypeName(TuningCarType), i);
                QuarterNamePartialHash = bStringHash(QuarterNameBuf);
                PartHash = bStringHash2("QUARTER", QuarterNamePartialHash);
                if (*(unsigned int*)QuarterPart == PartHash)
                {
                    switch (CarSlotID)
                    {
                    case 57: // DECAL_LEFT_QUARTER
                        return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, TuningCarType, 57, bStringHash2(LayoutID == 1 ? "DECAL_LEFT_QUARTER_RECT_MEDIUM" : "DECAL_LEFT_QUARTER_RECT_SMALL", QuarterNamePartialHash), 0, -1);
                    case 58: // DECAL_RIGHT_QUARTER
                        return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, TuningCarType, 58, bStringHash2(LayoutID == 1 ? "DECAL_RIGHT_QUARTER_RECT_MEDIUM" : "DECAL_RIGHT_QUARTER_RECT_SMALL", QuarterNamePartialHash), 0, -1);
                    }
                }
            }
        }
        
        TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, TuningCarType, CarSlotID, 0, -1);
        for (i = 1; TheCarPart; TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, TuningCarType, CarSlotID, 0, -1))
        {
            if (*((char*)TheCarPart + 4) == CarPartID)
            {
                if (i == LayoutID)
                    return TheCarPart;
                ++i;
            }
        }
    }
    return 0;
}