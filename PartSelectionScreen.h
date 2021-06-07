#include "stdio.h"
#include "InGameFunctions.h"
#include "ChooseRimBrand.h"
#include "includes\IniReader.h"

void __fastcall PartSelectionScreen_SetupBodyShop(DWORD* PartSelectionScreen, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int TuningCarType = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    sprintf(CarTypeName, GetCarTypeName(TuningCarType));
    sprintf(CarININame, "UnlimiterData\\%s.ini", CarTypeName);
    CIniReader CarINI(CarININame);
    CIniReader GeneralINI("UnlimiterData\\_General.ini");

    char const* PartSelectionScreenPackage = (char const*)PartSelectionScreen[1];

    PartSelectionScreen_ResetCategories(&PartSelectionScreen[19]);
    if (CarINI.ReadInteger("BodyShop", "FrontBumper", GeneralINI.ReadInteger("BodyShop", "FrontBumper", 1)) != 0)     PartSelectionScreen_AddCategory(PartSelectionScreen, 1, bStringHash("VISUAL_PART_FRONT_BUMPER"), bStringHash("VISUAL_PART_FRONT_BUMPER"), 0); // FRONT_BUMPER
    if (CarINI.ReadInteger("BodyShop", "RearBumper", GeneralINI.ReadInteger("BodyShop", "RearBumper", 1)) != 0)       PartSelectionScreen_AddCategory(PartSelectionScreen, 2, bStringHash("VISUAL_PART_REAR_BUMPER"), bStringHash("VISUAL_PART_REAR_BUMPER"), 0); // REAR_BUMPER
    if (CarINI.ReadInteger("BodyShop", "Skirt", GeneralINI.ReadInteger("BodyShop", "Skirt", 1)) != 0)                 PartSelectionScreen_AddCategory(PartSelectionScreen, 11, bStringHash("VISUAL_PART_SIDE_SKIRT"), bStringHash("VISUAL_PART_SIDE_SKIRT"), 0); // SKIRT
    if (CarINI.ReadInteger("BodyShop", "Fender", GeneralINI.ReadInteger("BodyShop", "Fender", 0)) != 0)               PartSelectionScreen_AddCategory(PartSelectionScreen, 23, bStringHash("VISUAL_PART_FRONT_FENDER"), bStringHash("VISUAL_PART_FRONT_FENDER"), 0); // FENDER
    if (CarINI.ReadInteger("BodyShop", "Quarter", GeneralINI.ReadInteger("BodyShop", "Quarter", 0)) != 0)             PartSelectionScreen_AddCategory(PartSelectionScreen, 24, 0x9F135643, bStringHash("VISUAL_PART_REAR_QUARTER"), 0); // QUARTER, VISUAL_PART_REAR_QUARTE??
    if (CarINI.ReadInteger("BodyShop", "Spoiler", GeneralINI.ReadInteger("BodyShop", "Spoiler", 1)) != 0)             PartSelectionScreen_AddCategory(PartSelectionScreen, 12, bStringHash("VISUAL_PART_SPOILER"), bStringHash("VISUAL_PART_SPOILER"), 0); // SPOILER
    if (CarINI.ReadInteger("BodyShop", "Hood", GeneralINI.ReadInteger("BodyShop", "Hood", 1)) != 0)                   PartSelectionScreen_AddCategory(PartSelectionScreen, 9, bStringHash("VISUAL_PART_HOOD"), bStringHash("VISUAL_PART_HOOD"), 0); // HOOD
    if (CarINI.ReadInteger("BodyShop", "Trunk", GeneralINI.ReadInteger("BodyShop", "Trunk", 0)) != 0)                 PartSelectionScreen_AddCategory(PartSelectionScreen, 10, bStringHash("VISUAL_PART_TRUNK"), bStringHash("VISUAL_PART_TRUNK"), 0); // TRUNK
    if (CarINI.ReadInteger("BodyShop", "RoofScoops", GeneralINI.ReadInteger("BodyShop", "RoofScoops", 1)) != 0)       PartSelectionScreen_AddCategory(PartSelectionScreen, 7, bStringHash("VISUAL_PART_ROOF_SCOOP"), bStringHash("VISUAL_PART_ROOF_SCOOP"), 0); // ROOF
    if (CarINI.ReadInteger("BodyShop", "Interior", GeneralINI.ReadInteger("BodyShop", "Interior", 0)) != 0)           PartSelectionScreen_AddCategory(PartSelectionScreen, 0, bStringHash("VISUAL_PART_CABIN"), bStringHash("VISUAL_PART_CABIN"), 0); // BASE
    if (CarINI.ReadInteger("BodyShop", "Roof", GeneralINI.ReadInteger("BodyShop", "Roof", 0)) != 0)                   PartSelectionScreen_AddCategory(PartSelectionScreen, 8, bStringHash("VISUAL_PART_ROOF"), bStringHash("VISUAL_PART_ROOF"), 0); // TOP
    if (CarINI.ReadInteger("BodyShop", "Headlights", GeneralINI.ReadInteger("BodyShop", "Headlights", 1)) != 0)       PartSelectionScreen_AddCategory(PartSelectionScreen, 14, bStringHash("VISUAL_PART_HEAD_LIGHTS"), bStringHash("VISUAL_PART_HEAD_LIGHT"), 0); // HEADLIGHT
    if (CarINI.ReadInteger("BodyShop", "Taillights", GeneralINI.ReadInteger("BodyShop", "Taillights", 1)) != 0)       PartSelectionScreen_AddCategory(PartSelectionScreen, 15, bStringHash("VISUAL_PART_TAIL_LIGHTS"), bStringHash("VISUAL_PART_TAIL_LIGHT"), 0); // BRAKELIGHT
    if (CarINI.ReadInteger("BodyShop", "Mirrors", GeneralINI.ReadInteger("BodyShop", "Mirrors", 1)) != 0)             PartSelectionScreen_AddCategory(PartSelectionScreen, 32, bStringHash("VISUAL_PART_SIDE_MIRRORS"), bStringHash("VISUAL_PART_SIDE_MIRROR"), 0); // WING_MIRROR
    if (CarINI.ReadInteger("BodyShop", "Exhaust", GeneralINI.ReadInteger("BodyShop", "Exhaust", 1)) != 0)             PartSelectionScreen_AddCategory(PartSelectionScreen, 16, bStringHash("VISUAL_PART_MUFFLERTIP"), bStringHash("VISUAL_PART_MUFFLER_TIP"), 0); // EXHAUST
    if (CarINI.ReadInteger("BodyShop", "Rims", GeneralINI.ReadInteger("BodyShop", "Rims", 1)) != 0)                   PartSelectionScreen_AddCategoryRims(PartSelectionScreen, bStringHash("VISUAL_PART_RIMS"), bStringHash("VISUAL_PART_RIMS"));
    if (CarINI.ReadInteger("BodyShop", "CarbonFiber", GeneralINI.ReadInteger("BodyShop", "CarbonFiber", 1)) != 0)     PartSelectionScreen_AddCategoryCarbon(PartSelectionScreen, bStringHash("VISUAL_PART_CARBON_FIBRE"), bStringHash("VISUAL_PART_CARBON_FIBRE"));
    if (CarINI.ReadInteger("BodyShop", "WideBodyKits", GeneralINI.ReadInteger("BodyShop", "WideBodyKits", 1)) != 0)   PartSelectionScreen_AddCategory(PartSelectionScreen, 6, bStringHash("VISUAL_PART_WIDE_BODY_KIT"), bStringHash("VISUAL_PART_WIDE_BODY_KIT"), 0); // WIDE_BODY
    
    int NumAttachments = CarINI.ReadInteger("BodyShop", "Attachments", GeneralINI.ReadInteger("BodyShop", "Attachments", 0));
    for (int i = 1; i <= NumAttachments; i++)
    {
        sprintf(AttachmentNameBuf, "VISUAL_PART_ATTACHMENT%d", i);
        PartSelectionScreen_AddCategory(PartSelectionScreen, 46 + i, bStringHash(AttachmentNameBuf), bStringHash(AttachmentNameBuf), 0);
    }

    if (*(BYTE*)0x839BC0)
    {
        *(BYTE*)0x839BC0 = 0;
        FEngSetLastButton(PartSelectionScreenPackage, 29);
    }
    else if (FEngGetLastButton(PartSelectionScreenPackage) == -1)
    {
        FEngSetLastButton(PartSelectionScreenPackage, 1);
    }
    unsigned int LastButton = FEngGetLastButton(PartSelectionScreenPackage);
    ThumbnailScroller_SnapToItem(&PartSelectionScreen[19], LastButton);
}

int GetPartsList(int CarSlotID, DWORD* PartsBList, unsigned int PartAttribFilter, unsigned int Unk)
{
    DWORD* NewBNode; // eax MAPDST
    DWORD* BTail; // ecx MAPDST
    int PartsCount = 0; // eax MAPDST

    int SomethingUnk = *(int*)0x8389B0;
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int TuningCarType = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    int CarPartID = GetCarPartFromSlot(CarSlotID);

    DWORD* TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, TuningCarType, CarSlotID, 0, -1);

    switch (CarSlotID)
    {
    case 0:  // BASE
    case 1:  // FRONT_BUMPER
    case 2:  // REAR_BUMPER
    case 6:  // WIDE_BODY
    case 7:  // ROOF
    case 8:  // TOP
    case 9:  // HOOD
    case 10: // TRUNK
    case 11: // SKIRT
    case 12: // SPOILER
    case 14: // HEADLIGHT
    case 15: // BRAKELIGHT
    case 16: // EXHAUST
    case 23: // FENDER
    case 24: // QUARTER
    case 32: // WING_MIRROR
    case 47: // DAMAGE_FRONT
    case 48: // DAMAGE_REAR
    case 49: // DAMAGE_LEFT
    case 50: // DAMAGE_RIGHT
    case 51: // DAMAGE_TOP
        while (TheCarPart)
        {
            unsigned int IsCF = CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CARBONFIBRE"), 0) != 0 ? 666 : 0;
            if (*((char*)TheCarPart + 4) == CarPartID && PartAttribFilter == IsCF)
            {
                if (UnlockSystem_IsCarPartUnlocked(CarCustomizeManager_GetPartUnlockFilter(), CarSlotID, TheCarPart, SomethingUnk)
                    && (CarSlotID != 9 || (*((BYTE*)TheCarPart + 5) & 0x1F) != 5))
                {
                    NewBNode = (DWORD*)j__malloc(0x10u);
                    if (NewBNode)
                    {
                        NewBNode[2] = (DWORD)TheCarPart;
                        NewBNode[3] = 0;

                        BTail = (DWORD*)PartsBList[1];
                        *BTail = (DWORD)NewBNode;
                        PartsBList[1] = (DWORD)NewBNode;
                        NewBNode[1] = (DWORD)BTail;
                        *NewBNode = (DWORD)PartsBList;

                        ++PartsCount;
                    }
                    else
                    {
                        NewBNode = 0;
                    }
                }
            }

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, TuningCarType, CarSlotID, 0, -1);
        }
        break;
    case 29: // FRONT_WHEEL
    case 30: // REAR_WHEEL
        while (TheCarPart)
        {
            if (*((char*)TheCarPart + 4) == CarPartID && IsRimAvailable(TuningCarType, TheCarPart, PartAttribFilter))
            {
                NewBNode = (DWORD*)j__malloc(0x10u);
                if (NewBNode)
                {
                    NewBNode[2] = (DWORD)TheCarPart;
                    NewBNode[3] = 0;

                    BTail = (DWORD*)PartsBList[1];
                    *BTail = (DWORD)NewBNode;
                    PartsBList[1] = (DWORD)NewBNode;
                    NewBNode[1] = (DWORD)BTail;
                    *NewBNode = (DWORD)PartsBList;

                    ++PartsCount;
                }
                else
                {
                    NewBNode = 0;
                }
            }

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, TuningCarType, CarSlotID, 0, -1);
        }
        break;
    case 52: // DECAL_HOOD
    case 53: // DECAL_FRONT_WINDOW
    case 54: // DECAL_REAR_WINDOW
    case 55: // DECAL_LEFT_DOOR
    case 56: // DECAL_RIGHT_DOOR
    case 57: // DECAL_LEFT_QUARTER
    case 58: // DECAL_RIGHT_QUARTER
        while (TheCarPart)
        {
            NewBNode = (DWORD*)j__malloc(0x10u);
            if (NewBNode)
            {
                NewBNode[2] = (DWORD)TheCarPart;
                NewBNode[3] = 0;

                BTail = (DWORD*)PartsBList[1];
                *BTail = (DWORD)NewBNode;
                PartsBList[1] = (DWORD)NewBNode;
                NewBNode[1] = (DWORD)BTail;
                *NewBNode = (DWORD)PartsBList;

                ++PartsCount;
            }
            else
            {
                NewBNode = 0;
            }

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, TuningCarType, CarSlotID, 0, -1);
        }
        break;
    case 63:  // BASE_PAINT
    case 68:  // PAINT_ENGINE
    case 69:  // PAINT_SPOILER
    case 70:  // PAINT_BRAKE
    case 71:  // PAINT_EXHAUST
    case 72:  // PAINT_AUDIO
    case 73:  // PAINT_RIM
    case 74:  // PAINT_SPINNER
    case 75:  // PAINT_ROOF
    case 76:  // PAINT_MIRROR
    case 77:  // VINYL_COLOUR0_0
    case 78:  // VINYL_COLOUR0_1
    case 79:  // VINYL_COLOUR0_2
    case 80:  // VINYL_COLOUR0_3
    case 81:  // VINYL_COLOUR1_0
    case 82:  // VINYL_COLOUR1_1
    case 83:  // VINYL_COLOUR1_2
    case 84:  // VINYL_COLOUR1_3
    case 85:  // VINYL_COLOUR2_0
    case 86:  // VINYL_COLOUR2_1
    case 87:  // VINYL_COLOUR2_2
    case 88:  // VINYL_COLOUR2_3
    case 89:  // VINYL_COLOUR3_0
    case 90:  // VINYL_COLOUR3_1
    case 91:  // VINYL_COLOUR3_2
    case 92:  // VINYL_COLOUR3_3
    case 164: // HUD_BACKING_COLOUR
    case 165: // HUD_NEEDLE_COLOUR
    case 166: // HUD_CHARACTER_COLOUR
        while (TheCarPart)
        {
            if (*((char*)TheCarPart + 4) == CarPartID
                && CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("BRAND_NAME"), 0) == PartAttribFilter)
            {
                NewBNode = (DWORD*)j__malloc(0x10u);
                if (NewBNode)
                {
                    NewBNode[2] = (DWORD)TheCarPart;
                    NewBNode[3] = 0;

                    BTail = (DWORD*)PartsBList[1];
                    *BTail = (DWORD)NewBNode;
                    PartsBList[1] = (DWORD)NewBNode;
                    NewBNode[1] = (DWORD)BTail;
                    *NewBNode = (DWORD)PartsBList;

                    ++PartsCount;
                }
                else
                {
                    NewBNode = 0;
                }
            }

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, TuningCarType, CarSlotID, 0, -1);
        }
        break;
    case 64: // VINYL_LAYER0
    case 65: // VINYL_LAYER1
    case 66: // VINYL_LAYER2
    case 67: // VINYL_LAYER3
        while (TheCarPart)
        {
            if (*((char*)TheCarPart + 4) == CarPartID && (*((BYTE*)TheCarPart + 5) & 0x1F) == PartAttribFilter)
            {
                if (UnlockSystem_IsCarPartUnlocked(CarCustomizeManager_GetPartUnlockFilter(), CarSlotID, TheCarPart, SomethingUnk)
                    && ((*((BYTE*)TheCarPart + 5) & 0x1F) != 22))
                {
                    NewBNode = (DWORD*)j__malloc(0x10u);
                    if (NewBNode)
                    {
                        NewBNode[2] = (DWORD)TheCarPart;
                        NewBNode[3] = 0;

                        BTail = (DWORD*)PartsBList[1];
                        *BTail = (DWORD)NewBNode;
                        PartsBList[1] = (DWORD)NewBNode;
                        NewBNode[1] = (DWORD)BTail;
                        *NewBNode = (DWORD)PartsBList;

                        ++PartsCount;
                    }
                    else
                    {
                        NewBNode = 0;
                    }
                }
            }

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, TuningCarType, CarSlotID, 0, -1);
        }
        break;
    default:
        return PartsCount;
    }
    return PartsCount;
}

// 0x566B7A
void __declspec(naked) WidebodyWarningCodeCave()
{
    _asm
    {
        cmp edi, 1
        je loc_566B8D
        cmp edi, 2
        jz loc_566B8D
        cmp edi, 11
        jz loc_566B8D
        cmp edi, 23
        jz loc_566B8D
        cmp edi, 24
        jnz loc_566C10

        loc_566B8D:
            push 0x566B8D
            retn

        loc_566C10:
            push 0x566C10
            retn
    }
}