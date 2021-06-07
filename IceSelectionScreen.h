#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"

DWORD GetCarTypeNameHashFromFECarConfig()
{
	DWORD* FECarConfig = (DWORD*)*(DWORD*)_FECarConfigRef;
	return (DWORD)(*(int(__thiscall**)(DWORD*))(*(DWORD*)FECarConfig + 8))(FECarConfig);
}

void __fastcall IceSelectionScreen_Setup(DWORD* IceSelectionScreen, void* EDX_Unused)
{
	// Read Part Options for the car
	DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
	int TuningCarType = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
	sprintf(CarTypeName, GetCarTypeName(TuningCarType));
	sprintf(CarININame, "UnlimiterData\\%s.ini", CarTypeName);
	CIniReader CarINI(CarININame);
	CIniReader GeneralINI("UnlimiterData\\_General.ini");

	char const* IceSelectionScreenPackage = (char const*)IceSelectionScreen[1];
	DWORD* AMenuOption;

	FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, bStringHash("CUSTOMIZE_TITLE_INTERIORS_SHOP"));

	switch (*(int*)0x838998)
	{
	case 0:
		if (IsCarPartsAnimLoadedForCar(GetCarTypeNameHashFromFECarConfig()))
		{
			if (FEGetAnimationState(1))
			{
				if (!FEGetAnimationState(0))
					FEDoCarPartAnimNow(0, 0, 1.0f);
			}
			else
			{
				FEDoCarPartAnimNow(1, 0, 1.0f);
			}
		}
		IceSelectionScreen[10] = bStringHash("HELP_CARSPECIALTIES_SHOP_MAIN");

		if (*(int*)0x83898C != 1)
			IceSelectionScreen_AddSelection(IceSelectionScreen, 163, bStringHash("VISUAL_PART_CUSTOM_HUDS"), bStringHash("UI_ICE_GAUGES"), -1, "UI_CustomHUDOverlay.fng");

		if (CarINI.ReadInteger("Specialties", "Neon", GeneralINI.ReadInteger("Specialties", "Neon", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x60);
			if (AMenuOption)
			{
				IceCategoryThing_Create(AMenuOption, 7, 158, bStringHash("VISUAL_PART_NEON"), bStringHash("UI_ICE_NEON"));
				*((BYTE*)AMenuOption + 34) = IsNeonCompletelyLocked();
				*((BYTE*)AMenuOption + 35) = 1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarINI.ReadInteger("Specialties", "WindowTint", GeneralINI.ReadInteger("Specialties", "WindowTint", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 153, bStringHash("VISUAL_PART_WINDOW_TINTING"), bStringHash("UI_ICE_TINT"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarINI.ReadInteger("Specialties", "HeadlightColor", GeneralINI.ReadInteger("Specialties", "HeadlightColor", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 159, bStringHash("VISUAL_PART_HEAD_LIGHT_COLOR"), bStringHash("UI_ICE_HEADLIGHTS"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarINI.ReadInteger("Specialties", "NosPurge", GeneralINI.ReadInteger("Specialties", "NosPurge", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 162, bStringHash("VISUAL_PART_NOS_PURGE"), bStringHash("UI_ICE_NOS_PURGE"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarINI.ReadInteger("Specialties", "Hydraulics", GeneralINI.ReadInteger("Specialties", "Hydraulics", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 161, bStringHash("VISUAL_PART_HYDRAULICS"), bStringHash("UI_ICE_HYDRAULICS"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarINI.ReadInteger("Specialties", "TrunkAudio", GeneralINI.ReadInteger("Specialties", "TrunkAudio", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x60);
			if (AMenuOption)
			{
				IceCategoryTrunkThing_Create(AMenuOption);
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarINI.ReadInteger("Specialties", "Spinners", GeneralINI.ReadInteger("Specialties", "Spinners", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 29, bStringHash("VISUAL_PART_SPINNERS"), bStringHash("UI_ICE_SPINNERS"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarINI.ReadInteger("Specialties", "SplitHoods", GeneralINI.ReadInteger("Specialties", "SplitHoods", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 9, bStringHash("VISUAL_PART_SPLIT_HOODS"), bStringHash("UI_ICE_HOOD"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = 0;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarINI.ReadInteger("Specialties", "Doors", GeneralINI.ReadInteger("Specialties", "Doors", 1)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 160, bStringHash("VISUAL_PART_SCISSOR_DOORS"), bStringHash("UI_ICE_DOORS"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = 2;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarINI.ReadInteger("Specialties", "LicensePlate", GeneralINI.ReadInteger("Specialties", "LicensePlate", 0)) != 0)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption, 33, bStringHash("VISUAL_PART_LICENSE_PLATES"), bStringHash("UI_ICE_LICENSE_PLATES"), 0, 1, *(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		FEngSetInvisible_Pkg(IceSelectionScreenPackage, 0xA2285688);
		FEngSetInvisible_Pkg(IceSelectionScreenPackage, 0xC318FF76);
		break;

	case 2:
		FEngSetVisible_Pkg(IceSelectionScreenPackage, 0xA2285688);
		FEngSetVisible_Pkg(IceSelectionScreenPackage, 0xC318FF76);
		FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, bStringHash("TRUNK_AUDIO_TITLE"));

		if (IsCarPartsAnimLoadedForCar(GetCarTypeNameHashFromFECarConfig()))
		{
			FEDoCarPartAnimNow(1, 1, 1.0f);
		}
		*((BYTE*)IceSelectionScreen + 360) = 1;
		IceSelectionScreen[10] = bStringHash("HELP_CARSPECIALTIES_SHOP_TRUNKLAYOUT_SELECT");

		AMenuOption = (DWORD*)j__malloc(0x5C);
		if (AMenuOption)
		{
			IceSelectionThing_Create(AMenuOption, 34, bStringHash("AUDIO_ICON_CONFIGURE_TRUNK_LAYOUT"), bStringHash("UI_ICE_TRUNK_AUDIO_LAYOUT_SELECTION"), 0, 1, *(char const**)gIceSideBarOverlayName);
			AMenuOption[21] = -1;
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			IceSelectionScreen_StartBrowsingParts(IceSelectionScreen);
		}
		else AMenuOption = 0;
		break;

	case 3:
		FEngSetInvisible_Pkg(IceSelectionScreenPackage, 0xA2285688);
		FEngSetInvisible_Pkg(IceSelectionScreenPackage, 0xC318FF76);
		FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, bStringHash("TRUNK_AUDIO_TITLE"));

		if (IsCarPartsAnimLoadedForCar(GetCarTypeNameHashFromFECarConfig()))
		{
			FEDoCarPartAnimNow(1, 1, 1.0f);
		}
		*((BYTE*)IceSelectionScreen + 360) = 1;
		IceSelectionScreen[10] = bStringHash("HELP_CARSPECIALTIES_SHOP_TRUNKLAYOUT_OR_COMPONENTS_SELECT");

		AMenuOption = (DWORD*)j__malloc(0x60);
		if (AMenuOption)
		{
			IceCategoryThing_Create(AMenuOption, 4, 35, bStringHash("AUDIO_ICON_COMPONENTS"), bStringHash("UI_ICE_TRUNK_AUDIO_COMPONENTS"));
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
		}
		else AMenuOption = 0;

		AMenuOption = (DWORD*)j__malloc(0x5C);
		if (AMenuOption)
		{
			IceSelectionThing_Create(AMenuOption, 34, bStringHash("AUDIO_ICON_CONFIGURE_TRUNK_LAYOUT"), bStringHash("UI_ICE_TRUNK_AUDIO_LAYOUT_SELECTION"), 0, 1, *(char const**)gIceSideBarOverlayName);
			AMenuOption[21] = -1;
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
		}
		else
		{
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, 0);
		}
		break;

	case 4:
	case 5:
		FEngSetInvisible_Pkg(IceSelectionScreenPackage, 0xA2285688);
		FEngSetInvisible_Pkg(IceSelectionScreenPackage, 0xC318FF76);
		FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, bStringHash("TRUNK_AUDIO_TITLE"));

		if (IsCarPartsAnimLoadedForCar(GetCarTypeNameHashFromFECarConfig()))
		{
			FEDoCarPartAnimNow(1, 1, 1.0f);
		}
		*((BYTE*)IceSelectionScreen + 360) = 1;
		IceSelectionScreen[10] = bStringHash("HELP_CARSPECIALTIES_SHOP_AUDIOCOMPONENTS_SELECT");

		for (int i = 35; i <= 46; ++i)
		{
			if (CarCustomizeManager_IsTrunkAudioSlotAvailable((DWORD*)gCarCustomizeManager, 0, i))
			{
				AMenuOption = (DWORD*)j__malloc(0x5C);
				if (AMenuOption)
				{
					IceSelectionThing_Create(AMenuOption, i, bStringHash("VISUAL_PART_TRUNK_AUDIO"), bStringHash("UI_ICE_TRUNK_AUDIO_COMPONENT"), 0, 1, *(char const**)gIceSideBarOverlayName);
					AMenuOption[21] = -1;
					(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
				}
				else
				{
					(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, 0);
				}
			}
		}
		break;

	default:
		break;
	}

	DWORD LastButton = FEngGetLastButton(IceSelectionScreenPackage);
	(*(void(__thiscall**)(DWORD*, int))(IceSelectionScreen[19] + 32))(IceSelectionScreen + 19, LastButton);
	IceSelectionScreen_SetExtraFunctionTextVisibility(IceSelectionScreen, 0);
}