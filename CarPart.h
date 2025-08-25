#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

float __fastcall CarPart_GetAppliedAttributeFParam(DWORD* _CarPart, void* EDX_Unused, DWORD namehash, float default_value)
{
    if (_CarPart)
    {
        float* Attribute = (float*)CarPart_GetNextAppliedAttribute(_CarPart, namehash, 0);
        if (Attribute) return Attribute[1];
    }
    return default_value;
}

int __fastcall CarPart_TrunkAudioSlotAvailable(DWORD* _CarPart, void* EDX_Unused, int CarSlotID)
{
    int result = 0;
    
    if (!_CarPart) return result;

    // Unlimiter NUMSLOTS attribute
	result = CarPart_GetAppliedAttributeUParam(_CarPart, CT_bStringHash("NUMSLOTS"), -1);
    if (result != -1) return CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO + result;

    // Vanilla implementation
    BYTE UpgradeLevel = *((BYTE*)_CarPart + 5) >> 5;
    if (!UpgradeLevel) return 0;

	DWORD CarTypeNameHash = CarPart_GetCarTypeNameHash(_CarPart);

    switch (UpgradeLevel)
    {
    case 3:
        switch (CarTypeNameHash)
        {
		case CT_bStringHash("CIVIC"):
		case CT_bStringHash("CELICA"):
		case CT_bStringHash("SUPRA"):
		case CT_bStringHash("RSX"):
		case CT_bStringHash("350Z"):
		case CT_bStringHash("GTO"):
		case CT_bStringHash("MUSTANGGT"):
		case CT_bStringHash("LANCEREVO8"):
		case CT_bStringHash("IMPREZAWRX"):
			result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_5; // 6 slots
            break;
        case CT_bStringHash("HUMMER"):
        case CT_bStringHash("NAVIGATOR"):
        case CT_bStringHash("ESCALADE"):
            result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_11; // 12 slots
            break;
        default:
            result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_4; // 5 slots
            break;
        }
        break;
    case 2:
        switch (CarTypeNameHash)
        {
        case CT_bStringHash("HUMMER"):
        case CT_bStringHash("NAVIGATOR"):
        case CT_bStringHash("ESCALADE"):
            result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_5; // 6 slots
            break;
        default:
            result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_3; // 4 slots
            break;
        }
        break;
    case 1:
        switch (CarTypeNameHash)
        {
        case CT_bStringHash("HUMMER"):
        case CT_bStringHash("NAVIGATOR"):
        case CT_bStringHash("ESCALADE"):
            result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_3; // 4 slots
            break;
        default:
            result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_1; // 2 slots
            break;
        }
        break;
    default:
		result = CarSlotID <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_0; // 1 slot
        break;
    }

    return result;
}