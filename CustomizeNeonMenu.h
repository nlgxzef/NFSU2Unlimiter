#include "stdio.h"
#include "InGameFunctions.h"

bool HasNeon(DWORD* Part)
{
	if (!Part) return false;
    if (CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("NEON"), 0)) return true;

    return (unsigned __int8)(*((BYTE*)Part + 5) >> 5) > 2u;
}

void __fastcall CustomizeNeonMenu_DoSpecialScroll(DWORD* CustomizeNeonMenu, void* EDX_Unused, DWORD* CategoryNode, bool on)
{
    if (CategoryNode)
    {
        GetAndDoFEPartAnim(CategoryNode[18], on, 1.0f);
    }
}

void __fastcall CustomizeNeonMenu_NotificationMessage(DWORD* CustomizeNeonMenu, void* EDX_Unused, DWORD message, DWORD *fe_obj, DWORD param1, DWORD param2)
{
    if (!*((BYTE*)CustomizeNeonMenu + 345))
        IconScrollerMenu_NotificationMessage(CustomizeNeonMenu, message, fe_obj, param1, param2);

    if (message > CT_bStringHash("PAD_BACK"))
    {
        switch (message)
        {
        case 0x9FBBD1D4:
            *((BYTE*)CustomizeNeonMenu + 123) = 1;
            break;
        case 0xC98356BA:
            if (*((BYTE*)CustomizeNeonMenu + 346))
            {
                BYTE v10 = *((BYTE*)CustomizeNeonMenu + 347);
                *((BYTE*)CustomizeNeonMenu + 346) = 0;
                if (v10)
                {
                    FEngSetLastButton((const char*)CustomizeNeonMenu[1], 0);
                    CarViewer_StopLookingAtParts(0, 1);
                    CustomizationScreenManager_ChangeToPrevScreen(CustomizationScreenManager_Instance());
                }
                else
                {
                    CustomizationScreenManager_ChangeToNextScreen(CustomizationScreenManager_Instance());
                }
            }
            break;
        case CT_bStringHash("EXIT_COMPLETE"):
            *((BYTE*)CustomizeNeonMenu + 346) = 1;
            break;
        }
    }
    else if (message == CT_bStringHash("PAD_BACK"))
    {
        if (*((BYTE*)CustomizeNeonMenu + 345))
        {
            CustomizeNeonMenu_StopBrowsingParts(CustomizeNeonMenu);
            //dword_838998 = 7;
        }
        else if (!FEngIsScriptRunning_Pkg((const char*)CustomizeNeonMenu[1], CT_bStringHash("_EVENT_HANDLER_"), CT_bStringHash("FORWARD")))
        {
            *((BYTE*)CustomizeNeonMenu + 347) = 1;
            CustomizeNeonMenu_DoSpecialScroll(CustomizeNeonMenu, EDX_Unused, (DWORD*)CustomizeNeonMenu[22], 0);
            FEngSendMessageToPackage(0x81F0B71A, (const char*)CustomizeNeonMenu[87]);
            FEngSetScript_Pkg_Hsh((const char*)CustomizeNeonMenu[1], CT_bStringHash("_EVENT_HANDLER_"), CT_bStringHash("FORWARD"), 1);
        }
    }
    else
    {
        if (message != CT_bStringHash("BUTTON_PRESSED"))
        {
            if (message == 0x34DC1BEC)
            {
                if (*((BYTE*)CustomizeNeonMenu + 345))
                    *((BYTE*)CustomizeNeonMenu + 123) = 0;
            }
            else if (message == 0x8ACAA8DF)
            {
                FEngSetVisible_Pkg((char*)CustomizeNeonMenu[1], CT_bStringHash("U2_ICON_HIGHLIGHT"));
                *((BYTE*)CustomizeNeonMenu + 345) = 0;
                *((BYTE*)CustomizeNeonMenu + 123) = 1;
                CarViewer_StopLookingAtParts(0, 1);
                IconScrollerMenu_StopDim(CustomizeNeonMenu);
                FEngSetControlMask((char*)CustomizeNeonMenu[1], 255);
            }
            return;
        }
        if (*(BYTE*)(CustomizeNeonMenu[22] + 34))
            return;
        DWORD *Part = RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_ENGINE);
        if (*(DWORD*)(CustomizeNeonMenu[22] + 72) == CARSLOTID_NEON_ENGINE && !HasNeon(Part))
        {
            DialogInterface_ShowOk((const char*)CustomizeNeonMenu[1], "GenericDialog_SMALL.fng", CT_bStringHash("CUSTOMIZE_CONFIRM_NEED_TO_UPGRADE_ENGINE"));
            dword_838970 = 0x34DC1BEC;
            return;
        }
        Part = RideInfo_GetPart((DWORD*)gTheRideInfo, CARSLOTID_TRUNK_AUDIO);
        if (*(DWORD*)(CustomizeNeonMenu[22] + 72) == CARSLOTID_NEON_TRUNK && !HasNeon(Part))
        {
            DialogInterface_ShowOk((const char*)CustomizeNeonMenu[1], "GenericDialog_SMALL.fng", CT_bStringHash("CUSTOMIZE_CONFIRM_NEED_TO_UPGRADE_TRUNK_AUDIO"));
            dword_838970 = 0x34DC1BEC;
            return;
        }
        if (!*(BYTE*)(CustomizeNeonMenu[22] + 34))
        {
            if (*((BYTE*)CustomizeNeonMenu + 345))
            {
                char const *v9 = (const char*)CustomizeNeonMenu[87];
                if (v9)
                    FEngSendMessageToPackage(CT_bStringHash("BUTTON_PRESSED"), v9);
            }
            else
            {
                CustomizeNeonMenu_StartBrowsingParts(CustomizeNeonMenu);
                //dword_838998 = 9;
            }
        }
    }
}

void __fastcall CustomizeNeonMenu_Setup(DWORD* CustomizeNeonMenu, void* EDX_Unused)
{
    char const* CustomizeNeonMenuPackage = (char const*)CustomizeNeonMenu[1];

    CustomizeNeonMenu[10] = CT_bStringHash("HELP_CARSPECIALTIES_NEON_SELECT");

    // Read Part Options for the car
    DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
    int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

    if (CarConfigs[CarTypeID].Specialties.UnderglowNeon) 
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 
            CARSLOTID_NEON, 
            CarConfigs[CarTypeID].Icons.SpecialtiesUnderglowNeon, 
            CarConfigs[CarTypeID].Names.SpecialtiesUnderglowNeon, 
            -1);

    if (CarConfigs[CarTypeID].Specialties.EngineNeon) 
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 
            CARSLOTID_NEON_ENGINE, 
            CarConfigs[CarTypeID].Icons.SpecialtiesEngineNeon, 
            CarConfigs[CarTypeID].Names.SpecialtiesEngineNeon, 
            -1);

    if (CarConfigs[CarTypeID].Specialties.CabinNeon)
    {
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 
            CARSLOTID_CABIN_NEON_FRAME,
            CarConfigs[CarTypeID].Icons.SpecialtiesCabinNeonLayout, 
            CarConfigs[CarTypeID].Names.SpecialtiesCabinNeonLayout, 
            -1);
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 
            CARSLOTID_NEON_CABIN, 
            CarConfigs[CarTypeID].Icons.SpecialtiesCabinNeon, 
            CarConfigs[CarTypeID].Names.SpecialtiesCabinNeon, 
            -1);
    }

    if (CarConfigs[CarTypeID].Specialties.TrunkNeon) 
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 
            CARSLOTID_NEON_TRUNK, 
            CarConfigs[CarTypeID].Icons.SpecialtiesTrunkNeon, 
            CarConfigs[CarTypeID].Names.SpecialtiesTrunkNeon, 
            -1);

    DWORD LastButton = FEngGetLastButton(CustomizeNeonMenuPackage);
    (*(void(__thiscall**)(DWORD*, DWORD))(CustomizeNeonMenu[19] + 32))(CustomizeNeonMenu + 19, LastButton);
    FEngSetInvisible_Pkg(CustomizeNeonMenuPackage, 0x5978FC5F);
    FEngSetLanguageHash(CustomizeNeonMenuPackage, CT_bStringHash("HEADER_TEXT"), CT_bStringHash("CUSTOMIZE_TITLE_INTERIORS_SHOP"));
}
