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
    int SomeRef; // eax MAPDST
    int CarSlotID; // ecx
    int CarSlotIDToCheck; // esi
    DWORD* TheCarPart; // eax MAPDST

    if (*(DWORD*)0x8389A0 == 1)
        return 1;
    SomeRef = ChoosePaintScreen[22];
    CarSlotID = 169;
    if (SomeRef)
        CarSlotID = *(DWORD*)(SomeRef + 72);
    switch (CarSlotID)
    {
    case 63: // BASE_PAINT
        return 1;
    case 68: // PAINT_ENGINE
        return CarCustomizeManager_IsEnginePaintable((DWORD*)gCarCustomizeManager_0);
    case 69: // PAINT_SPOILER
        CarSlotIDToCheck = 12;
        goto CheckIfPaintable;
    case 70: // PAINT_BRAKE
        return CarCustomizeManager_AreBrakesPaintable((DWORD*)gCarCustomizeManager_0);
    case 71: // PAINT_EXHAUST
        CarSlotIDToCheck = 16;
        goto CheckIfPaintable;
    case 72: // TRUNK_AUDIO
        return CarCustomizeManager_IsTrunkPaintable((DWORD*)gCarCustomizeManager_0, EDX_Unused);
    case 73: // PAINT_RIM
    case 74: // PAINT_SPINNER
        CarSlotIDToCheck = 29;
        goto CheckIfPaintable;
    case 75: // PAINT_ROOF
        CarSlotIDToCheck = 7;
        goto CheckIfPaintable;
    case 76: // PAINT_MIRROR
        CarSlotIDToCheck = 32;
    CheckIfPaintable:
        TheCarPart = CarCustomizeManager_GetInstalledPart((DWORD*)gCarCustomizeManager, CarSlotIDToCheck);
        if (!TheCarPart
            || (*((BYTE*)TheCarPart + 5) & 0xE0) == 0
            || (CarSlotIDToCheck == 12 || CarSlotIDToCheck == 7 || CarSlotIDToCheck == 32) // SPOILER, ROOF, WING_MIRROR
            && (CarPart_GetAppliedAttributeUParam(TheCarPart, bStringHash("CARBONFIBRE"), 0)
                || CarPart_GetAppliedAttributeUParam(TheCarPart, 0x6509EC92, 0))) // ALUMINUM ??
        {
            return 0;
        }
        if (SomeRef)
        {
            if (CarSlotID == 74) result = CarPart_HasAppliedAttribute(TheCarPart, bStringHash("SPINNER_TEXTURE"));
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

void __fastcall ChoosePaintScreen_BuildPaintCategoryList(DWORD* ChoosePaintScreen, void* EDX_Unused)
{
    DWORD* CarPart; // edi
    unsigned int NumColours; // eax
    DWORD* APaintSelectionThing; // edi MAPDST
    bool IsLocked; // al MAPDST

    if (!*(DWORD*)0x8389A0)
    {
        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 63);
            PaintSelectionThing_Create(APaintSelectionThing, 63, bStringHash("PAINT_MOD_BASE"), bStringHash("PAINT_BASE"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 69);
            PaintSelectionThing_Create(APaintSelectionThing, 69, bStringHash("PAINT_MOD_PART_SPOILER"), bStringHash("PAINT_SPOILER"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 75);
            PaintSelectionThing_Create(APaintSelectionThing, 75, bStringHash("PAINT_MOD_PART_ROOF"), bStringHash("PAINT_ROOF"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 76);
            PaintSelectionThing_Create(APaintSelectionThing, 76, bStringHash("PAINT_MOD_PART_MIRROR"), bStringHash("PAINT_MIRROR"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 73);
            PaintSelectionThing_Create(APaintSelectionThing, 73, bStringHash("PAINT_MOD_PART_RIMS"), bStringHash("PAINT_RIM"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 74);
            PaintSelectionThing_Create(APaintSelectionThing, 74, bStringHash("PAINT_MOD_PART_RIMS"), bStringHash("PAINT_SPINNER"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 70);
            PaintSelectionThing_Create(APaintSelectionThing, 70, bStringHash("PAINT_MOD_PART_BRAKE_CALIPERS"), bStringHash("PAINT_BRAKE"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 68);
            PaintSelectionThing_Create(APaintSelectionThing, 68, bStringHash("PAINT_MOD_PART_ENGINE_ACCENT"), bStringHash("PAINT_ENGINE"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = 0; //ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 72);
            PaintSelectionThing_Create(APaintSelectionThing, 72, bStringHash("PAINT_MOD_PART_TRUNK_AUDIO"), bStringHash("PAINT_AUDIO"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }

        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            IsLocked = ChoosePaintScreen_IsPaintMenuLocked(ChoosePaintScreen, 71);
            PaintSelectionThing_Create(APaintSelectionThing, 71, bStringHash("PAINT_MOD_PART_MUFFLER_TIP"), bStringHash("PAINT_EXHAUST"), 0, IsLocked);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
        {
            APaintSelectionThing = 0;
        }
        goto LABEL_42;
    }
    if (*(DWORD*)0x8389A0 != 1)
    {
        if (*(DWORD*)0x8389A0 != 2)
            goto LABEL_42;
        APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
        if (APaintSelectionThing)
        {
            PaintSelectionThing_Create(APaintSelectionThing, 97, bStringHash("PAINT_ICON_1"), bStringHash("PAINT_CHOOSE_DECAL_COLORS"), 0, 0);
            (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
        }
        else
            APaintSelectionThing = 0;
        goto LABEL_42;
    }
    CarPart = (DWORD*)*(DWORD*)0x839BB8;
    if (CarPart)
    {
        NumColours = CarPart_GetAppliedAttributeUParam(CarPart, bStringHash("NUMREMAPCOLOURS"), 0);
        for (int i = 1; i <= NumColours; i++)
        {
            APaintSelectionThing = (DWORD*)j__malloc(0x4Cu);
            if (APaintSelectionThing)
            {
                PaintSelectionThing_Create(APaintSelectionThing, ChoosePaintScreen_GetCarSlotIDForVinylPaintLayer(ChoosePaintScreen, *(DWORD*)0x8389A8, i), FEngHashString("PAINT_ICON_%d", i), bStringHash("PAINT_CHOOSE_VINYL_COLORS"), 0, 0);
                (*(void(__thiscall**)(DWORD*, DWORD*))(*(DWORD*)ChoosePaintScreen + 24))(ChoosePaintScreen, APaintSelectionThing);
            }
            else
            {
                APaintSelectionThing = 0;
            }
        }
    }
LABEL_42:
    (*(void(__thiscall**)(DWORD*, int))(ChoosePaintScreen[19] + 32))(ChoosePaintScreen + 19, 1);
    if (ChoosePaintScreen[22])
        ChoosePaintScreen[93] = ChoosePaintScreen_GetPaintBrandHashFromCarSlotId(ChoosePaintScreen, *(DWORD*)(ChoosePaintScreen[22] + 72));
    else
        ChoosePaintScreen[93] = bStringHash("GLOSS");
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

    switch (MessageHash)
    {
    case 0x5073EF13: // PAD_LTRIGGER
        switch (CurrPaintType)
        {
        case 0x2DAAB07: // GLOSS
            ChoosePaintScreen[93] = 0x66350F0;
            break;
        case 0x3437A52: // METAL
            ChoosePaintScreen[93] = 0x2DAAB07;
            break;
        case 0x3797533: // PEARL
            ChoosePaintScreen[93] = 0x3437A52;
            break;
        case 0x16BDCE: // HOSE
            ChoosePaintScreen[93] = 0x3797533;
            break;
        case 0x66350F0: // MUFFLER
            ChoosePaintScreen[93] = 0x16BDCE;
            break;
        }
        break;

    case 0xD9FEEC59: // PAD_RTRIGGER
        switch (CurrPaintType)
        {
        case 0x2DAAB07: // GLOSS
            ChoosePaintScreen[93] = 0x3437A52;
            break;
        case 0x3437A52: // METAL
            ChoosePaintScreen[93] = 0x3797533;
            break;
        case 0x3797533: // PEARL
            ChoosePaintScreen[93] = 0x16BDCE;
            break;
        case 0x16BDCE: // HOSE
            ChoosePaintScreen[93] = 0x66350F0;
            break;
        case 0x66350F0: // MUFFLER
            ChoosePaintScreen[93] = 0x2DAAB07;
            break;
        }
        break;
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
    ChoosePaintScreen_BuildPaintList(ChoosePaintScreen);
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
        (DWORD*)gCarCustomizeManager_0,
        *(DWORD*)(v7 + 72),
        *(DWORD**)(v6 + 8));
    v8 = *(DWORD**)(ChoosePaintScreen[90] + 16);
    if (v8)
        FEngSetScript_Obj(v8, "Highlight", 1);
    v10 = PlayerCareerState_GetCarPartCost(
        (DWORD*)ThePlayerCareer,
        *(DWORD*)(ChoosePaintScreen[22] + 72),
        *(DWORD**)(ChoosePaintScreen[90] + 8));
    FEPrintf_ObjName(PackageName, "CASH", "%$d", v10);
    (*(void(__thiscall**)(DWORD*))(*ChoosePaintScreen + 16))(ChoosePaintScreen);
}