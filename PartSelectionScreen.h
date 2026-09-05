#include "stdio.h"
#include "InGameFunctions.h"
#include "PartLink.h"
#include "ChooseRimBrand.h"

#define _CurrCarSlotID 0x8389A8

#define _PartNode_vtbl 0x79AFDC

void __fastcall PartSelectionScreen_DoSpecialScroll(DWORD* PartSelectionScreen, void* EDX_Unused, DWORD* CategoryNode, bool on)
{
    if (CategoryNode)
    {
        if (dword_838990 == 0)
        {
            GetAndDoFEPartAnim(CategoryNode[3], on, 1.0f);
        }
    }
}

void PartSelectionScreen_AddCategoryCarbonChecked(DWORD* PSS, unsigned int TextureHash, unsigned int LanguageHash)
{
    static const int CarbonSlots[] =
    {
        CARSLOTID_HOOD, CARSLOTID_SPOILER, CARSLOTID_ROOF,
        CARSLOTID_LEFT_SIDE_MIRROR, CARSLOTID_RIGHT_SIDE_MIRROR,
        CARSLOTID_DOOR_LEFT, CARSLOTID_DOOR_RIGHT,
    };

    for (int i = 0; i < (int)(sizeof(CarbonSlots) / sizeof(CarbonSlots[0])); i++)
    {
        if (!PartLink_IsSlotHidden(CarbonSlots[i]))
        {
            PartSelectionScreen_AddCategoryCarbon(PSS, TextureHash, LanguageHash);
            return;
        }
    }
}

void PartSelectionScreen_AddCategoryChecked(DWORD* PSS, unsigned int CarSlotID, unsigned int TextureHash, unsigned int LanguageHash, bool unk)
{
    if (PartLink_IsSlotHidden((int)CarSlotID)) return;

    PartSelectionScreen_AddCategory(PSS, CarSlotID, TextureHash, LanguageHash, unk);
}

void __fastcall PartSelectionScreen_SetupBodyShop(DWORD* PartSelectionScreen, void* EDX_Unused)
{
    RideInfo_UpdatePartsEnabled((DWORD*)gTheRideInfo, nullptr);
    PartLinkTraceCategories();

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    char const* PartSelectionScreenPackage = (char const*)PartSelectionScreen[1];

    PartSelectionScreen_ResetCategories(&PartSelectionScreen[19]);
    if (CarConfigs[CarTypeID].BodyShop.FrontBumper)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_FRONT_BUMPER,
            CarConfigs[CarTypeID].Icons.BodyShopFrontBumper,
            CarConfigs[CarTypeID].Names.BodyShopFrontBumper,
            0); // FRONT_BUMPER
    
    if (CarConfigs[CarTypeID].BodyShop.RearBumper)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_REAR_BUMPER,
            CarConfigs[CarTypeID].Icons.BodyShopRearBumper,
            CarConfigs[CarTypeID].Names.BodyShopRearBumper,
            0); // REAR_BUMPER
    
    if (CarConfigs[CarTypeID].BodyShop.Skirt)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_SKIRT,
            CarConfigs[CarTypeID].Icons.BodyShopSkirt,
            CarConfigs[CarTypeID].Names.BodyShopSkirt,
            0); // SKIRT
    
    if (CarConfigs[CarTypeID].BodyShop.Fender)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_FENDER,
            CarConfigs[CarTypeID].Icons.BodyShopFender,
            CarConfigs[CarTypeID].Names.BodyShopFender,
            0); // FENDER
 
    if (CarConfigs[CarTypeID].BodyShop.Quarter)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_QUARTER,
            CarConfigs[CarTypeID].Icons.BodyShopQuarter,
            CarConfigs[CarTypeID].Names.BodyShopQuarter,
            0); // QUARTER
    
    if (CarConfigs[CarTypeID].BodyShop.Spoiler)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_SPOILER,
            CarConfigs[CarTypeID].Icons.BodyShopSpoiler,
            CarConfigs[CarTypeID].Names.BodyShopSpoiler,
            0); // SPOILER
    
    if (CarConfigs[CarTypeID].BodyShop.Hood)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_HOOD,
            CarConfigs[CarTypeID].Icons.BodyShopHood,
            CarConfigs[CarTypeID].Names.BodyShopHood,
            0); // HOOD

    if (CarConfigs[CarTypeID].BodyShop.Engine && !CarConfigs[CarTypeID].Main.SyncVisualPartsWithPhysics)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_ENGINE,
            CarConfigs[CarTypeID].Icons.BodyShopEngine,
            CarConfigs[CarTypeID].Names.BodyShopEngine,
            0); // ENGINE
    
    if (CarConfigs[CarTypeID].BodyShop.Trunk)
        PartSelectionScreen_AddCategoryChecked(PartSelectionScreen,
            CARSLOTID_TRUNK,
            CarConfigs[CarTypeID].Icons.BodyShopTrunk,
            CarConfigs[CarTypeID].Names.BodyShopTrunk,
            0); // TRUNK
    
    if (CarConfigs[CarTypeID].BodyShop.RoofScoops)
        PartSelectionScreen_AddCategoryChecked(PartSelectionScreen,
            CARSLOTID_ROOF,
            CarConfigs[CarTypeID].Icons.BodyShopRoofScoops,
            CarConfigs[CarTypeID].Names.BodyShopRoofScoops,
            0); // ROOF
    
    if (CarConfigs[CarTypeID].BodyShop.Interior)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_BASE,
            CarConfigs[CarTypeID].Icons.BodyShopInterior,
            CarConfigs[CarTypeID].Names.BodyShopInterior,
            0); // BASE
    
    if (CarConfigs[CarTypeID].BodyShop.Roof)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_TOP,
            CarConfigs[CarTypeID].Icons.BodyShopRoof,
            CarConfigs[CarTypeID].Names.BodyShopRoof,
            0); // TOP
    
    if (CarConfigs[CarTypeID].BodyShop.Headlights)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_HEADLIGHT,
            CarConfigs[CarTypeID].Icons.BodyShopHeadlights,
            CarConfigs[CarTypeID].Names.BodyShopHeadlights,
            0); // HEADLIGHT
    
    if (CarConfigs[CarTypeID].BodyShop.Taillights)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_BRAKELIGHT,
            CarConfigs[CarTypeID].Icons.BodyShopTaillights,
            CarConfigs[CarTypeID].Names.BodyShopTaillights,
            0); // BRAKELIGHT
    
    if (CarConfigs[CarTypeID].BodyShop.Mirrors)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_WING_MIRROR,
            CarConfigs[CarTypeID].Icons.BodyShopMirrors,
            CarConfigs[CarTypeID].Names.BodyShopMirrors,
            0); // WING_MIRROR
    
    if (CarConfigs[CarTypeID].BodyShop.Exhaust)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_EXHAUST,
            CarConfigs[CarTypeID].Icons.BodyShopExhaust,
            CarConfigs[CarTypeID].Names.BodyShopExhaust,
            0); // EXHAUST
    
    if (CarConfigs[CarTypeID].BodyShop.Rims)
        PartSelectionScreen_AddCategoryRims(
            PartSelectionScreen,
            CarConfigs[CarTypeID].Icons.BodyShopRims,
            CarConfigs[CarTypeID].Names.BodyShopRims);

    if (CarConfigs[CarTypeID].BodyShop.Brakes && !CarConfigs[CarTypeID].Main.SyncVisualPartsWithPhysics)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_FRONT_BRAKE,
            CarConfigs[CarTypeID].Icons.BodyShopBrakes,
            CarConfigs[CarTypeID].Names.BodyShopBrakes,
            0); // FRONT_BRAKE
    
    if (CarConfigs[CarTypeID].BodyShop.CarbonFiber)
        PartSelectionScreen_AddCategoryCarbonChecked(
            PartSelectionScreen,
            CarConfigs[CarTypeID].Icons.BodyShopCarbonFiber,
            CarConfigs[CarTypeID].Names.BodyShopCarbonFiber);
    
    if (CarConfigs[CarTypeID].BodyShop.WideBodyKits)
        PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            CARSLOTID_WIDE_BODY,
            CarConfigs[CarTypeID].Icons.BodyShopWideBodyKits,
            CarConfigs[CarTypeID].Names.BodyShopWideBodyKits,
            0); // WIDE_BODY
    
    int NumAttachments = CarConfigs[CarTypeID].BodyShop.Attachments;

    static const int AttachmentSlots[11] = {
        CARSLOTID_DAMAGE_FRONT,
        CARSLOTID_DAMAGE_REAR,
        CARSLOTID_DAMAGE_LEFT,
        CARSLOTID_DAMAGE_RIGHT,
        CARSLOTID_DAMAGE_TOP,
        CARSLOTID_DOOR_PANEL_LEFT,
        CARSLOTID_DOOR_PANEL_RIGHT,
        CARSLOTID_DOOR_SILL_LEFT,
        CARSLOTID_DOOR_SILL_RIGHT,
        CARSLOTID_HOOD_UNDER,
        CARSLOTID_TRUNK_UNDER,
    };

    int AttachmentAvailable[] = {
        CarConfigs[CarTypeID].BodyShop.Attachment0,
        CarConfigs[CarTypeID].BodyShop.Attachment1,
        CarConfigs[CarTypeID].BodyShop.Attachment2,
        CarConfigs[CarTypeID].BodyShop.Attachment3,
        CarConfigs[CarTypeID].BodyShop.Attachment4,
        CarConfigs[CarTypeID].BodyShop.Attachment5,
        CarConfigs[CarTypeID].BodyShop.Attachment6,
        CarConfigs[CarTypeID].BodyShop.Attachment7,
        CarConfigs[CarTypeID].BodyShop.Attachment8,
        CarConfigs[CarTypeID].BodyShop.Attachment9,
        CarConfigs[CarTypeID].BodyShop.Attachment10
    };

    DWORD AttachmentIcons[] = {
        CarConfigs[CarTypeID].Icons.BodyShopAttachment0,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment1,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment2,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment3,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment4,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment5,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment6,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment7,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment8,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment9,
        CarConfigs[CarTypeID].Icons.BodyShopAttachment10
    };
    DWORD AttachmentNames[] = {
        CarConfigs[CarTypeID].Names.BodyShopAttachment0,
        CarConfigs[CarTypeID].Names.BodyShopAttachment1,
        CarConfigs[CarTypeID].Names.BodyShopAttachment2,
        CarConfigs[CarTypeID].Names.BodyShopAttachment3,
        CarConfigs[CarTypeID].Names.BodyShopAttachment4,
        CarConfigs[CarTypeID].Names.BodyShopAttachment5,
        CarConfigs[CarTypeID].Names.BodyShopAttachment6,
        CarConfigs[CarTypeID].Names.BodyShopAttachment7,
        CarConfigs[CarTypeID].Names.BodyShopAttachment8,
        CarConfigs[CarTypeID].Names.BodyShopAttachment9,
        CarConfigs[CarTypeID].Names.BodyShopAttachment10
    };

    for (int i = 0; i < NumAttachments; i++)
    {
        if (AttachmentAvailable[i])
            PartSelectionScreen_AddCategoryChecked(
            PartSelectionScreen,
            AttachmentSlots[i],
            AttachmentIcons[i],
            AttachmentNames[i],
            0);
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

    //PartSelectionScreen_DoSpecialScroll(PartSelectionScreen, EDX_Unused, PartSelectionScreen[22], 1);
}

int __cdecl GetMenuIDForVinylGroup(unsigned int group)
{
    int result = 0; // eax
    int NumVinylGroups = VinylGroups.size();

    if (group > NumVinylGroups) return 0;

    for (int i = 0; i < NumVinylGroups; i++)
    {
        if (VinylGroups[i].Index == group)
        {
            result = i;
            break;
        }
    }

    return result;
}

void __fastcall PartSelectionScreen_SetupVinyls(DWORD* PartSelectionScreen, void* EDX_Unused)
{
    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    char const* PartSelectionScreenPackage = (char const*)PartSelectionScreen[1];

    PartSelectionScreen_ResetCategories(&PartSelectionScreen[19]);

    // Add categories
    bool HasNoCustomVinyls = CarConfigs[CarTypeID].Paint.VinylsCustom == 0;

    int VinylGroupsCount = VinylGroups.size();
    DWORD VinylBrandIconHash = -1;
    DWORD VinylBrandNameHash = -1;

    for (int i = 0; i < VinylGroupsCount; i++)
    {
        if (i == 0)
        {
            if (HasNoCustomVinyls) continue;

            VinylBrandIconHash = CarConfigs[CarTypeID].Icons.PaintVinylsCustom;
            VinylBrandNameHash = CarConfigs[CarTypeID].Names.PaintVinylsCustom;

            if (VinylBrandIconHash == -1) VinylBrandIconHash = VinylGroups[i].TextureHash;
            if (VinylBrandNameHash == -1) VinylBrandNameHash = VinylGroups[i].StringHash;
        }
        else
        {
            VinylBrandIconHash = VinylGroups[i].TextureHash;
            VinylBrandNameHash = VinylGroups[i].StringHash;
        }

        PartSelectionScreen_AddVinylCategory(PartSelectionScreen, VinylGroups[i].Index, VinylBrandIconHash, VinylBrandNameHash);
    }

    unsigned int LastButton = FEngGetLastButton(PartSelectionScreenPackage);

    if (LastButton == -1)
    {
        DWORD* InstalledVinyl = CarCustomizeManager_GetInstalledVinyl((DWORD*)gCarCustomizeManager, *(int*)_CurrCarSlotID);
        if (!InstalledVinyl) InstalledVinyl = CarCustomizeManager_GetVinylInSetup((DWORD*)gCarCustomizeManager, *(int*)_CurrCarSlotID);

        if (InstalledVinyl)
        {
			LastButton = *((BYTE*)InstalledVinyl + 5) & 0x1F;
            FEngSetLastButton(PartSelectionScreenPackage, LastButton);
        }
    }
    
    ThumbnailScroller_SnapToItem(&PartSelectionScreen[19], LastButton);

    *((BYTE*)PartSelectionScreen + 471) = 1;
    *((BYTE*)PartSelectionScreen + 465) = 1;
    PartSelectionScreen[10] = 0x6109E4B5;
}

bool DoesPaintPartMatchFilter(DWORD* TheCarPart, DWORD AttributeHash, int DefaultValue, int PartAttribFilter, int CarSlotID)
{
    switch (CarSlotID)
    {
    case 164: // HUD_BACKING_COLOUR
    case 165: // HUD_NEEDLE_COLOUR
    case 166: // HUD_CHARACTER_COLOUR
        return 1;
    default:
        return CarPart_GetAppliedAttributeUParam(TheCarPart, AttributeHash, DefaultValue) == PartAttribFilter;
    }
    
    return 1;
}

int GetPartsList(int CarSlotID, DWORD* PartsBList, unsigned int PartAttribFilter, unsigned int Unk)
{
    DWORD* NewBNode; // eax MAPDST
    DWORD* BTail; // ecx MAPDST
    int PartsCount = 0; // eax MAPDST

    int SomethingUnk = *(int*)0x8389B0;
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
    int CarPartID = GetCarPartFromSlot(CarSlotID);

    int BogusCarSlotID = CarSlotID;

    //if (PartLink_EnabledForRide((DWORD*)gTheRideInfo)) PartLink_Resolve((DWORD*)gTheRideInfo);

    DWORD* TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, CarTypeID, CarSlotID, 0, -1);

    switch (CarSlotID)
    {
    case CARSLOTID_BASE:
    case CARSLOTID_FRONT_BUMPER:
    case CARSLOTID_REAR_BUMPER:
    case CARSLOTID_WIDE_BODY:
    case CARSLOTID_ROOF:
    case CARSLOTID_TOP:
    case CARSLOTID_HOOD:
    case CARSLOTID_TRUNK:
    case CARSLOTID_SKIRT:
    case CARSLOTID_SPOILER:
    case CARSLOTID_ENGINE:
    case CARSLOTID_HEADLIGHT:
    case CARSLOTID_BRAKELIGHT:
    case CARSLOTID_EXHAUST:
    case CARSLOTID_FENDER:
    case CARSLOTID_QUARTER:
    case CARSLOTID_FRONT_BRAKE:
    case CARSLOTID_REAR_BRAKE:
    case CARSLOTID_WING_MIRROR:
    case CARSLOTID_DAMAGE_FRONT:
    case CARSLOTID_DAMAGE_REAR:
    case CARSLOTID_DAMAGE_LEFT:
    case CARSLOTID_DAMAGE_RIGHT:
    case CARSLOTID_DAMAGE_TOP:
    case CARSLOTID_DOOR_PANEL_LEFT:
    case CARSLOTID_DOOR_PANEL_RIGHT:
    case CARSLOTID_DOOR_SILL_LEFT:
    case CARSLOTID_DOOR_SILL_RIGHT:
    case CARSLOTID_HOOD_UNDER:
    case CARSLOTID_TRUNK_UNDER:
        while (TheCarPart)
        {
            unsigned int IsCF = CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("CARBONFIBRE"), 0) != 0 ? 666 : 0;
            if (*((char*)TheCarPart + 4) == CarPartID && PartAttribFilter == IsCF)
            {
                if (UnlockSystem_IsCarPartUnlocked(CarCustomizeManager_GetPartUnlockFilter(), CarSlotID, TheCarPart, SomethingUnk)
                    && (CarSlotID != CARSLOTID_HOOD || (*((BYTE*)TheCarPart + 5) & 0x1F) != 5)
                    && !PartLink_IsHiddenFromMenu(TheCarPart) && !PartLink_IsSlotHidden(CarSlotID)
                    && PartLink_IsPartFiltered(TheCarPart, CarTypeID, CarSlotID))
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

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1);
        }
        break;
    case CARSLOTID_FRONT_WHEEL:
    case CARSLOTID_REAR_WHEEL:
        while (TheCarPart)
        {
            if (*((char*)TheCarPart + 4) == CarPartID && IsRimAvailable(CarTypeID, TheCarPart, PartAttribFilter))
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

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1);
        }
        break;
    case CARSLOTID_DECAL_HOOD:
    case CARSLOTID_DECAL_FRONT_WINDOW:
    case CARSLOTID_DECAL_REAR_WINDOW:
    case CARSLOTID_DECAL_LEFT_DOOR:
    case CARSLOTID_DECAL_RIGHT_DOOR:
    case CARSLOTID_DECAL_LEFT_QUARTER:
    case CARSLOTID_DECAL_RIGHT_QUARTER:
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

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1);
        }
        break;
    case CARSLOTID_BASE_PAINT:
    case CARSLOTID_PAINT_ENGINE:
    case CARSLOTID_PAINT_SPOILER:
    case CARSLOTID_PAINT_BRAKE:
    case CARSLOTID_PAINT_EXHAUST:
    case CARSLOTID_PAINT_AUDIO:
    case CARSLOTID_PAINT_RIM:
    case CARSLOTID_PAINT_SPINNER:
    case CARSLOTID_PAINT_ROOF:
    case CARSLOTID_PAINT_MIRROR:
    case CARSLOTID_VINYL_COLOUR0_0:
    case CARSLOTID_VINYL_COLOUR0_1:
    case CARSLOTID_VINYL_COLOUR0_2:
    case CARSLOTID_VINYL_COLOUR0_3:
    case CARSLOTID_VINYL_COLOUR1_0:
    case CARSLOTID_VINYL_COLOUR1_1:
    case CARSLOTID_VINYL_COLOUR1_2:
    case CARSLOTID_VINYL_COLOUR1_3:
    case CARSLOTID_VINYL_COLOUR2_0:
    case CARSLOTID_VINYL_COLOUR2_1:
    case CARSLOTID_VINYL_COLOUR2_2:
    case CARSLOTID_VINYL_COLOUR2_3:
    case CARSLOTID_VINYL_COLOUR3_0:
    case CARSLOTID_VINYL_COLOUR3_1:
    case CARSLOTID_VINYL_COLOUR3_2:
    case CARSLOTID_VINYL_COLOUR3_3:
    case CARSLOTID_HUD_BACKING_COLOUR:
    case CARSLOTID_HUD_NEEDLE_COLOUR:
    case CARSLOTID_HUD_CHARACTER_COLOUR:
        if (UnifyPaints && 
            (CarSlotID != CARSLOTID_HUD_BACKING_COLOUR 
            && CarSlotID != CARSLOTID_HUD_NEEDLE_COLOUR 
            && CarSlotID != CARSLOTID_HUD_CHARACTER_COLOUR)) // todo: unify hud paint later
        {
            for (int s = 0; s < 5; s++)
            {
                BogusCarSlotID = PaintCarSlotIDsToLookFor[s];

                TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, CarTypeID, BogusCarSlotID, 0, -1);
                while (TheCarPart)
                {
                    if (*((char*)TheCarPart + 4) == GetCarPartFromSlot(BogusCarSlotID)
                        && DoesPaintPartMatchFilter(TheCarPart, CT_bStringHash("BRAND_NAME"), 0, PartAttribFilter, CarSlotID))
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

                    TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, BogusCarSlotID, 0, -1);
                }
            }
        }
        else
        {
            while (TheCarPart)
            {
                if (*((char*)TheCarPart + 4) == CarPartID
                    && DoesPaintPartMatchFilter(TheCarPart, CT_bStringHash("BRAND_NAME"), 0, PartAttribFilter, CarSlotID))
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

                TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1);
            }
        }
        break;
        
    case CARSLOTID_VINYL_LAYER0:
    case CARSLOTID_VINYL_LAYER1:
    case CARSLOTID_VINYL_LAYER2:
    case CARSLOTID_VINYL_LAYER3:
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

            TheCarPart = CarPartDatabase_NewGetNextCarPart((DWORD*)_CarPartDB, TheCarPart, CarTypeID, CarSlotID, 0, -1);
        }
        break;
    default:
        return PartsCount;
    }
    return PartsCount;
}

void __fastcall PartSelectionScreen_BuildPartsList(DWORD* PartSelectionScreen, void* EDX_Unused, int slot)
{
    DWORD* Node; // ebx
    DWORD* PartsListHead; // eax
    DWORD* CurrPart; // esi
    DWORD* APartNode; // eax
    DWORD* CarPart; // ecx
    DWORD* NewNode; // eax
    DWORD* InstalledPart; // esi
    DWORD* VinylInSetup; // eax
    DWORD* v11; // eax
    DWORD* PartsListTail; // [esp+8h] [ebp-8h]
    DWORD* CategoryNode; // [esp+8h] [ebp-8h]
    DWORD* ThumbnailScroller; // [esp+Ch] [ebp-4h]

    ThumbnailScroller = PartSelectionScreen + 44;
    PartSelectionScreen_ResetCategories(ThumbnailScroller);
    Node = 0;
    PartsListHead = (DWORD*)(PartSelectionScreen[22] + 56);
    PartSelectionScreen[114] = 0;
    PartsListTail = PartsListHead;
    if ((DWORD*)*PartsListHead != PartsListHead)
    {
        CurrPart = (DWORD*)*PartsListHead;
        do
        {
            APartNode = (DWORD*)j__malloc(0x30u);
            if (APartNode)
            {
                CarPart = (DWORD*)CurrPart[2];
                APartNode[3] = (DWORD)CarPart;
                APartNode[4] = 0;
                APartNode[5] = 0;
                APartNode[6] = 0;
                *((BYTE*)APartNode + 32) = 0;
                *((BYTE*)APartNode + 33) = 1;
                *((BYTE*)APartNode + 34) = 0;
                *((BYTE*)APartNode + 35) = 0;
                *APartNode = _PartNode_vtbl;
                APartNode[11] = (DWORD)CarPart;
            }
            else
            {
                APartNode = 0;
            }
            NewNode = ThumbnailScroller_AddNode(ThumbnailScroller, APartNode, 1, -1);
            if (NewNode/* && NewNode[9] != 0x73C4EC2D*/)
            {
                if (CarPart)
                {
                    NewNode[9] = CarPart_GetAppliedAttributeUParam(CarPart, CT_bStringHash("TEXTUREHASH"), CT_bStringHash("GENERIC_ICON_PARTS")); // Icon via attribute
                }
                else
                {
                    NewNode[9] = CT_bStringHash("GENERIC_ICON_PARTS"); // GENERIC_ICON_PARTS
                }
                NewNode[7] |= 0x400000u;
            }
            ++PartSelectionScreen[114];
            CurrPart = (DWORD*)*CurrPart;
        } while (CurrPart != PartsListTail);
        CategoryNode = (DWORD*)PartSelectionScreen[22];
        if (!CategoryNode)
            goto LABEL_23;
        if (*((BYTE*)PartSelectionScreen + 471))
        {
            InstalledPart = RideInfo_GetPart((DWORD*)gTheRideInfo, *(int*)_CurrCarSlotID);
            if (InstalledPart)
                goto LABEL_16;
            VinylInSetup = CarCustomizeManager_GetVinylInSetup((DWORD*)gCarCustomizeManager, *(int*)_CurrCarSlotID);
        }
        else
        {
            VinylInSetup = RideInfo_GetPart((DWORD*)gTheRideInfo, slot);
        }
        InstalledPart = VinylInSetup;
    LABEL_16:
        if (*((BYTE*)CategoryNode + 52)
            && InstalledPart
            && (!*((BYTE*)PartSelectionScreen + 469) || CarPart_GetAppliedAttributeUParam(InstalledPart, 0x721AFF7C, 0)) // CARBONFIBRE
            && (!*((BYTE*)PartSelectionScreen + 471) || (*((BYTE*)InstalledPart + 5) & 0x1F) == (BYTE)slot))
        {
            ThumbnailScroller_SnapToItem(ThumbnailScroller, (int)InstalledPart);
        LABEL_26:
            *((BYTE*)ThumbnailScroller + 47) = 1;
            ThumbnailScroller_SetVisibility(ThumbnailScroller, 1);
            return;
        }
    LABEL_23:
        v11 = (DWORD*)ThumbnailScroller[1];
        if (v11)
            Node = v11 - 1;
        ThumbnailScroller_SnapToNode(ThumbnailScroller, Node);
        goto LABEL_26;
    }
}

// 0x566B7A
void __declspec(naked) WidebodyWarningCodeCave()
{
    _asm
    {
        push eax
        push edi
        push ebx
        call IsCustomWidebody
        test al, al
        jnz loc_566C10

        loc_566B8D :
            add esp, 8
            pop eax
            push 0x566B8D
            retn

        loc_566C10 :
            add esp, 8
            pop eax
            push 0x566C10
            retn
    }
}
/*
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
            push 0x566C10 // 0x566B8D
            retn

        loc_566C10:
            push 0x566C10
            retn
    }
}*/

char const* __fastcall PartSelectionScreen_GetPartName(DWORD* PartSelectionScreen, void* EDX_Unused, DWORD* TheCarPart)
{
    unsigned int AppliedAttributeUParam; // eax
    char const* result; // eax

    if (!TheCarPart)
        return ""; // NONE
    if (!CarPart_HasAppliedAttribute(TheCarPart, 0x4732DA07)) // LANGUAGEHASH
        return CarPart_GetName(TheCarPart); // Debug Name
    AppliedAttributeUParam = CarPart_GetAppliedAttributeUParam(TheCarPart, 0x4732DA07, 0); // Name from LANGUAGEHASH
    result = SearchForString(AppliedAttributeUParam);
    if (!result)
        return SearchForString(0xC01A6F63); // DEFAULT_STRING_ERROR
    return result;
}

bool PartSelectionScreen_CanPaintThisPart(DWORD* TheCarPart)
{
    int UpgradeLevel = (*((BYTE*)TheCarPart + 5) >> 5);
    int CarSlotID = *((BYTE*)TheCarPart + 4);
    bool Paint = 0;

    switch (CarSlotID)
    {
        case CARSLOTID_ENGINE:
        case CARSLOTID_TRUNK_AUDIO:
        case CARSLOTID_FRONT_BRAKE:
            Paint = (UpgradeLevel > 2) || (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("UNPAINTABLE"), 1) == 0);
            break;
        case CARSLOTID_SPOILER:
        case CARSLOTID_ROOF:
        case CARSLOTID_WING_MIRROR:
            Paint = (UpgradeLevel > 0) 
                && (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("UNPAINTABLE"), 0) == 0) 
                && (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("CARBONFIBRE"), 0) == 0);
            break;
        case CARSLOTID_FRONT_WHEEL:
        case CARSLOTID_REAR_WHEEL:
        case CARSLOTID_EXHAUST:
            Paint = (UpgradeLevel > 0);
            break;
        case CARSLOTID_FRONT_BUMPER:
        case CARSLOTID_REAR_BUMPER:
        case CARSLOTID_SKIRT:
        case CARSLOTID_WIDE_BODY:
            Paint = (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("UNPAINTABLE"), 0) == 0)
                && (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("CARBONFIBRE"), 0) == 0);
            break;
    }

    return Paint;
}

bool PartSelectionScreen_CanNeonThisPart(DWORD* TheCarPart)
{
    int UpgradeLevel = (*((BYTE*)TheCarPart + 5) >> 5);
    int CarSlotID = *((BYTE*)TheCarPart + 4);
    bool Neon = 0;

    switch (CarSlotID)
    {
    case CARSLOTID_ENGINE:
    case CARSLOTID_TRUNK_AUDIO:
        Neon = (UpgradeLevel > 2) || (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("NEON"), 1) != 0);
        break;
    case CARSLOTID_FRONT_BUMPER:
    case CARSLOTID_REAR_BUMPER:
    case CARSLOTID_SKIRT:
    case CARSLOTID_WIDE_BODY:
        Neon = 1;
        break;
    }

    return Neon;
}

void __fastcall PartSelectionScreen_RefreshHeader(DWORD* PartSelectionScreen, void* EDX_Unused)
{
    PartSelectionScreen_RefreshHeader_Game(PartSelectionScreen);

    // Read Part Options for the car
    //DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    //int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    //FEngSetLanguageHash((const char*)PartSelectionScreen[1], 0x42ADB44C, CarConfigs[CarTypeID].Category.BodyShop);

    RefreshDetailsPane((char const*)PartSelectionScreen[1], PartSelectionScreen + 45, PartSelectionScreen + 44, (DWORD*)PartSelectionScreen[47]);
}

void __fastcall PartSelectionScreen_StartBrowsingParts(DWORD* PartSelectionScreen, void* EDX_Unused)
{
    PartSelectionScreen_StartBrowsingParts_Game(PartSelectionScreen);

    SetDetailsPaneVisibility((char const*)PartSelectionScreen[1], 1);

    PartSelectionScreen_DoSpecialScroll(PartSelectionScreen, EDX_Unused, (DWORD*)PartSelectionScreen[22], 1);
}

void __fastcall PartSelectionScreen_StopBrowsingParts(DWORD* PartSelectionScreen, void* EDX_Unused, bool ass)
{
    PartSelectionScreen_StopBrowsingParts_Game(PartSelectionScreen, ass);

    SetDetailsPaneVisibility((char const*)PartSelectionScreen[1], 0);

    PartSelectionScreen_DoSpecialScroll(PartSelectionScreen, EDX_Unused, (DWORD*)PartSelectionScreen[22], 0);
}

void __fastcall PartSelectionScreen_SetupCarbonParts(DWORD* PartSelectionScreen, void* EDX_Unused, unsigned int slot)
{
    int PartUnlockFilter; // eax MAPDST
    bool locked; // bl MAPDST
    unsigned int icon; // ecx MAPDST
    DWORD* NewNode; // eax MAPDST
    DWORD* v12; // eax MAPDST
    DWORD* Node; // eax
    DWORD* v16; // eax

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    PartSelectionScreen_ResetCategories(&PartSelectionScreen[19]);
    PartSelectionScreen[113] = 0;
    PartSelectionScreen_AddCategoryChecked(
        PartSelectionScreen, 
        CARSLOTID_HOOD, 
        CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberHood,
        CarConfigs[CarTypeID].Names.BodyShopCarbonFiberHood,
        1);// VISUAL_PART_CARBON_FIBRE_HOODS

    PartSelectionScreen_AddCategoryChecked(
        PartSelectionScreen, 
        CARSLOTID_SPOILER,
        CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberSpoiler,
        CarConfigs[CarTypeID].Names.BodyShopCarbonFiberSpoiler,
        1);// VISUAL_PART_CARBON_FIBRE_SPOILERS

    PartSelectionScreen_AddCategoryChecked(
        PartSelectionScreen, 
        CARSLOTID_ROOF,
        CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberRoofScoop,
        CarConfigs[CarTypeID].Names.BodyShopCarbonFiberRoofScoop,
        1);// VISUAL_PART_CARBON_FIBRE_ROOF_SCOOP

    PartSelectionScreen_AddCategoryChecked(
        PartSelectionScreen, 
        CARSLOTID_WING_MIRROR,
        CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberMirrors,
        CarConfigs[CarTypeID].Names.BodyShopCarbonFiberMirrors,
        1);// VISUAL_PART_CARBON_FIBRE_MIRRORS

    /*
    CFDoor:
    PartUnlockFilter = CarCustomizeManager_GetPartUnlockFilter();
    locked = UnlockSystem_IsUnlockableUnlocked(PartUnlockFilter, 85, 3, *(DWORD*)0x8389B0) == 0;
    if (*(DWORD*)RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_DOOR_CARBON) == CT_bStringHash("CARBON FIBRE NONE"))
    {
        NewNode = (DWORD*)j__malloc(0x40u);
        if (NewNode)
        {
            NewNode[3] = CARSLOTID_DOOR_CARBON;
            NewNode[4] = 0;
            NewNode[5] = 0;
            NewNode[6] = 0;
            *((BYTE*)NewNode + 32) = 0;
            *((BYTE*)NewNode + 33) = 1;
            *NewNode = _CategoryNode_vtbl;
            NewNode[11] = CarConfigs[CarTypeID].Names.BodyShopCarbonFiberDoors; // VISUAL_PART_CARBON_FIBRE_DOOR
            NewNode[12] = 0;
            *((BYTE*)NewNode + 52) = 1;
            NewNode[14] = (DWORD)(NewNode + 14);
            NewNode[15] = (DWORD)(NewNode + 14);
            *((BYTE*)NewNode + 34) = locked;
            *((BYTE*)NewNode + 35) = 1;
        }
        else
        {
            NewNode = 0;
        }
        v12 = ThumbnailScroller_AddNode(PartSelectionScreen + 19, NewNode, 1, -1);
        if (!v12)
            goto CFTrunk;
        icon = CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberDoors; // VISUAL_PART_CARBON_FIBRE_DOOR
    }
    else
    {
        NewNode = (DWORD*)j__malloc(0x40u);
        if (NewNode)
        {
            NewNode[3] = CARSLOTID_DOOR_CARBON;
            NewNode[4] = 0;
            NewNode[5] = 0;
            NewNode[6] = 0;
            *((BYTE*)NewNode + 32) = 0;
            *((BYTE*)NewNode + 33) = 1;
            *NewNode = _CategoryNode_vtbl;
            NewNode[11] = CarConfigs[CarTypeID].Names.BodyShopCarbonFiberRevertDoors; // VISUAL_PART_REVERT_CARBON_FIBRE_DOOR
            NewNode[12] = 0;
            *((BYTE*)NewNode + 52) = 1;
            NewNode[14] = (DWORD)(NewNode + 14);
            NewNode[15] = (DWORD)(NewNode + 14);
            *((BYTE*)NewNode + 34) = locked;
            *((BYTE*)NewNode + 35) = 1;
        }
        else
        {
            NewNode = 0;
        }
        v12 = ThumbnailScroller_AddNode(PartSelectionScreen + 19, NewNode, 1, -1);
        if (!v12)
            goto CFTrunk;
        icon = CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberRevertDoors; // VISUAL_PART_DE_CARBON_FIBRE_DOOR
    }
    if (v12[9] != icon)
    {
        v12[9] = icon;
        v12[7] |= 0x400000u;
    }*/
    
CFTrunk:
    //++PartSelectionScreen[113];
    PartUnlockFilter = CarCustomizeManager_GetPartUnlockFilter();
    locked = UnlockSystem_IsUnlockableUnlocked(PartUnlockFilter, 86, 3, *(DWORD*)0x8389B0) == 0;
    if (*(DWORD*)RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_TRUNK_CARBON) == CT_bStringHash("CARBON FIBRE NONE"))
    {
        NewNode = (DWORD*)j__malloc(0x40u);
        if (NewNode)
        {
            NewNode[3] = CARSLOTID_TRUNK_CARBON;
            NewNode[4] = 0;
            NewNode[5] = 0;
            NewNode[6] = 0;
            *((BYTE*)NewNode + 32) = 0;
            *((BYTE*)NewNode + 33) = 1;
            *NewNode = _CategoryNode_vtbl;
            NewNode[11] = CarConfigs[CarTypeID].Names.BodyShopCarbonFiberTrunk; // VISUAL_PART_CARBON_FIBRE_TRUNK
            NewNode[12] = 0;
            *((BYTE*)NewNode + 52) = 1;
            NewNode[14] = (DWORD)(NewNode + 14);
            NewNode[15] = (DWORD)(NewNode + 14);
            *((BYTE*)NewNode + 34) = locked;
            *((BYTE*)NewNode + 35) = 1;
        }
        else
        {
            NewNode = 0;
        }
        v12 = ThumbnailScroller_AddNode(PartSelectionScreen + 19, NewNode, 1, -1);
        if (!v12)
            goto CFWidebody;
        icon = CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberTrunk; // VISUAL_PART_CARBON_FIBRE_TRUNK
    }
    else
    {
        NewNode = (DWORD*)j__malloc(0x40u);
        if (NewNode)
        {
            NewNode[3] = CARSLOTID_TRUNK_CARBON;
            NewNode[4] = 0;
            NewNode[5] = 0;
            NewNode[6] = 0;
            *((BYTE*)NewNode + 32) = 0;
            *((BYTE*)NewNode + 33) = 1;
            *NewNode = _CategoryNode_vtbl;
            NewNode[11] = CarConfigs[CarTypeID].Names.BodyShopCarbonFiberRevertTrunk; // VISUAL_PART_REVERT_CARBON_FIBRE_TRUNK
            NewNode[12] = 0;
            *((BYTE*)NewNode + 52) = 1;
            NewNode[14] = (DWORD)(NewNode + 14);
            NewNode[15] = (DWORD)(NewNode + 14);
            *((BYTE*)NewNode + 34) = locked;
            *((BYTE*)NewNode + 35) = 1;
        }
        else
        {
            NewNode = 0;
        }
        v12 = ThumbnailScroller_AddNode(PartSelectionScreen + 19, NewNode, 1, -1);
        if (!v12)
            goto CFWidebody;
        icon = CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberRevertTrunk; // VISUAL_PART_DE_CARBON_FIBRE_TRUNK
    }
    if (v12[9] != icon)
    {
        v12[9] = icon;
        v12[7] |= 0x400000u;
    }
CFWidebody:
    ++PartSelectionScreen[113];
    PartUnlockFilter = CarCustomizeManager_GetPartUnlockFilter();
    locked = UnlockSystem_IsUnlockableUnlocked(PartUnlockFilter, 85, 3, *(DWORD*)0x8389B0) == 0;
    if (*(DWORD*)RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_KIT_CARBON) == CT_bStringHash("CARBON FIBRE NONE"))
    {
        NewNode = (DWORD*)j__malloc(0x40u);
        if (NewNode)
        {
            NewNode[3] = CARSLOTID_KIT_CARBON;
            NewNode[4] = 0;
            NewNode[5] = 0;
            NewNode[6] = 0;
            *((BYTE*)NewNode + 32) = 0;
            *((BYTE*)NewNode + 33) = 1;
            *NewNode = _CategoryNode_vtbl;
            NewNode[11] = CarConfigs[CarTypeID].Names.BodyShopCarbonFiberWidebody; // VISUAL_PART_CARBON_FIBRE_KITS
            NewNode[12] = 0;
            *((BYTE*)NewNode + 52) = 1;
            NewNode[14] = (DWORD)(NewNode + 14);
            NewNode[15] = (DWORD)(NewNode + 14);
            *((BYTE*)NewNode + 34) = locked;
            *((BYTE*)NewNode + 35) = 1;
        }
        else
        {
            NewNode = 0;
        }
        v12 = ThumbnailScroller_AddNode(PartSelectionScreen + 19, NewNode, 1, -1);
        if (!v12)
            goto FinishMenu;
        icon = CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberWidebody; // VISUAL_PART_CARBON_FIBRE_KIT
    }
    else
    {
        NewNode = (DWORD*)j__malloc(0x40u);
        if (NewNode)
        {
            NewNode[3] = CARSLOTID_KIT_CARBON;
            NewNode[4] = 0;
            NewNode[5] = 0;
            NewNode[6] = 0;
            *((BYTE*)NewNode + 32) = 0;
            *((BYTE*)NewNode + 33) = 1;
            *NewNode = _CategoryNode_vtbl;
            NewNode[11] = CarConfigs[CarTypeID].Names.BodyShopCarbonFiberRevertWidebody; // VISUAL_PART_REVERT_CARBON_FIBRE_KITS
            NewNode[12] = 0;
            *((BYTE*)NewNode + 52) = 1;
            NewNode[14] = (DWORD)(NewNode + 14);
            NewNode[15] = (DWORD)(NewNode + 14);
            *((BYTE*)NewNode + 34) = locked;
            *((BYTE*)NewNode + 35) = 1;
        }
        else
        {
            NewNode = 0;
        }
        v12 = ThumbnailScroller_AddNode(PartSelectionScreen + 19, NewNode, 1, -1);
        if (!v12)
            goto FinishMenu;
        icon = CarConfigs[CarTypeID].Icons.BodyShopCarbonFiberRevertWidebody; // VISUAL_PART_DE_CARBON_FIBRE_KIT
    }
    if (v12[9] != icon)
    {
        v12[9] = icon;
        v12[7] |= 0x400000u;
    }
FinishMenu:
    ++PartSelectionScreen[113];
    Node = (DWORD*)ThumbnailScroller_FindNode((PartSelectionScreen + 19), slot);
    if (!Node)
    {
        v16 = (DWORD*)PartSelectionScreen[20];
        if (v16)
            Node = v16 - 1;
        else
            Node = 0;
    }
    ThumbnailScroller_SnapToNode((PartSelectionScreen + 19), Node);
    bList_TraversebList(PartSelectionScreen + 20, 0);
    *((BYTE*)PartSelectionScreen + 123) = 1;
    ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 1);
    *((BYTE*)PartSelectionScreen + 465) = 1;
    PartSelectionScreen_RefreshHeader(PartSelectionScreen, EDX_Unused);
}

#define byte_8381E1 *(BYTE*)0x8381E1
#define dword_7FA1A8 *(int*)0x7FA1A8
#define dword_8763EC *(float*)0x8763EC
#define dword_83A9D8 *(DWORD*)0x83A9D8
#define dword_83898C *(DWORD*)0x83898C
#define dword_838970 *(DWORD*)0x838970
#define dword_838990 *(DWORD*)0x838990
#define dword_8389A0 *(DWORD*)0x8389A0

void __fastcall PartSelectionScreen_NotificationMessage(DWORD* PartSelectionScreen, void* EDX_Unused, DWORD message, DWORD* fe_obj, DWORD param1, DWORD param2)
{
    DWORD* v5 = nullptr;
    DWORD* v7 = (DWORD*)PartSelectionScreen[22];

    if ((message == 0xC407210 || message == 0x406415E3) && v7[3] == 29)
    {
        DialogInterface_ShowThreeButtons((char const*)PartSelectionScreen[1], "GenericDialog_ThreeButton.fng",
            CT_bStringHash((char*)"CUSTOMIZE_FRONT_WHEEL"),
            CT_bStringHash((char*)"CUSTOMIZE_REAR_WHEEL"),
            CT_bStringHash((char*)"CUSTOMIZE_ALL_WHEELS"),
            CT_bStringHash((char*)"CUSTOMIZE_FRONT_WHEEL"),
            CT_bStringHash((char*)"CUSTOMIZE_REAR_WHEEL"),
            CT_bStringHash((char*)"CUSTOMIZE_ALL_WHEELS"),
            0xB4EDEB6D,
            1,
            2,
            CT_bStringHash((char*)"CUSTOMIZE_CHOOSE_FRONT_REAR_WHEEL"));
        return;
    }
    else if (message == CT_bStringHash((char*)"CUSTOMIZE_FRONT_WHEEL"))
    {
        RimsToCustomize = 1;
        PartSelectionScreen_StartBrowsingWheels(PartSelectionScreen);
        MakeFrontendSound(8, PartSelectionScreen[12]);
    }
    else if (message == CT_bStringHash((char*)"CUSTOMIZE_REAR_WHEEL"))
    {
        RimsToCustomize = -1;
        PartSelectionScreen_StartBrowsingWheels(PartSelectionScreen);
        MakeFrontendSound(8, PartSelectionScreen[12]);
    }
    else if (message == CT_bStringHash((char*)"CUSTOMIZE_ALL_WHEELS"))
    {
        RimsToCustomize = 0;
        PartSelectionScreen_StartBrowsingWheels(PartSelectionScreen);
        MakeFrontendSound(8, PartSelectionScreen[12]);
    }
    else
    {
        PartSelectionScreen_NotificationMessage_Game(PartSelectionScreen, message, fe_obj, param1, param2);
    }
    /*
    if ((message != 0x51123592) && (message != 0x406415E3) && (message != 0xC407210))
    {
		PartSelectionScreen_NotificationMessage_Game(PartSelectionScreen, message, fe_obj, param1, param2);
        return;
	}

    // Sound
    if (*((BYTE*)v7 + 34) && (message == 0xC407210 || message == 0x406415E3)) {
        if (byte_8381E1 != -1)
            EAXSound_PlayUISoundFX(*(DWORD**)_g_pEAXSound, byte_8381E1);
        return;
    }

    switch (message) {
    case 0x9803F6E2: {
        sub_538130(PartSelectionScreen + 69);

        switch (PartSelectionScreen[84]) {
        case -10:
            for (int i = 0; i < dword_7FA1A8; ++i)
                PartSelectionScreen_ScrollHorizontal(PartSelectionScreen, 1);
            break;
        case -1:
        case 1:
            PartSelectionScreen_ScrollHorizontal(PartSelectionScreen, PartSelectionScreen[84]);
            break;
        case 10:
            for (int j = 0; j < dword_7FA1A8; ++j)
                PartSelectionScreen_ScrollHorizontal(PartSelectionScreen, -1);
            break;
        default:
            break;
        }

        sub_538130(PartSelectionScreen + 91);

        float verticalInput = dword_8763EC;
        int verticalDir = 0;
        if (verticalInput > 0)
            verticalDir = -1;
        else if (verticalInput < 0)
            verticalDir = 1;

        switch (verticalDir) {
        case -10:
            for (int k = 0; k < dword_7FA1A8; ++k)
                PartSelectionScreen_ScrollVertical(PartSelectionScreen, 1);
            break;
        case -1:
        case 1:
            PartSelectionScreen_ScrollVertical(PartSelectionScreen, verticalDir);
            break;
        case 10:
            for (int m = 0; m < dword_7FA1A8; ++m)
                PartSelectionScreen_ScrollVertical(PartSelectionScreen, -1);
            break;
        default:
            break;
        }

        if (sub_52D090(*((float*)PartSelectionScreen + 120))) { // Mouse on OK button?
            FEngSendMessageToPackage(0xC407210, (const char*)PartSelectionScreen[1]);
        }
        break;
    }
                   // PAD_DOWN
    case 0x911C0A4B:
        PartSelectionScreen_ScrollVertical(PartSelectionScreen, 1);
        break;

        // PAD_LEFT
    case 0x9120409E:
        PartSelectionScreen_ScrollHorizontal(PartSelectionScreen, -1);
        break;

        // PAD_RIGHT
    case 0xB5971BF1:
        PartSelectionScreen_ScrollHorizontal(PartSelectionScreen, 1);
        break;

        // EXIT_COMPLETE
    case 0xD05FC3A3:
        *((BYTE*)PartSelectionScreen + 476) = 1;
        dword_83A9D8 = FEngMapJoyParamToJoyport(param1);
        ExitToRoamHelper((char const*)PartSelectionScreen[1]);
        break;

        // PAD_ACCEPT
    case 0x406415E3:
    case 0x0C407210:
    {
        if ((DWORD*)PartSelectionScreen[20] == PartSelectionScreen + 20 || *((BYTE*)v7 + 34))
            return;

        if (!*((BYTE*)PartSelectionScreen + 468) && v7[3] == 29) // FRONT_WHEEL
        { 
            PartSelectionScreen_StartBrowsingWheels(PartSelectionScreen);
            MakeFrontendSound(8, PartSelectionScreen[12]);
            return;
        }

        if (*((BYTE*)PartSelectionScreen + 469))
        {
            switch (v7[3])
            {
            case 96:
            { // TRUNK_CARBON
                DWORD* TrunkCarbonPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 96);
                DWORD dialogText = (*((DWORD*)TrunkCarbonPart) == 0xC8D313AC) // CARBON FIBRE NONE
                    ? (*(int*)_FEDatabase == 1 && !CarCustomizeManager_IsPartPurchased((DWORD*)gCarCustomizeManager, 96, TrunkCarbonPart) ? 0x3175F2B0 : 0x2B950C36) // CONFIRM_PURCHASE_CONVERT_TRUNK_TO_CARBON // CONFIRM_CONVERT_TRUNK_TO_CARBON
                    : 0xA645A29E; // CONFIRM_REVERT_TRUNK_FROM_CARBON

                PartSelectionScreen_SetControlMask();
                DialogInterface_ShowTwoButtons(
                    (char const*)PartSelectionScreen[1],
                    "",
                    0xF770, // YES
                    0x61C, // NO
                    0x51123592, // btn1
                    0xB4623F67, // btn2
                    0xB4623F67, // default btn
                    1,
                    dialogText
                );
                dword_838970 = 0xB4623F67;
                *((BYTE*)PartSelectionScreen + 123) = 0;
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 44), 0);
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "DIM", 1);
                MakeFrontendSound(8, PartSelectionScreen[12]);
                return;
            }

            case 95:
            { // DOOR_CARBON
                DWORD* DoorCarbonPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 95);
                DWORD dialogText = (*((DWORD*)DoorCarbonPart) == 0xC8D313AC) // CARBON FIBRE NONE
                    ? (*(int*)_FEDatabase == 1 && !CarCustomizeManager_IsPartPurchased((DWORD*)gCarCustomizeManager, 95, DoorCarbonPart) ? 0x3175F2B0 : 0x2B950C36) // CONFIRM_PURCHASE_CONVERT_TRUNK_TO_CARBON // CONFIRM_CONVERT_TRUNK_TO_CARBON
                    : 0xA645A29E; // CONFIRM_REVERT_TRUNK_FROM_CARBON

                PartSelectionScreen_SetControlMask();
                DialogInterface_ShowTwoButtons(
                    (char const*)PartSelectionScreen[1],
                    "",
                    0xF770, // YES
                    0x61C, // NO
                    0x51123592, // btn1
                    0xB4623F67, // btn2
                    0xB4623F67, // default btn
                    1,
                    dialogText
                );
                dword_838970 = 0xB4623F67;
                *((BYTE*)PartSelectionScreen + 123) = 0;
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 44), 0);
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "DIM", 1);
                MakeFrontendSound(8, PartSelectionScreen[12]);
                return;
            }

            case 93:
            { // KIT_CARBON
                DWORD* WideBodyPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 6); // WIDE_BODY
                bool HasWideBody = WideBodyPart && ((*((BYTE*)WideBodyPart + 5) >> 5) != 0);
                bool AreVinylsCompatibleWithCF = true;

                for (int i = 0; i < 4; ++i) {
                    DWORD* InstalledVinyl = RideInfo_GetPart((DWORD*)gTheRideInfo, i + 64); // VINYL_LAYER0-3
                    if (InstalledVinyl) {
                        BYTE VinylGroupIndex = *((BYTE*)InstalledVinyl + 5) & 0x1F;
                        if (HasWideBody) {
                            if (VinylGroupIndex != 20) // VINYL_GROUP_HOOD
                                AreVinylsCompatibleWithCF = false;
                        }
                        else {
                            switch (VinylGroupIndex) {
                            case 14: // VINYL_GROUP_FLAG
                            case 15: // VINYL_GROUP_FLAG_SET
                            case 21: // VINYL_GROUP_AFTER_MARKET
                            case 22: // VINYL_GROUP_MANUFACTURER
                            case 23: // VINYL_GROUP_BODY
                            case 24: // VINYL_GROUP_UNIQUE
                            case 25: // VINYL_GROUP_TOP_LAYER
                            case 26: // VINYL_GROUP_ART_FACTORY
                                AreVinylsCompatibleWithCF = false;
                                break;
                            }
                        }
                    }
                }

                DWORD* KitCarbonPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 93); // KIT_CARBON
                DWORD dialogText = (*((DWORD*)KitCarbonPart) == 0xC8D313AC) // CARBON FIBRE NONE
                    ? (AreVinylsCompatibleWithCF
                        ? (*(int*)_FEDatabase == 1 && !CarCustomizeManager_IsPartPurchased((DWORD*)gCarCustomizeManager, 93, KitCarbonPart) ? 0xB7C84DA4 : 0x9E274DAA) // CONFIRM_PURCHASE_CONVERT_KIT_TO_CARBON / CONFIRM_CONVERT_KIT_TO_CARBON
                        : 0xEE2C85BB) // CUSTOMIZE_CONFIRM_VINYL_CARBON_KIT
                    : 0x660BDF52; // CONFIRM_REVERT_KIT_FROM_CARBON

                PartSelectionScreen_SetControlMask();
                DialogInterface_ShowTwoButtons(
                    (char const*)PartSelectionScreen[1],
                    "",
                    0xF770, // YES
                    0x61C, // NO
                    0x51123592, // btn1
                    0xB4623F67, // btn2
                    0xB4623F67, // default btn
                    1,
                    dialogText
                );
                dword_838970 = 0xB4623F67;
                *((BYTE*)PartSelectionScreen + 123) = 0;
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "DIM", 1);
                MakeFrontendSound(8, PartSelectionScreen[12]);
                return;
            }

            case 6: { // WIDE_BODY
                DWORD* KitCarbonPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 93); // KIT_CARBON
                bool hasKitCarbon = KitCarbonPart && ((*((BYTE*)KitCarbonPart + 5) >> 5) != 0);

                bool HasIncompatibleVinyls = false;
                for (int i = 0; i < 4; ++i) {
                    DWORD* InstalledVinyl = RideInfo_GetPart((DWORD*)gTheRideInfo, i + 64); // VINYL_LAYER0-3
                    if (InstalledVinyl && (*((BYTE*)InstalledVinyl + 5) & 0x1F) != 20) // VINYL_GROUP_HOOD
                        HasIncompatibleVinyls = true;
                }

                if (hasKitCarbon && HasIncompatibleVinyls) {
                    PartSelectionScreen_SetControlMask();
                    DialogInterface_ShowOk("", (char const*)aGenericDialog_SMALL_fng, 0x82DFD62A); // CUSTOMIZE_CONFIRM_WIDEBODY_VINYL_CARBON
                    *((BYTE*)PartSelectionScreen + 123) = 0;
                    ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);
                    *((BYTE*)PartSelectionScreen + 223) = 0;
                    ThumbnailScroller_SetVisibility((PartSelectionScreen + 44), 0);
                    FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "MENUFADEOUT", 1);
                    MakeFrontendSound(8, PartSelectionScreen[12]);
                    PartSelectionScreen_StartBrowsingParts(PartSelectionScreen, EDX_Unused);
                    return;
                }
                break;
            }

            case 9:
            { // HOOD
                if (*((BYTE*)PartSelectionScreen + 473))
                    break;

                bool HasNoHoodVinyls = true;

                for (int i = 0; i < 4; ++i)
                {
                    DWORD* InstalledVinyl = RideInfo_GetPart((DWORD*)gTheRideInfo, i + 64); // VINYL_LAYER0-3
                    if (InstalledVinyl && ((*((BYTE*)InstalledVinyl + 5) & 0x1F) == 0x14))
                    {
                        HasNoHoodVinyls = false;
                        break;
                    }
                }

                bool HasDecals = false;
                for (int j = 0; j < 8; ++j)
                {
                    DWORD* InstalledDecal = RideInfo_GetPart((DWORD*)gTheRideInfo, j + 97); // DECAL_HOOD_TEX0-7
                    if (InstalledDecal)
                    {
                        HasDecals = true;
                        break;
                    }
                }

                if (!HasNoHoodVinyls)
                {
                    PartSelectionScreen_SetControlMask();
                    if (HasDecals)
                    {
                        DialogInterface_ShowOk("", (char const*)aGenericDialog_SMALL_fng, 0x9796DFB4); // CUSTOMIZE_CONFIRM_REMOVE_HOOD_DECALS_VINYLS
                        dword_838970 = 0xB4623F67;
                    }
                    else
                    {
                        DialogInterface_ShowOk("", (char const*)aGenericDialog_SMALL_fng, 0xB3BBB11D); // CUSTOMIZE_CONFIRM_VINYL_CARBON_HOOD
                        dword_838970 = 0xB4623F67;
                    }
                }
                else if (HasDecals)
                {
                    PartSelectionScreen_SetControlMask();
                    DialogInterface_ShowOk("", (char const*)aGenericDialog_SMALL_fng, 0x903854F0); // CUSTOMIZE_CONFIRM_REMOVE_HOOD_DECALS
                    dword_838970 = 0xB4623F67;
                }
                else
                {
                    PartSelectionScreen_StartBrowsingParts(PartSelectionScreen, EDX_Unused);
                }

                *((BYTE*)PartSelectionScreen + 123) = 0;
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);
                *((BYTE*)PartSelectionScreen + 223) = 0;
                ThumbnailScroller_SetVisibility((PartSelectionScreen + 44), 0);
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "MENUFADEOUT", 1);
                MakeFrontendSound(8, PartSelectionScreen[12]);
                break;
            }

            default:
                break;
            }
        }
        else if (v7[3] == 94) // carbon parts
        {
            PartSelectionScreen_StartBrowsingCarbonParts(PartSelectionScreen);
            MakeFrontendSound(8, PartSelectionScreen[12]);
            return;
        }
        
        if (!*((BYTE*)PartSelectionScreen + 473))
        {
            PartSelectionScreen_StartBrowsingParts(PartSelectionScreen, EDX_Unused);
        }

        break;
    }

    // PAD_BACK
    case 0x911AB364:
    {
        if (*((BYTE*)PartSelectionScreen + 473)) {
            PartSelectionScreen_StopBrowsingParts(PartSelectionScreen, EDX_Unused, 1);
        }
        else if (dword_83898C || *((BYTE*)PartSelectionScreen + 471) || *((BYTE*)PartSelectionScreen + 469))
        {
            if (*((BYTE*)PartSelectionScreen + 468))
            {
                *((BYTE*)PartSelectionScreen + 468) = 0;
                *((BYTE*)PartSelectionScreen + 467) = 1;
            }
            else if (*((BYTE*)PartSelectionScreen + 469))
            {
                *((BYTE*)PartSelectionScreen + 469) = 0;
                *((BYTE*)PartSelectionScreen + 467) = 1;
            }
            else
            {
                *((BYTE*)PartSelectionScreen + 467) = 0;
            }

            *((BYTE*)PartSelectionScreen + 123) = 0;
            ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);

            if (!FEngIsScriptRunning_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, 0xDE6EFF34)) // FORWARD
            {
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "FORWARD", 1); // FORWARD
            }

            *((BYTE*)PartSelectionScreen + 466) = 0;
            *((BYTE*)PartSelectionScreen + 476) = 1;
            MakeFrontendSound(10, PartSelectionScreen[12]);
        }
        else
        {
            PartSelectionScreen_SetControlMask();
            DialogInterface_ShowTwoButtons(
                (char const*)PartSelectionScreen[1],
                "",
                0xF770, // YES
                0x61C, // NO
                0xD05FC3A3, // btn 1
                0x1FAB5998, // btn 2
                0x1FAB5998, // default
                1,
                0xD76E0B5A // SHOP_EXIT_CONFIRMATION
            );
            dword_838970 = 0xB4623F67;
            *((BYTE*)PartSelectionScreen + 466) = 0;
            *((BYTE*)PartSelectionScreen + 123) = 0;
            ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 0);
            FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "MENUFADEOUT", 1);
        }
        break;
    }

    // EXIT_COMPLETE
    case 0xE1FDE1D1:
    {
        if (*((BYTE*)PartSelectionScreen + 474))
        {
            *((BYTE*)PartSelectionScreen + 474) = 0;
            break;
        }

        if (!dword_83898C && !*((BYTE*)PartSelectionScreen + 469) && !*((BYTE*)PartSelectionScreen + 471))
        {
            if (!*((BYTE*)PartSelectionScreen + 468) && !*((BYTE*)PartSelectionScreen + 467))
                break;
        }

        if (!*((BYTE*)PartSelectionScreen + 468))
        {
            if (*((BYTE*)PartSelectionScreen + 469))
            {
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "CALL", 1);
                *((BYTE*)PartSelectionScreen + 123) = 0;
                *((BYTE*)PartSelectionScreen + 223) = 0;
                *((BYTE*)PartSelectionScreen + 466) = 1;
                PartSelectionScreen_SetupCarbonParts(PartSelectionScreen, EDX_Unused, 9);
            }
            else if (*((BYTE*)PartSelectionScreen + 467))
            {
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "MENUFADE", 1);
                *((BYTE*)PartSelectionScreen + 123) = 0;
                *((BYTE*)PartSelectionScreen + 223) = 0;
                *((BYTE*)PartSelectionScreen + 466) = 1;
                CarCustomizeManager_ResetPreviewToPaintSetup((DWORD*)gCarCustomizeManager);
                *((BYTE*)PartSelectionScreen + 476) = 0;
                *((BYTE*)PartSelectionScreen + 467) = 0;
                PartSelectionScreen_Setup(PartSelectionScreen);
            }
            else
            {
                CarCustomizeManager_ResetPreviewToPaintSetup((DWORD*)gCarCustomizeManager);
                FEngSetLastButton((const char*)PartSelectionScreen[1], -1);
                DWORD* manager = CustomizationScreenManager_Instance();
                CustomizationScreenManager_ChangeToPrevScreen(manager);
            }
        }
        else
        {
            DWORD* manager = CustomizationScreenManager_Instance();
            CustomizationScreenManager_ChangeToNextScreen(manager);
        }

        CarViewer_StopLookingAtParts(0, 1);
        break;
    }

    // PAD_UP
    case 0x72619778:
        PartSelectionScreen_ScrollVertical(PartSelectionScreen, -1);
        break;

        // INIT_COMPLETE
    case 0x35F8620B:
        PartSelectionScreen_UnsetControlMask();
        *((BYTE*)PartSelectionScreen + 475) = 1;
        *((BYTE*)PartSelectionScreen + 123) = 1;
        *((BYTE*)PartSelectionScreen + 223) = 1;

        if (!dword_838990)
            CareerSpeech_HandleFESpeechEvent((DWORD*)0x8635BC, 2);

        cFrontendDatabase_AllocBackupDB((DWORD*)_FEDatabase_0, 0);
        break;

    // BUY_AND_INSTALL_SELECTED_PART
    case 0x9F00F07:
    {
        PartSelectionScreen_UnsetControlMask();
        DWORD* selected = *(DWORD**)(PartSelectionScreen_GetSelectedPart(PartSelectionScreen) + 44);
        int slot = *(DWORD*)(PartSelectionScreen[22] + 12);

        PlayerCareerState_BuyCarPart((DWORD*)ThePlayerCareer, slot, selected);
        CarCustomizeManager_InstallPart((DWORD*)gCarCustomizeManager, slot, selected);
        FEPrintf((const char*)PartSelectionScreen[1], 0xEB3A688A, "%$d", *(int*)PlayerCash);

        *((BYTE*)PartSelectionScreen + 123) = 1;
        ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 1);
        PartSelectionScreen_StopBrowsingParts(PartSelectionScreen, EDX_Unused, 0);
        break;
    }

    // CONFIRM_AND_INSTALL
    case 0x51123592:
    {
        PartSelectionScreen_UnsetControlMask();

        if (*((BYTE*)PartSelectionScreen + 469))
        {
            DWORD* CategoryNode = (DWORD*)PartSelectionScreen[22];
            int slot = CategoryNode[3];
            if (slot == 96 || slot == 95 || slot == 93) // TRUNK_CARBON, DOOR_CARBON, KIT_CARBON
            {
                DWORD* l_head = (DWORD*)&l_head; // Initialize bList
                DWORD* l_tail = (DWORD*)&l_head;
                //v71 = 0;
                int partid = GetCarPartFromSlot(slot);

                DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
                int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

                DWORD* TheCarPart = CarPartDatabase_NewGetFirstCarPart(
                    (DWORD*)_CarPartDB,
                    CarTypeID,
                    slot,
                    0,
                    -1);

                for (; TheCarPart;
                    TheCarPart = CarPartDatabase_NewGetNextCarPart(
                        (DWORD*)_CarPartDB,
                        TheCarPart,
                        CarTypeID,
                        slot,
                        0,
                        -1))
                {
                    if (*((BYTE*)TheCarPart + 4) == partid && CarCustomizeManager_IsPartAvailable((DWORD*)gCarCustomizeManager, slot, TheCarPart))
                    {
                        DWORD* APartNode = (DWORD*)j__malloc(0x10u);
                        if (APartNode)
                        {
                            APartNode[2] = (DWORD)TheCarPart;
                            APartNode[3] = 0;
                        }
                        else
                        {
                            APartNode = 0;
                        }
                        DWORD* last = l_tail;
                        *l_tail = (DWORD)APartNode;
                        l_tail = APartNode;
                        APartNode[1] = (DWORD)last;
                        *APartNode = (DWORD)&l_head;
                    }
                }
                if (*(DWORD*)RideInfo_GetPart((DWORD*)gTheRideInfo, slot) == 0xC8D313AC) // CARBON FIBRE NONE
                {
                    TheCarPart = (DWORD*)l_head[2];
                    if (*(DWORD*)TheCarPart == 0xC8D313AC) // CARBON FIBRE NONE
                        TheCarPart = (DWORD*)l_tail[2];
                }
                else
                {
                    TheCarPart = (DWORD*)l_head[2];
                    if (*(DWORD*)TheCarPart != 0xC8D313AC) // CARBON FIBRE NONE
                        TheCarPart = (DWORD*)l_tail[2];
                }
                if (*(int*)_FEDatabase == 1 && !CarCustomizeManager_IsPartPurchased((DWORD*)gCarCustomizeManager, slot, TheCarPart))
                {
                    PlayerCareerState_BuyCarPart((DWORD*)ThePlayerCareer, slot, TheCarPart);
                }
                CarCustomizeManager_InstallPart((DWORD*)gCarCustomizeManager, slot, TheCarPart);
                PartSelectionScreen_SetupCarbonParts(PartSelectionScreen, EDX_Unused, slot);
                CarCustomizeManager_PlaySound((DWORD*)gCarCustomizeManager, (DWORD*)TheCarPart);
                *((BYTE*)PartSelectionScreen + 123) = 1;
                ThumbnailScroller_SetVisibility(PartSelectionScreen + 19, 1);
                FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7Cu, "MENUFADE", 1);
                //v71 = -1;
                sub_4DEF30((DWORD*)&l_head); // Clean up bList
                return;
            }
        }

        DWORD* SelectedPart = PartSelectionScreen_GetSelectedPart(PartSelectionScreen);

        if (SelectedPart)
        {
            DWORD* TheCarPart = (DWORD*)SelectedPart[11];

            if (*((BYTE*)PartSelectionScreen + 471))
            {

                CarCustomizeManager_AddVinylToPaintSetup(
                    (DWORD*)gCarCustomizeManager,
                    *(int*)_CurrCarSlotID,
                    TheCarPart
                );

                if (TheCarPart && CarPart_GetAppliedAttributeUParam(TheCarPart, 0x39DD714, 0))
                {
                    *(DWORD**)0x839BB8 = TheCarPart;
                    dword_8389A0 = 1;
                    DWORD* manager = CustomizationScreenManager_Instance();
                    CustomizationScreenManager_ChangeToNextScreen(manager);
                }
                else
                {
                    DWORD* manager = CustomizationScreenManager_Instance();
                    CustomizationScreenManager_ChangeToPrevScreen(manager);
                }
                break;
            }

            CarCustomizeManager_InstallPart((DWORD*)gCarCustomizeManager, *(int*)(PartSelectionScreen[22] + 12), TheCarPart);
            CarCustomizeManager_PlaySound((DWORD*)gCarCustomizeManager, TheCarPart);
        }
        
        *((BYTE*)PartSelectionScreen + 123) = 1;
        ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 1);
        FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "MENUFADE", 1);
        break;
    }
    // Message: Set some internal toggle
    case 0x34F8A1DE:
        *((BYTE*)PartSelectionScreen + 223) = 1;
        break;

        // Message: Back button / cancel dialog confirmed
    case 0x1FAB5998: // No button
    case 0x34DC1BEC: // OK button??
    {
        PartSelectionScreen_SetControlMask();

        *((BYTE*)PartSelectionScreen + 123) = 1;
        ThumbnailScroller_SetVisibility((PartSelectionScreen + 19), 1);
        *((BYTE*)PartSelectionScreen + 466) = 1;

        if (*((BYTE*)PartSelectionScreen + 473))
        {
            *((BYTE*)PartSelectionScreen + 223) = 1;
            ThumbnailScroller_SetVisibility((PartSelectionScreen + 44), 1);
            FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "UNDIM", 1);
        }
        else
        {
            FEngSetScript_Pkg((const char*)PartSelectionScreen[1], 0x47FF4E7C, "MENUFADE", 1);
        }
        break;
    }

    default:
        break;
    }
    */
}
