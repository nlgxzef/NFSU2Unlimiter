#pragma once

void __declspec(naked) CarLotFixCodeCaveWrite()
{
	_asm
	{
		mov byte ptr ds : [CarLotUnlockData + ecx] , 0
		push 0x513CDA
		retn
	}
}

void __declspec(naked) CarLotFixCodeCaveRead()
{
	_asm
	{
		mov al, [CarLotUnlockData + esi]
		test al, al
		push 0x513CF6
		retn
	}
}

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

// 0x636BF7
void __declspec(naked) DoUnlimiterStuffCodeCave()
{
	// Get count
	_asm
	{
		mov dword ptr ds : [_CarTypeInfoArray] , eax
		sub eax, 0x0C
		mov eax, [eax]
		mov CarArraySize, eax
		mov eax, dword ptr ds : [_CarTypeInfoArray] 
		pushad
	}

	CarArraySize -= 8;
	CarCount = CarArraySize / SingleCarTypeInfoBlockSize;

	// Do required stuff
	//CountRandomEngageStrings();

	// Replacement model if model not found in array
	if (ReplacementCar > CarCount) ReplacementCar = 1;

	// Car Type Unlimiter
	injector::WriteMemory<int>(0x41AB83, CarArraySize, true); // StreamingTrafficCarManager::Init
	injector::WriteMemory<int>(0x5165BC, CarArraySize, true); // FEPlayerCarDB::DefaultStockCars
	injector::WriteMemory<int>(0x5207B3, CarArraySize, true); // DebugCarCustomizeScreen::BuildOptionsLists
	injector::WriteMemory<int>(0x609348, CarArraySize, true); // sub_6091D0
	injector::WriteMemory<int>(0x6099B1, CarArraySize, true); // sub_6097D0
	injector::WriteMemory<int>(0x636C24, CarArraySize, true); // LoaderCarInfo

	injector::WriteMemory<BYTE>(0x5596CB, CarCount, true); // IceSelectionScreen::Setup
	injector::WriteMemory<BYTE>(0x5EFC5A, CarCount, true); // sub_5EFAC0
	injector::WriteMemory<BYTE>(0x89D2DC, CarCount, true); // QuantCarType (gets set before unlimiter so we need to overwrite it here)
	injector::WriteMemory<BYTE>(0x89D2E0, CarCount + 1, true); // QuantCarType
	injector::WriteMemory<BYTE>(0x610150, CarCount, true); // GetCarTypeInfoFromHash
	injector::WriteMemory<BYTE>(0x61C671, CarCount, true); // CarLoader::LoadAllPartsAnims
	injector::WriteMemory<BYTE>(0x6372B4, CarCount, true); // RideInfo::FillWithPreset
	//injector::WriteMemory<BYTE>(0x4EAE48, CarCount, true); // GarageMainScreen::GarageMainScreen
	injector::WriteMemory<BYTE>(0x513D1D, CarCount, true); // PlayerCareerState::BuildUnlockedCareerCarList -> UICareerCarLot::BuildCarList

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

	// Fix misc stats
	FixComputeMiscStats();

	// Continue
	_asm
	{
		popad
		push 0x636BFC
		retn
	}
}

// 0x636D6C
void __declspec(naked) DoUnlimiterStuffCodeCave2()
{
	// Get count
	// Get count
	_asm
	{
		mov dword ptr ds : [_CarPartPartsTable] , edx
		sub edx, 4
		mov edx, [edx]
		mov CarPartPartsTableSize, edx
		mov edx, dword ptr ds : [_CarPartPartsTable]
		pushad
	}

	CarPartCount = CarPartPartsTableSize / SingleCarPartSize;

	if (CarPartCount > 16000)
	{
		// Fix quantizers
		injector::WriteMemory<int>(0x89D420, CarPartCount, true); // QuantPartIndex
		injector::WriteMemory<int>(0x89D424, CarPartCount + 1, true);

		injector::WriteMemory<int>(0x5F04A4, CarPartCount, true); // sub_5F0320
		injector::WriteMemory<int>(0x5F0675, CarPartCount + 1, true); // sub_5F04D0
	}

	// Continue
	_asm popad;
	_asm push 0x636D72;
	_asm retn;
}

void __declspec(naked) PerformanceConfigFixCodeCave()
{
	_asm mov eax, 1;
	_asm pushad;

	FillUpPerformanceConfig();

	_asm popad;
	_asm push 0x5994C0;
	_asm retn;
}