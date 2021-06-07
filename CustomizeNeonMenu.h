#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"

void __fastcall CustomizeNeonMenu_Setup(DWORD* CustomizeNeonMenu, void* EDX_Unused)
{
    // Read Part Options for the car
    sprintf(CarTypeName, GetCarTypeName(*(int*)TopOrFullScreenRide));
    sprintf(CarININame, "UnlimiterData\\%s.ini", CarTypeName);
    CIniReader CarINI(CarININame);
    CIniReader GeneralINI("UnlimiterData\\_General.ini");

    char const* CustomizeNeonMenuPackage = (char const*)CustomizeNeonMenu[1];

    CustomizeNeonMenu[10] = bStringHash("HELP_CARSPECIALTIES_NEON_SELECT");

    CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 154, bStringHash("NEON_ICON_BASE"), bStringHash("UI_ICE_NEON_UNDERCAR"), -1);
    if (CarINI.ReadInteger("Specialties", "EngineNeon", GeneralINI.ReadInteger("Specialties", "EngineNeon", 1)) != 0)
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 155, bStringHash("NEON_ICON_ENGINE"), bStringHash("UI_ICE_NEON_ENGINE"), -1);
    if (CarINI.ReadInteger("Specialties", "CabinNeon", GeneralINI.ReadInteger("Specialties", "CabinNeon", 1)) != 0)
    {
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 156, bStringHash("NEON_ICON_CABIN"), bStringHash("UI_ICE_NEON_CABIN"), -1);
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 158, bStringHash("NEON_ICON_CABIN_CONFIGURE"), bStringHash("UI_ICE_NEON_CABIN_LAYOUT"), -1);
    }
    if (CarINI.ReadInteger("Specialties", "TrunkNeon", GeneralINI.ReadInteger("Specialties", "TrunkNeon", 1)) != 0)
        CustomizeNeonMenu_AddSelection(CustomizeNeonMenu, 157, bStringHash("NEON_ICON_TRUNK"), bStringHash("UI_ICE_NEON_TRUNK"), -1);

    DWORD LastButton = FEngGetLastButton(CustomizeNeonMenuPackage);
    (*(void(__thiscall**)(DWORD*, DWORD))(CustomizeNeonMenu[19] + 32))(CustomizeNeonMenu + 19, LastButton);
    FEngSetInvisible_Pkg(CustomizeNeonMenuPackage, 0x5978FC5F);
    FEngSetLanguageHash(CustomizeNeonMenuPackage, 0x42ADB44C, bStringHash("CUSTOMIZE_TITLE_INTERIORS_SHOP"));
}
