#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

// 0x563C4C
void __declspec(naked) PushCantPaintTrunkDialogText()
{
    _asm
    {
        mov ebx, 0x740E3098 // CUSTOMIZE_CANT_PAINT_STOCK_TRUNK
        push 0x5636CD
        retn
    }
}

// 0x55CEB0
void __declspec(naked) TrunkAnimCodeCave_ChoosePaintScreen_StopBrowsingPaints()
{
    _asm
    {
        cmp eax, 0x48
        jz TrunkAnimClose
        cmp eax, 0x44
        jnz loc_55CEE9

        EngineAnimClose:
            push 0x55CEB5
            retn

        TrunkAnimClose:
            mov ecx, dword ptr ds : [_FECarConfigRef]
            mov eax, dword ptr ds : [ecx]
            call dword ptr ds : [eax + 8]
            push eax
            call IsCarPartsAnimLoadedForCar
            add esp, 4
            test al, al
            jz loc_55CEE0
            push 0x3F800000
            push 0
            push 1
            call FEDoCarPartAnimNow
            add esp, 0x0C
            push 0x55CEF5
            retn

        loc_55CEE0 :
            push 0x55CEE0
            retn

        loc_55CEE9:
            push 0x55CEE9
            retn
    }
}

// 0x55CCBE
void __declspec(naked) TrunkAnimCodeCave_ChoosePaintScreen_StartBrowsingPaints()
{
    _asm
    {
        mov dword ptr ds: [esi + 0x28], 0x4E4485FE
        mov byte ptr ds: [esi + 0x188], 1
        cmp edi, 0x48
        jz TrunkAnimOpen
        cmp edi, 0x44

        EngineAnimOpen:
            push 0x55CCCF
            retn

        TrunkAnimOpen:
            mov ecx, dword ptr ds: [_FECarConfigRef]
            mov eax, dword ptr ds : [ecx]
            call dword ptr ds: [eax+8]
            push eax
            call IsCarPartsAnimLoadedForCar
            add esp,4
            test al,al
            jz loc_55CCFC
            push 0x3F800000
            push 1
            push 1
            call FEDoCarPartAnimNow
            add esp, 0x0C
            push 0x55CD11
            retn

        loc_55CCFC:
            push 0x55CCFC
            retn
    }
}

bool __fastcall ChoosePaintScreen_CanPaintThisPart(DWORD* ChoosePaintScreen, void* EDX_Unused)
{
    bool result = 1; // al
    int CategoryNode; // eax MAPDST
    int CarSlotID; // ecx
    int CarSlotIDToCheck; // esi
    DWORD* TheCarPart; // eax MAPDST
    int State = *(DWORD*)__PaintMenuState;

    if (State == 1)
        return 1;
    CategoryNode = ChoosePaintScreen[22];
    CarSlotID = CAR_SLOT_ID::__LAST_NONRENDER;
    if (CategoryNode)
        CarSlotID = *(DWORD*)(CategoryNode + 72);
    switch (CarSlotID)
    {
    case CAR_SLOT_ID::BASE_PAINT:
        return 1;
    case CAR_SLOT_ID::PAINT_ENGINE:
        return CarCustomizeManager_IsEnginePaintable((DWORD*)gCarCustomizeManager, EDX_Unused);
    case CAR_SLOT_ID::PAINT_SPOILER:
        CarSlotIDToCheck = CAR_SLOT_ID::SPOILER;
        goto CheckIfPaintable;
    case CAR_SLOT_ID::PAINT_BRAKE:
        return CarCustomizeManager_AreBrakesPaintable((DWORD*)gCarCustomizeManager, EDX_Unused);
    case CAR_SLOT_ID::PAINT_EXHAUST:
        CarSlotIDToCheck = CAR_SLOT_ID::EXHAUST;
        goto CheckIfPaintable;
    case CAR_SLOT_ID::PAINT_AUDIO:
        return CarCustomizeManager_IsTrunkPaintable((DWORD*)gCarCustomizeManager, EDX_Unused);
    case CAR_SLOT_ID::PAINT_RIM:
    case CAR_SLOT_ID::PAINT_SPINNER:
        CarSlotIDToCheck = CAR_SLOT_ID::FRONT_WHEEL;
        goto CheckIfPaintable;
    case CAR_SLOT_ID::PAINT_ROOF:
        CarSlotIDToCheck = CAR_SLOT_ID::ROOF;
        goto CheckIfPaintable;
    case CAR_SLOT_ID::PAINT_MIRROR:
        CarSlotIDToCheck = CAR_SLOT_ID::WING_MIRROR;
    CheckIfPaintable:
        TheCarPart = RideInfo_GetPart((DWORD*)gTheRideInfo, CarSlotIDToCheck);
        if (!TheCarPart
            || (*((BYTE*)TheCarPart + 5) & 0xE0) == 0
            || (CarSlotIDToCheck == CAR_SLOT_ID::SPOILER 
                || CarSlotIDToCheck == CAR_SLOT_ID::ROOF 
                || CarSlotIDToCheck == CAR_SLOT_ID::WING_MIRROR) // SPOILER, ROOF, WING_MIRROR
            && (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("CARBONFIBRE"), 0)
                || CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("UNPAINTABLE"), 0))) // UNPAINTABLE
        {
            return 0;
        }
        if (CategoryNode)
        {
            if (CarSlotID == CAR_SLOT_ID::SPINNER)
            {
                 result = CarPart_HasAppliedAttribute(TheCarPart, CT_bStringHash("SPINNER_TEXTURE"))
                     || (TheCarPart != RideInfo_GetPart((DWORD*)gTheRideInfo, CAR_SLOT_ID::REAR_WHEEL));
            }
        }
        else
            result = 1;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}


DWORD __fastcall ChoosePaintScreen_GetPaintBrandHashFromCarSlotId(DWORD* ChoosePaintScreen, void* EDX_Unused, int CarSlotID)
{
    unsigned int FirstAvailableType = 0; // eax

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    int Min = CarConfigs[CarTypeID].Paint.PaintCustom ? 0 : 1;

    for (int i = Min; i < PaintGroups.size(); i++)
    {
        switch (CarSlotID)
        {
        case CAR_SLOT_ID::BASE_PAINT:
        case CAR_SLOT_ID::PAINT_SPOILER:
        case CAR_SLOT_ID::PAINT_ROOF:
        case CAR_SLOT_ID::PAINT_MIRROR:
        case CAR_SLOT_ID::PAINT_AUDIO:
            if (PaintGroups[i].AvailableForBody) FirstAvailableType = PaintGroups[i].BrandNameHash;
            break;
        case CAR_SLOT_ID::PAINT_ENGINE:
            if (PaintGroups[i].AvailableForHoses) FirstAvailableType = PaintGroups[i].BrandNameHash;
            break;
        case CAR_SLOT_ID::PAINT_BRAKE:
            if (PaintGroups[i].AvailableForBrakes) FirstAvailableType = PaintGroups[i].BrandNameHash;
            break;
        case CAR_SLOT_ID::PAINT_EXHAUST:
            if (PaintGroups[i].AvailableForMuffler) FirstAvailableType = PaintGroups[i].BrandNameHash;
            break;
        case CAR_SLOT_ID::PAINT_RIM:
        case CAR_SLOT_ID::PAINT_SPINNER:
            if (PaintGroups[i].AvailableForRims) FirstAvailableType = PaintGroups[i].BrandNameHash;
            break;
        case CAR_SLOT_ID::VINYL_COLOUR0_0:
        case CAR_SLOT_ID::VINYL_COLOUR0_1:
        case CAR_SLOT_ID::VINYL_COLOUR0_2:
        case CAR_SLOT_ID::VINYL_COLOUR0_3:
        case CAR_SLOT_ID::VINYL_COLOUR1_0:
        case CAR_SLOT_ID::VINYL_COLOUR1_1:
        case CAR_SLOT_ID::VINYL_COLOUR1_2:
        case CAR_SLOT_ID::VINYL_COLOUR1_3:
        case CAR_SLOT_ID::VINYL_COLOUR2_0:
        case CAR_SLOT_ID::VINYL_COLOUR2_1:
        case CAR_SLOT_ID::VINYL_COLOUR2_2:
        case CAR_SLOT_ID::VINYL_COLOUR2_3:
        case CAR_SLOT_ID::VINYL_COLOUR3_0:
        case CAR_SLOT_ID::VINYL_COLOUR3_1:
        case CAR_SLOT_ID::VINYL_COLOUR3_2:
        case CAR_SLOT_ID::VINYL_COLOUR3_3:
            if (PaintGroups[i].AvailableForVinyl) FirstAvailableType = PaintGroups[i].BrandNameHash;
            else FirstAvailableType = 0x3E871F1; // VINYL
            break;
        }

        if (FirstAvailableType) break;
    }

    return FirstAvailableType;
}

void __fastcall ChoosePaintScreen_BuildPaintCategoryList(DWORD* ChoosePaintScreen, void* EDX_Unused)
{
    DWORD* CarPart; // edi
    unsigned int NumColours; // eax
    DWORD* APaintSelectionThing; // edi MAPDST
    bool IsLocked; // al MAPDST

    int State = *(DWORD*)__PaintMenuState;

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    switch (State)
    {
    case 0: // Paint Category Menu
    default:
        if (CarConfigs[CarTypeID].Paint.BasePaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::BASE_PAINT);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::BASE_PAINT,
                    CarConfigs[CarTypeID].Icons.PaintBasePaint,
                    CarConfigs[CarTypeID].Names.PaintBasePaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.SpoilerPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_SPOILER);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_SPOILER,
                    CarConfigs[CarTypeID].Icons.PaintSpoilerPaint,
                    CarConfigs[CarTypeID].Names.PaintSpoilerPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.RoofScoopPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_ROOF);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_ROOF,
                    CarConfigs[CarTypeID].Icons.PaintRoofScoopPaint,
                    CarConfigs[CarTypeID].Names.PaintRoofScoopPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.MirrorPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_MIRROR);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_MIRROR,
                    CarConfigs[CarTypeID].Icons.PaintMirrorPaint,
                    CarConfigs[CarTypeID].Names.PaintMirrorPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.RimPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_RIM);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_RIM,
                    CarConfigs[CarTypeID].Icons.PaintRimPaint,
                    CarConfigs[CarTypeID].Names.PaintRimPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.SpinnerPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_SPINNER);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_SPINNER,
                    CarConfigs[CarTypeID].Icons.PaintSpinnerPaint,
                    CarConfigs[CarTypeID].Names.PaintSpinnerPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.BrakePaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_BRAKE);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_BRAKE,
                    CarConfigs[CarTypeID].Icons.PaintBrakePaint,
                    CarConfigs[CarTypeID].Names.PaintBrakePaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.EnginePaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_ENGINE);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_ENGINE,
                    CarConfigs[CarTypeID].Icons.PaintEnginePaint,
                    CarConfigs[CarTypeID].Names.PaintEnginePaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.TrunkPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = 0; //ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_AUDIO);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_AUDIO,
                    CarConfigs[CarTypeID].Icons.PaintTrunkPaint,
                    CarConfigs[CarTypeID].Names.PaintTrunkPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        if (CarConfigs[CarTypeID].Paint.ExhaustPaint)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, CAR_SLOT_ID::PAINT_EXHAUST);
                PaintSelectionThing_Create(APaintSelectionThing,
                    CAR_SLOT_ID::PAINT_EXHAUST,
                    CarConfigs[CarTypeID].Icons.PaintExhaustPaint,
                    CarConfigs[CarTypeID].Names.PaintExhaustPaint,
                    0,
                    IsLocked);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else APaintSelectionThing = 0;
        }

        break;

    case 1: // Vinyl Paint
        CarPart = (DWORD*)*(DWORD*)0x839BB8;
        if (CarPart)
        {
            NumColours = CarPart_GetAppliedAttributeUParam(CarPart, CT_bStringHash("NUMREMAPCOLOURS"), 0);
            for (int i = 1; i <= NumColours; i++)
            {
                APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
                if (APaintSelectionThing)
                {
                    PaintSelectionThing_Create(APaintSelectionThing,
                        ChoosePaintScreen_GetCarSlotIDForVinylPaintLayer(ChoosePaintScreen, *(int*)_CurrCarSlotID, i),
                        FEngHashString("PAINT_ICON_%d", i),
                        CarPart_GetAppliedAttributeUParam(CarPart, FEngHashString("COLOR%dLANGUAGEHASH", i-1), CT_bStringHash("PAINT_CHOOSE_VINYL_COLORS")),
                        0,
                        0);
                    (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
                }
                else APaintSelectionThing = 0;
            }
        }
        break;

    /*case 2: // Decal Paint - Cut Feature
        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            PaintSelectionThing_Create(APaintSelectionThing, 97, bStringHash("PAINT_ICON_1"), bStringHash("PAINT_CHOOSE_DECAL_COLORS"), 0, 0);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
            APaintSelectionThing = 0;
        break;*/
    }

    (*(void(__thiscall**)(DWORD*, int))(ChoosePaintScreen[19] + 32))(ChoosePaintScreen + 19, 1); // RefreshHeader
    if (ChoosePaintScreen[22])
        ChoosePaintScreen[93] = ChoosePaintScreen_GetPaintBrandHashFromCarSlotId(ChoosePaintScreen, EDX_Unused, *(DWORD*)(ChoosePaintScreen[22] + 72));
    else
        ChoosePaintScreen[93] = PaintGroups[1].BrandNameHash;
}

int GetPaintIDFromType(DWORD PaintTypeHash)
{
    for (int i = 0; i < PaintGroups.size(); i++)
    {
        if (PaintTypeHash == PaintGroups[i].BrandNameHash) return i;
    }

    return 0;
}

DWORD GetPaintBrandNameHashFromType(DWORD PaintTypeHash)
{
    for (int i = 0; i < PaintGroups.size(); i++)
    {
        if (PaintTypeHash == PaintGroups[i].BrandNameHash) return PaintGroups[i].StringHash;
    }

    return 0;
}

int GetNumAvailablePaintIDFromType(int MinID, int CarSlotID)
{
    int result = 0;
    
    for (int i = MinID; i < PaintGroups.size(); i++)
    {
        if (UnifyPaints)
        {
            result++;
        }
        else switch (CarSlotID)
        {
        case CAR_SLOT_ID::BASE_PAINT:
        case CAR_SLOT_ID::PAINT_SPOILER:
        case CAR_SLOT_ID::PAINT_ROOF:
        case CAR_SLOT_ID::PAINT_MIRROR:
        case CAR_SLOT_ID::PAINT_AUDIO:
            if (PaintGroups[i].AvailableForBody) result++;
            break;
        case CAR_SLOT_ID::PAINT_ENGINE:
            if (PaintGroups[i].AvailableForHoses) result++;
            break;
        case CAR_SLOT_ID::PAINT_BRAKE:
            if (PaintGroups[i].AvailableForBrakes) result++;
            break;
        case CAR_SLOT_ID::PAINT_EXHAUST:
            if (PaintGroups[i].AvailableForMuffler) result++;
            break;
        case CAR_SLOT_ID::PAINT_RIM:
        case CAR_SLOT_ID::PAINT_SPINNER:
            if (PaintGroups[i].AvailableForRims) result++;
            break;
        case CAR_SLOT_ID::VINYL_COLOUR0_0:
        case CAR_SLOT_ID::VINYL_COLOUR0_1:
        case CAR_SLOT_ID::VINYL_COLOUR0_2:
        case CAR_SLOT_ID::VINYL_COLOUR0_3:
        case CAR_SLOT_ID::VINYL_COLOUR1_0:
        case CAR_SLOT_ID::VINYL_COLOUR1_1:
        case CAR_SLOT_ID::VINYL_COLOUR1_2:
        case CAR_SLOT_ID::VINYL_COLOUR1_3:
        case CAR_SLOT_ID::VINYL_COLOUR2_0:
        case CAR_SLOT_ID::VINYL_COLOUR2_1:
        case CAR_SLOT_ID::VINYL_COLOUR2_2:
        case CAR_SLOT_ID::VINYL_COLOUR2_3:
        case CAR_SLOT_ID::VINYL_COLOUR3_0:
        case CAR_SLOT_ID::VINYL_COLOUR3_1:
        case CAR_SLOT_ID::VINYL_COLOUR3_2:
        case CAR_SLOT_ID::VINYL_COLOUR3_3:
            if (PaintGroups[i].AvailableForVinyl) result++;
            break;
        }
    }

    return result;
}

int GetNextAvailablePaintIDFromType(int CurrID, int MinID, int CarSlotID, int Direction)
{
    int num = 0;
    int *AvailableGroups = (int*)j__malloc(sizeof(int) * PaintGroups.size());

	// Get all available paint groups for the current car slot
    for (int i = MinID; i < PaintGroups.size(); i++)
    {
        if (UnifyPaints)
        {
            AvailableGroups[num++] = i;
        }
        else switch (CarSlotID)
        {
        case CAR_SLOT_ID::BASE_PAINT:
        case CAR_SLOT_ID::PAINT_SPOILER:
        case CAR_SLOT_ID::PAINT_ROOF:
        case CAR_SLOT_ID::PAINT_MIRROR:
        case CAR_SLOT_ID::PAINT_AUDIO:
            if (PaintGroups[i].AvailableForBody) AvailableGroups[num++] = i;
            break;
        case CAR_SLOT_ID::PAINT_ENGINE:
            if (PaintGroups[i].AvailableForHoses) AvailableGroups[num++] = i;
            break;
        case CAR_SLOT_ID::PAINT_BRAKE:
            if (PaintGroups[i].AvailableForBrakes) AvailableGroups[num++] = i;
            break;
        case CAR_SLOT_ID::PAINT_EXHAUST:
            if (PaintGroups[i].AvailableForMuffler) AvailableGroups[num++] = i;
            break;
        case CAR_SLOT_ID::PAINT_RIM:
        case CAR_SLOT_ID::PAINT_SPINNER:
            if (PaintGroups[i].AvailableForRims) AvailableGroups[num++] = i;
            break;
        case CAR_SLOT_ID::VINYL_COLOUR0_0:
        case CAR_SLOT_ID::VINYL_COLOUR0_1:
        case CAR_SLOT_ID::VINYL_COLOUR0_2:
        case CAR_SLOT_ID::VINYL_COLOUR0_3:
        case CAR_SLOT_ID::VINYL_COLOUR1_0:
        case CAR_SLOT_ID::VINYL_COLOUR1_1:
        case CAR_SLOT_ID::VINYL_COLOUR1_2:
        case CAR_SLOT_ID::VINYL_COLOUR1_3:
        case CAR_SLOT_ID::VINYL_COLOUR2_0:
        case CAR_SLOT_ID::VINYL_COLOUR2_1:
        case CAR_SLOT_ID::VINYL_COLOUR2_2:
        case CAR_SLOT_ID::VINYL_COLOUR2_3:
        case CAR_SLOT_ID::VINYL_COLOUR3_0:
        case CAR_SLOT_ID::VINYL_COLOUR3_1:
        case CAR_SLOT_ID::VINYL_COLOUR3_2:
        case CAR_SLOT_ID::VINYL_COLOUR3_3:
            if (PaintGroups[i].AvailableForVinyl) AvailableGroups[num++] = i;
            break;
        }
    }

	// Locate the current paint group ID in the available groups
    int loc = 0;

    for (int i = 0; i < num; i++)
    {
        if (AvailableGroups[i] == CurrID)
        {
            // If found, set the current ID to the index in the available groups
            loc = i;
            break;
        }
	}

	// Move to the next paint group in the specified direction
    loc += Direction;
    if (loc < 0) loc = num - 1;
    if (loc >= num) loc = 0;

    loc = AvailableGroups[loc];

	j__free(AvailableGroups);

    return loc;
}

void __fastcall ChoosePaintScreen_RedrawSwatches(DWORD* ChoosePaintScreen, void* EDX_Unused, int NewHighlightID)
{
    // Calculate datum offset
    char const* PackageName = (char const*)ChoosePaintScreen[1];
    DWORD* dtm = (DWORD*)ChoosePaintScreen[86];
    DWORD* head = ChoosePaintScreen + 86;
    DWORD* TheCarPart;
    DWORD* FEObj;
    DWORD FEObjHsh;
    int NumHorDatums = ChoosePaintScreen[94]; // ebx MAPDST
    int NumDatums = ChoosePaintScreen[95]; // edi
    int CurrHighlightID;
    BYTE r, g, b, a;
    int i = 0;

    int DatumOffset_Prev = DatumOffset;

    if (dtm != head)
    {
        // Get current highlighted item
        CurrHighlightID = NewHighlightID; //bList_TraversebList(ChoosePaintScreen + 86, (DWORD*)ChoosePaintScreen[90]) - 1;

        if (CurrHighlightID < DatumOffset)
        {
            DatumOffset -= NumHorDatums;
        }

        if (CurrHighlightID >= DatumOffset + NumPaintSwatchObjects)
        {
            DatumOffset += NumHorDatums;
            if (DatumOffset > NumDatums + NumHorDatums - NumPaintSwatchObjects) DatumOffset = NumDatums - (NumDatums % NumHorDatums) - NumPaintSwatchObjects;
        }

        if (DatumOffset < 0) DatumOffset = 0;
    }

    // Refresh swatches if necessary
    if (DatumOffset_Prev != DatumOffset)
    {
        // Make all swatch objects invisible first and remove their FEObject pointers
        while ((dtm != head) && i < NumPaintSwatchObjects)
        {
            FEObjHsh = FEngHashString("PAINT_SWATCH_%d", i + 1);
            FEngSetInvisible_Pkg(PackageName, FEObjHsh);
            //dtm[4] = 0; // FEObject
            ++i;
            dtm = (DWORD*)dtm[0]; //next
        }

        // Refresh
        i = 0;
        int j = 0;
        dtm = (DWORD*)ChoosePaintScreen[86];
        while ((dtm != head) /* && i < NumPaintSwatchObjects*/ && j < NumDatums)
        {
            if (j >= DatumOffset)
            {
                if (i < NumPaintSwatchObjects)
                {
                    TheCarPart = (DWORD*)dtm[2];
                    FEObjHsh = FEngHashString("PAINT_SWATCH_%d", i + 1);
                    FEObj = (DWORD*)FEngFindObject(PackageName, FEObjHsh);
                    if (FEObj) // set color
                    {
                        FEngSetVisible(FEObj);

                        r = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPRED"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("RED"), 0)) % 256;
                        g = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPGREEN"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("GREEN"), 0)) % 256;
                        b = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPBLUE"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("BLUE"), 0)) % 256;
                        a = 255; // CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPALPHA"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("ALPHA"), 0)) % 256;

                        FEngSetColor(FEObj, b | ((g | ((r | 0xFFFFFF00) << 8)) << 8));
                        dtm[4] = (DWORD)FEObj;
                        ++i;
                    }
                    
                }
                else
                {
                    dtm[4] = 0; // FEObject
                }
                
            }
            else
            {
                dtm[4] = 0; // FEObject
            }

            ++j;
            dtm = (DWORD*)dtm[0]; //next
        }
    }
}

void __fastcall ChoosePaintScreen_BuildPaintList(DWORD* ChoosePaintScreen, void* EDX_Unused)
{
    DWORD PartsBList[2];
    DWORD* TheCarPart;

    char const* PackageName = (char const*)ChoosePaintScreen[1];

    DWORD* dtm = (DWORD*)ChoosePaintScreen[86];
    DWORD* head = ChoosePaintScreen + 86;
    DWORD* tail;
    DWORD* nxt; // ecx
    DWORD* prv; // edx

    DWORD FEObjHsh;
    DWORD* FEObj;
    DWORD* PaintDatum;
    DWORD color;
    BYTE r, g, b, a;
    float x, y;

    int SelectedDatum = 0;
    int StartDatum = 0;

    if (dtm != ChoosePaintScreen + 86) // Clean the datum list
    {
        do
        {
            nxt = (DWORD*)dtm[0];
            prv = (DWORD*)dtm[1];
            prv[0] = dtm[0];
            nxt[1] = (DWORD)prv;
            j__free(dtm);
            dtm = (DWORD*)head[0];
        } while ((DWORD*)head[0] != head);
    }
    
    // Make all swatch objects invisible first
    for (int i = 0; i < NumPaintSwatchObjects; i++)
    {
        FEObjHsh = FEngHashString("PAINT_SWATCH_%d", i + 1);
        FEngSetInvisible_Pkg(PackageName, FEObjHsh);
    }

    DWORD* CategoryNode = (DWORD*)ChoosePaintScreen[22];
    ChoosePaintScreen[90] = 0; // highlighted
    ChoosePaintScreen[95] = 0; // num swatches

    if (CategoryNode)
    {
        int CarSlotID = CategoryNode[18];

        // Get brand of the installed part
        DWORD* InstalledPart = CarCustomizeManager_GetPaintSetupPart((DWORD*)gCarCustomizeManager, CarSlotID);
        if (!InstalledPart) InstalledPart = RideInfo_GetPart((DWORD*)gTheRideInfo, CarSlotID);

        if (InstalledPart)
        {
            if (*((BYTE*)ChoosePaintScreen + 396)) ChoosePaintScreen[93] = CarPart_GetAppliedAttributeUParam(InstalledPart, 0xEBB03E66, 0); // BRAND_NAME
        }

        *((BYTE*)ChoosePaintScreen + 396) = 0;
        PartsBList[0] = (DWORD)PartsBList;
        PartsBList[1] = (DWORD)PartsBList;

        // Get parts list
        GetPartsList(CarSlotID, PartsBList, ChoosePaintScreen[93], 0);

        // Traverse the list, create datums and color the Paint Swatch objects
        DWORD* curr_s = (DWORD*)PartsBList[0];

        while ((curr_s != PartsBList)/* && (ChoosePaintScreen[95] < NumPaintSwatchObjects)*/)
        {
            TheCarPart = (DWORD*)curr_s[2];

            if (TheCarPart && ChoosePaintScreen_IsPaintAvailable(ChoosePaintScreen, TheCarPart))
            {
                if (TheCarPart == InstalledPart) SelectedDatum = ChoosePaintScreen[95];

                // Make visible
                FEObjHsh = FEngHashString("PAINT_SWATCH_%d", ChoosePaintScreen[95] + 1);
                FEObj = (DWORD*)FEngFindObject(PackageName, FEObjHsh);
                if (FEObj) FEngSetVisible(FEObj);
                
                // Add datum
                PaintDatum = (DWORD*)j__malloc(0x14u);
                if (PaintDatum)
                {
                    PaintDatum[2] = (DWORD)TheCarPart;
                    PaintDatum[3] = CarSlotID;
                    PaintDatum[4] = (DWORD)FEObj;

                    // add to datum list
                    tail = (DWORD*)ChoosePaintScreen[87];
                    tail[0] = (DWORD)PaintDatum;
                    ChoosePaintScreen[87] = (DWORD)PaintDatum;
                    PaintDatum[1] = (DWORD)tail;
                    PaintDatum[0] = (DWORD)(ChoosePaintScreen + 86);

                    if (FEObj) // set color
                    {
                        r = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPRED"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("RED"), 0)) % 256;
                        g = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPGREEN"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("GREEN"), 0)) % 256;
                        b = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPBLUE"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("BLUE"), 0)) % 256;
                        a = 255; // CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("DISPALPHA"), CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("ALPHA"), 0)) % 256;

                        FEngSetColor(FEObj, b | ((g | ((r | 0xFFFFFF00) << 8)) << 8));
                    }
                }
                else
                {
                    PaintDatum = 0;
                }
                
            }

            // Get next
            ++ChoosePaintScreen[95];
            curr_s = (DWORD*)curr_s[0];
        }

        // Show or hide locked icon
        if (ChoosePaintScreen[86] == (DWORD)(ChoosePaintScreen + 86)) // no datum
            FEngSetScript_Pkg(PackageName, 0x23A43844, "SHOW", 1);
        else
        {
            FEngSetScript_Pkg(PackageName, 0x23A43844, "HIDE", 1);

            // Refresh if selected one is out of bounds
            if (SelectedDatum >= NumPaintSwatchObjects) ChoosePaintScreen_RedrawSwatches(ChoosePaintScreen, EDX_Unused, SelectedDatum);

            // Highlight selected
            if (SelectedDatum) ChoosePaintScreen[90] = (DWORD)bList_GetNode(ChoosePaintScreen + 86, SelectedDatum);
            else ChoosePaintScreen[90] = ChoosePaintScreen[86];

            DWORD* Highlight = (DWORD*)ChoosePaintScreen[90];
            if (Highlight)
            {
                FEngGetCenter((DWORD*)Highlight[4], &x, &y);
                y = y + *((float*)ChoosePaintScreen + 97);
                x = x + *((float*)ChoosePaintScreen + 96);
                FEngSetCenter(*((DWORD**)ChoosePaintScreen + 92), x, y);
            }

        }

        // Cleanup
        curr_s = (DWORD*)PartsBList[0];
        while (PartsBList[0] != (DWORD)PartsBList)
        {
            nxt = (DWORD*)curr_s[0];
            prv = (DWORD*)curr_s[1];
            prv[0] = curr_s[0];
            nxt[1] = (DWORD)prv;
            j__free(curr_s);
            curr_s = (DWORD*)PartsBList[0];
        }
    }
}

void __fastcall ChoosePaintScreen_ScrollPaintTypes(DWORD* ChoosePaintScreen, void* EDX_Unused, unsigned int MessageHash)
{
    DWORD v4; // eax
    DWORD* v5; // eax
    DWORD v6; // ecx
    DWORD v7; // eax
    DWORD* v8; // eax
    int v10; // eax

    DWORD CurrPaintType = ChoosePaintScreen[93];
    char const* PackageName = (char const* )ChoosePaintScreen[1];

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    DWORD* CategoryNode = (DWORD*)ChoosePaintScreen[22];
    int CarSlotID = 63; // BASE_PAINT
    if (CategoryNode) CarSlotID = CategoryNode[18];

    int Min = CarConfigs[CarTypeID].Paint.PaintCustom ? 0 : 1;
    int Curr = GetPaintIDFromType(CurrPaintType);

    if (GetNumAvailablePaintIDFromType(Min, CarSlotID) > 1)
    {
        switch (MessageHash)
        {
        case 0x5073EF13: // PAD_LTRIGGER
            //if (--Curr < Min) Curr = Max;
            Curr = GetNextAvailablePaintIDFromType(Curr, Min, CarSlotID, -1);
            break;

        case 0xD9FEEC59: // PAD_RTRIGGER
            //if (++Curr > Max) Curr = Min;
            Curr = GetNextAvailablePaintIDFromType(Curr, Min, CarSlotID, 1);
            break;
        }

        ChoosePaintScreen[93] = PaintGroups[Curr].BrandNameHash;
    }

    if (ChoosePaintScreen[93] == CurrPaintType)
        return;

    v4 = ChoosePaintScreen[90];
    if (v4)
    {
        v5 = *(DWORD**)(v4 + 16);
        if (v5)
            FEngSetScript_Obj(v5, "Unhighlight", 1);
    }
    ChoosePaintScreen_BuildPaintList(ChoosePaintScreen, EDX_Unused);
    if ((DWORD*)ChoosePaintScreen[86] == ChoosePaintScreen + 86)
    {
        FEngSetInvisible_Pkg(PackageName, 0x9BA6D658);
        FEngSetScript_Pkg(PackageName, 0x23A43844, "SHOW", 1);
        if (*(int*)0x850078 == 1)
            FEngSetInvisible_Pkg(PackageName, 0x13C45E);
        goto RefreshHeader;
    }
    FEngSetVisible_Pkg(PackageName, 0x9BA6D658);
    FEngSetScript_Pkg(PackageName, 0x9BA6D658, "BLINK", 1);
    FEngSetScript_Pkg(PackageName, 0x23A43844, "HIDE", 1);
    if (*(int*)0x850078 == 1)
        FEngSetVisible_Pkg(PackageName, 0x13C45E);
    v6 = ChoosePaintScreen[90];
    if (!v6 || (v7 = ChoosePaintScreen[22]) == 0)
    {
    RefreshHeader:
        (*(void(__thiscall**)(DWORD*))(*ChoosePaintScreen + 16))(ChoosePaintScreen);
        return;
    }
    CarCustomizeManager_PreviewPaintPart(
        (DWORD*)gCarCustomizeManager,
        *(DWORD*)(v7 + 72),
        *(DWORD**)(v6 + 8));
    v8 = *(DWORD**)(ChoosePaintScreen[90] + 16);
    if (v8)
        FEngSetScript_Obj(v8, "Highlight", 1);
    v10 = PlayerCareerState_GetCarPartCost_Game(
        (DWORD*)ThePlayerCareer,
        *(DWORD*)(ChoosePaintScreen[22] + 72),
        *(DWORD**)(ChoosePaintScreen[90] + 8));
    FEPrintf_ObjName(PackageName, "CASH", "%$d", v10);
    (*(void(__thiscall**)(DWORD*))(*ChoosePaintScreen + 16))(ChoosePaintScreen);
}

void __fastcall ChoosePaintScreen_RefreshHeader(DWORD* ChoosePaintScreen, void* EDX_Unused)
{
    DWORD* CategoryNode; // eax MAPDST
    DWORD hsh; // eax MAPDST
    DWORD CarSlotID; // ecx
    DWORD* Highlight; // eax
    float newx; // [esp+0h] [ebp-18h]
    float newy; // [esp+4h] [ebp-14h]
    float y; // [esp+Ch] [ebp-Ch] BYREF
    float x; // [esp+10h] [ebp-8h] BYREF
    //float v12; // [esp+14h] [ebp-4h] BYREF

    CategoryNode = (DWORD*)ChoosePaintScreen[22];
    if (CategoryNode)
        hsh = CategoryNode[11];
    else
        hsh = 0;
    FEngSetLanguageHash_obj((DWORD*)ChoosePaintScreen[77], hsh);
    CategoryNode = (DWORD*)ChoosePaintScreen[22];
    CarSlotID = 169;                              // MISC
    if (CategoryNode)
        CarSlotID = CategoryNode[18];

    int State = *(DWORD*)__PaintMenuState;

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    int Min = CarConfigs[CarTypeID].Paint.PaintCustom ? 0 : 1;

    if (GetNumAvailablePaintIDFromType(Min, CarSlotID) > 1)
    {
        FEngSetVisible_Pkg((char*)ChoosePaintScreen[1], 0x9DE5BA07); // LTRIGGER_ICON
        FEngSetVisible_Pkg((char*)ChoosePaintScreen[1], 0x7A1FF30D); // RTRIGGER_ICON
    }
    else
    {
        
        FEngSetInvisible_Pkg((const char*)ChoosePaintScreen[1], 0x9DE5BA07); // LTRIGGER_ICON
        FEngSetInvisible_Pkg((const char*)ChoosePaintScreen[1], 0x7A1FF30D); // RTRIGGER_ICON
    }
    
    //if (State == 1) FEngSetInvisible_Pkg((const char*)ChoosePaintScreen[1], 0x31A9C0A); // PAINTTYPEGROUP
    //hsh = bStringHash2("_PAINT", ChoosePaintScreen[93]);
    hsh = GetPaintBrandNameHashFromType(ChoosePaintScreen[93]);
    FEngPrintf((DWORD*)ChoosePaintScreen[91], "");
    FEngSetLanguageHash_obj((DWORD*)ChoosePaintScreen[91], hsh);

    Highlight = (DWORD*)ChoosePaintScreen[90];
    if (Highlight)
    {
        FEngGetCenter((DWORD*)Highlight[4], &x, &y);
        //FEngGetCenter((DWORD*)Highlight[4], &x, &v12);
        newy = y + *((float*)ChoosePaintScreen + 97);
        newx = x + *((float*)ChoosePaintScreen + 96);
        FEngSetCenter(*((DWORD**)ChoosePaintScreen + 92), newx, newy);
    }
}

void __fastcall ChoosePaintScreen_ScrollVertically(DWORD* ChoosePaintScreen, void* EDX_Unused, unsigned int MessageHash)
{
    int CurrHighlightID; // ecx
    int NumHorDatums = ChoosePaintScreen[94]; // ebx MAPDST
    int NumDatums = ChoosePaintScreen[95]; // edi
    int NewHighlightID; // eax
    DWORD* Node; // eax

    DWORD* PaintDatumList = (DWORD*)(ChoosePaintScreen + 86);

    if ((DWORD*)ChoosePaintScreen[86] != ChoosePaintScreen + 86
        && NumDatums > NumHorDatums)
    {
        CurrHighlightID = bList_TraversebList(ChoosePaintScreen + 86, (DWORD*)ChoosePaintScreen[90]) - 1;

        switch (MessageHash)
        {
        case 0x72619778: // PAD_UP
            if (CurrHighlightID < NumHorDatums) // if in first line
            {
                NewHighlightID = CurrHighlightID + NumDatums - (NumDatums % NumHorDatums);
                if (NewHighlightID >= NumDatums) NewHighlightID -= NumHorDatums;
            }
            else // other lines
            {
                NewHighlightID = CurrHighlightID - NumHorDatums;
            }
            break;

        case 0x911C0A4B: // PAD_DOWN
            if (CurrHighlightID < NumDatums - NumHorDatums) // if in last line
            {
                NewHighlightID = NumHorDatums + CurrHighlightID;
            }
            else
            {
                NewHighlightID = CurrHighlightID + (NumDatums % NumHorDatums) - NumDatums;
                if (NewHighlightID < 0) NewHighlightID += NumHorDatums;
            }
            break;
        }

        // Scroll paint datum list to ensure new selection is visible
        ChoosePaintScreen_RedrawSwatches(ChoosePaintScreen, EDX_Unused, NewHighlightID);

        Node = bList_GetNode(PaintDatumList, NewHighlightID/* - DatumOffset*/);
    
        if (Node)
            ChoosePaintScreen[90] = (DWORD)Node;
    }
}

void __fastcall ChoosePaintScreen_ScrollPaints(DWORD* ChoosePaintScreen, void* EDX_Unused, unsigned int MessageHash)
{
    DWORD* CurrHighlight; // edi
    DWORD* NewHighlight; // eax
    DWORD* FEObj; // eax
    const char* pkg; // esi
    int CarPartCost; // eax
    float x; // [esp+0h] [ebp-10h]
    float y; // [esp+4h] [ebp-Ch]

    if ((DWORD*)ChoosePaintScreen[86] != ChoosePaintScreen + 86)
    {
        CurrHighlight = (DWORD*)ChoosePaintScreen[90];

        if (MessageHash == 0x9120409E || MessageHash == 0xB5971BF1)
        {
            ChoosePaintScreen_ScrollHorizontally(ChoosePaintScreen, MessageHash);
        }
        else if (MessageHash == 0x72619778 || MessageHash == 0x911C0A4B)
        {
            ChoosePaintScreen_ScrollVertically(ChoosePaintScreen, EDX_Unused, MessageHash);
        }

        NewHighlight = (DWORD*)ChoosePaintScreen[90];

        if (NewHighlight)
        {
            if (NewHighlight != CurrHighlight)
            {
                FEObj = (DWORD*)CurrHighlight[4];
                if (FEObj) FEngSetScript_Obj(FEObj, "Unhighlight", 1);

                FEObj = (DWORD*)NewHighlight[4];
                if (FEObj) FEngSetScript_Obj(FEObj, "Highlight", 1);
            }
            
            if (ChoosePaintScreen[22])
            {
                CarCustomizeManager_PreviewPaintPart((DWORD*)gCarCustomizeManager, *(DWORD*)(ChoosePaintScreen[22] + 72), (DWORD*)NewHighlight[2]);
                FEngGetCenter((DWORD*)NewHighlight[4], &x, &y);
                y = y + *((float*)ChoosePaintScreen + 97);
                x = x + *((float*)ChoosePaintScreen + 96);
                FEngSetCenter(*((DWORD**)ChoosePaintScreen + 92), x, y);
                pkg = (const char*)ChoosePaintScreen[1];
                CarPartCost = PlayerCareerState_GetCarPartCost_Game((DWORD*)ThePlayerCareer, *(DWORD*)(ChoosePaintScreen[22] + 72), (DWORD*)NewHighlight[2]);
                FEPrintf_ObjName(pkg, "Cash", "%$d", CarPartCost);
            }
            /*
            char const* name = GetLocalizedString(GetPaintBrandNameHashFromType(ChoosePaintScreen[93]));
            int CurrHighlightID = bList_TraversebList(ChoosePaintScreen + 86, (DWORD*)ChoosePaintScreen[90]);
            FEngPrintf((DWORD*)ChoosePaintScreen[91], "%s^%d/%d", name, CurrHighlightID, ChoosePaintScreen[95]); // fix "capa del palvo"*/
        }
    }
}