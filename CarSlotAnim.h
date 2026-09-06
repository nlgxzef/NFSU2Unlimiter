#pragma once

enum CarSlotAnimLocation : BYTE
{
    None = 0xFF,
    Hood = 0,
    Trunk = 1,
    LeftDoor = 2,
    RightDoor = 3
};

struct CarSlotAnim
{
    CarSlotAnimLocation primary;
    CarSlotAnimLocation secondary;
};

struct CarSlotAnimHookup
{
    CarSlotAnim slots[CARSLOTID_NUM];
};

CarSlotAnimHookup* CarSlotAnimHookupTable;
CarSlotAnimHookup CarSlotAnimHookupTable_ExtraAttachments;

void CarSlotAnimHookup_InitForExtraAttachments()
{
	// Copy old hookup table to new one
	memcpy(&CarSlotAnimHookupTable_ExtraAttachments, CarSlotAnimHookupTable, sizeof(CarSlotAnimHookup));
    
    for (int i = 0; i < 6; i++)
    {
        CarSlotAnimHookupTable_ExtraAttachments.slots[ExtraAttachmentSlots[i]].primary = CarSlotAnimLocation::None;
        CarSlotAnimHookupTable_ExtraAttachments.slots[ExtraAttachmentSlots[i]].secondary = CarSlotAnimLocation::None;
	}
}

// Unused

struct CarSlotAnimHide
{
    BYTE data[0x80];
};

CarSlotAnimHide* CarSlotAnimHideOpenTable;
CarSlotAnimHide* CarSlotAnimHideClosedTable;