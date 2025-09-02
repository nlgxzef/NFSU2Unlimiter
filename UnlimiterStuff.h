#include "stdafx.h"
#include "stdio.h"
#include <string>
#include <windows.h>
#include "includes\injector\injector.hpp"
#include "includes\ini.h"

using namespace std;

int CarCount, ReplacementCar, CarArraySize, CarPartCount, CarPartPartsTableSize, TrafficCarCount, TheCounter;
bool AllNewCarsInitiallyUnlocked, AllNewCarsCanBeDrivenByAI, DisappearingWheelsFix, ExpandMemoryPools, AddOnOpponentsPartsFix, WorldCrashFixes, EnableFNGFixes, CabinNeonFix, RaceEngageDialogFix, RandomNameHook, ExtendFeCarLimits, DisableTextureReplacement, ExportCameraInfoIni, StreamingTrafficCarManagerFix;

BYTE RandomlyChooseableCarConfigsNorthAmerica[256], RandomlyChooseableCarConfigsRestOfWorld[256], RandomlyChooseableSUVs[256], CarLotUnlockData[256] = { 0 };
int UnlockedAtBootQuickRaceNorthAmerica[256], UnlockedAtBootQuickRaceRestOfWorld[256], PerfConfigTables[512];
int RandomCarCount, RandomSUVCount, CarTypeID_Temp, CarTypeHash_Temp, CarOriginResultTemp, LinkLicensePlateToTrunk_Temp;

char AttachmentNameBuf[64];

#include "InGameFunctions.h"
#include "UserCalls.h"
#include "UnlockablesMap.h"
#include "DialogInterface.h"
#include "CameraInfo.h"
#include "CarTypeInfo.h"
#include "CarPart.h"
#include "CarRenderInfo.h"
#include "RideInfo.h"
#include "RidePhysicsInfo.h"
#include "CarLoader.h"
#include "CarCustomizeManager.h"
#include "CustomizationScreenManager.h"
#include "SelectCarCameraMover.h"
#include "GarageMainScreen.h"
#include "PlayerCareerState.h"
#include "StarGazerGuide.h"
#include "DetailsPane.h"
#include "PartSelectionScreen.h"
#include "ChooseRimBrand.h"
#include "IceCategoryTrunkThing.h"
#include "IceSelectionScreen.h"
#include "IcePartsBrowser.h"
#include "NeonPartsBrowser.h"
#include "CustomizeNeonMenu.h"
#include "ChooseDecalCategory.h"
#include "ChooseDecalScreen.h"
#include "ChoosePaintScreen.h"
#include "ChooseVinylLayerScreen.h"
#include "ChooseSpinnerBrand.h"
#include "ChooseCustomizeCategory.h"
#include "ChoosePerfCategory.h"
#include "ChoosePerformancePackages.h"
#include "PerfPart.h"
#include "BuyPerfPartWidget.h"
#include "PartInfoWidget.h"
#include "BuyPerformanceParts.h"
#include "ChoosePaintShopCategory.h"
#include "RimsBrowser.h"
#include "FEPackage.h"
#include "CareerEventData.h"
#include "StreamingTrafficCarManager.h"
#include "RandomCharacterNames.h"
#include "Helpers.h"
#include "UnlimiterData.h"
#include "CodeCaves.h"
#include "FeCarLimits.h"

int Init()
{
	CurrentWorkingDirectory = std::filesystem::current_path();

	auto UnlimiterSettings = CurrentWorkingDirectory / "NFSU2UnlimiterSettings.ini";
	mINI::INIFile NFSU2UnlimiterSettingsINIFile(UnlimiterSettings.string());
	mINI::INIStructure Settings;
	NFSU2UnlimiterSettingsINIFile.read(Settings);

	// Main
	ReplacementCar = mINI_ReadInteger(Settings, "Main", "ReplacementModel", 1);
	AllNewCarsInitiallyUnlocked = mINI_ReadInteger(Settings, "Main", "AllNewCarsInitiallyUnlocked", 0) != 0;
	AllNewCarsCanBeDrivenByAI = mINI_ReadInteger(Settings, "Main", "AllNewCarsCanBeDrivenByAI", 0) != 0;
	RandomNameHook = mINI_ReadInteger(Settings, "Main", "EnableRandomNameHook", 1) != 0;

	// Fixes
	DisappearingWheelsFix = mINI_ReadInteger(Settings, "Fixes", "DisappearingWheelsFix", 1) != 0;
	WorldCrashFixes = mINI_ReadInteger(Settings, "Fixes", "WorldCrashFixes", 1) != 0;
	CabinNeonFix = mINI_ReadInteger(Settings, "Fixes", "CabinNeonFix", 1) != 0;
	RaceEngageDialogFix = mINI_ReadInteger(Settings, "Fixes", "RaceEngageDialogFix", 1) != 0;
	EnableFNGFixes = mINI_ReadInteger(Settings, "Fixes", "FNGFix", 0) != 0;
	StreamingTrafficCarManagerFix = mINI_ReadInteger(Settings, "Fixes", "StreamingTrafficCarManagerFix", 0) != 0;

	// Misc
	ExpandMemoryPools = mINI_ReadInteger(Settings, "Misc", "ExpandMemoryPools", 1) != 0;
	AddOnOpponentsPartsFix = mINI_ReadInteger(Settings, "Misc", "ForceStockPartsOnAddOnOpponents", 0) != 0;
	ExtendFeCarLimits = mINI_ReadInteger(Settings, "Misc", "ExtendFeCarLimits", 0) != 0;// Doubles the amount of stock and tuned cars a player can have in a profile.

	// Debug
	DisableTextureReplacement = mINI_ReadInteger(Settings, "Debug", "DisableTextureReplacement", 0) != 0;
	ExportCameraInfoIni = mINI_ReadInteger(Settings, "Debug", "ExportCameraInfo", 0) != 0;
	EnableReleasePrintf = mINI_ReadInteger(Settings, "Debug", "EnableReleasePrintf", EnableReleasePrintf) != 0;

	// Count Cars Automatically
	injector::MakeJMP(0x636BF7, DoUnlimiterStuffCodeCave, true); // LoaderCarInfo
	injector::MakeJMP(0x636D6C, DoUnlimiterStuffCodeCave2, true);

	// Fix car lot unlock crash
	for (TheCounter = 0; TheCounter < 256; TheCounter++) CarLotUnlockData[TheCounter] = 1; // fill with 1s
	injector::MakeRangedNOP(0x513C5A, 0x513C6C, true); // disable game's own array
	injector::MakeJMP(0x513CD5, CarLotFixCodeCaveWrite, true); // PlayerCareerState::BuildUnlockedCareerCarList
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

	// Render LOD A brakes in game world
	injector::MakeRangedNOP(0x62E01D, 0x62E020, true); // CarRenderInfo::Render
	injector::WriteMemory<WORD>(0x62E01D, 0xC930, true); // xor cl,cl

	// Fix logos and give traffic cars their own prefix
	injector::MakeJMP(0x511E60, GetCarTypeLogoHash, true); // 10 references

	// Extra Customization
	// Body shop
	injector::MakeCALL(0x54FAFB, PartSelectionScreen_SetupBodyShop, true); // PartSelectionScreen::Setup
	injector::MakeJMP(0x566B7A, WidebodyWarningCodeCave, true); // PartSelectionScreen::NotificationMessage

	// Specialties shop & Trunk Audio
	injector::MakeCALL(0x55C097, IceSelectionScreen_Setup, true); // IceSelectionScreen::IceSelectionScreen
	injector::WriteMemory(0x79D688, &IceSelectionScreen_Setup, true); // IceSelectionScreen::vtable

	// Rims menu
	injector::MakeCALL(0x5530A7, ChooseRimBrand_Setup, true); // ChooseRimBrand::ChooseRimBrand
	injector::WriteMemory(0x79CA8C, &ChooseRimBrand_Setup, true); // ChooseRimBrand::vtable

	// Remove rim size restrictions
	injector::MakeJMP(0x539B70, IsRimAvailable, true); // 4 references

	// Allow brands to hide their names on rim selection screen + details pane
	injector::WriteMemory(0x79D87C, &RimsBrowser_NotificationMessage, true); // RimsBrowser::vtable
	injector::WriteMemory(0x79D884, &RimsBrowser_RefreshHeader, true); // RimsBrowser::vtable
	injector::MakeCALL(0x567D34, RimsBrowser_StartBrowsingRims, true); // RimsBrowser::NotificationMessage
	injector::MakeCALL(0x568010, RimsBrowser_StartBrowsingRims, true); // RimsBrowser::NotificationMessage
	injector::MakeCALL(0x567C4B, RimsBrowser_StopBrowsingRims, true); // RimsBrowser::NotificationMessage
	injector::MakeCALL(0x567D0C, RimsBrowser_StopBrowsingRims, true); // RimsBrowser::NotificationMessage
	injector::MakeCALL(0x567E65, RimsBrowser_StopBrowsingRims, true); // RimsBrowser::NotificationMessage
		
	// Vinyls menu
	injector::MakeCALL(0x54FAF2, PartSelectionScreen_SetupVinyls, true); // PartSelectionScreen::SetupVinyls
	injector::MakeCALL(0x5221A8, GetIconHashForVinylGroup, true); // ChooseVinylLayerScreen::AddLayerOption
	injector::MakeCALL(0x5221E9, GetIconHashForVinylGroup, true); // ChooseVinylLayerScreen::AddLayerOption

	// Neon menu
	injector::MakeCALL(0x55356B, CustomizeNeonMenu_Setup, true); // CustomizeNeonMenu::CustomizeNeonMenu
	injector::WriteMemory(0x79CB04, &CustomizeNeonMenu_NotificationMessage, true); // CustomizeNeonMenu::vtable
	injector::WriteMemory(0x79CB10, &CustomizeNeonMenu_Setup, true); // CustomizeNeonMenu::vtable

	// Decals menu
	injector::MakeCALL(0x553195, ChooseDecalCategory_Setup, true); // ChooseDecalCategory::ChooseDecalCategory
	injector::WriteMemory(0x79CAA8, &ChooseDecalCategory_Setup, true); // ChooseDecalCategory::vtable
	injector::MakeJMP(0x521940, CarCustomizeManager_GetLayoutPart, true); //CarCustomizeManager::GetLayoutPart, 4 references

	// Paint menu
	injector::MakeCALL(0x563C64, ChoosePaintScreen_BuildPaintCategoryList, true); // ChoosePaintScreen::Setup
	injector::MakeCALL(0x5635C5, ChoosePaintScreen_CanPaintThisPart, true); // ChoosePaintScreen::NotificationMessage
	injector::WriteMemory(0x79D8FC, &ChoosePaintScreen_RefreshHeader, true); // ChoosePaintScreen::vtable
	//injector::MakeCALL(0x530885, ChoosePaintScreen_GetPaintBrandHashFromCarSlotId, true); // ChoosePaintScreen::IsPaintMenuLocked (shows locked)
	injector::MakeCALL(0x53C122, ChoosePaintScreen_GetPaintBrandHashFromCarSlotId, true); // ChoosePaintScreen::BuildPaintCategoryList
	injector::MakeCALL(0x55CC67, ChoosePaintScreen_GetPaintBrandHashFromCarSlotId, true); // ChoosePaintScreen::StartBrowsingPaints
	injector::MakeCALL(0x55D09C, ChoosePaintScreen_GetPaintBrandHashFromCarSlotId, true); // ChoosePaintScreen::StopBrowsingPaints
	//injector::MakeCALL(0x56344C, ChoosePaintScreen_GetPaintBrandHashFromCarSlotId, true); // ChoosePaintScreen::NotificationMessage (crash)
	//injector::MakeCALL(0x563A1E, ChoosePaintScreen_GetPaintBrandHashFromCarSlotId, true); // ChoosePaintScreen::NotificationMessage (crash)
	// Restore trunk audio paint
	injector::WriteMemory<BYTE>(0x50F625, 1, true); // PAINT_AUDIO - ChoosePaintScreen::GetPaintBrandHashFromCarSlotId
	injector::MakeRangedNOP(0x55CCBE, 0x55CCCF, true); // ChoosePaintScreen::StartBrowsingPaints
	injector::MakeJMP(0x55CCBE, TrunkAnimCodeCave_ChoosePaintScreen_StartBrowsingPaints, true);
	injector::MakeJMP(0x55CEB0, TrunkAnimCodeCave_ChoosePaintScreen_StopBrowsingPaints, true);  // ChoosePaintScreen::StopBrowsingPaints
	injector::WriteMemory(0x563C4C, &PushCantPaintTrunkDialogText, true); // ChoosePaintScreen::NotificationMessage
	injector::MakeCALL(0x55CC75, ChoosePaintScreen_BuildPaintList, true); // ChoosePaintScreen::StartBrowsingPaints
	injector::MakeCALL(0x55CCA9, ChoosePaintScreen_BuildPaintList, true); // ChoosePaintScreen::StartBrowsingPaints
	injector::MakeCALL(0x55D146, ChoosePaintScreen_BuildPaintList, true); // ChoosePaintScreen::ScrollPaintTypes
	injector::MakeCALL(0x563C9A, ChoosePaintScreen_BuildPaintList, true); // ChoosePaintScreen::Setup
	injector::MakeCALL(0x55D313, ChoosePaintScreen_ScrollVertically, true); // ChoosePaintScreen::ScrollPaints
	injector::MakeCALL(0x563A01, ChoosePaintScreen_ScrollPaints, true); // ChoosePaintScreen::NotificationMessage

	// Carbon fiber menu
	injector::MakeCALL(0x567091, PartSelectionScreen_SetupCarbonParts, true); // PartSelectionScreen::NotificationMessage
	injector::MakeCALL(0x5677C8, PartSelectionScreen_SetupCarbonParts, true); // PartSelectionScreen::NotificationMessage

	// Add details pane for Parts
	injector::MakeCALL(0x54FB02, PartSelectionScreen_RefreshHeader, true); // PartSelectionScreen::Setup
	injector::MakeCALL(0x546019, PartSelectionScreen_RefreshHeader, true); // PartSelectionScreen::SetupCarbonParts
	//injector::MakeCALL(0x54FA96, PartSelectionScreen_RefreshHeader, true); // PartSelectionScreen::ScrollHorizontal
	injector::MakeCALL(0x55EEDF, PartSelectionScreen_RefreshHeader, true); // PartSelectionScreen::StartBrowsingParts
	injector::MakeCALL(0x55ED53, PartSelectionScreen_RefreshHeader, true); // PartSelectionScreen::ScrollVertical

	// Add animations + details pane
	injector::MakeCALL(0x566854, PartSelectionScreen_StartBrowsingParts, true); // PartSelectionScreen::NotificationMessage
	injector::MakeCALL(0x56659A, PartSelectionScreen_StopBrowsingParts, true); // PartSelectionScreen::NotificationMessage

	// Add animations for Specialties
	injector::MakeCALL(0x556A87, IceSelectionScreen_DoSpecialScroll, true); // IceSelectionScreen::StartBrowsingParts
	injector::MakeCALL(0x568B7B, IceSelectionScreen_DoSpecialScroll, true); // IceSelectionScreen::NotificationMessage

	// Add animations for Neon
	injector::MakeJMP(0x50F290, CustomizeNeonMenu_DoSpecialScroll, true); // 4 references

	// Custom part icons
	injector::MakeCALL(0x55EE65, PartSelectionScreen_BuildPartsList, true); // PartSelectionScreen::StartBrowsingParts

	// New NotificationMessage (Rear Rims + CF Doors)
	injector::WriteMemory(0x79D760, &PartSelectionScreen_NotificationMessage, true); // PartSelectionScreen::vtable

	// Add details pane for ICE
	//injector::MakeCALL(0x55C09E, IceSelectionScreen_RefreshHeader, true); // IceSelectionScreen::IceSelectionScreen
	//injector::WriteMemory(0x79D684, &IceSelectionScreen_RefreshHeader, true); // IceSelectionScreen::vtable
	injector::MakeCALL(0x560A32, IcePartsBrowser_RefreshHeader, true); // IcePartsBrowser::Scroll
	injector::MakeCALL(0x56C070, IcePartsBrowser_RefreshHeader, true); // IcePartsBrowser::IcePartsBrowser

	// Add details pane for Neon
	injector::MakeCALL(0x55CBCC, NeonPartsBrowser_RefreshHeader, true); // NeonPartsBrowser::Scroll
	injector::MakeCALL(0x5533FE, NeonPartsBrowser_RefreshHeader, true); // NeonPartsBrowser::NeonPartsBrowser

	// Add details pane for Decals
	injector::MakeCALL(0x560175, ChooseDecalScreen_RefreshHeader, true); // ChooseDecalScreen::ScrollParts
	injector::MakeCALL(0x56858B, ChooseDecalScreen_RefreshHeader, true); // ChooseDecalScreen::ChooseDecalScreen
	injector::MakeCALL(0x56BCB8, ChooseDecalScreen_RefreshHeader, true); // ChooseDecalScreen::NotificationMessage

	// Fix double message while changing colors for Decals
	injector::MakeJMP(0x56BE1D, DoubleMessageFixCodeCave_ChooseDecalScreen_ToggleColors, true); // ChooseDecalScreen::NotificationMessage

	// Add details pane for Spinners
	injector::WriteMemory(0x79D8A8, &ChooseSpinnerBrand_NotificationMessage, true); // ChooseSpinnerBrand::vtable
	injector::WriteMemory(0x79D8B0, &ChooseSpinnerBrand_RefreshHeader, true); // ChooseSpinnerBrand::vtable
	injector::MakeCALL(0x569895, ChooseSpinnerBrand_StartBrowsingRims, true); // ChooseSpinnerBrand::NotificationMessage
	injector::MakeCALL(0x569A52, ChooseSpinnerBrand_StartBrowsingRims, true); // ChooseSpinnerBrand::NotificationMessage
	injector::MakeCALL(0x5697B0, ChooseSpinnerBrand_StopBrowsingRims, true); // ChooseSpinnerBrand::NotificationMessage
	injector::MakeCALL(0x56986B, ChooseSpinnerBrand_StopBrowsingRims, true); // ChooseSpinnerBrand::NotificationMessage
	injector::MakeCALL(0x5698AF, ChooseSpinnerBrand_StopBrowsingRims, true); // ChooseSpinnerBrand::NotificationMessage

	// Fix going back to ice browser from any neon selection
	//injector::MakeCALL(0x55BE07, CustomizationScreenManager_CalcPrevScreen, true); // CustomizationScreenManager::ChangeToPrevScreen

	// refreshheader crash workaround for "empty" part
	//injector::MakeCALL(0x545662, PartSelectionScreen_GetPartName, true); // PartSelectionScreen::RefreshHeader

	// Performance Menu
	injector::MakeCALL(0x556D6C, ChoosePerfCategory_Setup, true); // ChoosePerfCategory::ChoosePerfCategory
	injector::WriteMemory(0x79CD94, &ChoosePerfCategory_Setup, true); // ChoosePerfCategory::vtable

	// Performance Packages Menu
	injector::MakeCALL(0x559EAE, ChoosePerformancePackages_Setup, true); // ChoosePerformancePackages::ChoosePerformancePackages
	injector::WriteMemory(0x79D2E4, &ChoosePerformancePackages_Setup, true); // ChoosePerformancePackages::vtable
	injector::MakeCALL(0x56214D, ChoosePerformancePackages_GetLangHashForPackageType, true); // ChoosePerformancePackages::RefreshHeader

	// Performance Parts Menu
	injector::MakeCALL(0x569E1E, BuyPerformanceParts_Setup, true); // BuyPerformanceParts::BuyPerformanceParts
	injector::WriteMemory(0x79D8CC, &BuyPerformanceParts_Setup, true); // BuyPerformanceParts::vtable
	injector::WriteMemory(0x79B62C, &BuyPerfPartWidget_Draw, true); // BuyPerfPartWidget::vtable
	injector::WriteMemory(0x79B5C4, &PartInfoWidget_Draw, true); // PartInfoWidget::vtable

	// Customize Main Menu
	injector::MakeCALL(0x556732, ChooseCustomizeCategory_Setup, true); // ChooseCustomizeCategory::ChooseCustomizeCategory
	injector::WriteMemory(0x79CD78, &ChooseCustomizeCategory_Setup, true); // ChooseCustomizeCategory::vtable

	// Paint Shop Menu
	injector::MakeCALL(0x557210, ChoosePaintShopCategory_Setup, true); // ChoosePaintShopCategory::ChoosePaintShopCategory
	injector::WriteMemory(0x79CDE4, &ChoosePaintShopCategory_Setup, true); // ChoosePaintShopCategory::vtable

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

	// Hook FindPartWithLevel and make it recursive
	injector::MakeCALL(0x637077, FindPartWithLevel, true); // RideInfo::SetStockParts
	injector::MakeCALL(0x639C5B, FindPartWithLevel, true); // RideInfo::SyncVisualPartsWithPhysics
	injector::MakeCALL(0x639CB6, FindPartWithLevel, true); // RideInfo::SyncVisualPartsWithPhysics

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

	// Get Parts List (customized)
	injector::MakeJMP(0x5449C0, GetPartsList, true); // 4 references
	injector::MakeJMP(0x539960, GetIcePartsList, true); // 7 references

	// Camera Info Stuff
	injector::MakeCALL(0x4A55F7, FindPartCameraInfo, true); // CarOrbiter::SetLookatPart
	injector::MakeCALL(0x4A5BF7, FindPartCameraInfo, true); // GarageMainScreen::SetAutoRotateParams
	injector::MakeNOP(0x4D8524, 2, true); // GarageMainScreen::HandleTick, update static camera with every car change instead of only SUV<-->Sedan
	injector::MakeJMP(0x4D855B, StaticCameraInfoCodeCave_GarageMainScreen_HandleTick, true);
	injector::MakeJMP(0x4EAF13, StaticCameraInfoCodeCave_GarageMainScreen_ctor, true);

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

	// Game Type Hooks
	//injector::MakeCALL(0x61B67C, IsUG1_Hash, true); // CarPartDatabase::GetCarPartDirect
	//injector::MakeCALL(0x61B686, IsUG2_Hash, true); // CarPartDatabase::GetCarPartDirect
	//injector::MakeRangedNOP(0x61B68E, 0x61B6BA, true);
	//injector::MakeJMP(0x61B68E, IsSUV_UnInlineCodeCave, true); // CarPartDatabase::GetCarPartDirect

	// (un)Sync Visual Parts With Physics
	// Engine & Brakes
	injector::MakeCALL(0x5263E5, RideInfo_SyncVisualPartsWithPhysics_Hook, true); // RaceStarter::AddCareerEventAIOpponents
	injector::MakeCALL(0x5322AF, RideInfo_SyncVisualPartsWithPhysics_Hook, true); // RaceStarter::MatchAICarPerformanceToPlayer
	injector::MakeCALL(0x639CF4, RideInfo_SyncVisualPartsWithPhysics_Hook, true); // RideInfo::RebuildPhysicsInfo
	// Camber
	injector::MakeCALL(0x6270C7, RidePhysicsInfo_GetCamberPercent, true); // CarRenderInfo::Render
	injector::MakeCALL(0x617554, RidePhysicsInfo_GetCamberPercent, true); // CarRenderInfo::RenderFast

	// Custom cost
	injector::MakeJMP(0x514280, PlayerCareerState_GetCarPartCost, true); // PlayerCareerState::GetCarPartCost, 21 references
		
	// Custom rep
	injector::MakeCALL(0x52354B, PlayerCareerState_GetCarPartRep, true); // StarGazerGuide::GetNumberOfStars
	injector::MakeJMP(0x5234C0, StarGazerGuide_GetNumberOfStars, true); // 4 references

	// Fix tire skids (WIP)
	injector::MakeJMP(0x5B4DC0, RidePhysicsInfo_RebuildPhysicsInfo, true); // 4 references

	// Fix rear rims
	injector::MakeCALL(0x61DCE5, CompositeRim, true); // CompositeSkin
	injector::MakeCALL(0x63205D, GetTempCarSkinTextures, true); // GetUsedCarTextureInfo
	injector::WriteMemory<DWORD>(0x6336BE, 0x608, true); // LoadedWheel::LoadedWheel, Seperate rear rims and fix them in game
	//injector::MakeJMP(0x6277FD, RearWheelLightMaterialCodeCave, true); // CarRenderInfo::Render
	//injector::MakeRangedNOP(0x55C263, 0x55C272, true); // CarCustomizeManager::InstallPart (disable setting rear rims from front)
	//injector::MakeRangedNOP(0x55C1A4, 0x55C1B6, true); // CarCustomizeManager::PreviewPart (disable previewing rear rims from front)
	//injector::WriteMemory<BYTE>(0x567BD2, 29, true); // RimsBrowser::NotificationMessage
	//injector::WriteMemory<BYTE>(0x567CB0, 29, true); // RimsBrowser::NotificationMessage
	//injector::WriteMemory<BYTE>(0x567F49, 29, true); // RimsBrowser::NotificationMessage
	//injector::WriteMemory<BYTE>(0x567FAA, 29, true); // RimsBrowser::NotificationMessage
	//injector::WriteMemory<BYTE>(0x5681E5, 29, true); // RimsBrowser::NotificationMessage
	//injector::WriteMemory<BYTE>(0x56824D, 29, true); // RimsBrowser::NotificationMessage
	//injector::WriteMemory<BYTE>(0x55F6A6, 29, true); // RimsBrowser::Setup
	//injector::WriteMemory<BYTE>(0x55F83B, 29, true); // RimsBrowser::ScrollVertical
	//injector::WriteMemory<BYTE>(0x55FAA8, 29, true); // RimsBrowser::BuildPartsIconList
	//injector::WriteMemory<BYTE>(0x55FD8A, 29, true); // RimsBrowser::StopBrowsingRims
	//injector::WriteMemory<BYTE>(0x55FDF1, 29, true); // RimsBrowser::StopBrowsingRims
	{
		injector::MakeCALL(0x55F2AC, RideInfo_SetPart_Rims, true); // ChooseRimBrand::NotificationMessage
		injector::MakeCALL(0x55F2B9, RideInfo_SetPart_Rims, true); // ChooseRimBrand::NotificationMessage
		injector::MakeCALL(0x55F69F, RideInfo_SetPart_Rims, true); // RimsBrowser::Setup
		injector::MakeCALL(0x55F6AC, RideInfo_SetPart_Rims, true); // RimsBrowser::Setup
		injector::MakeCALL(0x55F834, RideInfo_SetPart_Rims, true); // RimsBrowser::ScrollVertical
		injector::MakeCALL(0x55F841, RideInfo_SetPart_Rims, true); // RimsBrowser::ScrollVertical
		injector::MakeCALL(0x55FAA1, RideInfo_SetPart_Rims, true); // RimsBrowser::BuildPartsIconList
		injector::MakeCALL(0x55FAAE, RideInfo_SetPart_Rims, true); // RimsBrowser::BuildPartsIconList
		injector::MakeCALL(0x55FD83, RideInfo_SetPart_Rims, true); // RimsBrowser::StopBrowsingRims
		injector::MakeCALL(0x55FD90, RideInfo_SetPart_Rims, true); // RimsBrowser::StopBrowsingRims
		injector::MakeCALL(0x55FDEA, RideInfo_SetPart_Rims, true); // RimsBrowser::StopBrowsingRims
		injector::MakeCALL(0x55FDF7, RideInfo_SetPart_Rims, true); // RimsBrowser::StopBrowsingRims
		injector::MakeCALL(0x567BCB, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567BD8, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567CA9, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567CB6, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567F42, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567F4F, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567FA3, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x567FB0, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x5681DE, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x5681EB, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x568246, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x568253, RideInfo_SetPart_Rims, true); // RimsBrowser::NotificationMessage
		injector::MakeCALL(0x55C26D, RideInfo_SetPart_Rims, true); // CarCustomizeManager::InstallPart
		injector::MakeCALL(0x55C1B1, RideInfo_SetPart_Rims, true); // CarCustomizeManager::PreviewPart
		// TODO: Also check ChooseSpinnerBrand or find a smarter way to do this shit
	}

	if (StreamingTrafficCarManagerFix)
	{
		// Fix random traffic car skins
		injector::MakeCALL(0x42F9B3, StreamingTrafficCarManager_Update, true); // TrafficTeleporter::DoTimeStep
		injector::MakeCALL(0x417A92, StreamingTrafficCarManager_ResetStats, true); // TrafficTeleporter::ctor
		injector::MakeCALL(0x41AB8F, StreamingTrafficCarManager_ResetStats, true); // StreamingTrafficCarManager::Init

		// Relocate StreamingTrafficCarManager to allow more traffic cars
		injector::WriteMemory(0x417A8E, &TheStreamingTrafficCarManager, true); // TrafficTeleporter::ctor
		injector::WriteMemory(0x417B7F, &TheStreamingTrafficCarManager, true); // TrafficTeleporter::DoSnapshot
		injector::WriteMemory(0x4221D4, &TheStreamingTrafficCarManager, true); // TrafficTeleporter::dtor
		injector::WriteMemory(0x42F9AF, &TheStreamingTrafficCarManager, true); // TrafficTeleporter::DoTimeStep
		injector::WriteMemory(0x4FEA5C, &TheStreamingTrafficCarManager, true); // RaceStarter::AddTrafficCars
		injector::WriteMemory(0x4FEA71, &TheStreamingTrafficCarManager, true); // RaceStarter::AddTrafficCars
		injector::WriteMemory(0x57F992, &TheStreamingTrafficCarManager, true); // TrackLoader::LoadHandler
		injector::WriteMemory(0x57F99C, &TheStreamingTrafficCarManager, true); // TrackLoader::LoadHandler
		injector::WriteMemory(0x57FA0D, &TheStreamingTrafficCarManager, true); // TrackLoader::LoadHandler
		injector::WriteMemory(0x580080, &TheStreamingTrafficCarManager, true); // BeginGameFlowLoadingFrontEnd
		injector::WriteMemory(0x5F4604, &TheStreamingTrafficCarManager, true); // Car::dtor
		injector::WriteMemory(0x77A991, &TheStreamingTrafficCarManager, true); // Static init??
		injector::WriteMemory(0x7819A1, &TheStreamingTrafficCarManager, true); // Static init??
	}
	
	// Texture caves
	if (!DisableTextureReplacement)
	{
		injector::MakeCALL(0x621079, CarRenderInfo_UpdateLightStateTextures, true); // CarRenderInfo::Render
		injector::MakeJMP(0x633FC1, CarRenderInfo_UpdateCarReplacementTextures, true); // CarRenderInfo::RethinkDamage
		injector::MakeCALL(0x63873D, CarRenderInfo_UpdateCarReplacementTextures, true); // CarRenderInfo::ctor
		injector::MakeCALL(0x633608, GetUsedCarTextureInfo, true); // LoadedSkin::ctor
		injector::MakeCALL(0x636855, GetUsedCarTextureInfo, true); // CarRenderInfo::SwitchSkin
		injector::MakeCALL(0x638472, GetUsedCarTextureInfo, true); // CarRenderInfo::ctor

		injector::MakeRangedNOP(0x61FF70, 0x620023, true); // Free up texture replacement slots #47-72 instead of assigning unused leftover decal stuff, CarRenderInfo::UpdateDecalTextures
		// Resize TexturesToLoadPerm and Temp (Also in LoadedSkin struct)
		injector::MakeCALL(0x63B248, CarLoader_LoadSkin, true); // CarLoader::ServiceLoading
		injector::MakeCALL(0x63B279, CarLoader_LoadSkin, true); // CarLoader::ServiceLoading
		injector::MakeCALL(0x639FBB, CarLoader_UnloadSkinTemporaries, true); // CarLoader::UnloadSkin
		injector::MakeCALL(0x639FC3, CarLoader_UnloadSkinPerms, true);
		injector::MakeCALL(0x61CB4A, CarLoader_LoadedSkinCallback, true); // CarLoader::LoadedSkinCallbackBridge
		injector::MakeCALL(0x6378AE, LoadedSkin_Create, true); // LoadedRideInfo::ctor
		
		injector::WriteMemory<int>(0x63B2BE, 0x31C + MaxPermTex * 4, true); // CarLoader::ServiceLoading (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B2E2, 0x31C + MaxPermTex * 4, true); // CarLoader::ServiceLoading (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B300, 0x31C + MaxPermTex * 4, true); // CarLoader::ServiceLoading (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63A071, 0x31C + MaxPermTex * 4, true); // CarLoader::UnloadRideInfo (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63A093, 0x31C + MaxPermTex * 4, true); // CarLoader::UnloadRideInfo (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63A0B1, 0x31C + MaxPermTex * 4, true); // CarLoader::UnloadRideInfo (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B023, 0x31C + MaxPermTex * 4, true); // CarLoader::AllocateRideInfo (LoadedSkin->NumLoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B2C8, 0x31C + MaxPermTex * 4 + 4, true); // CarLoader::ServiceLoading (LoadedSkin->LoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B2E9, 0x31C + MaxPermTex * 4 + 4, true); // CarLoader::ServiceLoading (LoadedSkin->LoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63A07B, 0x31C + MaxPermTex * 4 + 4, true); // CarLoader::UnloadRideInfo (LoadedSkin->LoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63A09A, 0x31C + MaxPermTex * 4 + 4, true); // CarLoader::UnloadRideInfo (LoadedSkin->LoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B01C, 0x31C + MaxPermTex * 4 + 4, true); // CarLoader::AllocateRideInfo (LoadedSkin->LoadedSkinLayersTemp)
		injector::WriteMemory<int>(0x63B02A, 8 + MaxPermTex * 4, true); // CarLoader::AllocateRideInfo

		// Neon texture
		hb_GetTextureInfo.fun = injector::MakeCALL(0x638858, GetNeonTextureInfo, true).get(); // CarRenderInfo::ctor
		injector::MakeRangedNOP(0x638868, 0x63886E, true); // Prevent the game from changing it back to original
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
	if (EnableFNGFixes)
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

	if (ExtendFeCarLimits)
	{
		InitFeCarLimits();
	}

	if (ExportCameraInfoIni)
	{
		Settings["Debug"]["ExportCameraInfo"] = std::to_string(0); // Disable after export
		NFSU2UnlimiterSettingsINIFile.write(Settings, true);
		ExportCameraInfo();
	}

	return 0;
}
