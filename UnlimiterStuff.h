#include "stdafx.h"
#include "stdio.h"
#include <string>
#include <windows.h>
#include "includes\injector\injector.hpp"
#include "includes\IniReader.h"

using namespace std;

int CarCount, CarArraySize, TrafficCarCount, TheCounter;
bool AllNewCarsInitiallyUnlocked, AllNewCarsCanBeDrivenByAI, DisappearingWheelsFix, ExpandMemoryPools, AddOnOpponentsPartsFix, WorldCrashFixes, ExtraCustomization, FNGFix, CabinNeonFix, RaceEngageDialogFix, RandomNameHook, RemoveRimSizeRestrictions;

BYTE RandomlyChooseableCarConfigsNorthAmerica[256], RandomlyChooseableCarConfigsRestOfWorld[256], RandomlyChooseableSUVs[256], CarLotUnlockData[256] = { 0 };
int UnlockedAtBootQuickRaceNorthAmerica[256], UnlockedAtBootQuickRaceRestOfWorld[256], PerfConfigTables[512];
int RandomCarCount, RandomSUVCount, CarTypeID_Temp, CarTypeHash_Temp, CarOriginResultTemp, LinkLicensePlateToTrunk_Temp, InitiallyUnlockedCarCount = 8;

char CarTypeName[15];
char AttachmentNameBuf[64];
char CarININame[MAX_PATH] = "";

#include "InGameFunctions.h"
#include "UserCalls.h"
#include "UnlockablesMap.h"
#include "CameraInfo.h"
#include "CarRenderInfo.h"
#include "RideInfo.h"
#include "RidePhysicsInfo.h"
#include "CarCustomizeManager.h"
#include "PartSelectionScreen.h"
#include "ChooseRimBrand.h"
#include "IceSelectionScreen.h"
#include "CustomizeNeonMenu.h"
#include "ChooseDecalCategory.h"
#include "ChoosePaintScreen.h"
#include "RimsBrowser.h"
#include "FEPackage.h"
#include "CareerEventData.h"
#include "RandomCharacterNames.h"
#include "Helpers.h"

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
	CarCount = CarArraySize / 0x890;

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
	injector::WriteMemory<BYTE>(0x610150, CarCount, true); // GetCarTypeInfoFromHash
	injector::WriteMemory<BYTE>(0x61C671, CarCount, true); // CarLoader::LoadAllPartsAnims
	injector::WriteMemory<BYTE>(0x6372B4, CarCount, true); // RideInfo::FillWithPreset
	injector::WriteMemory<BYTE>(0x4EAE48, CarCount, true); // GarageMainScreen::GarageMainScreen
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

void __declspec(naked) PerformanceConfigFixCodeCave()
{
	_asm mov eax, 1;
	_asm pushad;

	FillUpPerformanceConfig();

	_asm popad;
	_asm push 0x5994C0;
	_asm retn;
}

int Init()
{
	CIniReader Settings("NFSU2UnlimiterSettings.ini");

	// Main
	AllNewCarsInitiallyUnlocked = Settings.ReadInteger("Main", "AllNewCarsInitiallyUnlocked", 0) != 0;
	AllNewCarsCanBeDrivenByAI = Settings.ReadInteger("Main", "AllNewCarsCanBeDrivenByAI", 0) != 0;
	ExtraCustomization = Settings.ReadInteger("Main", "EnableExtraCustomization", 1) != 0;
	RandomNameHook = Settings.ReadInteger("Main", "EnableRandomNameHook", 1) != 0;
	// Fixes
	DisappearingWheelsFix = Settings.ReadInteger("Fixes", "DisappearingWheelsFix", 1) != 0;
	WorldCrashFixes = Settings.ReadInteger("Fixes", "WorldCrashFixes", 1) != 0;
	CabinNeonFix = Settings.ReadInteger("Fixes", "CabinNeonFix", 1) != 0;
	RaceEngageDialogFix = Settings.ReadInteger("Fixes", "RaceEngageDialogFix", 1) != 0;
	FNGFix = Settings.ReadInteger("Fixes", "FNGFix", 1) != 0;
	// Misc
	ExpandMemoryPools = Settings.ReadInteger("Misc", "ExpandMemoryPools", 1) != 0;
	AddOnOpponentsPartsFix = Settings.ReadInteger("Misc", "ForceStockPartsOnAddOnOpponents", 0) != 0;

	// Count Cars Automatically
	injector::MakeJMP(0x00636BF7, DoUnlimiterStuffCodeCave, true);

	// Fix car lot unlock crash
	for (TheCounter = 0; TheCounter < 256; TheCounter++) CarLotUnlockData[TheCounter] = 1; // fill with 1s
	injector::MakeRangedNOP(0x513C5A, 0x513C6C, true); // disable game's own array
	injector::MakeJMP(0x513CD5, CarLotFixCodeCaveWrite, true);
	injector::MakeJMP(0x513CF0, CarLotFixCodeCaveRead, true);

	if (AllNewCarsInitiallyUnlocked)
	{
		injector::MakeNOP(0x513CD5, 5, true);
	}

	// Fix Performance Config
	injector::WriteMemory(0x59945D, PerfConfigTables + 1, true); // LoaderPerformanceConfigChunk
	injector::WriteMemory(0x59A457, PerfConfigTables + 1, true); // GetMinMaxPerformanceMeasure
	injector::WriteMemory(0x59A460, PerfConfigTables + 1, true); // GetMinMaxPerformanceMeasure
	injector::WriteMemory(0x5B69CF, PerfConfigTables + 1, true); // RidePhysicsInfo::GetPerformanceMeasure
	injector::WriteMemory(0x5B6B41, PerfConfigTables + 1, true); // RidePhysicsInfo::MatchPerformance
	injector::WriteMemory(0x5B6BA8, PerfConfigTables + 1, true); // RidePhysicsInfo::MatchPerformance
	injector::WriteMemory(0x59A46E, PerfConfigTables, true); // GetMinMaxPerformanceMeasure
	injector::WriteMemory(0x5B6B33, PerfConfigTables, true); // RidePhysicsInfo::MatchPerformance
	injector::WriteMemory(0x5B6B99, PerfConfigTables, true); // RidePhysicsInfo::MatchPerformance
	injector::WriteMemory(0x59946E, PerfConfigTables + 511, true); // LoaderPerformanceConfigChunk
	injector::MakeJMP(0x5994BB, PerformanceConfigFixCodeCave, true); // LoaderPerformanceConfigChunk, Fill up empty space by copying the values over to work around the crash

	// Fix Invisible Wheels
	if (DisappearingWheelsFix) injector::WriteMemory<BYTE>(0x60c5a9, 0x01, true); // CarPartCuller::CullParts

	// Disable Tire Mask Rendering
	injector::MakeRangedNOP(0x62DEB9, 0x62DEBF, true); // CarRenderInfo::Render
	injector::MakeJMP(0x62DEB9, 0x62DF66, true);

	// Extra Customization
	if (ExtraCustomization)
	{
		// Body shop
		injector::MakeCALL(0x54FAFB, PartSelectionScreen_SetupBodyShop, true); // PartSelectionScreen::Setup
		injector::MakeJMP(0x566B7A, WidebodyWarningCodeCave, true); // PartSelectionScreen::NotificationMessage

		// Specialties shop & Trunk Audio
		injector::MakeCALL(0x55C097, IceSelectionScreen_Setup, true); // IceSelectionScreen::IceSelectionScreen
		injector::WriteMemory(0x79D688, &IceSelectionScreen_Setup, true); // IceSelectionScreen::vtable

		// Rims menu
		CIniReader RimBrandsINI("UnlimiterData\\_RimBrands.ini");
		int RimBrandsCount = RimBrandsINI.ReadInteger("RimBrands", "NumberOfRimBrands", -1);
		if (RimBrandsCount != -1)
		{
			injector::MakeCALL(0x5530A7, ChooseRimBrand_Setup, true); // ChooseRimBrand::ChooseRimBrand
			injector::WriteMemory(0x79CA8C, &ChooseRimBrand_Setup, true); // ChooseRimBrand::vtable

			// Remove rim size restrictions
			RemoveRimSizeRestrictions = RimBrandsINI.ReadInteger("RimBrands", "RemoveRimSizeRestrictions", 0) != 0;

			injector::MakeJMP(0x539B70, IsRimAvailable, true); // 4 references

			// Allow brands to hide their names on rim selection screen
			injector::WriteMemory(0x79D884, RimsBrowser_RefreshHeader, true); // RimsBrowser::vtable
		}
		
		// Neon menu
		injector::MakeCALL(0x55356B, CustomizeNeonMenu_Setup, true); // CustomizeNeonMenu::CustomizeNeonMenu
		injector::WriteMemory(0x79CB10, &CustomizeNeonMenu_Setup, true); // CustomizeNeonMenu::vtable

		// Decals menu
		injector::MakeCALL(0x553195, ChooseDecalCategory_Setup, true); // ChooseDecalCategory::ChooseDecalCategory
		injector::WriteMemory(0x79CAA8, &ChooseDecalCategory_Setup, true); // ChooseDecalCategory::vtable
		injector::MakeJMP(0x521940, CarCustomizeManager_GetLayoutPart, true); //CarCustomizeManager::GetLayoutPart, 4 references

		// Paint menu
		injector::MakeCALL(0x563C64, ChoosePaintScreen_BuildPaintCategoryList, true); // ChoosePaintScreen::Setup
		injector::MakeCALL(0x5635C5, ChoosePaintScreen_CanPaintThisPart, true); // ChoosePaintScreen::NotificationMessage
		// Restore trunk audio paint
		injector::WriteMemory<BYTE>(0x50F625, 1, true); // PAINT_AUDIO - ChoosePaintScreen::GetPaintBrandHashFromCarSlotId
		injector::MakeRangedNOP(0x55CCBE, 0x55CCCF, true); // ChoosePaintScreen::StartBrowsingPaints
		injector::MakeJMP(0x55CCBE, TrunkAnimCodeCave_ChoosePaintScreen_StartBrowsingPaints, true);
		injector::MakeJMP(0x55CEB0, TrunkAnimCodeCave_ChoosePaintScreen_StopBrowsingPaints, true);  // ChoosePaintScreen::StopBrowsingPaints
		injector::WriteMemory(0x563C4C, &PushCantPaintTrunkDialogText, true); // ChoosePaintScreen::NotificationMessage

		// Clean out (null) and (badptr) strings
		injector::WriteMemory<unsigned char>(0x786D58, 0x00, true);
		injector::WriteMemory<unsigned char>(0x786D4C, 0x00, true);

		// Allow user to reinstall and customize stock gauges
		injector::MakeNOP(0x564154, 2, true);
		injector::WriteMemory<unsigned char>(0x5641A8, 0xEB, true);

		// Hook RideInfo_SetPart (78 references)
		injector::MakeJMP(0x631D20, RideInfo_SetPart, true);

		// Hook RideInfo_UpdatePartsEnabled (31 references)
		injector::MakeJMP(0x61BCD0, RideInfo_UpdatePartsEnabled, true);

		// Fix engine textures in the game world
		injector::WriteMemory<int>(0x57F6BF, 0xB8, true); // TrackLoader::LoadHandler
		injector::WriteMemory<float>(0x620BC9, 1, true); // CarRenderInfo::Render

		// Make trunk contents visible in the game world
		injector::WriteMemory<float>(0x620C02, 0.7f, true); // CarRenderInfo::Render (trunk audio brightness)
		injector::WriteMemory<char>(0x630C27, 0xEB, true); // CarRenderInfo::Render

		// Render door panels in the game world even if the doors are closed
		injector::MakeNOP(0x620C32, 3, true); // CarRenderInfo::Render
		injector::WriteMemory<WORD>(0x620C32, 0x01B2, true); // mov dl, 01

		// Read our custom unlockable map
		injector::WriteMemory(0x501F18, MapCarSlotToUnlockable, true); // MapCarPartToUnlockable

		// Fix part counts (GetPartsList)
		injector::MakeJMP(0x5449C0, GetPartsList, true); // 4 references

		// Camera Info Stuff
		injector::MakeCALL(0x4A55F7, FindPartCameraInfo, true); // CarOrbiter::SetLookatPart
		injector::MakeCALL(0x4A5BF7, FindPartCameraInfo, true); // GarageMainScreen::SetAutoRotateParams

		// Add Powder Coat and High Heat categories to Body Paint
		injector::MakeJMP(0x55D0D0, ChoosePaintScreen_ScrollPaintTypes, true); // 3 references
		injector::MakeRangedNOP(0x563A6E, 0x563A87, true); // ChoosePaintScreen::NotificationMessage, Switch and play sound for any paint type
		// ChoosePaintScreen::StartBrowsingPaints, show arrows for these paint types
		injector::WriteMemory<BYTE>(0x55CE85, 0, true); // PAINT_ENGINE
		injector::WriteMemory<BYTE>(0x55CE88, 0, true); // PAINT_EXHAUST
		injector::WriteMemory<BYTE>(0x55CE89, 0, true); // PAINT_AUDIO

		// Apply attributes by hooking CarRenderInfo functions
		injector::MakeCALL(0x639091, CarRenderInfo_UpdateWheelYRenderOffset, true); // CarRenderInfo::CarRenderInfo
		injector::MakeCALL(0x61ACD0, CarRenderInfo_RenderNeon, true); // RenderFrontEndCarNeon
		injector::MakeCALL(0x61B26E, CarRenderInfo_RenderNeon, true); // RenderCarNeon
		//injector::MakeJMP(0x61548A, CarRenderInfoAttributesCodeCave, true); // CarRenderInfo::UpdateCarReplacementTextures
		injector::MakeJMP(0x6304C4, ShowEngineAttrCodeCave, true); // CarRenderInfo::Render

		// Fix tire skids (WIP)
		injector::MakeJMP(0x5B4DC0, RidePhysicsInfo_RebuildPhysicsInfo, true); // 4 references
	}

	// Fix Cabin Neon
	if (CabinNeonFix)
	{
		injector::MakeJMP(0x6227C8, CabinNeonFixCodeCave1, true); // CarRenderInfo::Render
		injector::MakeJMP(0x622BD9, CabinNeonFixCodeCave2, true); // CarRenderInfo::Render
	}

	// Fix WorldChallenges Crash
	if (WorldCrashFixes)
	{
		injector::WriteMemory<BYTE>(0x500E2F, 0x7F, true); // AddUnlockedZone (ShopDataDesc)
		injector::WriteMemory<BYTE>(0x500E6F, 0x7F, true); // AddUnlockedZone (CareerEventData)
		injector::WriteMemory<BYTE>(0x500EB0, 0x7F, true); // AddUnlockedZone
		injector::WriteMemory<BYTE>(0x500F0B, 0x7F, true); // GetNextUnlockedTriggerZone
		injector::WriteMemory<BYTE>(0x500F1F, 0x7F, true); // GetNextUnlockedTriggerZone
		injector::WriteMemory<int>(0x500F38, 0x7F, true); // Reset
		injector::WriteMemory<BYTE>(0x53345B, 0x7F, true); // PlayerCareerState::RecalcUnlockedZones
		injector::WriteMemory<BYTE>(0x5334BB, 0x7F, true); // PlayerCareerState::RecalcUnlockedZones
		injector::WriteMemory<BYTE>(0x53352A, 0x7F, true); // PlayerCareerState::RecalcUnlockedZones
		injector::WriteMemory<BYTE>(0x53355B, 0x7F, true); // PlayerCareerState::RecalcUnlockedZones
		injector::WriteMemory<BYTE>(0x53362B, 0x7F, true); // PlayerCareerState::RecalcUnlockedZones
	}

	// Check CarRenderInfo::Render data
	injector::MakeJMP(0x6253ED, LinkLicensePlateToTrunkCodeCave, true); // LinkLicensePlateToTrunk
	injector::MakeCALL(0x62331B, ShowTrunkUnderInFE, true); // ShowTrunkUnderInFE
	injector::MakeCALL(0x623479, ShowTrunkUnderInFE, true); // ShowTrunkUnderInFE
	//injector::MakeCALL(0x615817, RemoveCentreBrakeWithCustomSpoiler, true); // RemoveCentreBrakeWithCustomSpoiler
	injector::MakeJMP(0x615817, RemoveCentreBrakeWithCustomSpoilerCodeCave, true); // RemoveCentreBrakeWithCustomSpoiler
	//injector::MakeCALL(0x623155, HasSunroof, true);
	injector::MakeJMP(0x60C854, HasSunroofCodeCave, true); // HasSunroof

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

	// Clone objects in FNG where needed
	if (FNGFix)
	{
		injector::MakeCALL(0x5557D4, CloneObjectstoShowMoreItemsInMenu, true); // FEPackageReader::Load
	}

	// Use specific engage message regardless from the stage, if exists
	// Example: "ENGAGE_STAGE_0_SPRINT_0"
	if (RaceEngageDialogFix)
	{
		injector::MakeCALL(0x4B551B, CareerEventData_GetEngageDialogBody, true); // RaceEngageDialog::Setup
	}

	// Unhardcode Random Names
	if (RandomNameHook)
	{
		injector::MakeCALL(0x499170, GetRandomCharacterNames, true); // EventEngageDialog::FillFromWithRandomName
		injector::MakeCALL(0x53EED8, GetRandomCharacterNames, true); // RaceStarter::AddAIOpponentCars
		injector::MakeCALL(0x53F043, GetRandomCharacterNames, true); // RaceStarter::AddRandomEncounterCars
		injector::MakeCALL(0x56DA3B, GetRandomCharacterNames, true); // DriftManager::BuildLeaderBoard
	}

	return 0;
}
