#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#pragma pack(push, 1)

struct CarPart
{
    DWORD PartNameHash;
    BYTE PartID;
    BYTE GroupNumber_UpgradeLevel;
    BYTE BaseModelNameHashSelector;
    BYTE CarTypeNameHashIndex;
    WORD NameOffset;
    WORD AttributeTableOffset;
    WORD ModelNameHashTableOffset;
};

#pragma pack(pop)

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
    if (result != -1) return CarSlotID <= CARSLOTID_TRUNK_AUDIO + result;

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
			result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_5; // 6 slots
            break;
        case CT_bStringHash("HUMMER"):
        case CT_bStringHash("NAVIGATOR"):
        case CT_bStringHash("ESCALADE"):
            result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_11; // 12 slots
            break;
        default:
            result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_4; // 5 slots
            break;
        }
        break;
    case 2:
        switch (CarTypeNameHash)
        {
        case CT_bStringHash("HUMMER"):
        case CT_bStringHash("NAVIGATOR"):
        case CT_bStringHash("ESCALADE"):
            result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_5; // 6 slots
            break;
        default:
            result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_3; // 4 slots
            break;
        }
        break;
    case 1:
        switch (CarTypeNameHash)
        {
        case CT_bStringHash("HUMMER"):
        case CT_bStringHash("NAVIGATOR"):
        case CT_bStringHash("ESCALADE"):
            result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_3; // 4 slots
            break;
        default:
            result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_1; // 2 slots
            break;
        }
        break;
    default:
		result = CarSlotID <= CARSLOTID_TRUNK_AUDIO_COMP_0; // 1 slot
        break;
    }

    return result;
}

bool __fastcall CarPart_HasExcludeDecal(DWORD* _CarPart, void* EDX_Unused, int CarSlotID)
{
	DWORD SlotNameHash = CarPart_GetAppliedAttributeUParam(_CarPart, CT_bStringHash("EXCLUDEDECAL"), 0);
	if (!SlotNameHash) return false;

    return SlotNameHash == bStringHash(GetCarSlotIDName(CarSlotID));
}

int __fastcall CarPart_GetExcludeDecal(DWORD* _CarPart, void* EDX_Unused)
{
    int slot = -1;
    DWORD SlotNameHash = CarPart_GetAppliedAttributeUParam(_CarPart, CT_bStringHash("EXCLUDEDECAL"), 0);
    if (!SlotNameHash) return slot;

    for (int i = CARSLOTID_MODEL_FIRST; i < CARSLOTID_NUM; i++)
    {
        if (SlotNameHash == bStringHash(GetCarSlotIDName(i)))
        {
            slot = i;
            break;
        }
	}

    return slot;
}