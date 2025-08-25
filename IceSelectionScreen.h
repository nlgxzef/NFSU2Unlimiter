#include "stdio.h"
#include "InGameFunctions.h"

DWORD GetCarTypeNameHashFromFECarConfig()
{
	DWORD* FECarConfig = (DWORD*)*(DWORD*)_FECarConfigRef;
	return (DWORD)(*(int(__thiscall**)(DWORD*))(*(DWORD*)FECarConfig + 8))(FECarConfig);
}

int GetIcePartsList(int CarSlotID, DWORD* PartsBList, unsigned int Unk)
{
	DWORD* NewBNode; // eax MAPDST
	DWORD* BTail; // ecx MAPDST
	int PartsCount = 0; // eax MAPDST

	int SomethingUnk = *(int*)0x8389B0;
	DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
	int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
	int CarPartID = GetCarPartFromSlot(CarSlotID);

	DWORD* TheCarPart = CarPartDatabase_NewGetFirstCarPart((DWORD*)_CarPartDB, CarTypeID, CarSlotID, 0, -1);

	switch (CarSlotID)
	{
	case CAR_SLOT_ID::HOOD: // Split Hoods
		while (TheCarPart)
		{
			if (*((char*)TheCarPart + 4) == CarPartID && (*((BYTE*)TheCarPart + 5) & 0x1F) == 5)
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
	case CAR_SLOT_ID::FRONT_WHEEL: // SPINNER
	case CAR_SLOT_ID::REAR_WHEEL:
		while (TheCarPart)
		{
			if (*((char*)TheCarPart + 4) == CarPartID && IsRimAvailable(CarTypeID, TheCarPart, CT_bStringHash("SPINNER")))
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
	default:
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
	
	case CAR_SLOT_ID::NEON_ENGINE: // Hide LED neons from other categories as they aren't supposed to render tubes
	case CAR_SLOT_ID::NEON_TRUNK:
	case CAR_SLOT_ID::NEON_CABIN:
		while (TheCarPart)
		{
			if (*((char*)TheCarPart + 4) == CarPartID && (CarPart_GetAppliedAttributeUParam(TheCarPart, CT_bStringHash("LED"), 0) == 0))
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
	}
	return PartsCount;
}

void __fastcall IceSelectionScreen_RefreshHeader(DWORD* IceSelectionScreen, void* EDX_Unused)
{
	IceSelectionScreen_RefreshHeader_Game(IceSelectionScreen);
}

void __fastcall IceSelectionScreen_Setup(DWORD* IceSelectionScreen, void* EDX_Unused)
{
	// Read Part Options for the car
	DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
	int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

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

		if ((*(int*)0x83898C != 1) && (CarConfigs[CarTypeID].Specialties.CustomGauges))
			IceSelectionScreen_AddSelection(
				IceSelectionScreen, 
				163, 
				bStringHash("VISUAL_PART_CUSTOM_HUDS"), 
				bStringHash("UI_ICE_GAUGES"),
				-1, 
				"UI_CustomHUDOverlay.fng");

		if (CarConfigs[CarTypeID].Specialties.Neon)
		{
			AMenuOption = (DWORD*)j__malloc(0x60);
			if (AMenuOption)
			{
				IceCategoryThing_Create(
					AMenuOption, 
					7, 
					158, 
					bStringHash("VISUAL_PART_NEON"), 
					bStringHash("UI_ICE_NEON"));
				*((BYTE*)AMenuOption + 34) = IsNeonCompletelyLocked();
				*((BYTE*)AMenuOption + 35) = 1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarConfigs[CarTypeID].Specialties.WindowTint)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption, 
					153, 
					bStringHash("VISUAL_PART_WINDOW_TINTING"), 
					bStringHash("UI_ICE_TINT"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarConfigs[CarTypeID].Specialties.HeadlightColor)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption, 
					159, 
					bStringHash("VISUAL_PART_HEAD_LIGHT_COLOR"), 
					bStringHash("UI_ICE_HEADLIGHTS"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarConfigs[CarTypeID].Specialties.NosPurge)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(AMenuOption,
					162, 
					bStringHash("VISUAL_PART_NOS_PURGE"), 
					bStringHash("UI_ICE_NOS_PURGE"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarConfigs[CarTypeID].Specialties.Hydrualics)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption,
					161, 
					bStringHash("VISUAL_PART_HYDRAULICS"), 
					bStringHash("UI_ICE_HYDRAULICS"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarConfigs[CarTypeID].Specialties.TrunkAudio)
		{
			AMenuOption = (DWORD*)j__malloc(0x60);
			if (AMenuOption)
			{
				IceCategoryTrunkThing_Create(AMenuOption);
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarConfigs[CarTypeID].Specialties.Spinners)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption, 
					29, 
					bStringHash("VISUAL_PART_SPINNERS"), 
					bStringHash("UI_ICE_SPINNERS"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = -1;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarConfigs[CarTypeID].Specialties.SplitHoods)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption, 
					9, 
					bStringHash("VISUAL_PART_SPLIT_HOODS"), 
					bStringHash("UI_ICE_HOOD"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = 0;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}
		
		if (CarConfigs[CarTypeID].Specialties.Doors)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption, 
					160, 
					bStringHash("VISUAL_PART_SCISSOR_DOORS"), 
					bStringHash("UI_ICE_DOORS"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
				AMenuOption[21] = 2;
				(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			}
			else AMenuOption = 0;
		}

		if (CarConfigs[CarTypeID].Specialties.LicensePlate)
		{
			AMenuOption = (DWORD*)j__malloc(0x5C);
			if (AMenuOption)
			{
				IceSelectionThing_Create(
					AMenuOption, 
					33, 
					bStringHash("VISUAL_PART_LICENSE_PLATES"), 
					bStringHash("UI_ICE_LICENSE_PLATES"), 
					0, 
					1, 
					*(char const**)gIceSideBarOverlayName);
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
			IceSelectionThing_Create(
				AMenuOption, 
				34, 
				bStringHash("AUDIO_ICON_CONFIGURE_TRUNK_LAYOUT"), 
				bStringHash("UI_ICE_TRUNK_AUDIO_LAYOUT_SELECTION"), 
				0, 
				1, 
				*(char const**)gIceSideBarOverlayName);
			AMenuOption[21] = -1;
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
			IceSelectionScreen_StartBrowsingParts_Game(IceSelectionScreen);
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
			IceCategoryThing_Create(
				AMenuOption, 
				4, 
				35, 
				bStringHash("AUDIO_ICON_COMPONENTS"), 
				bStringHash("UI_ICE_TRUNK_AUDIO_COMPONENTS"));
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
		}
		else AMenuOption = 0;

		AMenuOption = (DWORD*)j__malloc(0x5C);
		if (AMenuOption)
		{
			IceSelectionThing_Create(
				AMenuOption, 
				34, 
				bStringHash("AUDIO_ICON_CONFIGURE_TRUNK_LAYOUT"), 
				bStringHash("UI_ICE_TRUNK_AUDIO_LAYOUT_SELECTION"), 
				0, 
				1, 
				*(char const**)gIceSideBarOverlayName);
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
			if (RideInfo_TrunkAudioSlotAvailable((DWORD*)gTheRideInfo, 0, i))
			{
				AMenuOption = (DWORD*)j__malloc(0x5C);
				if (AMenuOption)
				{
					IceSelectionThing_Create(
						AMenuOption, 
						i, 
						bStringHash("VISUAL_PART_TRUNK_AUDIO"), 
						bStringHash("UI_ICE_TRUNK_AUDIO_COMPONENT"), 
						0, 
						1, 
						*(char const**)gIceSideBarOverlayName);
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