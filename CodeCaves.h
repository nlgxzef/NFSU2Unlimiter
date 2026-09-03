#pragma once

void __declspec(naked) CarLotFixCodeCaveWrite()
{
	_asm
	{
		cmp ecx, 256
		jae Skip

		mov byte ptr ds : [CarLotUnlockData + ecx] , 0
		
			Skip:
		push 0x513CDA
		retn
	}
}

void __declspec(naked) CarLotFixCodeCaveRead()
{
	_asm
	{
		cmp esi, 256
		jae OutOfRange
		
		mov al, [CarLotUnlockData + esi]
		jmp Done

			OutOfRange :
		xor al, al          // a bogus index reads as locked rather than as garbage

			Done :
		test al, al
		push 0x513CF6
		retn
	}
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
	//FillCarPickerArrays();

	// load configs into UnlimiterData structs
	//LoadFNGFixes();
	LoadPaintGroups();
	LoadRimBrands();
	LoadVinylGroups();
	LoadStarGazer();
	LoadCameraInfo();

	// Fix misc stats
	//FixComputeMiscStats();

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

// 0x61B67C
int IsUG1_Hash(DWORD CarTypeNameHash)
{
	return IsUG1(GetCarTypeIDFromHash(CarTypeNameHash));
}

// 0x61B686
int IsUG2_Hash(DWORD CarTypeNameHash)
{
	return IsUG2(GetCarTypeIDFromHash(CarTypeNameHash));
}

// 0x61B68E
int IsSUV_Hash(DWORD CarTypeNameHash)
{
	return IsSUV(GetCarTypeIDFromHash(CarTypeNameHash));
}

void __declspec(naked) IsSUV_UnInlineCodeCave()
{
	_asm
	{
		mov dword ptr ds : [esp + 0x28] , eax // IsUG2 result

		push ebp
		call IsSUV_Hash
		mov dword ptr ds : [esp + 0x20] , eax
		add esp, 4
		push 0x61B6BA
		retn
	}
}