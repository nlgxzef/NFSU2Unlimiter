#include "stdio.h"
#include "InGameFunctions.h"

bool __fastcall CarCustomizeManager_IsEnginePaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
	DWORD* Part = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_ENGINE);

    if (!CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("UNPAINTABLE"), 1)) return true;

    return (unsigned __int8)(*((BYTE*)Part + 5) >> 5) > 2u;
}

bool __fastcall CarCustomizeManager_IsTrunkPaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
    DWORD* Part = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_TRUNK_AUDIO);

    if (!CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("UNPAINTABLE"), 1)) return true;

    return (unsigned __int8)(*((BYTE*)Part + 5) >> 5) > 2u;
}

bool __fastcall CarCustomizeManager_AreBrakesPaintable(DWORD* CarCustomizeManager, void* EDX_Unused)
{
    DWORD* FPart = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_FRONT_BRAKE);
    DWORD* RPart = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_REAR_BRAKE);

    if (!CarPart_GetAppliedAttributeUParam(FPart, CT_bStringHash("UNPAINTABLE"), 1)
        || !CarPart_GetAppliedAttributeUParam(RPart, CT_bStringHash("UNPAINTABLE"), 1)) return true;

    return (unsigned __int8)(*((BYTE*)FPart + 5) >> 5) > 2u // FRONT_BRAKE
        || (unsigned __int8)(*((BYTE*)RPart + 5) >> 5) > 2u; // REAR_BRAKE
}

DWORD* __fastcall CarCustomizeManager_GetLayoutPart(DWORD* CarCustomizeManager, void* EDX_Unused, int CarSlotID, int LayoutID)
{
    DWORD* WideBodyPart; // eax
    DWORD* QuarterPart; // eax
    DWORD* HoodPart; // eax
    int CarTypeID; // edi
    DWORD* TheCarPart; // eax
    int CarPartID; // [esp+14h] [ebp+8h]
    char NameBuf[64];
    DWORD NamePartialHash;
    DWORD PartHash;

    if (CarSlotID >= CARSLOTID_DECAL_HOOD && CarSlotID <= CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER)
    {
        CarPartID = GetCarPartFromSlot(CarSlotID);
        DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
        CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

        WideBodyPart = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_WIDE_BODY);
        QuarterPart = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_QUARTER);
        HoodPart = RideInfo_GetPart(CarCustomizeManager + 592, CARSLOTID_HOOD);

        if (WideBodyPart)
        {
            if (*((BYTE*)WideBodyPart + 5) >> 5)
                CarSlotID = CarCustomizeManager_TranslateToWidebodyLayoutSlotIfNecessary(CarSlotID);
        }
        if (QuarterPart && (CarSlotID == CARSLOTID_DECAL_LEFT_QUARTER || CarSlotID == CARSLOTID_DECAL_RIGHT_QUARTER))
        {
            for (int i = 0; i <= 99; i++)
            {
                if (i == 0) sprintf(NameBuf, "%s_", GetCarTypeName(CarTypeID));
                else sprintf(NameBuf, "%s_KIT%02d_", GetCarTypeName(CarTypeID), i);

                NamePartialHash = bStringHash(NameBuf);
                PartHash = bStringHash2("QUARTER", NamePartialHash);
                
                if (*(unsigned int*)QuarterPart == PartHash)
                {
                    switch (CarSlotID)
                    {
                    case CARSLOTID_DECAL_LEFT_QUARTER: // DECAL_LEFT_QUARTER
                        return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CARSLOTID_DECAL_LEFT_QUARTER, bStringHash2(LayoutID == 1 ? "DECAL_LEFT_QUARTER_RECT_MEDIUM" : "DECAL_LEFT_QUARTER_RECT_SMALL", NamePartialHash), 0, -1);
                    case CARSLOTID_DECAL_RIGHT_QUARTER: // DECAL_RIGHT_QUARTER
                        return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CARSLOTID_DECAL_RIGHT_QUARTER, bStringHash2(LayoutID == 1 ? "DECAL_RIGHT_QUARTER_RECT_MEDIUM" : "DECAL_RIGHT_QUARTER_RECT_SMALL", NamePartialHash), 0, -1);
                    }
                }
            }
        }
        if (HoodPart && CarSlotID == CARSLOTID_DECAL_HOOD)
        {
            for (int i = 0; i <= 99; i++)
            {
                if (i == 0) sprintf(NameBuf, "%s_", GetCarTypeName(CarTypeID));
                else sprintf(NameBuf, "%s_STYLE%02d_", GetCarTypeName(CarTypeID), i);
                NamePartialHash = bStringHash(NameBuf);
                PartHash = bStringHash2("HOOD", NamePartialHash);

                if (*(unsigned int*)HoodPart == PartHash)
                {
                    return CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CARSLOTID_DECAL_HOOD, bStringHash2(LayoutID == 1 ? "DECAL_HOOD_RECT_MEDIUM" : "DECAL_HOOD_RECT_SMALL", NamePartialHash), 0, -1);
                }
            }
        }
        
        TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, CarTypeID, CarSlotID, 0, -1);
        for (int i = 1; TheCarPart; TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1))
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

DWORD* __fastcall CarCustomizeManager_GetLayoutInSetup(DWORD* CarCustomizeManager, void *EDX_Unused, int slot)
{
    DWORD* LastSelPartInSetup; // edi
    DWORD* SelPartInSetup; // esi
    int SelPartSlot; // eax

    if (slot < CARSLOTID_DECAL_HOOD)
        return 0;
    if (slot > CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER)
        return 0;
    LastSelPartInSetup = CarCustomizeManager + 18;
    if ((DWORD*)CarCustomizeManager[18] == CarCustomizeManager + 18)
        return 0;
    SelPartInSetup = (DWORD*)CarCustomizeManager[18];
    while (1)
    {
        if (SelPartInSetup[3] == slot)
            break;
        SelPartInSetup = (DWORD*)*SelPartInSetup;
        if (SelPartInSetup == LastSelPartInSetup)
            return 0;
    }
    return (DWORD*)SelPartInSetup[2];
}