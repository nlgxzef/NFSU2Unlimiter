#include "stdafx.h"
#include "stdio.h"
#include <string>
#include <windows.h>
#include "includes\injector\injector.hpp"
#include "includes\IniReader.h"
using namespace std;

int CarCount, CarArraySize, TrafficCarCount, TheCounter;
bool NewCarsInitiallyUnlocked, NewCarsCanBeDrivenByAI, DisappearingWheelsFix, ExpandMemoryPools, AddOnOpponentsPartsFix, EnableUnlimiterData;

BYTE RandomlyChooseableCarConfigsNorthAmerica[256], RandomlyChooseableCarConfigsRestOfWorld[256], RandomlyChooseableSUVs[256];
int UnlockedAtBootQuickRaceNorthAmerica[256], UnlockedAtBootQuickRaceRestOfWorld[256];
int RandomCarCount, RandomSUVCount, CarTypeID_Temp, LinkLicensePlateToTrunk_Temp, InitiallyUnlockedCarCount = 8;

char CarININame[260] = "";

DWORD DoUnlimiterStuffCodeCaveExit = 0x636BFC;
DWORD RideInfo_SetStockParts = 0x637040;

bool DoesFileExist(char const* _path) {

	std::ifstream ifile(_path);
	return (bool)ifile;
}

bool IsSUV(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return *(bool*)((*(DWORD*)0x8A1CCC) + CarTypeID * 0x890 + 0x88A);
}

bool IsTraffic(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return *(BYTE*)((*(DWORD*)0x8A1CCC) + CarTypeID * 0x890 + 0x844) == 2;
}

void __declspec(naked) DoUnlimiterStuffCodeCave()
{
	// Get count
	_asm mov dword ptr ds : [0x8A1CCC] , eax;
	_asm sub eax, 0x0C;
	_asm mov eax, [eax];
	_asm mov CarArraySize, eax;
	_asm mov eax, dword ptr ds : [0x8A1CCC] ;
	_asm pushad;
	CarArraySize -= 8;
	CarCount = CarArraySize / 0x890;

	// Do required stuff

	// Car Type Unlimiter
	injector::WriteMemory<int>(0x41AB83, CarArraySize, true); // StreamingTrafficCarManager::Init
	injector::WriteMemory<int>(0x5165BC, CarArraySize, true); // FEPlayerCarDB::DefaultStockCars
	injector::WriteMemory<int>(0x5207B3, CarArraySize, true); // DebugCarCustomizeScreen::BuildOptionsLists
	injector::WriteMemory<int>(0x609348, CarArraySize, true); // sub_6091D0
	injector::WriteMemory<int>(0x6099B1, CarArraySize, true); // sub_6097D0
	injector::WriteMemory<int>(0x636C24, CarArraySize, true); // LoaderCarInfo

	injector::WriteMemory<BYTE>(0x5596CB, CarCount, true); // IceSelectionScreen::Setup
	injector::WriteMemory<BYTE>(0x5EFC5A, CarCount, true); // sub_5EFAC0
	injector::WriteMemory<BYTE>(0x610150, CarCount, true); // GetCarTypeInfoFromHash
	injector::WriteMemory<BYTE>(0x61C671, CarCount, true); // CarLoader::LoadAllPartsAnims
	injector::WriteMemory<BYTE>(0x6372B4, CarCount, true); // RideInfo::FillWithPreset
	injector::WriteMemory<BYTE>(0x4EAE48, CarCount, true); // sub_4EA900
	injector::WriteMemory<BYTE>(0x513D1D, CarCount, true); // sub_513C50

	// Make them available as opponents
	if (NewCarsCanBeDrivenByAI)
	{
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
			if (!IsTraffic(TheCounter))
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

		// Crash Fix
		injector::WriteMemory<BYTE>(0x5B6B3C, 0xEB, true);
	}
	
	// Unlock new cars right at the beginning
	if (NewCarsInitiallyUnlocked)
	{
		for (TheCounter = 0; TheCounter < InitiallyUnlockedCarCount; TheCounter++) // Read current arrays
		{
			UnlockedAtBootQuickRaceNorthAmerica[TheCounter] = injector::ReadMemory<int>(0x7F7C08 + 4 * TheCounter, true);
			UnlockedAtBootQuickRaceRestOfWorld[TheCounter] = injector::ReadMemory<int>(0x7F7C28 + 4 * TheCounter, true);
		}

		// Add new cars there
		for (TheCounter = 0x2E; TheCounter < CarCount; TheCounter++)
		{
			if (!IsTraffic(TheCounter))
			{
				UnlockedAtBootQuickRaceNorthAmerica[InitiallyUnlockedCarCount] = TheCounter;
				UnlockedAtBootQuickRaceRestOfWorld[InitiallyUnlockedCarCount++] = TheCounter;
			}
		}

		// Introduce new arrays to the game
		injector::WriteMemory(0x529D22, UnlockedAtBootQuickRaceNorthAmerica, true); // Start
		injector::WriteMemory(0x529D2E, UnlockedAtBootQuickRaceNorthAmerica + 4 * InitiallyUnlockedCarCount, true); // End
		injector::WriteMemory(0x529D3B, UnlockedAtBootQuickRaceRestOfWorld, true); // Start
		injector::WriteMemory(0x529D48, UnlockedAtBootQuickRaceRestOfWorld + 4 * InitiallyUnlockedCarCount, true); // End

	}

	// Continue
	_asm popad;
	_asm jmp DoUnlimiterStuffCodeCaveExit;
}

char* (*GetCarTypeName)(int CarTypeID) = (char* (*)(int))0x610110;
DWORD DoLinkLPToTrunk = 0x625401;
DWORD DontLinkLPToTrunk = 0x6259FE;

int LinkLicensePlateToTrunk_Game(int CarTypeID)
{
	switch (CarTypeID)
	{
	case 0:
	case 1:
	case 5:
	case 8:
	case 9:
	case 12:
	case 13:
	case 14:
	case 17:
	case 25:
		return 1;
	default:
		return 0;
	}

	return 0;
}

int LinkLicensePlateToTrunk(int CarTypeID)
{
	sprintf(CarININame, "UnlimiterData\\%s.ini", GetCarTypeName(CarTypeID));

	CIniReader UnlimiterCarData(CarININame);
	return UnlimiterCarData.ReadInteger("CarRenderInfo", "LinkLicensePlateToTrunk", LinkLicensePlateToTrunk_Game(CarTypeID));
}

void __declspec(naked) LinkLicensePlateToTrunkCodeCave()
{
	_asm mov CarTypeID_Temp, eax;
	_asm pushad;
	
	if (LinkLicensePlateToTrunk(CarTypeID_Temp))
	{
		_asm popad;
		_asm jmp DoLinkLPToTrunk;
	}
	else
	{
		_asm popad;
		_asm jmp DontLinkLPToTrunk;
	}
}

int(*ShowTrunkUnderInFE_Game)(int CarTypeID) = (int(*)(int))0x60C8F0;

int ShowTrunkUnderInFE(int CarTypeID)
{
	sprintf(CarININame, "UnlimiterData\\%s.ini", GetCarTypeName(CarTypeID));

	CIniReader UnlimiterCarData(CarININame);
	return UnlimiterCarData.ReadInteger("CarRenderInfo", "ShowTrunkUnderInFE", ShowTrunkUnderInFE_Game(CarTypeID));
}

int(*RemoveCentreBrakeWithCustomSpoiler_Game)(int CarTypeID) = (int(*)(int))0x60C8A0;

int RemoveCentreBrakeWithCustomSpoiler(int CarTypeID)
{
	sprintf(CarININame, "UnlimiterData\\%s.ini", GetCarTypeName(CarTypeID));

	CIniReader UnlimiterCarData(CarININame);
	return UnlimiterCarData.ReadInteger("CarRenderInfo", "RemoveCentreBrakeWithCustomSpoiler", RemoveCentreBrakeWithCustomSpoiler_Game(CarTypeID));
}

// int(*HasSunroof_Game)(int CarTypeID) = (int(*)(int))0x60C850;
int HasSunroof_Game(int CarTypeID)
{
	switch (CarTypeID)
	{
	case 3:
	case 5:
	case 18:
	case 23:
		return 1;
	default:
		return 0;
	}

	return 0;
}

DWORD DoesHaveSunroof = 0x60C86A;
DWORD DoesNotHaveSunroof = 0x60C870;

int HasSunroof(int CarTypeID)
{
	sprintf(CarININame, "UnlimiterData\\%s.ini", GetCarTypeName(CarTypeID));

	CIniReader UnlimiterCarData(CarININame);
	return UnlimiterCarData.ReadInteger("CarRenderInfo", "HasSunroof", HasSunroof_Game(CarTypeID));
}

void __declspec(naked) HasSunroofCodeCave()
{
	_asm mov CarTypeID_Temp, eax;
	_asm pushad;

	if (HasSunroof(CarTypeID_Temp))
	{
		_asm popad;
		_asm jmp DoesHaveSunroof;
	}
	else
	{
		_asm popad;
		_asm jmp DoesNotHaveSunroof;
	}
}

void __declspec(naked) BuildRandomRideCodeCave()
{
	_asm
	{
		je LessRandomParts
		cmp eax,0x2E
		jl MoreRandomParts

		LessRandomParts:
			push 0x6376F1
			retn

		MoreRandomParts:
			push 0x637654
			retn
	}
}

int Init()
{
	CIniReader Settings("NFSU2UnlimiterSettings.ini");

	// Main
	NewCarsInitiallyUnlocked = Settings.ReadInteger("Main", "NewCarsInitiallyUnlocked", 1) == 1;
	NewCarsCanBeDrivenByAI = Settings.ReadInteger("Main", "NewCarsCanBeDrivenByAI", 0) == 1;
	EnableUnlimiterData = Settings.ReadInteger("Main", "EnableUnlimiterData", 1) == 1;
	// Fixes
	DisappearingWheelsFix = Settings.ReadInteger("Fixes", "DisappearingWheelsFix", 1) == 1;
	AddOnOpponentsPartsFix = Settings.ReadInteger("Fixes", "AddOnOpponentsPartsFix", 1) == 1;
	// Misc
	ExpandMemoryPools = Settings.ReadInteger("Misc", "ExpandMemoryPools", 0) == 1;
	
	// Count Cars Automatically
	injector::MakeJMP(0x00636BF7, DoUnlimiterStuffCodeCave, true);

	// Fix Invisible Wheels
	if (DisappearingWheelsFix) injector::WriteMemory<BYTE>(0x60c5a9, 0x01, true);

	// Check CarRenderInfo::Render data
	if (EnableUnlimiterData)
	{
		injector::MakeJMP(0x6253ED, LinkLicensePlateToTrunkCodeCave, true); // LinkLicensePlateToTrunk
		injector::MakeCALL(0x62331B, ShowTrunkUnderInFE, true); // ShowTrunkUnderInFE
		injector::MakeCALL(0x623479, ShowTrunkUnderInFE, true); // ShowTrunkUnderInFE
		injector::MakeCALL(0x615817, RemoveCentreBrakeWithCustomSpoiler, true); // RemoveCentreBrakeWithCustomSpoiler
		//injector::MakeCALL(0x623155, HasSunroof, true);
		injector::MakeJMP(0x60C854, HasSunroofCodeCave, true); // HasSunroof
	}
	
	// Expand Memory Pools (ty Berkay and Aero_)
	if (ExpandMemoryPools)
	{
		// FEngMemoryPoolSize (InitFEngMemoryPool)
		injector::WriteMemory<int>(0x8F5790, 800000, true); 

		// CarLoaderPoolSizesD
		injector::WriteMemory<int>(0x7FA9C8, 22000, true);
		injector::WriteMemory<int>(0x7FA9CC, 18000, true);
		injector::WriteMemory<int>(0x7FA9D0, 32000, true);

		// CarLoaderPoolSizesR
		injector::WriteMemory<int>(0x7FA9D4, 22000, true);
		injector::WriteMemory<int>(0x7FA9D8, 18000, true);
		injector::WriteMemory<int>(0x7FA9DC, 32000, true);

		// TrackStreamingPoolSizes
		injector::WriteMemory<int>(0x79DC54, 120000, true);
		injector::WriteMemory<int>(0x79DC58, 86000, true);
		injector::WriteMemory<int>(0x79DC5C, 72000, true);

		// Fixes disappearing objects (ePolySlotPool)
		injector::WriteMemory<uint32_t>(0x48CD62, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x48CD6C, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x48CD91, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x48CDA2, 0xFA000, true);
	}

	// Force Stock Parts for Add-On Cars to fix Missing Parts on Opponents' Cars
	if (AddOnOpponentsPartsFix)
	{
		injector::MakeRangedNOP(0x63764E, 0x637654, true); // RideInfo::BuildRandomRide
		injector::MakeJMP(0x63764E, BuildRandomRideCodeCave, true);
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x75BCC7) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.2 NTSC speed2.exe (4,57 MB (4.800.512 bytes)).", "NFSU2 Unlimiter", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;

}
