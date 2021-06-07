#include "stdio.h"
#include "InGameFunctions.h"

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

    DWORD* CarPartIDNames = (DWORD*)_CarPartIDNames;
    DWORD* CarSlotIDNames = (DWORD*)_CarSlotIDNames;

    CarType = *RideInfo;
    memset(RideInfo + 526, 1u, 0xA8u);
    *((WORD*)RideInfo + 1136) = 257;
    for (i = 0; i < 170; ++i)
    {
        TheCarPart = (DWORD*)RideInfo[i + 356];
        if (TheCarPart)
        {
            if (CarPart_HasAppliedAttribute(TheCarPart, bStringHash("EXCLUDEDECAL")))
            {
                for (j = CarPart_GetNextAppliedAttribute(TheCarPart, bStringHash("EXCLUDEDECAL"), 0); // GetFirstAppliedAttribute
                    j;
                    j = CarPart_GetNextAppliedAttribute(TheCarPart, bStringHash("EXCLUDEDECAL"), j))
                {
                    CarPartIDName = j[1];
                    for (k = 0; k < 170; ++k)
                    {
                        if (bStringHash((char const*)CarPartIDNames[2 * k + 1]) == CarPartIDName)
                            break;
                    }
                    if (k != 69)
                        *((BYTE*)RideInfo + CarSlotIDNames[2 * k] + 2104) = 0;
                }
            }
        }

        switch (i)
        {
        case 161: // HYDRAULICS
            if (TheCarPart)
                *((BYTE*)RideInfo + 1409) = *((BYTE*)TheCarPart + 5) >> 5;
            break;

        case 29: // FRONT_WHEEL
            if (IsBrowsingBrakePaint())
                *((BYTE*)RideInfo + 2104 + 29) = 0; // FRONT_WHEEL visibility
            break;

        case 9: // HOOD
            HoodPart = (DWORD*)RideInfo[356 + 9];
            if (HoodPart)
            {
                HoodUnderPartHash = CarPart_GetAppliedAttributeUParam(HoodPart, bStringHash("HOODUNDER"), 0);
                if (HoodUnderPartHash)
                    RideInfo[356 + 25] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 25, HoodUnderPartHash, 0, -1); // HOOD_UNDER
                else
                    RideInfo[356 + 25] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 25, 0, 0, -1);

                // Show engine if our custom attribute is present
                ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, bStringHash("SHOWENGINE"), 0);
                if (ShowEngineThruHood)
                    *((BYTE*)RideInfo + 2104 + 13) = 1; // ENGINE visibility
            }
            break;

        case 10: // TRUNK
            TrunkPart = (DWORD*)RideInfo[356 + 10];
            if (TrunkPart)
            {
                TrunkUnderPartHash = CarPart_GetAppliedAttributeUParam(TrunkPart, bStringHash("TRUNKUNDER"), 0);
                if (TrunkUnderPartHash)
                    RideInfo[356 + 26] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 26, TrunkUnderPartHash, 0, -1); // HOOD_UNDER
                else
                    RideInfo[356 + 26] = (DWORD)CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarType, 26, 0, 0, -1);

                // Show engine if our custom attribute is present
                ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, bStringHash("SHOWTRUNK"), 0);
                if (ShowAudioThruTrunk)
                    *((BYTE*)RideInfo + 2104 + 34) = 1; // TRUNK_AUDIO visibility
            }
            break;

        case 13: // ENGINE
            if (TheCarPart && *(int*)_TheGameFlowManager == 3)
            {
                *((BYTE*)RideInfo + 2104 + 9) = 1; // HOOD visibility
                *((BYTE*)RideInfo + 2104 + 25) = 1; // HOOD_UNDER visibility
                *((BYTE*)RideInfo + 2104 + 52) = 1; // DECAL_HOOD visibility
                *((BYTE*)RideInfo + 2104 + 13) = 1; // ENGINE visibility
            }
            break;

        case 8: // TOP
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
                for (int i = 0; i <= 99; i++)
                {
                    sprintf(KitNameBuf, "%s_STYLE%02d_", GetCarTypeName(CarType), i);
                    KitNamePartialHash = bStringHash(KitNameBuf);
                    PartHash = bStringHash2("TOP", KitNamePartialHash);
                    if (*TheCarPart == PartHash)
                    {
                        break;
                    }
                    if (i == 99) // Set stock parts if invalid
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

        case 24: // QUARTER
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
       
        case 6: // WIDE_BODY
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

                sprintf(PartNameBuf, "%s_KITW0%d_DOOR_", CarTypeName, KitNumber);
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
}

void __fastcall RideInfo_SetPart(DWORD* RideInfo, void* EDX_Unused, int CarSlotID, DWORD* CarPartToSet)
{
	if (RideInfo && CarSlotID != 25) // HOOD_UNDER
	{
		RideInfo[CarSlotID + 356] = (DWORD)CarPartToSet;
		RideInfo_UpdatePartsEnabled(RideInfo, EDX_Unused);
	}
}