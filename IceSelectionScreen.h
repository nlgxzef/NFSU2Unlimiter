#include "stdio.h"
#include "InGameFunctions.h"

DWORD GetCarTypeNameHashFromFECarConfig()
{
	DWORD* FECarConfig = (DWORD*)*(DWORD*)_FECarConfigRef;
	return (DWORD)(*(int(__thiscall**)(DWORD*))(*(DWORD*)FECarConfig + 8))(FECarConfig);
}

void __fastcall IceSelectionScreen_DoSpecialScroll(DWORD* IceSelectionScreen, void* EDX_Unused, DWORD* CategoryNode, bool on)
{
	IceSelectionScreen_DoSpecialScroll_Game(IceSelectionScreen, CategoryNode, on);

	if (CategoryNode)
	{
		GetAndDoFEPartAnim(CategoryNode[18], on, 1.0f);
	}
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

	FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, CT_bStringHash("CUSTOMIZE_TITLE_INTERIORS_SHOP"));

	DWORD TrunkSlotIcons[12] = {
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot1,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot2,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot3,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot4,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot5,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot6,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot7,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot8,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot9,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot10,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot11,
		CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioSlot12
	};
	DWORD TrunkSlotNames[12] = {
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot1,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot2,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot3,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot4,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot5,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot6,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot7,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot8,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot9,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot10,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot11,
		CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioSlot12
	};

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
		IceSelectionScreen[10] = CT_bStringHash("HELP_CARSPECIALTIES_SHOP_MAIN");

		if ((*(int*)0x83898C != 1) && (CarConfigs[CarTypeID].Specialties.CustomGauges))
			IceSelectionScreen_AddSelection(
				IceSelectionScreen, 
				CAR_SLOT_ID::CUSTOM_HUD, 
				CarConfigs[CarTypeID].Icons.SpecialtiesCustomGauges,
				CarConfigs[CarTypeID].Names.SpecialtiesCustomGauges,
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
					CAR_SLOT_ID::CABIN_NEON_FRAME, 
					CarConfigs[CarTypeID].Icons.SpecialtiesNeon,
					CarConfigs[CarTypeID].Names.SpecialtiesNeon);
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
					CAR_SLOT_ID::WINDOW_TINT, 
					CarConfigs[CarTypeID].Icons.SpecialtiesWindowTint,
					CarConfigs[CarTypeID].Names.SpecialtiesWindowTint,
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
					CAR_SLOT_ID::HEADLIGHT_BULB, 
					CarConfigs[CarTypeID].Icons.SpecialtiesHeadlightColor,
					CarConfigs[CarTypeID].Names.SpecialtiesHeadlightColor,
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
					CAR_SLOT_ID::NOS_PURGE, 
					CarConfigs[CarTypeID].Icons.SpecialtiesNosPurge,
					CarConfigs[CarTypeID].Names.SpecialtiesNosPurge,
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
					CAR_SLOT_ID::HYDRAULICS, 
					CarConfigs[CarTypeID].Icons.SpecialtiesHydraulics,
					CarConfigs[CarTypeID].Names.SpecialtiesHydraulics,
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
				IceCategoryTrunkThing_Create(
					AMenuOption,
					EDX_Unused,
					CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudio,
					CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudio);
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
					CAR_SLOT_ID::FRONT_WHEEL, 
					CarConfigs[CarTypeID].Icons.SpecialtiesSpinners,
					CarConfigs[CarTypeID].Names.SpecialtiesSpinners,
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
					CAR_SLOT_ID::HOOD, 
					CarConfigs[CarTypeID].Icons.SpecialtiesSplitHoods,
					CarConfigs[CarTypeID].Names.SpecialtiesSplitHoods,
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
					CAR_SLOT_ID::DOOR_STYLE, 
					CarConfigs[CarTypeID].Icons.SpecialtiesDoors,
					CarConfigs[CarTypeID].Names.SpecialtiesDoors,
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
					CAR_SLOT_ID::LICENSE_PLATE, 
					CarConfigs[CarTypeID].Icons.SpecialtiesLicensePlate,
					CarConfigs[CarTypeID].Names.SpecialtiesLicensePlate,
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
		FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudio);

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
				CAR_SLOT_ID::TRUNK_AUDIO, 
				CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioLayout,
				CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioLayout,
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
		FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudio);

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
				CAR_SLOT_ID::TRUNK_AUDIO_COMP_0, 
				CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioComponents,
				CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioComponents);
			(*(void(__thiscall**)(DWORD*, DWORD*))(*IceSelectionScreen + 24))(IceSelectionScreen, AMenuOption);
		}
		else AMenuOption = 0;

		AMenuOption = (DWORD*)j__malloc(0x5C);
		if (AMenuOption)
		{
			IceSelectionThing_Create(
				AMenuOption, 
				CAR_SLOT_ID::TRUNK_AUDIO,
				CarConfigs[CarTypeID].Icons.SpecialtiesTrunkAudioLayout,
				CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudioLayout,
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
		FEngSetLanguageHash(IceSelectionScreenPackage, 0x42ADB44C, CarConfigs[CarTypeID].Names.SpecialtiesTrunkAudio);

		if (IsCarPartsAnimLoadedForCar(GetCarTypeNameHashFromFECarConfig()))
		{
			FEDoCarPartAnimNow(1, 1, 1.0f);
		}
		*((BYTE*)IceSelectionScreen + 360) = 1;
		IceSelectionScreen[10] = bStringHash("HELP_CARSPECIALTIES_SHOP_AUDIOCOMPONENTS_SELECT");

		for (int i = CAR_SLOT_ID::TRUNK_AUDIO_COMP_0; i <= CAR_SLOT_ID::TRUNK_AUDIO_COMP_11; ++i)
		{
			if (RideInfo_TrunkAudioSlotAvailable((DWORD*)gTheRideInfo, 0, i))
			{
				AMenuOption = (DWORD*)j__malloc(0x5C);
				if (AMenuOption)
				{
					IceSelectionThing_Create(
						AMenuOption, 
						i, 
						TrunkSlotIcons[i - CAR_SLOT_ID::TRUNK_AUDIO_COMP_0],
						TrunkSlotNames[i - CAR_SLOT_ID::TRUNK_AUDIO_COMP_0],
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