#pragma once

void FillCarPickerArrays()
{
	bool UnlockRegionalCars = 0;

	int RandomCarCount = injector::ReadMemory<int>(0x4FEB9D, true);
	int RandomSUVCount = injector::ReadMemory<int>(0x4FEBDB, true);
	int InitiallyUnlockedCarCount = 8;

	// Check if Extra Options is present. If so, read UnlockRegionalCars value from its config file.
	if (GetModuleHandleA("NFSU2ExtraOptions.asi"))
	{
		auto ExtraOptionsSettings = CurrentWorkingDirectory / "NFSU2ExtraOptionsSettings.ini";
		mINI::INIFile NFSU2ExtraOptionsSettingsINIFile(ExtraOptionsSettings.string());
		mINI::INIStructure Settings;
		NFSU2ExtraOptionsSettingsINIFile.read(Settings);

		UnlockRegionalCars = mINI_ReadInteger(Settings, "Gameplay", "UnlockRegionalCars", 1) != 0;
	}

	// Read current arrays
	for (int i = 0; i < RandomCarCount; i++)
	{
		RandomlyChooseableCarConfigsNorthAmerica[i] = injector::ReadMemory<BYTE>(0x7F6DA4 + i, true);
		RandomlyChooseableCarConfigsRestOfWorld[i] = injector::ReadMemory<BYTE>(0x7F6DC0 + i, true);
	}

	for (int i = 0; i < RandomSUVCount; i++) RandomlyChooseableSUVs[i] = injector::ReadMemory<BYTE>(0x7F6DDC + i, true);

	// Add regional cars
	if (UnlockRegionalCars)
	{
		for (int i = 0; i < 2; i++)
		{
			RandomlyChooseableCarConfigsNorthAmerica[RandomCarCount] = EUExclusiveCars[i];
			RandomlyChooseableCarConfigsRestOfWorld[RandomCarCount++] = USExclusiveCars[i];
		}
	}

	// Add add-on cars
	for (int i = 46; i < CarCount; i++)
	{
		if (CanCarBeDrivenByAI(i) && (IsRacer(i)))
		{
			if (IsSUV(i)) RandomlyChooseableSUVs[RandomSUVCount++] = i;
			else
			{
				RandomlyChooseableCarConfigsNorthAmerica[RandomCarCount] = i;
				RandomlyChooseableCarConfigsRestOfWorld[RandomCarCount++] = i;
			}
		}
	}

	// Introduce new arrays to the game
	injector::WriteMemory(0x4FEBA2, RandomlyChooseableCarConfigsNorthAmerica, true);
	injector::WriteMemory(0x4FEBA9, RandomlyChooseableCarConfigsRestOfWorld, true);
	injector::WriteMemory(0x4FEBE0, RandomlyChooseableSUVs, true);

	injector::WriteMemory<int>(0x4FEB9D, RandomCarCount, true);
	injector::WriteMemory<int>(0x4FEBDB, RandomSUVCount, true);


	// Initially unlocked cars
	for (int i = 0; i < InitiallyUnlockedCarCount; i++) // Read current arrays
	{
		UnlockedAtBootQuickRaceNorthAmerica[i] = injector::ReadMemory<int>(0x7F7C08 + 4 * i, true);
		UnlockedAtBootQuickRaceRestOfWorld[i] = injector::ReadMemory<int>(0x7F7C28 + 4 * i, true);
	}

	// Add regional cars
	if (UnlockRegionalCars)
	{
		for (int i = 0; i < 1; i++) // Only unlock CIVIC or CORSA
		{
			UnlockedAtBootQuickRaceNorthAmerica[InitiallyUnlockedCarCount] = EUExclusiveCars[i];
			UnlockedAtBootQuickRaceRestOfWorld[InitiallyUnlockedCarCount++] = USExclusiveCars[i];
		}
	}

	// Add new cars there
	for (int i = 46; i < CarCount; i++)
	{
		if (IsInitiallyUnlocked(i) && (IsRacer(i)))
		{
			UnlockedAtBootQuickRaceNorthAmerica[InitiallyUnlockedCarCount] = i;
			UnlockedAtBootQuickRaceRestOfWorld[InitiallyUnlockedCarCount++] = i;
			CarLotUnlockData[i] = 1;
		}
	}

	// Introduce new arrays to the game
	injector::WriteMemory(0x529D22, UnlockedAtBootQuickRaceNorthAmerica, true); // Start
	injector::WriteMemory(0x529D2E, UnlockedAtBootQuickRaceNorthAmerica + 4 * InitiallyUnlockedCarCount, true); // End
	injector::WriteMemory(0x529D3B, UnlockedAtBootQuickRaceRestOfWorld, true); // Start
	injector::WriteMemory(0x529D48, UnlockedAtBootQuickRaceRestOfWorld + 4 * InitiallyUnlockedCarCount, true); // End

}

float* TimingStatsKludgeFactor060;
float* TimingStatsKludgeFactor0100;
void FixComputeMiscStats()
{
	TimingStatsKludgeFactor060 = new float[CarCount];
	TimingStatsKludgeFactor0100 = new float[CarCount];

	// Copy original values
	for (int i = 0; i < 46; i++)
	{
		TimingStatsKludgeFactor060[i] = CarConfigs[i].Stats.TimingKludgeFactor060 != 0.0f
			? CarConfigs[i].Stats.TimingKludgeFactor060
			: *((float*)0x007FC120 + i);
		TimingStatsKludgeFactor0100[i] = CarConfigs[i].Stats.TimingKludgeFactor0100 != 0.0f
			? CarConfigs[i].Stats.TimingKludgeFactor0100
			: *((float*)0x007FC1D8 + i);
	}

	// Fill the rest of the cars
	for (int i = 46, j = 0; i < CarCount; i++, j++)
	{
		if (j > 45)
		{
			j = 0;
		}

		TimingStatsKludgeFactor060[i] = CarConfigs[i].Stats.TimingKludgeFactor060 != 0.0f
			? CarConfigs[i].Stats.TimingKludgeFactor060
			: *((float*)0x007FC120 + j);
		TimingStatsKludgeFactor0100[i] = CarConfigs[i].Stats.TimingKludgeFactor0100 != 0.0f
			? CarConfigs[i].Stats.TimingKludgeFactor0100
			: *((float*)0x007FC1D8 + j);
	}

	injector::WriteMemory(0x005B089F, TimingStatsKludgeFactor060, true);
	injector::WriteMemory(0x005B08D6, TimingStatsKludgeFactor0100, true);
}

int(*LoaderCarInfo_Game)(bChunk*) = (int(*)(bChunk*))0x636BE0;

int LoaderCarInfo_Hook(bChunk* chunk)
{
	int result = LoaderCarInfo_Game(chunk);

	int TypeBits = 1;
	int PartBits = 1;
	int CarPartCount = 0;

	if (result)
	{
		switch (chunk->ID)
		{
		case BCHUNK_SPEED_CARTYPEINFO_TABLE: // Unlimiter stuff Part 1
			CarArraySize = chunk->Size - 8;
			CarCount = CarArraySize / SingleCarTypeInfoBlockSize;
			//CarTypeInfoArray = *(CarTypeInfo**)_CarTypeInfoArray;

			// Replacement model if model not found in array
			if (ReplacementCar > CarCount) ReplacementCar = 1;

			// Car Type Unlimiter
			injector::WriteMemory<int>(0x41AB83, CarArraySize, true); // StreamingTrafficCarManager::Init
			injector::WriteMemory<int>(0x5165BC, CarArraySize, true); // FEPlayerCarDB::DefaultStockCars
			injector::WriteMemory<int>(0x5207B3, CarArraySize, true); // DebugCarCustomizeScreen::BuildOptionsLists
			injector::WriteMemory<int>(0x609348, CarArraySize, true); // sub_6091D0
			injector::WriteMemory<int>(0x6099B1, CarArraySize, true); // sub_6097D0
			injector::WriteMemory<int>(0x636C24, CarArraySize, true); // LoaderCarInfo

			CarCountByte = (CarCount > 255) ? 255 : (BYTE)CarCount;

			injector::WriteMemory<BYTE>(0x5596CB, CarCountByte, true); // IceSelectionScreen::Setup
			injector::WriteMemory<BYTE>(0x5EFC5A, CarCountByte, true); // sub_5EFAC0
			injector::WriteMemory<BYTE>(0x89D2DC, CarCountByte, true); // QuantCarType (gets set before unlimiter so we need to overwrite it here)
			injector::WriteMemory<BYTE>(0x89D2E0, (BYTE)(CarCountByte + 1), true); // QuantCarType
			injector::WriteMemory<BYTE>(0x610150, CarCountByte, true); // GetCarTypeInfoFromHash
			injector::WriteMemory<BYTE>(0x61C671, CarCountByte, true); // CarLoader::LoadAllPartsAnims
			injector::WriteMemory<BYTE>(0x6372B4, CarCountByte, true); // RideInfo::FillWithPreset
			injector::WriteMemory<BYTE>(0x513D1D, CarCountByte, true); // PlayerCareerState::BuildUnlockedCareerCarList -> UICareerCarLot::BuildCarList

			if (CarCountByte > 0)
			{
				TypeBits = 1;
				while ((1 << TypeBits) < CarCountByte + 1) TypeBits++;

				injector::WriteMemory<int>(0x89D2D4, TypeBits, true); // QuantCarType bit count

			}

			// Make them available as opponents
			LoadCarConfigs();
			FillCarPickerArrays();

			// load configs into UnlimiterData structs
			//LoadFNGFixes();
			LoadPaintGroups();
			LoadRimBrands();
			LoadVinylGroups();
			LoadStarGazer();
			LoadCameraInfo();
			LoadPartLinks();
			LoadPresetCarOverrides();

			// Fix misc stats
			FixComputeMiscStats();

			break;

		case BCHUNK_SPEED_CARPART_PACK_HEADER: // Unlimiter stuff Part 2
			DBCarPart = (CarPartDatabase*)_CarPartDB;
			CarPartCount = DBCarPart->NumParts;

			// Fix quantizers
			PartBits = 1;
			while ((1 << PartBits) < CarPartCount + 1) PartBits++;
			injector::WriteMemory<int>(0x89D418, PartBits, true);         // QuantPartIndex bit count

			injector::WriteMemory<int>(0x89D420, CarPartCount, true); // QuantPartIndex
			injector::WriteMemory<int>(0x89D424, CarPartCount + 1, true);

			injector::WriteMemory<int>(0x5EFC43, CarPartCount, true); // sub_5EFAC0
			injector::WriteMemory<int>(0x5F04A4, CarPartCount, true); // sub_5F0320
			injector::WriteMemory<int>(0x5F0675, CarPartCount + 1, true); // sub_5F04D0

			break;

		case BCHUNK_SPEED_CARPART_ANIMHOOKUP_TABLE:
			CarSlotAnimHookupTable = *(CarSlotAnimHookup**)0x8A1CDC;

			//CarSlotAnimHookup_InitForExtraAttachments();
			break;
		}
	}

	return result;
}