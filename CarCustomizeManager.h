#include "stdio.h"
#include "InGameFunctions.h"

bool __fastcall CarCustomizeManager_IsEnginePaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
	DWORD* Part = RideInfo_GetPart(CarCustomizeManager + 592, CAR_SLOT_ID::ENGINE);

    if (!CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("UNPAINTABLE"), 1)) return true;

    return (unsigned __int8)(*((BYTE*)Part + 5) >> 5) > 2u;
}

bool __fastcall CarCustomizeManager_IsTrunkPaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
    DWORD* Part = RideInfo_GetPart(CarCustomizeManager + 592, CAR_SLOT_ID::TRUNK_AUDIO);

    if (!CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("UNPAINTABLE"), 1)) return true;

    return (unsigned __int8)(*((BYTE*)Part + 5) >> 5) > 2u;
}

bool __fastcall CarCustomizeManager_AreBrakesPaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
    DWORD* FPart = RideInfo_GetPart(CarCustomizeManager + 592, CAR_SLOT_ID::FRONT_BRAKE);
    DWORD* RPart = RideInfo_GetPart(CarCustomizeManager + 592, CAR_SLOT_ID::REAR_BRAKE);

    if (!CarPart_GetAppliedAttributeUParam(FPart, CT_bStringHash("UNPAINTABLE"), 1)
        || !CarPart_GetAppliedAttributeUParam(RPart, CT_bStringHash("UNPAINTABLE"), 1)) return true;

    return (unsigned __int8)(*((BYTE*)FPart + 5) >> 5) > 2u // FRONT_BRAKE
        || (unsigned __int8)(*((BYTE*)RPart + 5) >> 5) > 2u; // REAR_BRAKE
}

DWORD* __fastcall CarCustomizeManager_GetLayoutPart(DWORD* CarCustomizeManager, void* EDX_Unused, int CarSlotID, int LayoutID)
{
    DWORD* WideBodyPart; // eax
    DWORD* QuarterPart; // eax
    int CarTypeID; // edi
    DWORD* TheCarPart; // eax
    int i; // ebx
    int CarPartID; // [esp+14h] [ebp+8h]
    char QuarterNameBuf[64];
    DWORD QuarterNamePartialHash;
    DWORD PartHash;

    if (CarSlotID >= CAR_SLOT_ID::DECAL_HOOD && CarSlotID <= CAR_SLOT_ID::WIDEBODY_DECAL_RIGHT_QUARTER)
    {
        CarPartID = GetCarPartFromSlot(CarSlotID);
        DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
        CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

        WideBodyPart = RideInfo_GetPart(CarCustomizeManager + 592, CAR_SLOT_ID::WIDE_BODY);
        QuarterPart = RideInfo_GetPart(CarCustomizeManager + 592, CAR_SLOT_ID::QUARTER);

        if (WideBodyPart)
        {
            if (*((BYTE*)WideBodyPart + 5) >> 5)
                CarSlotID = CarCustomizeManager_TranslateToWidebodyLayoutSlotIfNecessary(CarSlotID);
        }
        if (QuarterPart)
        {
            for (int i = 1; i <= 99; i++)
            {
                sprintf(QuarterNameBuf, "%s_KIT%02d_", GetCarTypeName(CarTypeID), i);
                QuarterNamePartialHash = bStringHash(QuarterNameBuf);
                PartHash = bStringHash2("QUARTER", QuarterNamePartialHash);
                if (*(unsigned int*)QuarterPart == PartHash)
                {
                    switch (CarSlotID)
                    {
                    case CAR_SLOT_ID::DECAL_LEFT_QUARTER: // DECAL_LEFT_QUARTER
                        return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CAR_SLOT_ID::DECAL_LEFT_QUARTER, bStringHash2(LayoutID == 1 ? "DECAL_LEFT_QUARTER_RECT_MEDIUM" : "DECAL_LEFT_QUARTER_RECT_SMALL", QuarterNamePartialHash), 0, -1);
                    case CAR_SLOT_ID::DECAL_RIGHT_QUARTER: // DECAL_RIGHT_QUARTER
                        return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CAR_SLOT_ID::DECAL_RIGHT_QUARTER, bStringHash2(LayoutID == 1 ? "DECAL_RIGHT_QUARTER_RECT_MEDIUM" : "DECAL_RIGHT_QUARTER_RECT_SMALL", QuarterNamePartialHash), 0, -1);
                    }
                }
            }
        }
        
        TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, CarTypeID, CarSlotID, 0, -1);
        for (i = 1; TheCarPart; TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1))
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