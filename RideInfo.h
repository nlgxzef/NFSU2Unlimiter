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

// A slot holds a CarPart pointer, but on a RideInfo the game has not finished filling in it can
// hold whatever was there before. A cop car preview handed this a slot containing 0x40800000,
// which is the float 4.0, and reading byte 5 of it faulted at 0x40800005.
// A slot offered as an attachment holds a part the player picked, so the code that fills it from
// somewhere else has to leave it alone. HOOD_UNDER and TRUNK_UNDER are written from the hood's and
// trunk's HOODUNDER attribute, and the four door slots from the widebody block, on every rebuild.
// That is why the parts listed and could be chosen but the car never changed: the choice was
// overwritten a moment later.
bool IsAttachmentDrivenSlot(int CarType, int Slot)
{
    if (CarType < 0 || CarType >= CarCount) return false;

    BodyShopSection& B = CarConfigs[CarType].BodyShop;

    bool Enabled[6] = { B.Attachment5, B.Attachment6, B.Attachment7, B.Attachment8, B.Attachment9, B.Attachment10 };

    for (int i = 0; i < 6; i++)
        if (ExtraAttachmentSlots[i] == Slot) return B.Attachments > 5 + i && Enabled[i];

    return false;
}

// The Body Shop only exposes FRONT_BUMPER... only FRONT_BRAKE as a category, and with brakes
// decoupled from the performance package nothing keeps REAR_BRAKE in step any more. Mirror the
// front brake's upgrade level onto the rear, which is what the game's own sync does.
// A slot holds a CarPart pointer, but on a RideInfo the game has not finished filling in it can
// hold whatever was there before. A cop car preview handed this a slot containing 0x40800000,
// which is the float 4.0, and reading byte 5 of it faulted at 0x40800005.
bool IsPlausiblePartPointer(DWORD Value)
{
    return Value >= 0x00010000 && Value <= 0xC0000000 && !(Value & 3);
}

void MirrorFrontBrakeToRear(DWORD* RideInfo)
{
    int CarType = *RideInfo;

    // CarConfigs was being indexed with whatever the first dword happened to be. On a valid
    // RideInfo that is the car type; on anything else it is a wild index into the array.
    if (CarType < 0 || CarType >= CarCount) return;

    if (CarConfigs[CarType].Main.SyncBrakesWithPhysics) return;
    if (!CarConfigs[CarType].Main.MirrorBrakes) return;

    DWORD FrontValue = RideInfo[356 + CARSLOTID_FRONT_BRAKE];
    if (!IsPlausiblePartPointer(FrontValue)) return;

    DWORD* FrontBrake = (DWORD*)FrontValue;

    int Level = *((BYTE*)FrontBrake + 5) >> 5;

    DWORD RearValue = RideInfo[356 + CARSLOTID_REAR_BRAKE];
    DWORD* RearBrake = IsPlausiblePartPointer(RearValue) ? (DWORD*)RearValue : nullptr;

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
    // Everything below indexes CarConfigs and the slot array off this pointer, so one check here
    // covers the lot. The car select builds a preview before the car is fully set up.
    if (!RideInfo || ((uintptr_t)RideInfo & 3)) return;
    if (*(int*)RideInfo < 0 || *(int*)RideInfo >= CarCount) return;
    
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
            HoodPart = (DWORD*)RideInfo[356 + CARSLOTID_HOOD];
            if (HoodPart)
            {
                HoodUnderPartHash = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("HOODUNDER"), 0);
                
                // One guard around the pair: putting it inside would rebind the else
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_HOOD_UNDER))
                {
                    if (HoodUnderPartHash)
                        RideInfo[356 + CARSLOTID_HOOD_UNDER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_HOOD_UNDER, HoodUnderPartHash, 0, -1); // HOOD_UNDER
                    else
                        RideInfo[356 + CARSLOTID_HOOD_UNDER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_HOOD_UNDER, 0, 0, -1);
                }

                // Show engine if our custom attribute is present
                ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("SHOWENGINE"), 0);
                if (ShowEngineThruHood)
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_ENGINE) = 1; // ENGINE visibility

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
            TrunkPart = (DWORD*)RideInfo[356 + CARSLOTID_TRUNK];
            if (TrunkPart)
            {
                TrunkUnderPartHash = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("TRUNKUNDER"), 0);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_TRUNK_UNDER))
                {
                    if (TrunkUnderPartHash)
                        RideInfo[356 + CARSLOTID_TRUNK_UNDER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_TRUNK_UNDER, TrunkUnderPartHash, 0, -1); // TRUNK_UNDER
                    else
                        RideInfo[356 + CARSLOTID_TRUNK_UNDER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_TRUNK_UNDER, 0, 0, -1);
                }
                // Show engine if our custom attribute is present
                ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("SHOWTRUNK"), 0);
                if (ShowAudioThruTrunk)
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_TRUNK_AUDIO) = 1; // TRUNK_AUDIO visibility

            }
            break;

        case CARSLOTID_ENGINE:
            if (TheCarPart && *(int*)_TheGameFlowManager == 3)
            {
                *((BYTE*)RideInfo + 2104 + CARSLOTID_HOOD) = 1; // HOOD visibility
                if (!IsAttachmentDrivenSlot(RideInfo[0], CARSLOTID_HOOD_UNDER)) 
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_HOOD_UNDER) = 1; // HOOD_UNDER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_HOOD) = 1; // DECAL_HOOD visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_ENGINE) = 1; // ENGINE visibility
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

            RideInfo[356 + CARSLOTID_DECAL_FRONT_WINDOW] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_FRONT_WINDOW, bStringHash2("DECAL_FRONT_WINDOW_WIDE_MEDIUM", KitNamePartialHash), 0, -1);
            RideInfo[356 + CARSLOTID_DECAL_REAR_WINDOW] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_REAR_WINDOW, bStringHash2("DECAL_REAR_WINDOW_WIDE_MEDIUM", KitNamePartialHash), 0, -1);

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
                        DWORD* QuarterDecalPartLeft = (DWORD*)RideInfo[356 + CARSLOTID_DECAL_LEFT_QUARTER];
                        DWORD* QuarterDecalPartRight = (DWORD*)RideInfo[356 + CARSLOTID_DECAL_RIGHT_QUARTER];

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
                                    RideInfo[356 + CARSLOTID_DECAL_LEFT_QUARTER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_LEFT_QUARTER, bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*QuarterDecalPartLeft == bStringHash2("DECAL_LEFT_QUARTER_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    RideInfo[356 + CARSLOTID_DECAL_LEFT_QUARTER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_LEFT_QUARTER, bStringHash2("DECAL_LEFT_QUARTER_RECT_SMALL", KitNamePartialHash), 0, -1);
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
                                    RideInfo[356 + CARSLOTID_DECAL_RIGHT_QUARTER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_RIGHT_QUARTER, bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*QuarterDecalPartRight == bStringHash2("DECAL_RIGHT_QUARTER_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    RideInfo[356 + CARSLOTID_DECAL_RIGHT_QUARTER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_RIGHT_QUARTER, bStringHash2("DECAL_RIGHT_QUARTER_RECT_SMALL", KitNamePartialHash), 0, -1);
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

				// Check KitNumber attribute, if present it overrides the byte value from UpgradeGroupID
				KitNumber = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("KITNUMBER"), KitNumber);

                CarTypeName = GetCarTypeName(CarType);
                sprintf(PartNameBuf, "%s_WIDE%d_", CarTypeName, KitNumber);
                KitNamePartialHash = bStringHash(PartNameBuf);

                LeftDoorDecalHash = bStringHash2("DECAL_LEFT_DOOR_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR, LeftDoorDecalHash, 0, -1);
                RightDoorDecalHash = bStringHash2("DECAL_RIGHT_DOOR_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR, RightDoorDecalHash, 0, -1);
                LeftQuarterDecalHash = bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER, LeftQuarterDecalHash, 0, -1);
                RightQuarterDecalHash = bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", KitNamePartialHash);
                RideInfo[356 + CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER, RightQuarterDecalHash, 0, -1);

                sprintf(PartNameBuf, "%s_KITW%02d_DOOR_", CarTypeName, KitNumber);
                DoorNamePartialHash = bStringHash(PartNameBuf);
                LeftDoorHash = bStringHash2("LEFT", DoorNamePartialHash);
                RideInfo[356 + CARSLOTID_DOOR_LEFT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_LEFT, LeftDoorHash, 0, -1);
                RightDoorHash = bStringHash2("RIGHT", DoorNamePartialHash);
                RideInfo[356 + CARSLOTID_DOOR_RIGHT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_RIGHT, RightDoorHash, 0, -1);
                
                LeftDoorPanelHash = bStringHash2("PANEL_LEFT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_LEFT))
                    RideInfo[356 + CARSLOTID_DOOR_PANEL_LEFT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_LEFT, LeftDoorPanelHash, 0, -1);
                RightDoorPanelHash = bStringHash2("PANEL_RIGHT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_RIGHT))
                    RideInfo[356 + CARSLOTID_DOOR_PANEL_RIGHT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_RIGHT, RightDoorPanelHash, 0, -1);
                LeftDoorSillHash = bStringHash2("SILL_LEFT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_LEFT))
                    RideInfo[356 + CARSLOTID_DOOR_SILL_LEFT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_LEFT, LeftDoorSillHash, 0, -1);
                RightDoorSillHash = bStringHash2("SILL_RIGHT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_RIGHT))
                    RideInfo[356 + CARSLOTID_DOOR_SILL_RIGHT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_RIGHT, RightDoorSillHash, 0, -1);

                *((BYTE*)RideInfo + 2104 + CARSLOTID_FRONT_BUMPER) = 0;     // FRONT_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_REAR_BUMPER) = 0;      // REAR_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_SKIRT) = 0;            // SKIRT visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_BODY) = 0;             // BODY visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_QUARTER) = 0;          // QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_FENDER) = 0;           // FENDER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDE_BODY) = 1;        // WIDE_BODY visibility
                // make WIDEBODY_DECALs visible, DECALs invisible
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_LEFT_DOOR) = 0;              // DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_RIGHT_DOOR) = 0;             // DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_LEFT_QUARTER) = 0;           // DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_RIGHT_QUARTER) = 0;          // DECAL_RIGHT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR) = 1;     // WIDEBODY_DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR) = 1;    // WIDEBODY_DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER) = 1;  // WIDEBODY_DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER) = 1; // WIDEBODY_DECAL_RIGHT_QUARTER visibility

                // If widebody allows customization, make bumpers visible and customizable

				if (IsCustomWidebody(TheCarPart, CARSLOTID_FRONT_BUMPER))
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_FRONT_BUMPER) = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_REAR_BUMPER))
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_REAR_BUMPER) = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_SKIRT))
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_SKIRT) = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_FENDER))
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_FENDER) = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_QUARTER))
                    *((BYTE*)RideInfo + 2104 + CARSLOTID_QUARTER) = 1;
            }
            else
            {
                RideInfo[356 + CARSLOTID_DOOR_LEFT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_LEFT, 0, 0, -1);
                RideInfo[356 + CARSLOTID_DOOR_RIGHT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_RIGHT, 0, 0, -1);
                
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_LEFT))
                    RideInfo[356 + CARSLOTID_DOOR_PANEL_LEFT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_LEFT, 0, 0, -1);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_RIGHT))
                    RideInfo[356 + CARSLOTID_DOOR_PANEL_RIGHT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_RIGHT, 0, 0, -1);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_LEFT))
                    RideInfo[356 + CARSLOTID_DOOR_SILL_LEFT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_LEFT, 0, 0, -1);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_RIGHT))
                    RideInfo[356 + CARSLOTID_DOOR_SILL_RIGHT] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_RIGHT, 0, 0, -1);

                *((BYTE*)RideInfo + 2104 + CARSLOTID_FRONT_BUMPER) = 1;     // FRONT_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_REAR_BUMPER) = 1;      // REAR_BUMPER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_SKIRT) = 1;            // SKIRT visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_BODY) = 1;             // BODY visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_QUARTER) = 1;          // QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_FENDER) = 1;           // FENDER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDE_BODY) = 0;        // WIDE_BODY visibility
                // make DECALs visible, WIDEBODY_DECALs invisible
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_LEFT_DOOR) = 1;              // DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_RIGHT_DOOR) = 1;             // DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_LEFT_QUARTER) = 1;           // DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_DECAL_RIGHT_QUARTER) = 1;          // DECAL_RIGHT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR) = 0;     // WIDEBODY_DECAL_LEFT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR) = 0;    // WIDEBODY_DECAL_RIGHT_DOOR visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER) = 0;  // WIDEBODY_DECAL_LEFT_QUARTER visibility
                *((BYTE*)RideInfo + 2104 + CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER) = 0; // WIDEBODY_DECAL_RIGHT_QUARTER visibility
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
	if (RideInfo)
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
    if (!RideInfo || ((uintptr_t)RideInfo & 3)) return;

    int CarType = *RideInfo;
    if (CarType < 0 || CarType >= CarCount) return;

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

DWORD __fastcall RideInfo_GetStockPartNameHash(DWORD* RideInfo, void* EDX_Unused, int CarSlotID)
{
    /*return CarConfigs[RideInfo[0]].StockParts.CustomStockParts
        ? CarConfigs[RideInfo[0]].StockParts.Parts[CarSlotID]
        : -1;*/
	return -1; // TODO: Implement custom stock parts in ini files
}

void __fastcall RideInfo_SetStockParts(DWORD* RideInfo, void* EDX_Unused, int use_paint_2)
{
    if (!RideInfo) return;

    int CarType = *RideInfo;
    if (CarType < 0 || CarType >= CarCount) return;

	DWORD* cti = GetCarTypeInfo(CarType);

    DWORD DefaultVinylColorHashes[] = {
        CT_bStringHash("VINYL_L1_COLOR09"),
        CT_bStringHash("VINYL_L1_COLOR02"),
        CT_bStringHash("VINYL_L2_COLOR11"),
        CT_bStringHash("VINYL_L1_COLOR01")
	};

    DWORD* part = 0;
    DWORD PartNameHash = 0;

    for (int i = CARSLOTID_MODEL_FIRST; i < CARSLOTID_WHEEL_MANUFACTURER; ++i)
    {
        if ((i < CARSLOTID_VINYL_LAYER0 || i > CARSLOTID_VINYL_LAYER3) 
            && (i < CARSLOTID_DECAL_HOOD_TEX0 || i > CARSLOTID_DECAL_RIGHT_QUARTER_TEX7))
        {
            switch (i)
            {
            case CARSLOTID_BASE_PAINT:
				PartNameHash = use_paint_2 ? cti[0x850 / 4] : cti[0x84C / 4];
                part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
                if (part) RideInfo[356 + i] = (DWORD)part;
                else // If the paint is invalid
                {
                    PartNameHash = CT_bStringHash("GLOSS_L1_COLOR01");
                    part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
                    if (part) RideInfo[356 + i] = (DWORD)part;
                }
                break;

            case CARSLOTID_ROOF:
                PartNameHash = CT_bStringHash("ROOF_STYLE00");
                goto ApplyPart;
                break;

            case CARSLOTID_NEON:
            case CARSLOTID_NEON_CABIN:
            case CARSLOTID_NEON_ENGINE:
            case CARSLOTID_NEON_TRUNK:
                PartNameHash = CT_bStringHash("NEON_NONE");
                goto ApplyPart;
                break;

            case CARSLOTID_HUD_BACKING_COLOUR:
            case CARSLOTID_HUD_NEEDLE_COLOUR:
            case CARSLOTID_HUD_CHARACTER_COLOUR:
                PartNameHash = CT_bStringHash("WHITE");

            ApplyPart:
                part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
                if (part) RideInfo[356 + i] = (DWORD)part;
                RideInfo_UpdatePartsEnabled(RideInfo, EDX_Unused);
                break;

            default:
				part = FindPartWithLevel(CarType, i, 0);
                if (part) RideInfo[356 + i] = (DWORD)part;
                RideInfo_UpdatePartsEnabled(RideInfo, EDX_Unused);
                break;

            
            }
        }

        // else: No need to assign, RideInfo inits them with 0

        // Check if ini has a custom stock part
        PartNameHash = RideInfo_GetStockPartNameHash(RideInfo, EDX_Unused, i);
        if (PartNameHash != -1) // apply the part
        {
            part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
            if (part) RideInfo_SetPart(RideInfo, EDX_Unused, i, part);
            else RideInfo_SetPart(RideInfo, EDX_Unused, i, FindPartWithLevel(CarType, i, 0));
        }
    }

    int CurrentVinylColor = 0;

    for (int i = CARSLOTID_VINYL_COLOUR0_0; i < CARSLOTID_VINYL_COLOUR3_3; ++i)
    {
		PartNameHash = DefaultVinylColorHashes[CurrentVinylColor];
		RideInfo[356 + i] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
        RideInfo_UpdatePartsEnabled(RideInfo, EDX_Unused);

		CurrentVinylColor = (CurrentVinylColor + 1) % 4;
    }

    //RideInfo_SetPart(RideInfo, EDX_Unused, CARSLOTID_WHEEL_MANUFACTURER, 0);
    //RideInfo_SetPart(RideInfo, EDX_Unused, CARSLOTID_MISC, 0);
    
}

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