#include "stdio.h"
#include "InGameFunctions.h"
#include "PartLink.h"

void __declspec(naked) BuildRandomRideCodeCave()
{
	_asm
	{
		je LessRandomParts
		cmp eax, 0x2E
		jl MoreRandomParts

		LessRandomParts :
		push 0x6376F1
			retn

			MoreRandomParts :
		push 0x637654
			retn
	}
}

bool IsCustomWidebody(DWORD* part, int slot)
{
    bool result = true;

    if (!part) return 0;

    switch (slot)
    {
    case CARSLOTID_FRONT_BUMPER:
        result = CarPart_GetAppliedAttributeUParam(
            part,
            CT_bStringHash("CUSTOM"),
            CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("CUSTOM_FRONT_BUMPER"), 0));
        break;
    case CARSLOTID_REAR_BUMPER:
        result = CarPart_GetAppliedAttributeUParam(
            part,
            CT_bStringHash("CUSTOM"),
            CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("CUSTOM_REAR_BUMPER"), 0));
        break;
    case CARSLOTID_SKIRT:
        result = CarPart_GetAppliedAttributeUParam(
            part,
            CT_bStringHash("CUSTOM"),
            CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("CUSTOM_SKIRT"), 0));
        break;
    case CARSLOTID_FENDER:
        result = CarPart_GetAppliedAttributeUParam(
            part,
            CT_bStringHash("CUSTOM"),
            CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("CUSTOM_FENDER"), 0));
        break;
    case CARSLOTID_QUARTER:
        result = CarPart_GetAppliedAttributeUParam(
            part,
            CT_bStringHash("CUSTOM"),
            CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("CUSTOM_QUARTER"), 0));
        break;
    }

    return result;
}

DWORD* FindPartWithLevel(int CarType, unsigned int slot_id, int upgrade_level); // defined further down

// The Body Shop only exposes FRONT_BUMPER... only FRONT_BRAKE as a category, and with brakes
// decoupled from the performance package nothing keeps REAR_BRAKE in step any more. Mirror the
// front brake's upgrade level onto the rear, which is what the game's own sync does.
void MirrorFrontBrakeToRear(DWORD* RideInfo)
{
    int CarType = *RideInfo;

    if (CarConfigs[CarType].Main.SyncBrakesWithPhysics) return;
    if (!CarConfigs[CarType].Main.MirrorBrakes) return;

    DWORD* FrontBrake = (DWORD*)RideInfo[356 + CARSLOTID_FRONT_BRAKE];
    if (!FrontBrake) return;

    int Level = *((BYTE*)FrontBrake + 5) >> 5;

    DWORD* RearBrake = (DWORD*)RideInfo[356 + CARSLOTID_REAR_BRAKE];
    if (RearBrake && (*((BYTE*)RearBrake + 5) >> 5) == Level) return;

    DWORD* NewRearBrake = FindPartWithLevel(CarType, CARSLOTID_REAR_BRAKE, Level);
    if (NewRearBrake) RideInfo[356 + CARSLOTID_REAR_BRAKE] = (DWORD)NewRearBrake;
}

bool __fastcall RideInfo_TrunkAudioSlotAvailable(DWORD* RideInfo, void* EDX_Unused, int CarSlotID)
{
    DWORD* TrunkAudioPart;
    bool result; // al
    int CarTypeID; // ecx

    if (CarSlotID < CARSLOTID_TRUNK_AUDIO_COMP_0)
        return 0;
    if (CarSlotID > CARSLOTID_TRUNK_AUDIO_COMP_11)
        return 0;
    TrunkAudioPart = (DWORD*)RideInfo[356 + CARSLOTID_TRUNK_AUDIO]; // TRUNK_AUDIO
    if (!TrunkAudioPart)
        return 0;

	return CarPart_TrunkAudioSlotAvailable(TrunkAudioPart, 0, CarSlotID);

    //int NumberOfSlots = CarPart_GetAppliedAttributeUParam(TrunkAudioPart, CT_bStringHash("NUMSLOTS"), 0);
    //if (NumberOfSlots) return CarSlotID <= CARSLOTID_TRUNK_AUDIO + NumberOfSlots;
    //
    //return RideInfo_TrunkAudioSlotAvailable_Game(RideInfo, CarSlotID);
}

void __fastcall RideInfo_UpdatePartsEnabled(DWORD* RideInfo, void* EDX_Unused)
{
    DWORD* TheCarPart; // eax
    DWORD* j; // ebx
    int CarPartIDName; // ebp
    int k; // edi
    DWORD *HoodPart; // edi
    DWORD *TrunkPart; // edi
    DWORD HoodUnderPartHash; // edx
    DWORD TrunkUnderPartHash; // edx
    DWORD PartHash;
    DWORD ShowEngineThruHood; // edx
    DWORD ShowAudioThruTrunk; // edx
    unsigned __int8 TheCarPart1; // al
    int KitNumber; // ebx
    const char* CarTypeName; // edi
    int KitNamePartialHash; // eax
    DWORD LeftDoorDecalHash; // eax
    DWORD RightDoorDecalHash; // eax
    DWORD LeftQuarterDecalHash; // eax
    DWORD RightQuarterDecalHash; // eax
    int DoorNamePartialHash; // edi
    DWORD LeftDoorHash; // eax
    DWORD RightDoorHash; // eax
    DWORD LeftDoorPanelHash; // eax
    DWORD RightDoorPanelHash; // eax
    DWORD LeftDoorSillHash; // eax
    DWORD RightDoorSillHash; // eax
    int CarType; // edx MAPDST
    int i; // [esp+1Ch] [ebp-84h]
    char KitNameBuf[64];
    char PartNameBuf[128]; // [esp+20h] [ebp-80h] BYREF
    int q;
    int filter = 0;

    DWORD* CarPartIDNames = (DWORD*)_CarPartIDNames;
    DWORD* CarSlotIDNames = (DWORD*)_CarSlotIDNames;

    CarType = *RideInfo;

    MirrorFrontBrakeToRear(RideInfo);

    memset(RideInfo + 526, 1u, 0xA8u);
    *((WORD*)RideInfo + 1136) = 257;
    for (i = CARSLOTID_MODEL_FIRST; i < CARSLOTID_NUM; ++i)
    {
        TheCarPart = (DWORD*)RideInfo[i + 356];
        if (TheCarPart)
        {
            // Hide excluded decal layout parts
			int ExcludeDecalSlot = CarPart_GetExcludeDecal(TheCarPart, EDX_Unused);
			if (ExcludeDecalSlot != -1 && ExcludeDecalSlot != CARSLOTID_PAINT_SPOILER)
				*((BYTE*)RideInfo + 2104 + ExcludeDecalSlot) = 0;
        }

        switch (i)
        {
        case CARSLOTID_HYDRAULICS:
            if (TheCarPart)
                *((BYTE*)RideInfo + 1409) = *((BYTE*)TheCarPart + 5) >> 5;
            break;

        case CARSLOTID_FRONT_WHEEL:
        case CARSLOTID_REAR_WHEEL:
            if (IsBrowsingBrakePaint())
                *((BYTE*)RideInfo + 2104 + i) = 0; // FRONT_WHEEL visibility
            break;

        case CARSLOTID_HOOD:
            HoodPart = (DWORD*)RideInfo[356 + 9];
            if (HoodPart)
            {
                HoodUnderPartHash = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("HOODUNDER"), 0);
                if (HoodUnderPartHash)
                    RideInfo[356 + 25] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 25, HoodUnderPartHash, 0, -1); // HOOD_UNDER
                else
                    RideInfo[356 + 25] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 25, 0, 0, -1);

                // Show engine if our custom attribute is present
                ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("SHOWENGINE"), 0);
                if (ShowEngineThruHood)
                    *((BYTE*)RideInfo + 2104 + 13) = 1; // ENGINE visibility

                // Hood decals
                for (int i = 0; i <= 99; i++) // Find which Hood the car has
                {
                    sprintf(KitNameBuf, "%s_STYLE%02d_", GetCarTypeName(CarType), i);
                    KitNamePartialHash = bStringHash(KitNameBuf);
                    PartHash = bStringHash2("HOOD", KitNamePartialHash);
                    if (*TheCarPart == PartHash)
                    {
                        if (i == 0) // stock
                        {
                            sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                            KitNamePartialHash = bStringHash(KitNameBuf);
                        }

                        // now check for the Hood layout
                        DWORD* HoodDecalPart = (DWORD*)RideInfo[356 + CARSLOTID_DECAL_HOOD];

                        if (HoodDecalPart)
                        {
                            for (int j = 0; j <= 99; j++) // Find which decal layout the car has
                            {
                                if (j == 0) sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                                else sprintf(KitNameBuf, "%s_STYLE%02d_", GetCarTypeName(CarType), j);
                                DWORD DecalNamePartialHash = bStringHash(KitNameBuf);

                                if (*HoodDecalPart == bStringHash2("DECAL_HOOD_RECT_MEDIUM", DecalNamePartialHash)) // Layout 1
                                {
                                    RideInfo[356 + CARSLOTID_DECAL_HOOD] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_HOOD, bStringHash2("DECAL_HOOD_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*HoodDecalPart == bStringHash2("DECAL_HOOD_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    RideInfo[356 + CARSLOTID_DECAL_HOOD] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_HOOD, bStringHash2("DECAL_HOOD_RECT_SMALL", KitNamePartialHash), 0, -1);
                                    break;
                                }
                            }
                        }

                        break;
                    }
                }
            }
            break;

        case CARSLOTID_TRUNK:
            TrunkPart = (DWORD*)RideInfo[356 + 10];
            if (TrunkPart)
            {
                TrunkUnderPartHash = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("TRUNKUNDER"), 0);
                if (TrunkUnderPartHash)
                    RideInfo[356 + 26] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 26, TrunkUnderPartHash, 0, -1); // HOOD_UNDER
                else
                    RideInfo[356 + 26] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 26, 0, 0, -1);

                // Show engine if our custom attribute is present
                ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("SHOWTRUNK"), 0);
                if (ShowAudioThruTrunk)
                    *((BYTE*)RideInfo + 2104 + 34) = 1; // TRUNK_AUDIO visibility

            }
            break;

        case CARSLOTID_ENGINE:
            if (TheCarPart && *(int*)_TheGameFlowManager == 3)
            {
                *((BYTE*)RideInfo + 2104 + 9) = 1; // HOOD visibility
                *((BYTE*)RideInfo + 2104 + 25) = 1; // HOOD_UNDER visibility
                *((BYTE*)RideInfo + 2104 + 52) = 1; // DECAL_HOOD visibility
                *((BYTE*)RideInfo + 2104 + 13) = 1; // ENGINE visibility
            }
            break;

        case CARSLOTID_TOP:
            // CARNAME_(STYLExx_)TOP
            if (!TheCarPart)
            {
                goto InvalidBasePart;
            }

            // Apply decal parts according to the roof style
            sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
            KitNamePartialHash = bStringHash(KitNameBuf);
            PartHash = bStringHash2("TOP", KitNamePartialHash);

            if (*TheCarPart == PartHash)
            {
                // do nothing
            }
            else
            {
                for (q = 0; q <= 99; q++)
                {
                    sprintf(KitNameBuf, "%s_STYLE%02d_", GetCarTypeName(CarType), q);
                    KitNamePartialHash = bStringHash(KitNameBuf);
                    PartHash = bStringHash2("TOP", KitNamePartialHash);
                    if (*TheCarPart == PartHash)
                    {
                        break;
                    }
                    if (q == 99) // Set stock parts if invalid
                    {
                    InvalidBasePart:
                        sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                        KitNamePartialHash = bStringHash(KitNameBuf);
                        break;
                    }
                }
            }

            RideInfo[356 + 53] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 53, bStringHash2("DECAL_FRONT_WINDOW_WIDE_MEDIUM", KitNamePartialHash), 0, -1);
            RideInfo[356 + 54] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 54, bStringHash2("DECAL_REAR_WINDOW_WIDE_MEDIUM", KitNamePartialHash), 0, -1);

            break;

        case CARSLOTID_QUARTER:
            if (TheCarPart)
            {
                for (int i = 0; i <= 99; i++) // Find which quarter the car has
                {
                    sprintf(KitNameBuf, "%s_KIT%02d_", GetCarTypeName(CarType), i);
                    KitNamePartialHash = bStringHash(KitNameBuf);
                    PartHash = bStringHash2("QUARTER", KitNamePartialHash);
                    if (*TheCarPart == PartHash)
                    {
                        if (i == 0) // stock
                        {
                            sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                            KitNamePartialHash = bStringHash(KitNameBuf);
                        }

                        // now check for the quarter layout
                        DWORD* QuarterDecalPartLeft = (DWORD*)RideInfo[356 + 57];
                        DWORD* QuarterDecalPartRight = (DWORD*)RideInfo[356 + 58];

                        // Left
                        if (QuarterDecalPartLeft)
                        {
                            for (int j = 0; j <= 99; j++) // Find which decal layout the car has
                            {
                                if (j == 0) sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                                else sprintf(KitNameBuf, "%s_KIT%02d_", GetCarTypeName(CarType), j);
                                DWORD DecalNamePartialHash = bStringHash(KitNameBuf);

                                if (*QuarterDecalPartLeft == bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", DecalNamePartialHash)) // Layout 1
                                {
                                    RideInfo[356 + 57] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 57, bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*QuarterDecalPartLeft == bStringHash2("DECAL_LEFT_QUARTER_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    RideInfo[356 + 57] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 57, bStringHash2("DECAL_LEFT_QUARTER_RECT_SMALL", KitNamePartialHash), 0, -1);
                                    break;
                                }
                            }
                        }

                        // Right
                        if (QuarterDecalPartRight)
                        {
                            for (int j = 0; j <= 99; j++) // Find which decal layout the car has
                            {
                                if (j == 0) sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                                else sprintf(KitNameBuf, "%s_KIT%02d_", GetCarTypeName(CarType), j);
                                DWORD DecalNamePartialHash = bStringHash(KitNameBuf);

                                if (*QuarterDecalPartRight == bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", DecalNamePartialHash)) // Layout 1
                                {
                                    RideInfo[356 + 58] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 58, bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*QuarterDecalPartRight == bStringHash2("DECAL_RIGHT_QUARTER_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    RideInfo[356 + 58] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 58, bStringHash2("DECAL_RIGHT_QUARTER_RECT_SMALL", KitNamePartialHash), 0, -1);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
            break;
       
        case CARSLOTID_WIDE_BODY:
            if (TheCarPart && (TheCarPart1 = *((BYTE*)TheCarPart + 5), TheCarPart1 >> 5))
            {
                KitNumber = TheCarPart1 & 0x1F;
                CarTypeName = GetCarTypeName(CarType);
                sprintf(PartNameBuf, "%s_WIDE%d_", CarTypeName, KitNumber);
                KitNamePartialHash = bStringHash(PartNameBuf);

                LeftDoorDecalHash = bStringHash2("DECAL_LEFT_DOOR_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + 59] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 59, LeftDoorDecalHash, 0, -1);
                RightDoorDecalHash = bStringHash2("DECAL_RIGHT_DOOR_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + 60] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 60, RightDoorDecalHash, 0, -1);
                LeftQuarterDecalHash = bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + 61] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 61, LeftQuarterDecalHash, 0, -1);
                RightQuarterDecalHash = bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + 62] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 62, RightQuarterDecalHash, 0, -1);

                sprintf(PartNameBuf, "%s_KITW%02d_DOOR_", CarTypeName, KitNumber);
                DoorNamePartialHash = bStringHash(PartNameBuf);
                LeftDoorHash = bStringHash2("LEFT", DoorNamePartialHash);
                RideInfo[356 + 17] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 17, LeftDoorHash, 0, -1);
                RightDoorHash = bStringHash2("RIGHT", DoorNamePartialHash);
                RideInfo[356 + 18] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 18, RightDoorHash, 0, -1);
                LeftDoorPanelHash = bStringHash2("PANEL_LEFT", DoorNamePartialHash);
                RideInfo[356 + 19] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 19, LeftDoorPanelHash, 0, -1);
                RightDoorPanelHash = bStringHash2("PANEL_RIGHT", DoorNamePartialHash);
                RideInfo[356 + 20] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 20, RightDoorPanelHash, 0, -1);
                LeftDoorSillHash = bStringHash2("SILL_LEFT", DoorNamePartialHash);
                RideInfo[356 + 21] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 21, LeftDoorSillHash, 0, -1);
                RightDoorSillHash = bStringHash2("SILL_RIGHT", DoorNamePartialHash);
                RideInfo[356 + 22] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 22, RightDoorSillHash, 0, -1);

                *((BYTE*)RideInfo + 2104 + 1) = 0; // FRONT_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + 2) = 0; // REAR_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + 11) = 0; // SKIRT visibility
                *((BYTE*)RideInfo + 2104 + 5) = 0; // BODY visibility
                *((BYTE*)RideInfo + 2104 + 24) = 0; // QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 23) = 0; // FENDER visibility
                *((BYTE*)RideInfo + 2104 + 6) = 1; // WIDE_BODY visibility
                // make WIDEBODY_DECALs visible, DECALs invisible
                *((BYTE*)RideInfo + 2104 + 55) = 0; // DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 56) = 0; // DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 57) = 0; // DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 58) = 0; // DECAL_RIGHT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 59) = 1; // WIDEBODY_DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 60) = 1; // WIDEBODY_DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 61) = 1; // WIDEBODY_DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 62) = 1; // WIDEBODY_DECAL_RIGHT_QUARTER visibility

                // If widebody allows customization, make bumpers visible and customizable
                //bool IsCustomizableWidebody = 0;

                if (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CUSTOM_FRONT_BUMPER"), 0))
                {
                    *((BYTE*)RideInfo + 2104 + 1) = 1; // FRONT_BUMPER visibility
                    //IsCustomizableWidebody = 1;
                }

                if (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CUSTOM_REAR_BUMPER"), 0))
                {
                    *((BYTE*)RideInfo + 2104 + 2) = 1; // REAR_BUMPER visibility
                    //IsCustomizableWidebody = 1;
                }

                if (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CUSTOM_SKIRT"), 0))
                {
                    *((BYTE*)RideInfo + 2104 + 11) = 1; // SKIRT visibility
                    //IsCustomizableWidebody = 1;
                }

                if (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CUSTOM_QUARTER"), 0))
                {
                    *((BYTE*)RideInfo + 2104 + 24) = 1; // QUARTER visibility
                    //IsCustomizableWidebody = 1;
                }

                if (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CUSTOM_FENDER"), 0))
                {
                    *((BYTE*)RideInfo + 2104 + 23) = 1; // FENDER visibility
                    //IsCustomizableWidebody = 1;
                }

                if (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CUSTOM"), 0))
                {
                    *((BYTE*)RideInfo + 2104 + 1) = 1; // FRONT_BUMPER visibility
                    *((BYTE*)RideInfo + 2104 + 2) = 1; // REAR_BUMPER visibility
                    *((BYTE*)RideInfo + 2104 + 11) = 1; // SKIRT visibility
                    *((BYTE*)RideInfo + 2104 + 24) = 1; // QUARTER visibility
                    *((BYTE*)RideInfo + 2104 + 23) = 1; // FENDER visibility
                    //IsCustomizableWidebody = 1;
                }
            }
            else
            {
                RideInfo[356 + 17] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 17, 0, 0, -1);
                RideInfo[356 + 18] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 18, 0, 0, -1);
                RideInfo[356 + 19] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 19, 0, 0, -1);
                RideInfo[356 + 20] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 20, 0, 0, -1);
                RideInfo[356 + 21] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 21, 0, 0, -1);
                RideInfo[356 + 22] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 22, 0, 0, -1);

                *((BYTE*)RideInfo + 2104 + 1) = 1; // FRONT_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + 2) = 1; // REAR_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + 11) = 1; // SKIRT visibility
                *((BYTE*)RideInfo + 2104 + 5) = 1; // BODY visibility
                *((BYTE*)RideInfo + 2104 + 24) = 1; // QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 23) = 1; // FENDER visibility
                *((BYTE*)RideInfo + 2104 + 6) = 0; // WIDE_BODY visibility
                // make DECALs visible, WIDEBODY_DECALs invisible
                *((BYTE*)RideInfo + 2104 + 55) = 1; // DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 56) = 1; // DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 57) = 1; // DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 58) = 1; // DECAL_RIGHT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 59) = 0; // WIDEBODY_DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 60) = 0; // WIDEBODY_DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + 61) = 0; // WIDEBODY_DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + 62) = 0; // WIDEBODY_DECAL_RIGHT_QUARTER visibility
            }
            break;
        }
    }

    {
        MainSection& M = CarConfigs[CarType].Main;

        if (M.AlwaysShowHoodUnder)  *((BYTE*)RideInfo + 2104 + CARSLOTID_HOOD_UNDER) = 1;
        if (M.AlwaysShowTrunkUnder) *((BYTE*)RideInfo + 2104 + CARSLOTID_TRUNK_UNDER) = 1;

        if (M.AlwaysShowDoorPanels)
        {
            *((BYTE*)RideInfo + 2104 + CARSLOTID_DOOR_PANEL_LEFT) = 1;
            *((BYTE*)RideInfo + 2104 + CARSLOTID_DOOR_PANEL_RIGHT) = 1;
        }

        if (M.AlwaysShowDoorSills)
        {
            *((BYTE*)RideInfo + 2104 + CARSLOTID_DOOR_SILL_LEFT) = 1;
            *((BYTE*)RideInfo + 2104 + CARSLOTID_DOOR_SILL_RIGHT) = 1;
        }

        // Attachment slots 5-10 are these same six. A slot offered as an attachment holds a part
        // the player chose, so it has to stay drawn whatever the hood, trunk or doors are doing;
        // enabling it as an attachment implies the always-show above.
        static const int ExtraAttachmentSlots[6] = {
            CARSLOTID_DOOR_PANEL_LEFT, CARSLOTID_DOOR_PANEL_RIGHT,
            CARSLOTID_DOOR_SILL_LEFT,  CARSLOTID_DOOR_SILL_RIGHT,
            CARSLOTID_HOOD_UNDER,      CARSLOTID_TRUNK_UNDER,
        };

        BodyShopSection& B = CarConfigs[CarType].BodyShop;

        bool ExtraEnabled[6] = {
            B.Attachment5, B.Attachment6, B.Attachment7,
            B.Attachment8, B.Attachment9, B.Attachment10,
        };

        for (int i = 0; i < 6; i++)
            if (B.Attachments > 5 + i && ExtraEnabled[i])
                *((BYTE*)RideInfo + 2104 + ExtraAttachmentSlots[i]) = 1;
    }

    PartLink_Resolve(RideInfo);
    PartLink_ApplyVisibility(RideInfo);
}

void __fastcall RideInfo_SetPart(DWORD* RideInfo, void* EDX_Unused, int CarSlotID, DWORD* CarPartToSet)
{
	if (RideInfo && CarSlotID != CARSLOTID_HOOD_UNDER)
	{
		RideInfo[CarSlotID + 356] = (DWORD)CarPartToSet;
		RideInfo_UpdatePartsEnabled(RideInfo, EDX_Unused);
	}
}

void __fastcall RideInfo_SetPart_Rims(DWORD* RideInfo, void* EDX_Unused, int CarSlotID, DWORD* CarPartToSet)
{
    if (RideInfo && (CarSlotID == CARSLOTID_FRONT_WHEEL || CarSlotID == CARSLOTID_REAR_WHEEL))
    {
        switch (RimsToCustomize)
        {
        case -1:
            RideInfo[CARSLOTID_REAR_WHEEL + 356] = (DWORD)CarPartToSet;
            break;
        case 0:
            RideInfo[CARSLOTID_FRONT_WHEEL + 356] = (DWORD)CarPartToSet;
            RideInfo[CARSLOTID_REAR_WHEEL + 356] = (DWORD)CarPartToSet;
            break;
        case 1:
        default:
            RideInfo[CARSLOTID_FRONT_WHEEL + 356] = (DWORD)CarPartToSet;
            break;
        }

        RideInfo_UpdatePartsEnabled(RideInfo, EDX_Unused);
    }
}

DWORD* FindPartWithLevel(int CarType, unsigned int slot_id, int upgrade_level)
{
    DWORD* result; // eax

    result = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, slot_id, 0, 0, upgrade_level);
    if (!result && upgrade_level > 0) // If not found, check previous level
        return FindPartWithLevel(CarType, slot_id, upgrade_level - 1);
    
    return result;
}

void __fastcall RideInfo_SyncVisualPartsWithPhysics_Hook(DWORD* RideInfo, void* EDX_Unused, bool perf, bool random)
{
    int CarType = *RideInfo;
    MainSection& M = CarConfigs[CarType].Main;

    if (!M.SyncVisualPartsWithPhysics && !M.SyncBrakesWithPhysics) return;

    // Snapshot the brake slots, let the game function run, then put them back if brakes are meant
    // to be chosen by hand. Aerodynamics is not in this list: it writes into RidePhysicsInfo
    // rather than the parts array, so it stays on the master flag.
    DWORD SavedFrontBrake = RideInfo[356 + CARSLOTID_FRONT_BRAKE];
    DWORD SavedRearBrake = RideInfo[356 + CARSLOTID_REAR_BRAKE];

    RideInfo_SyncVisualPartsWithPhysics(RideInfo, perf, random);

    if (!M.SyncBrakesWithPhysics)
    {
        RideInfo[356 + CARSLOTID_FRONT_BRAKE] = SavedFrontBrake;
        RideInfo[356 + CARSLOTID_REAR_BRAKE] = SavedRearBrake;

        MirrorFrontBrakeToRear(RideInfo);
    }
}

/*
DWORD __fastcall RideInfo_GetStockPartNameHash(DWORD* RideInfo, void* EDX_Unused, int CarSlotID)
{
    return CarConfigs[RideInfo[0]].StockParts.CustomStockParts
        ? CarConfigs[RideInfo[0]].StockParts.Parts[CarSlotID]
        : -1;
}



*/

DWORD g_displayHUDprefix = CT_bStringHash("3RDPERSON_");
DWORD* g_displayHUDattributes = (DWORD*)0x839BF0;

void GetRidePartAttributes()
{
    DWORD* part = RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_CUSTOM_HUD);
    if (part)
    {
        g_displayHUDattributes[0] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("HUDINDEX"), 0);
        g_displayHUDprefix = bStringHash2("_", CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("TEXTURE_NAME"), CT_bStringHash("3RDPERSON")));
    }
    
    part = RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_HUD_BACKING_COLOUR);
    if (part)
    {
        g_displayHUDattributes[13] = (DWORD)part;
        g_displayHUDattributes[3] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("RED"), 0);
        g_displayHUDattributes[2] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("GREEN"), 0);
        g_displayHUDattributes[1] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BLUE"), 0);
    }

    part = RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_HUD_NEEDLE_COLOUR);
    if (part)
    {
        g_displayHUDattributes[15] = (DWORD)part;
        g_displayHUDattributes[11] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("RED"), 0);
        g_displayHUDattributes[10] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("GREEN"), 0);
        g_displayHUDattributes[9] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BLUE"), 0);
    }

    part = RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_HUD_CHARACTER_COLOUR);
    if (part)
    {
        g_displayHUDattributes[14] = (DWORD)part;
        g_displayHUDattributes[7] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("RED"), 0);
        g_displayHUDattributes[6] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("GREEN"), 0);
        g_displayHUDattributes[5] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BLUE"), 0);
    }
}