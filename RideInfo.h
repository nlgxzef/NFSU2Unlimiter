#pragma once

#include "stdio.h"
#include "InGameFunctions.h"
#include "PartLink.h"
#include "CarPart.h"
#include "RidePhysicsInfoStruct.h"

#pragma pack(push, 1)
struct RideInfo
{
    int Type;
    char InstanceIndex;
    char HasDash;
    char CanBeVertexDamaged;
    char SkinType;
    char _pad008[8];
    RidePhysicsInfo mPhysics;
    signed __int8 field_580;
    unsigned __int8 HydraulicsLevel;
    char _pad582[2];
    unsigned int mCompositeSkinHash;
    unsigned int mCompositeWheelHash;
    unsigned int mCompositeSpinnerHash;
    CarPart* mPartsTable[170];
    char mPartsEnabled[170];
    char _pad8E2[2];
    CarPart* PreviewPart;
    int field_8E8;
    char _pad8EC[4];
};
#pragma pack(pop)

static_assert(offsetof(RideInfo, mPhysics) == 0x10, "RideInfo::mPhysics");
static_assert(offsetof(RideInfo, HydraulicsLevel) == 0x581, "RideInfo::HydraulicsLevel");
static_assert(offsetof(RideInfo, mCompositeSkinHash) == 0x584, "RideInfo::mCompositeSkinHash");
static_assert(offsetof(RideInfo, mPartsTable) == 0x590, "RideInfo::mPartsTable");
static_assert(offsetof(RideInfo, mPartsEnabled) == 0x838, "RideInfo::mPartsEnabled");
static_assert(offsetof(RideInfo, PreviewPart) == 0x8E4, "RideInfo::PreviewPart");
static_assert(sizeof(RideInfo) == 0x8F0, "RideInfo size");

bool HoodDecalsOnCustomHoods = true;

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

void MirrorFrontBrakeToRear(RideInfo* ride_info)
{
    int CarType = ride_info->Type;

    // CarConfigs was being indexed with whatever the first dword happened to be. On a valid
    // RideInfo that is the car type; on anything else it is a wild index into the array.
    if (CarType < 0 || CarType >= CarCount) return;

    if (CarConfigs[CarType].Main.SyncBrakesWithPhysics) return;
    if (!CarConfigs[CarType].Main.MirrorBrakes) return;

    DWORD FrontValue = (DWORD)ride_info->mPartsTable[CARSLOTID_FRONT_BRAKE];
    if (!IsPlausiblePartPointer(FrontValue)) return;

    CarPart* FrontBrake = (CarPart*)FrontValue;

    int Level = FrontBrake->GroupNumber_UpgradeLevel >> 5;

    DWORD RearValue = (DWORD)ride_info->mPartsTable[CARSLOTID_REAR_BRAKE];
    CarPart* RearBrake = IsPlausiblePartPointer(RearValue) ? (CarPart*)RearValue : nullptr;

    if (RearBrake && (RearBrake->GroupNumber_UpgradeLevel >> 5 == Level)) return;

    CarPart* NewRearBrake = (CarPart*)FindPartWithLevel(CarType, CARSLOTID_REAR_BRAKE, Level);
    if (NewRearBrake) ride_info->mPartsTable[CARSLOTID_REAR_BRAKE] = NewRearBrake;
}

bool RideInfo_HasDifferentWheels(RideInfo* ride_info)
{
	CarPart* FrontWheel = ride_info->mPartsTable[CARSLOTID_FRONT_WHEEL];
	CarPart* RearWheel = ride_info->mPartsTable[CARSLOTID_REAR_WHEEL];

    if (!FrontWheel && !RearWheel) return true; // if empty ride

	return FrontWheel != RearWheel;
}

bool __fastcall RideInfo_TrunkAudioSlotAvailable(RideInfo* ride_info, void* EDX_Unused, int CarSlotID)
{
    CarPart* TrunkAudioPart;
    bool result; // al
    int CarTypeID; // ecx

    if (CarSlotID < CARSLOTID_TRUNK_AUDIO_COMP_0)
        return 0;
    if (CarSlotID > CARSLOTID_TRUNK_AUDIO_COMP_11)
        return 0;
    TrunkAudioPart = ride_info->mPartsTable[CARSLOTID_TRUNK_AUDIO]; // TRUNK_AUDIO
    if (!TrunkAudioPart)
        return 0;

	return CarPart_TrunkAudioSlotAvailable(TrunkAudioPart, 0, CarSlotID);

    //int NumberOfSlots = CarPart_GetAppliedAttributeUParam(TrunkAudioPart, CT_bStringHash("NUMSLOTS"), 0);
    //if (NumberOfSlots) return CarSlotID <= CARSLOTID_TRUNK_AUDIO + NumberOfSlots;
    //
    //return RideInfo_TrunkAudioSlotAvailable_Game(ride_info, CarSlotID);
}

void __fastcall RideInfo_UpdatePartsEnabled(RideInfo* ride_info, void* EDX_Unused)
{
    // Everything below indexes CarConfigs and the slot array off this pointer, so one check here
    // covers the lot. The car select builds a preview before the car is fully set up.
    if (!ride_info || ((uintptr_t)ride_info & 3)) return;
    if (ride_info->Type < 0 || ride_info->Type >= CarCount) return;
    
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

    CarType = ride_info->Type;

    MirrorFrontBrakeToRear(ride_info);

    memset(ride_info->mPartsEnabled, 1, sizeof(ride_info->mPartsEnabled));
    for (i = CARSLOTID_MODEL_FIRST; i < CARSLOTID_NUM; ++i)
    {
        TheCarPart = (DWORD*)ride_info->mPartsTable[i];
        if (TheCarPart)
        {
            // Hide excluded decal layout parts
			int ExcludeDecalSlot = HoodDecalsOnCustomHoods ? -1 : CarPart_GetExcludeDecal((CarPart*)TheCarPart, EDX_Unused);
			if (ExcludeDecalSlot != -1 && ExcludeDecalSlot != CARSLOTID_PAINT_SPOILER)
				ride_info->mPartsEnabled[ExcludeDecalSlot] = 0;
        }

        switch (i)
        {
        case CARSLOTID_HYDRAULICS:
            if (TheCarPart)
                ride_info->HydraulicsLevel = *((BYTE*)TheCarPart + 5) >> 5;
            break;

        case CARSLOTID_FRONT_WHEEL:
        case CARSLOTID_REAR_WHEEL:
            if (IsBrowsingBrakePaint())
                ride_info->mPartsEnabled[i] = 0; // FRONT_WHEEL visibility
            break;

        case CARSLOTID_HOOD:
            HoodPart = (DWORD*)ride_info->mPartsTable[CARSLOTID_HOOD];
            if (HoodPart)
            {
                HoodUnderPartHash = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("HOODUNDER"), 0);
                
                // One guard around the pair: putting it inside would rebind the else
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_HOOD_UNDER))
                {
                    if (HoodUnderPartHash)
                        ride_info->mPartsTable[CARSLOTID_HOOD_UNDER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_HOOD_UNDER, HoodUnderPartHash, 0, -1); // HOOD_UNDER
                    else
                        ride_info->mPartsTable[CARSLOTID_HOOD_UNDER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_HOOD_UNDER, 0, 0, -1);
                }

                // Show engine if our custom attribute is present
                ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("SHOWENGINE"), 0);
                if (ShowEngineThruHood)
                    ride_info->mPartsEnabled[CARSLOTID_ENGINE] = 1; // ENGINE visibility

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
                        DWORD* HoodDecalPart = (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_HOOD];

                        if (HoodDecalPart)
                        {
                            for (int j = 0; j <= 99; j++) // Find which decal layout the car has
                            {
                                if (j == 0) sprintf(KitNameBuf, "%s_", GetCarTypeName(CarType));
                                else sprintf(KitNameBuf, "%s_STYLE%02d_", GetCarTypeName(CarType), j);
                                DWORD DecalNamePartialHash = bStringHash(KitNameBuf);
                                DWORD* NewHoodDecalPart = nullptr;

                                if (*HoodDecalPart == bStringHash2("DECAL_HOOD_RECT_MEDIUM", DecalNamePartialHash)) // Layout 1
                                    NewHoodDecalPart = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_HOOD, bStringHash2("DECAL_HOOD_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                else if (*HoodDecalPart == bStringHash2("DECAL_HOOD_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                    NewHoodDecalPart = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_HOOD, bStringHash2("DECAL_HOOD_RECT_SMALL", KitNamePartialHash), 0, -1);
                                else
                                    continue;

                                if (NewHoodDecalPart) ride_info->mPartsTable[CARSLOTID_DECAL_HOOD] = (CarPart*)NewHoodDecalPart;

                                break;
                            }
                        }

                        break;
                    }
                }
            }
            break;

        case CARSLOTID_TRUNK:
            TrunkPart = (DWORD*)ride_info->mPartsTable[CARSLOTID_TRUNK];
            if (TrunkPart)
            {
                TrunkUnderPartHash = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("TRUNKUNDER"), 0);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_TRUNK_UNDER))
                {
                    if (TrunkUnderPartHash)
                        ride_info->mPartsTable[CARSLOTID_TRUNK_UNDER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_TRUNK_UNDER, TrunkUnderPartHash, 0, -1); // TRUNK_UNDER
                    else
                        ride_info->mPartsTable[CARSLOTID_TRUNK_UNDER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_TRUNK_UNDER, 0, 0, -1);
                }
                // Show engine if our custom attribute is present
                ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("SHOWTRUNK"), 0);
                if (ShowAudioThruTrunk)
                    ride_info->mPartsEnabled[CARSLOTID_TRUNK_AUDIO] = 1; // TRUNK_AUDIO visibility

            }
            break;

        case CARSLOTID_ENGINE:
            if (TheCarPart && *(int*)_TheGameFlowManager == 3)
            {
                ride_info->mPartsEnabled[CARSLOTID_HOOD] = 1; // HOOD visibility
                if (!IsAttachmentDrivenSlot(ride_info->Type, CARSLOTID_HOOD_UNDER)) 
                    ride_info->mPartsEnabled[CARSLOTID_HOOD_UNDER] = 1; // HOOD_UNDER visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_HOOD] = 1; // DECAL_HOOD visibility
                ride_info->mPartsEnabled[CARSLOTID_ENGINE] = 1; // ENGINE visibility
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

            ride_info->mPartsTable[CARSLOTID_DECAL_FRONT_WINDOW] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_FRONT_WINDOW, bStringHash2("DECAL_FRONT_WINDOW_WIDE_MEDIUM", KitNamePartialHash), 0, -1);
            ride_info->mPartsTable[CARSLOTID_DECAL_REAR_WINDOW] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_REAR_WINDOW, bStringHash2("DECAL_REAR_WINDOW_WIDE_MEDIUM", KitNamePartialHash), 0, -1);

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
                        DWORD* QuarterDecalPartLeft = (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER];
                        DWORD* QuarterDecalPartRight = (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_RIGHT_QUARTER];

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
                                    ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_LEFT_QUARTER, bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*QuarterDecalPartLeft == bStringHash2("DECAL_LEFT_QUARTER_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_LEFT_QUARTER, bStringHash2("DECAL_LEFT_QUARTER_RECT_SMALL", KitNamePartialHash), 0, -1);
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
                                    ride_info->mPartsTable[CARSLOTID_DECAL_RIGHT_QUARTER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_RIGHT_QUARTER, bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", KitNamePartialHash), 0, -1);
                                    break;
                                }
                                else if (*QuarterDecalPartRight == bStringHash2("DECAL_RIGHT_QUARTER_RECT_SMALL", DecalNamePartialHash)) // Layout 2
                                {
                                    ride_info->mPartsTable[CARSLOTID_DECAL_RIGHT_QUARTER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DECAL_RIGHT_QUARTER, bStringHash2("DECAL_RIGHT_QUARTER_RECT_SMALL", KitNamePartialHash), 0, -1);
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
                ride_info->mPartsTable[CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR, LeftDoorDecalHash, 0, -1);
                RightDoorDecalHash = bStringHash2("DECAL_RIGHT_DOOR_RECT_MEDIUM", KitNamePartialHash);
                ride_info->mPartsTable[CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR, RightDoorDecalHash, 0, -1);
                LeftQuarterDecalHash = bStringHash2("DECAL_LEFT_QUARTER_RECT_MEDIUM", KitNamePartialHash);
                ride_info->mPartsTable[CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER, LeftQuarterDecalHash, 0, -1);
                RightQuarterDecalHash = bStringHash2("DECAL_RIGHT_QUARTER_RECT_MEDIUM", KitNamePartialHash);
                ride_info->mPartsTable[CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER, RightQuarterDecalHash, 0, -1);

                sprintf(PartNameBuf, "%s_KITW%02d_DOOR_", CarTypeName, KitNumber);
                DoorNamePartialHash = bStringHash(PartNameBuf);
                LeftDoorHash = bStringHash2("LEFT", DoorNamePartialHash);
                ride_info->mPartsTable[CARSLOTID_DOOR_LEFT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_LEFT, LeftDoorHash, 0, -1);
                RightDoorHash = bStringHash2("RIGHT", DoorNamePartialHash);
                ride_info->mPartsTable[CARSLOTID_DOOR_RIGHT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_RIGHT, RightDoorHash, 0, -1);
                
                LeftDoorPanelHash = bStringHash2("PANEL_LEFT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_LEFT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_PANEL_LEFT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_LEFT, LeftDoorPanelHash, 0, -1);
                RightDoorPanelHash = bStringHash2("PANEL_RIGHT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_RIGHT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_PANEL_RIGHT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_RIGHT, RightDoorPanelHash, 0, -1);
                LeftDoorSillHash = bStringHash2("SILL_LEFT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_LEFT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_SILL_LEFT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_LEFT, LeftDoorSillHash, 0, -1);
                RightDoorSillHash = bStringHash2("SILL_RIGHT", DoorNamePartialHash);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_RIGHT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_SILL_RIGHT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_RIGHT, RightDoorSillHash, 0, -1);

                ride_info->mPartsEnabled[CARSLOTID_FRONT_BUMPER] = 0;     // FRONT_BUMPER visibility
                ride_info->mPartsEnabled[CARSLOTID_REAR_BUMPER] = 0;      // REAR_BUMPER visibility
                ride_info->mPartsEnabled[CARSLOTID_SKIRT] = 0;            // SKIRT visibility
                ride_info->mPartsEnabled[CARSLOTID_BODY] = 0;             // BODY visibility
                ride_info->mPartsEnabled[CARSLOTID_QUARTER] = 0;          // QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_FENDER] = 0;           // FENDER visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDE_BODY] = 1;        // WIDE_BODY visibility
                // make WIDEBODY_DECALs visible, DECALs invisible
                ride_info->mPartsEnabled[CARSLOTID_DECAL_LEFT_DOOR] = 0;              // DECAL_LEFT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_RIGHT_DOOR] = 0;             // DECAL_RIGHT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_LEFT_QUARTER] = 0;           // DECAL_LEFT_QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_RIGHT_QUARTER] = 0;          // DECAL_RIGHT_QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR] = 1;     // WIDEBODY_DECAL_LEFT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR] = 1;    // WIDEBODY_DECAL_RIGHT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER] = 1;  // WIDEBODY_DECAL_LEFT_QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER] = 1; // WIDEBODY_DECAL_RIGHT_QUARTER visibility

                // If widebody allows customization, make bumpers visible and customizable

				if (IsCustomWidebody(TheCarPart, CARSLOTID_FRONT_BUMPER))
                    ride_info->mPartsEnabled[CARSLOTID_FRONT_BUMPER] = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_REAR_BUMPER))
                    ride_info->mPartsEnabled[CARSLOTID_REAR_BUMPER] = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_SKIRT))
                    ride_info->mPartsEnabled[CARSLOTID_SKIRT] = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_FENDER))
                    ride_info->mPartsEnabled[CARSLOTID_FENDER] = 1;
                if (IsCustomWidebody(TheCarPart, CARSLOTID_QUARTER))
                    ride_info->mPartsEnabled[CARSLOTID_QUARTER] = 1;
            }
            else
            {
                ride_info->mPartsTable[CARSLOTID_DOOR_LEFT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_LEFT, 0, 0, -1);
                ride_info->mPartsTable[CARSLOTID_DOOR_RIGHT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_RIGHT, 0, 0, -1);
                
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_LEFT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_PANEL_LEFT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_LEFT, 0, 0, -1);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_PANEL_RIGHT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_PANEL_RIGHT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_PANEL_RIGHT, 0, 0, -1);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_LEFT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_SILL_LEFT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_LEFT, 0, 0, -1);
                if (!IsAttachmentDrivenSlot(CarType, CARSLOTID_DOOR_SILL_RIGHT))
                    ride_info->mPartsTable[CARSLOTID_DOOR_SILL_RIGHT] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, CARSLOTID_DOOR_SILL_RIGHT, 0, 0, -1);

                ride_info->mPartsEnabled[CARSLOTID_FRONT_BUMPER] = 1;     // FRONT_BUMPER visibility
                ride_info->mPartsEnabled[CARSLOTID_REAR_BUMPER] = 1;      // REAR_BUMPER visibility
                ride_info->mPartsEnabled[CARSLOTID_SKIRT] = 1;            // SKIRT visibility
                ride_info->mPartsEnabled[CARSLOTID_BODY] = 1;             // BODY visibility
                ride_info->mPartsEnabled[CARSLOTID_QUARTER] = 1;          // QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_FENDER] = 1;           // FENDER visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDE_BODY] = 0;        // WIDE_BODY visibility
                // make DECALs visible, WIDEBODY_DECALs invisible
                ride_info->mPartsEnabled[CARSLOTID_DECAL_LEFT_DOOR] = 1;              // DECAL_LEFT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_RIGHT_DOOR] = 1;             // DECAL_RIGHT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_LEFT_QUARTER] = 1;           // DECAL_LEFT_QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_DECAL_RIGHT_QUARTER] = 1;          // DECAL_RIGHT_QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_LEFT_DOOR] = 0;     // WIDEBODY_DECAL_LEFT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_RIGHT_DOOR] = 0;    // WIDEBODY_DECAL_RIGHT_DOOR visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_LEFT_QUARTER] = 0;  // WIDEBODY_DECAL_LEFT_QUARTER visibility
                ride_info->mPartsEnabled[CARSLOTID_WIDEBODY_DECAL_RIGHT_QUARTER] = 0; // WIDEBODY_DECAL_RIGHT_QUARTER visibility
            }
            break;
        }
    }

    {
        MainSection& M = CarConfigs[CarType].Main;

        if (M.AlwaysShowHoodUnder)  ride_info->mPartsEnabled[CARSLOTID_HOOD_UNDER] = 1;
        if (M.AlwaysShowTrunkUnder) ride_info->mPartsEnabled[CARSLOTID_TRUNK_UNDER] = 1;

        if (M.AlwaysShowDoorPanels)
        {
            ride_info->mPartsEnabled[CARSLOTID_DOOR_PANEL_LEFT] = 1;
            ride_info->mPartsEnabled[CARSLOTID_DOOR_PANEL_RIGHT] = 1;
        }

        if (M.AlwaysShowDoorSills)
        {
            ride_info->mPartsEnabled[CARSLOTID_DOOR_SILL_LEFT] = 1;
            ride_info->mPartsEnabled[CARSLOTID_DOOR_SILL_RIGHT] = 1;
        }

        BodyShopSection& B = CarConfigs[CarType].BodyShop;

        bool ExtraEnabled[6] = {
            B.Attachment5, B.Attachment6, B.Attachment7,
            B.Attachment8, B.Attachment9, B.Attachment10,
        };

        for (int i = 0; i < 6; i++)
            if (B.Attachments > 5 + i && ExtraEnabled[i])
                ride_info->mPartsEnabled[ExtraAttachmentSlots[i]] = 1;
    }

    PartLink_Resolve((DWORD*)ride_info);
    PartLink_ApplyVisibility((DWORD*)ride_info);
}

void __fastcall RideInfo_SetPart(RideInfo* ride_info, void* EDX_Unused, int CarSlotID, DWORD* CarPartToSet)
{
	if (ride_info)
	{
		ride_info->mPartsTable[CarSlotID] = (CarPart*)CarPartToSet;
		RideInfo_UpdatePartsEnabled(ride_info, EDX_Unused);
	}
}

void __fastcall RideInfo_SetPart_Rims(RideInfo* ride_info, void* EDX_Unused, int CarSlotID, DWORD* CarPartToSet)
{
    if (ride_info && (CarSlotID == CARSLOTID_FRONT_WHEEL || CarSlotID == CARSLOTID_REAR_WHEEL))
    {
        switch (RimsToCustomize)
        {
        case -1:
            ride_info->mPartsTable[CARSLOTID_REAR_WHEEL] = (CarPart*)CarPartToSet;
            break;
        case 0:
            ride_info->mPartsTable[CARSLOTID_FRONT_WHEEL] = (CarPart*)CarPartToSet;
            ride_info->mPartsTable[CARSLOTID_REAR_WHEEL] = (CarPart*)CarPartToSet;
            break;
        case 1:
        default:
            ride_info->mPartsTable[CARSLOTID_FRONT_WHEEL] = (CarPart*)CarPartToSet;
            break;
        }

        RideInfo_UpdatePartsEnabled(ride_info, EDX_Unused);
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

DWORD __fastcall RideInfo_GetStockPartNameHash(RideInfo* ride_info, void* EDX_Unused, int CarSlotID)
{
    return CarConfigs[ride_info->Type].StockParts.CustomStockParts
        ? CarConfigs[ride_info->Type].StockParts.Parts[CarSlotID]
        : -1;
}

void __fastcall RideInfo_SetStockParts(RideInfo* ride_info, void* EDX_Unused, int use_paint_2)
{
    if (!ride_info) return;

    int CarType = ride_info->Type;
    if (CarType < 0 || CarType >= CarCount) return;

    CarTypeInfo* cti = &CarTypeInfoArray[ride_info->Type];

    DWORD DefaultVinylColorHashes[] = {
        CT_bStringHash("VINYL_L1_COLOR09"),
        CT_bStringHash("VINYL_L1_COLOR02"),
        CT_bStringHash("VINYL_L2_COLOR11"),
        CT_bStringHash("VINYL_L1_COLOR01")
	};

    DWORD* part = 0;
    DWORD PartNameHash = 0;

    for (int i = CARSLOTID_MODEL_FIRST; i < CARSLOTID_UL_TIRE_SMOKE; ++i)
    {
        if ((i < CARSLOTID_VINYL_LAYER0 || i > CARSLOTID_VINYL_LAYER3) 
            && (i < CARSLOTID_DECAL_HOOD_TEX0 || i > CARSLOTID_DECAL_RIGHT_QUARTER_TEX7))
        {
            switch (i)
            {
            case CARSLOTID_BASE_PAINT:
                PartNameHash = use_paint_2 ? cti->DefaultBasePaint2 : cti->DefaultBasePaint;
                part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
                if (part) ride_info->mPartsTable[i] = (CarPart*)part;
                else // If the paint is invalid
                {
                    PartNameHash = CT_bStringHash("GLOSS_L1_COLOR01");
                    part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
                    if (part) ride_info->mPartsTable[i] = (CarPart*)part;
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
                if (part) ride_info->mPartsTable[i] = (CarPart*)part;
                RideInfo_UpdatePartsEnabled(ride_info, EDX_Unused);
                break;

            default:
				part = FindPartWithLevel(CarType, i, 0);
                if (part) ride_info->mPartsTable[i] = (CarPart*)part;
                RideInfo_UpdatePartsEnabled(ride_info, EDX_Unused);
                break;

            
            }
        }

        // else: No need to assign, RideInfo inits them with 0

        // Check if ini has a custom stock part
        PartNameHash = RideInfo_GetStockPartNameHash(ride_info, EDX_Unused, i);
        if (PartNameHash != -1) // apply the part
        {
            part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
            if (part) RideInfo_SetPart(ride_info, EDX_Unused, i, part);
            else RideInfo_SetPart(ride_info, EDX_Unused, i, FindPartWithLevel(CarType, i, 0));
        }
    }

    int CurrentVinylColor = 0;

    for (int i = CARSLOTID_VINYL_COLOUR0_0; i < CARSLOTID_VINYL_COLOUR3_3; ++i)
    {
        PartNameHash = RideInfo_GetStockPartNameHash(ride_info, EDX_Unused, i);
        if (PartNameHash == -1) PartNameHash = DefaultVinylColorHashes[CurrentVinylColor];

		ride_info->mPartsTable[i] = (CarPart*)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, i, PartNameHash, 0, -1);
        RideInfo_UpdatePartsEnabled(ride_info, EDX_Unused);

		CurrentVinylColor = (CurrentVinylColor + 1) % 4;
    }
}

void(__thiscall* RideInfo_BuildRandomRide_Game)(RideInfo* ride_info, int random) = (void(__thiscall*)(RideInfo*, int))0x6374B0;
void(__thiscall* RideInfo_SetRandomPart)(RideInfo* ride_info, int slot, int level) = (void(__thiscall*)(RideInfo*, int, int))0x6371A0;
void __fastcall RideInfo_BuildRandomRide(RideInfo* ride_info, void *EDX_Unused, int random)
{
    switch (CarConfigs[ride_info->Type].RandomParts.CustomRandomParts)
    {
    case 1: // Skip vanilla randomization
        break;
    default:
        RideInfo_BuildRandomRide_Game(ride_info, random);
        break;
    }

    if (CarConfigs[ride_info->Type].RandomParts.CustomRandomParts)
    {
        int RandomRearRimProbability = bRandom(100);

        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_FRONT_BUMPER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_FRONT_BUMPER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_REAR_BUMPER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_REAR_BUMPER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_SKIRT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_SKIRT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_FENDER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_FENDER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_QUARTER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_QUARTER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_SPOILER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_SPOILER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HOOD]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HOOD, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_ENGINE] && !CarConfigs[ride_info->Type].Main.SyncVisualPartsWithPhysics) RideInfo_SetRandomPart(ride_info, CARSLOTID_ENGINE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_TRUNK]) RideInfo_SetRandomPart(ride_info, CARSLOTID_TRUNK, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_ROOF]) RideInfo_SetRandomPart(ride_info, CARSLOTID_ROOF, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_BASE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_BASE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_TOP]) RideInfo_SetRandomPart(ride_info, CARSLOTID_TOP, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HEADLIGHT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HEADLIGHT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_BRAKELIGHT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_BRAKELIGHT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_WING_MIRROR]) RideInfo_SetRandomPart(ride_info, CARSLOTID_WING_MIRROR, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_EXHAUST]) RideInfo_SetRandomPart(ride_info, CARSLOTID_EXHAUST, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_FRONT_WHEEL])
        {
            RideInfo_SetRandomPart(ride_info, CARSLOTID_FRONT_WHEEL, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_REAR_WHEEL] && RandomRearRimProbability < CarConfigs[ride_info->Type].RandomParts.RandomRearRimProbability)
            {
                RideInfo_SetRandomPart(ride_info, CARSLOTID_REAR_WHEEL, -1);
            }
            else
            {
                RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_REAR_WHEEL, (DWORD*)ride_info->mPartsTable[CARSLOTID_FRONT_WHEEL]);
			}
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_FRONT_BRAKE] && !CarConfigs[ride_info->Type].Main.SyncBrakesWithPhysics)
        {
            RideInfo_SetRandomPart(ride_info, CARSLOTID_FRONT_BRAKE, -1);
            RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_REAR_BRAKE, (DWORD*)ride_info->mPartsTable[CARSLOTID_FRONT_BRAKE]);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_WIDE_BODY]) RideInfo_SetRandomPart(ride_info, CARSLOTID_WIDE_BODY, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DAMAGE_FRONT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DAMAGE_FRONT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DAMAGE_REAR]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DAMAGE_REAR, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DAMAGE_LEFT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DAMAGE_LEFT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DAMAGE_RIGHT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DAMAGE_RIGHT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DAMAGE_TOP]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DAMAGE_TOP, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DOOR_PANEL_LEFT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DOOR_PANEL_LEFT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DOOR_PANEL_RIGHT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DOOR_PANEL_RIGHT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DOOR_SILL_LEFT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DOOR_SILL_LEFT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DOOR_SILL_RIGHT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DOOR_SILL_RIGHT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HOOD_UNDER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HOOD_UNDER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_TRUNK_UNDER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_TRUNK_UNDER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_BASE_PAINT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_BASE_PAINT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_SPOILER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_SPOILER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_ROOF]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_ROOF, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_MIRROR]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_MIRROR, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_RIM]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_RIM, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_SPINNER]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_SPINNER, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_BRAKE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_BRAKE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_ENGINE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_ENGINE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_AUDIO]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_AUDIO, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_PAINT_EXHAUST]) RideInfo_SetRandomPart(ride_info, CARSLOTID_PAINT_EXHAUST, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_LAYER0])
        {
            RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_LAYER0, -1);

            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR0_0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR0_0, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR0_1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR0_1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR0_2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR0_2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR0_3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR0_3, -1);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_LAYER1]) 
        {
            RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_LAYER1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR1_0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR1_0, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR1_1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR1_1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR1_2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR1_2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR1_3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR1_3, -1);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_LAYER2])
        {
           RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_LAYER2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR2_0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR2_0, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR2_1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR2_1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR2_2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR2_2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR2_3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR2_3, -1);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_LAYER3])
        {
            RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_LAYER3, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR3_0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR3_0, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR3_1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR3_1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR3_2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR3_2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_VINYL_COLOUR3_3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_VINYL_COLOUR3_3, -1);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX0, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX1, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX2, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX3, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX4, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX5, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX6, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_FRONT_WINDOW_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_FRONT_WINDOW_TEX7, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX0, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX1, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX2, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX3, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX4, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX5, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX6, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_REAR_WINDOW_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_REAR_WINDOW_TEX7, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX0, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX1, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX2, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX3, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX4, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX5, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX6, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_HOOD_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_HOOD_TEX7, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX0, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX1, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX2, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX3, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX4, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX5, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX6, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_DOOR_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_DOOR_TEX7, -1);
        if (CarConfigs[ride_info->Type].RandomParts.MatchSideDecals)
        {
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX0, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX0]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX1, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX1]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX2, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX2]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX3, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX3]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX4, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX4]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX5, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX5]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX6, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX6]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_DOOR_TEX7, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_DOOR_TEX7]);
        }
        else
        {
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX0, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX3, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX4, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX5, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX6, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_DOOR_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_DOOR_TEX7, -1);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX0, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX1, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX2, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX3, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX4, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX5, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX6, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_LEFT_QUARTER_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_LEFT_QUARTER_TEX7, -1);
        if (CarConfigs[ride_info->Type].RandomParts.MatchSideDecals)
        {
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX0, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX0]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX1, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX1]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX2, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX2]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX3, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX3]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX4, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX4]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX5, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX5]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX6, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX6]);
			RideInfo_SetPart(ride_info, EDX_Unused, CARSLOTID_DECAL_RIGHT_QUARTER_TEX7, (DWORD*)ride_info->mPartsTable[CARSLOTID_DECAL_LEFT_QUARTER_TEX7]);
        }
        else
        {
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX0]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX0, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX1]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX1, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX2]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX2, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX3]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX3, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX4]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX4, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX5]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX5, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX6]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX6, -1);
            if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DECAL_RIGHT_QUARTER_TEX7]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DECAL_RIGHT_QUARTER_TEX7, -1);
        }
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_CUSTOM_HUD]) RideInfo_SetRandomPart(ride_info, CARSLOTID_CUSTOM_HUD, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HUD_BACKING_COLOUR]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HUD_BACKING_COLOUR, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HUD_NEEDLE_COLOUR]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HUD_NEEDLE_COLOUR, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HUD_CHARACTER_COLOUR]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HUD_CHARACTER_COLOUR, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_NEON]) RideInfo_SetRandomPart(ride_info, CARSLOTID_NEON, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_NEON_ENGINE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_NEON_ENGINE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_NEON_CABIN]) RideInfo_SetRandomPart(ride_info, CARSLOTID_NEON_CABIN, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_CABIN_NEON_FRAME]) RideInfo_SetRandomPart(ride_info, CARSLOTID_CABIN_NEON_FRAME, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_NEON_TRUNK]) RideInfo_SetRandomPart(ride_info, CARSLOTID_NEON_TRUNK, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_WINDOW_TINT]) RideInfo_SetRandomPart(ride_info, CARSLOTID_WINDOW_TINT, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HEADLIGHT_BULB]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HEADLIGHT_BULB, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_NOS_PURGE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_NOS_PURGE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_UL_EXHAUST_FLAME]) RideInfo_SetRandomPart(ride_info, CARSLOTID_UL_EXHAUST_FLAME, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_UL_TIRE_SMOKE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_UL_TIRE_SMOKE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_HYDRAULICS]) RideInfo_SetRandomPart(ride_info, CARSLOTID_HYDRAULICS, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_TRUNK_AUDIO]) RideInfo_SetRandomPart(ride_info, CARSLOTID_TRUNK_AUDIO, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_DOOR_STYLE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_DOOR_STYLE, -1);
        if (CarConfigs[ride_info->Type].RandomParts.Parts[CARSLOTID_LICENSE_PLATE]) RideInfo_SetRandomPart(ride_info, CARSLOTID_LICENSE_PLATE, -1);
	}
}

void __fastcall RideInfo_SyncBrakesWithPhysics(RideInfo* ride_info, void* EDX_Unused, int CarType)
{
    const BYTE* Installed = (const BYTE*)ride_info->mPhysics.mUpgrades.PerfPartInstalled;

    int Level = Installed[PERF_PART_BR_CROSS_DRILLED_ROTORS] 
        ? 1 : Installed[PERF_PART_BR_LARGE_DIAMETER_ROTORS]
        ? 2 : Installed[PERF_PART_BR_CROSS_DRILLED_AND_SLOTTED_ROTORS]
        ? 3 : 0;

    DWORD* Brake = FindPartWithLevel(CarType, CARSLOTID_FRONT_BRAKE, Level);
    if (!Brake) return;

    ride_info->mPartsTable[CARSLOTID_FRONT_BRAKE] = (CarPart*)Brake;
    RideInfo_UpdatePartsEnabled(ride_info, EDX_Unused);
}

void __fastcall RideInfo_SyncVisualPartsWithPhysics_Hook(RideInfo* ride_info, void* EDX_Unused, bool perf, bool random)
{
    if (!ride_info || ((uintptr_t)ride_info & 3)) return;

    int CarType = ride_info->Type;
    if (CarType < 0 || CarType >= CarCount) return;

    MainSection& M = CarConfigs[CarType].Main;

    // Snapshot the brake slots
    DWORD SavedFrontBrake = (DWORD)ride_info->mPartsTable[CARSLOTID_FRONT_BRAKE];
    DWORD SavedRearBrake = (DWORD)ride_info->mPartsTable[CARSLOTID_REAR_BRAKE];

    if (M.SyncVisualPartsWithPhysics) RideInfo_SyncVisualPartsWithPhysics((DWORD*)ride_info, perf, random);
    else if (random) RideInfo_BuildRandomRide(ride_info, EDX_Unused, random); // only randomize the parts, not the physics

    if (M.SyncBrakesWithPhysics)
    {
		RideInfo_SyncBrakesWithPhysics(ride_info, EDX_Unused, CarType);
    }
	else // restore the brake slots to their original values
    {
        ride_info->mPartsTable[CARSLOTID_FRONT_BRAKE] = (CarPart*)SavedFrontBrake;
        ride_info->mPartsTable[CARSLOTID_REAR_BRAKE] = (CarPart*)SavedRearBrake;

        MirrorFrontBrakeToRear(ride_info);
    }
}

DWORD g_displayHUDprefix = CT_bStringHash("3RDPERSON_");
DWORD* g_displayHUDattributes = (DWORD*)0x839BF0;

void GetRidePartAttributes()
{
    RideInfo* ride_info = (RideInfo*)gTheRideInfo;

    DWORD* part = (DWORD*)ride_info->mPartsTable[CARSLOTID_CUSTOM_HUD];
    if (part)
    {
        g_displayHUDattributes[0] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("HUDINDEX"), 0);
        g_displayHUDprefix = bStringHash2("_", CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("TEXTURE_NAME"), CT_bStringHash("3RDPERSON")));
    }
    
    part = (DWORD*)ride_info->mPartsTable[CARSLOTID_HUD_BACKING_COLOUR];
    if (part)
    {
        g_displayHUDattributes[13] = (DWORD)part;
        g_displayHUDattributes[3] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("RED"), 0);
        g_displayHUDattributes[2] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("GREEN"), 0);
        g_displayHUDattributes[1] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BLUE"), 0);
    }

    part = (DWORD*)ride_info->mPartsTable[CARSLOTID_HUD_NEEDLE_COLOUR];
    if (part)
    {
        g_displayHUDattributes[15] = (DWORD)part;
        g_displayHUDattributes[11] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("RED"), 0);
        g_displayHUDattributes[10] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("GREEN"), 0);
        g_displayHUDattributes[9] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BLUE"), 0);
    }

    part = (DWORD*)ride_info->mPartsTable[CARSLOTID_HUD_CHARACTER_COLOUR];
    if (part)
    {
        g_displayHUDattributes[14] = (DWORD)part;
        g_displayHUDattributes[7] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("RED"), 0);
        g_displayHUDattributes[6] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("GREEN"), 0);
        g_displayHUDattributes[5] = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BLUE"), 0);
    }
}

void __fastcall RideInfo_SetCompositeNameHash(RideInfo* ride_info, void* EDX_Unused, int id)
{
    char buf[64]; // [esp+8h] [ebp-40h] BYREF
    CarTypeInfo* cti = &CarTypeInfoArray[ride_info->Type];

    bSPrintf(buf, "DUMMY_SKIN%d", id);
    ride_info->mCompositeSkinHash = cti->IsSkinnable() ? bStringHash(buf) : 0;
    bSPrintf(buf, "DUMMY_WHEEL%d", id);
    ride_info->mCompositeWheelHash = bStringHash(buf);
    bSPrintf(buf, "DUMMY_SPINNER%d", id);
    ride_info->mCompositeSpinnerHash = bStringHash(buf);
}