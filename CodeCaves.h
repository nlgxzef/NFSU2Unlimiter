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

// 0x636BF7
void __declspec(naked) DoUnlimiterStuffCodeCave()
{
	// Get count
	_asm mov dword ptr ds : [_CarTypeInfoArray] , eax;
	_asm sub eax, 0x0C;
	_asm mov eax, [eax];
	_asm mov CarArraySize, eax;
	_asm mov eax, dword ptr ds : [_CarTypeInfoArray] ;
	_asm pushad;
	CarArraySize -= 8;
	CarCount = CarArraySize / SingleCarTypeInfoBlockSize;

	// Do required stuff
	//CountRandomEngageStrings();

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
	injector::WriteMemory<BYTE>(0x513D1D, CarCount, true); // sub_513C50 -> UICareerCarLot::BuildCarList

	// Make them available as opponents

	RandomCarCount = injector::ReadMemory<int>(0x4FEB9D, true);
	RandomSUVCount = injector::ReadMemory<int>(0x4FEBDB, true);

	for (TheCounter = 0; TheCounter < RandomCarCount; TheCounter++) // Read current arrays
	{
		RandomlyChooseableCarConfigsNorthAmerica[TheCounter] = injector::ReadMemory<BYTE>(0x7F6DA4 + TheCounter, true);
		RandomlyChooseableCarConfigsRestOfWorld[TheCounter] = injector::ReadMemory<BYTE>(0x7F6DC0 + TheCounter, true);
	}

	for (TheCounter = 0; TheCounter < RandomSUVCount; TheCounter++) RandomlyChooseableSUVs[TheCounter] = injector::ReadMemory<BYTE>(0x7F6DDC + TheCounter, true);

	// Add new cars there
	for (TheCounter = 0x2E; TheCounter < CarCount; TheCounter++)
	{
		if (CanCarBeDrivenByAI(TheCounter) && (IsRacer(TheCounter)))
		{
			if (IsSUV(TheCounter)) RandomlyChooseableSUVs[RandomSUVCount++] = TheCounter;
			else
			{
				RandomlyChooseableCarConfigsNorthAmerica[RandomCarCount] = TheCounter;
				RandomlyChooseableCarConfigsRestOfWorld[RandomCarCount++] = TheCounter;
			}
		}
	}

	// Introduce new arrays to the game
	injector::WriteMemory(0x4FEBA2, RandomlyChooseableCarConfigsNorthAmerica, true);
	injector::WriteMemory(0x4FEBA9, RandomlyChooseableCarConfigsRestOfWorld, true);
	injector::WriteMemory(0x4FEBE0, RandomlyChooseableSUVs, true);

	injector::WriteMemory<int>(0x4FEB9D, RandomCarCount, true);
	injector::WriteMemory<int>(0x4FEBDB, RandomSUVCount, true);

	// Crash Fix (Worked around by cloning the values instead)
	//injector::WriteMemory<BYTE>(0x5B6B3C, 0xEB, true); // RidePhysicsInfo::MatchPerformance

	// Unlock new cars right at the beginning
	for (TheCounter = 0; TheCounter < InitiallyUnlockedCarCount; TheCounter++) // Read current arrays
	{
		UnlockedAtBootQuickRaceNorthAmerica[TheCounter] = injector::ReadMemory<int>(0x7F7C08 + 4 * TheCounter, true);
		UnlockedAtBootQuickRaceRestOfWorld[TheCounter] = injector::ReadMemory<int>(0x7F7C28 + 4 * TheCounter, true);
	}

	// Add new cars there
	for (TheCounter = 0x2E; TheCounter < CarCount; TheCounter++)
	{
		if (IsInitiallyUnlocked(TheCounter) && (IsRacer(TheCounter)))
		{
			UnlockedAtBootQuickRaceNorthAmerica[InitiallyUnlockedCarCount] = TheCounter;
			UnlockedAtBootQuickRaceRestOfWorld[InitiallyUnlockedCarCount++] = TheCounter;
			CarLotUnlockData[TheCounter] = 1;
		}
	}

	// Introduce new arrays to the game
	injector::WriteMemory(0x529D22, UnlockedAtBootQuickRaceNorthAmerica, true); // Start
	injector::WriteMemory(0x529D2E, UnlockedAtBootQuickRaceNorthAmerica + 4 * InitiallyUnlockedCarCount, true); // End
	injector::WriteMemory(0x529D3B, UnlockedAtBootQuickRaceRestOfWorld, true); // Start
	injector::WriteMemory(0x529D48, UnlockedAtBootQuickRaceRestOfWorld + 4 * InitiallyUnlockedCarCount, true); // End



	// Continue
	_asm popad;
	_asm push 0x636BFC;
	_asm retn;
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