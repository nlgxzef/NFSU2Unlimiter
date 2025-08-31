#pragma once

#include "stdafx.h"
#include "stdio.h"
#include "InGameFunctions.h"
#include "GlobalVariables.h"
#include "Helpers.h"
#include "includes\ini.h"

void LoadCarConfigs()
{
	CarConfig ACarConfig;
	std::vector<CarConfig> CarConfigs_temp;

	auto GeneralINIPath = CurrentWorkingDirectory / "UnlimiterData" / "_General.ini";
	mINI::INIFile GeneralINIFile(GeneralINIPath.string());
	mINI::INIStructure GeneralINI;
	GeneralINIFile.read(GeneralINI);

	char CarININame[MAX_PATH];

	// Read default car config first
	DefaultCarConfig.CarTypeInfo = (DWORD*)((*(DWORD*)_CarTypeInfoArray) + ReplacementCar * SingleCarTypeInfoBlockSize);

	// Main
	DefaultCarConfig.Main.ForceLODA = mINI_ReadInteger(GeneralINI, "Main", "ForceLODA", 0) != 0;
	DefaultCarConfig.Main.InitiallyUnlocked = mINI_ReadInteger(GeneralINI, "Main", "InitiallyUnlocked", -1);
	DefaultCarConfig.Main.GameType = mINI_ReadInteger(GeneralINI, "Main", "GameType", GetDefaultGameType(ReplacementCar));
	DefaultCarConfig.Main.InductionType = mINI_ReadInteger(GeneralINI, "Main", "InductionType", 0);
	DefaultCarConfig.Main.ScaleBrakesWithRims = mINI_ReadInteger(GeneralINI, "Main", "ScaleBrakesWithRims", 1);
	DefaultCarConfig.Main.SyncVisualPartsWithPhysics = mINI_ReadInteger(GeneralINI, "Main", "SyncVisualPartsWithPhysics", 1);
	DefaultCarConfig.Main.CanBeDrivenByAI = mINI_ReadInteger(GeneralINI, "Main", "CanBeDrivenByAI", 1) != 0;

	// CarRenderInfo
	DefaultCarConfig.RenderInfo.LinkLicensePlateToTrunk = mINI_ReadInteger(GeneralINI, "CarRenderInfo", "LinkLicensePlateToTrunk", GetDefaultLinkLicensePlateToTrunk(ReplacementCar)) != 0;
	DefaultCarConfig.RenderInfo.ShowTrunkUnderInFE = mINI_ReadInteger(GeneralINI, "CarRenderInfo", "ShowTrunkUnderInFE", GetDefaultShowTrunkUnderInFE(ReplacementCar)) != 0;
	DefaultCarConfig.RenderInfo.RemoveCentreBrakeWithCustomSpoiler = mINI_ReadInteger(GeneralINI, "CarRenderInfo", "RemoveCentreBrakeWithCustomSpoiler", GetDefaultRemoveCentreBrakeWithCustomSpoiler(ReplacementCar)) != 0;
	DefaultCarConfig.RenderInfo.HasSunroof = mINI_ReadInteger(GeneralINI, "CarRenderInfo", "HasSunroof", GetDefaultHasSunroof(ReplacementCar)) != 0;

	// Stats
	DefaultCarConfig.Stats.TimingKludgeFactor060 = mINI_ReadFloat(GeneralINI, "Stats", "TimingKludgeFactor060");
	DefaultCarConfig.Stats.TimingKludgeFactor0100 = mINI_ReadFloat(GeneralINI, "Stats", "TimingKludgeFactor0100");

	// StarGazer
	DefaultCarConfig.StarGazer.StartingRep = mINI_ReadInteger(GeneralINI, "StarGazer", "StartingRep", 0);

	// Textures
	DefaultCarConfig.Textures.HeadlightOn = mINI_ReadInteger(GeneralINI, "Textures", "HeadlightOn", 0) != 0;
	DefaultCarConfig.Textures.BrakelightOn = mINI_ReadInteger(GeneralINI, "Textures", "BrakelightOn", 0) != 0;
	DefaultCarConfig.Textures.CentreBrakelightOn = mINI_ReadInteger(GeneralINI, "Textures", "CentreBrakelightOn", 0) != 0;
	DefaultCarConfig.Textures.ReverseOn = mINI_ReadInteger(GeneralINI, "Textures", "ReverseOn", 0) != 0;
	DefaultCarConfig.Textures.DamageLights = mINI_ReadInteger(GeneralINI, "Textures", "DamageLights", 0) != 0;
	DefaultCarConfig.Textures.CentreBrake = mINI_ReadInteger(GeneralINI, "Textures", "CentreBrake", 0) != 0;
	DefaultCarConfig.Textures.Reverse = mINI_ReadInteger(GeneralINI, "Textures", "Reverse", 0) != 0;
	DefaultCarConfig.Textures.BrakelightOnfInGame = mINI_ReadInteger(GeneralINI, "Textures", "BrakelightOnfInGame", 0) != 0;
	DefaultCarConfig.Textures.TireInnerMask = mINI_ReadInteger(GeneralINI, "Textures", "TireInnerMask", 0) != 0;

	// Category
	DefaultCarConfig.Category.BodyShop = mINI_ReadInteger(GeneralINI, "Category", "BodyShop", 1) != 0;
	DefaultCarConfig.Category.Performance = mINI_ReadInteger(GeneralINI, "Category", "Performance", 1) != 0;
	DefaultCarConfig.Category.Paint = mINI_ReadInteger(GeneralINI, "Category", "Paint", 1) != 0;
	DefaultCarConfig.Category.Specialties = mINI_ReadInteger(GeneralINI, "Category", "Specialties", 1) != 0;

	// BodyShop
	DefaultCarConfig.BodyShop.FrontBumper = mINI_ReadInteger(GeneralINI, "BodyShop", "FrontBumper", 1) != 0;
	DefaultCarConfig.BodyShop.RearBumper = mINI_ReadInteger(GeneralINI, "BodyShop", "RearBumper", 1) != 0;
	DefaultCarConfig.BodyShop.Skirt = mINI_ReadInteger(GeneralINI, "BodyShop", "Skirt", 1) != 0;
	DefaultCarConfig.BodyShop.Fender = mINI_ReadInteger(GeneralINI, "BodyShop", "Fender", 0) != 0;
	DefaultCarConfig.BodyShop.Quarter = mINI_ReadInteger(GeneralINI, "BodyShop", "Quarter", 0) != 0;
	DefaultCarConfig.BodyShop.Spoiler = mINI_ReadInteger(GeneralINI, "BodyShop", "Spoiler", 1) != 0;
	DefaultCarConfig.BodyShop.Hood = mINI_ReadInteger(GeneralINI, "BodyShop", "Hood", 1) != 0;
	DefaultCarConfig.BodyShop.Engine = mINI_ReadInteger(GeneralINI, "BodyShop", "Engine", 0) != 0;
	DefaultCarConfig.BodyShop.Trunk = mINI_ReadInteger(GeneralINI, "BodyShop", "Trunk", 0) != 0;
	DefaultCarConfig.BodyShop.RoofScoops = mINI_ReadInteger(GeneralINI, "BodyShop", "RoofScoops", 1) != 0;
	DefaultCarConfig.BodyShop.Interior = mINI_ReadInteger(GeneralINI, "BodyShop", "Interior", 0) != 0;
	DefaultCarConfig.BodyShop.Roof = mINI_ReadInteger(GeneralINI, "BodyShop", "Roof", 0) != 0;
	DefaultCarConfig.BodyShop.Brakes = mINI_ReadInteger(GeneralINI, "BodyShop", "Brakes", 0) != 0;
	DefaultCarConfig.BodyShop.Headlights = mINI_ReadInteger(GeneralINI, "BodyShop", "Headlights", 1) != 0;
	DefaultCarConfig.BodyShop.Taillights = mINI_ReadInteger(GeneralINI, "BodyShop", "Taillights", 1) != 0;
	DefaultCarConfig.BodyShop.Mirrors = mINI_ReadInteger(GeneralINI, "BodyShop", "Mirrors", 1) != 0;
	DefaultCarConfig.BodyShop.Exhaust = mINI_ReadInteger(GeneralINI, "BodyShop", "Exhaust", 1) != 0;
	DefaultCarConfig.BodyShop.Rims = mINI_ReadInteger(GeneralINI, "BodyShop", "Rims", 1) != 0;
	DefaultCarConfig.BodyShop.RimsCustom = mINI_ReadInteger(GeneralINI, "BodyShop", "RimsCustom", 0) != 0;
	DefaultCarConfig.BodyShop.CarbonFiber = mINI_ReadInteger(GeneralINI, "BodyShop", "CarbonFiber", 1) != 0;
	DefaultCarConfig.BodyShop.WideBodyKits = mINI_ReadInteger(GeneralINI, "BodyShop", "WideBodyKits", 1) != 0;
	DefaultCarConfig.BodyShop.Attachments = mINI_ReadInteger(GeneralINI, "BodyShop", "Attachments", 0) % 6;
	DefaultCarConfig.BodyShop.Attachment0 = mINI_ReadInteger(GeneralINI, "BodyShop", "Attachment0", DefaultCarConfig.BodyShop.Attachments > 0) != 0;
	DefaultCarConfig.BodyShop.Attachment1 = mINI_ReadInteger(GeneralINI, "BodyShop", "Attachment1", DefaultCarConfig.BodyShop.Attachments > 1) != 0;
	DefaultCarConfig.BodyShop.Attachment2 = mINI_ReadInteger(GeneralINI, "BodyShop", "Attachment2", DefaultCarConfig.BodyShop.Attachments > 2) != 0;
	DefaultCarConfig.BodyShop.Attachment3 = mINI_ReadInteger(GeneralINI, "BodyShop", "Attachment3", DefaultCarConfig.BodyShop.Attachments > 3) != 0;
	DefaultCarConfig.BodyShop.Attachment4 = mINI_ReadInteger(GeneralINI, "BodyShop", "Attachment4", DefaultCarConfig.BodyShop.Attachments > 4) != 0;

	// Performance
	DefaultCarConfig.Performance.Engine = mINI_ReadInteger(GeneralINI, "Performance", "Engine", 1) != 0;
	DefaultCarConfig.Performance.ECU = mINI_ReadInteger(GeneralINI, "Performance", "ECU", 1) != 0;
	DefaultCarConfig.Performance.Transmission = mINI_ReadInteger(GeneralINI, "Performance", "Transmission", 1) != 0;
	DefaultCarConfig.Performance.Chassis = mINI_ReadInteger(GeneralINI, "Performance", "Chassis", 1) != 0;
	DefaultCarConfig.Performance.Nitrous = mINI_ReadInteger(GeneralINI, "Performance", "Nitrous", 1) != 0;
	DefaultCarConfig.Performance.Tires = mINI_ReadInteger(GeneralINI, "Performance", "Tires", 1) != 0;
	DefaultCarConfig.Performance.Brakes = mINI_ReadInteger(GeneralINI, "Performance", "Brakes", 1) != 0;
	DefaultCarConfig.Performance.WeightReduction = mINI_ReadInteger(GeneralINI, "Performance", "WeightReduction", 1) != 0;
	DefaultCarConfig.Performance.Induction = mINI_ReadInteger(GeneralINI, "Performance", "Induction", 1) != 0;
	DefaultCarConfig.Performance.Aerodynamics = mINI_ReadInteger(GeneralINI, "Performance", "Aerodynamics", 0) != 0;
	DefaultCarConfig.Performance.TestDyno = mINI_ReadInteger(GeneralINI, "Performance", "TestDyno", 1) != 0;

	// Paint
	DefaultCarConfig.Paint.Paint = mINI_ReadInteger(GeneralINI, "Paint", "Paint", 1) != 0;
	DefaultCarConfig.Paint.PaintCustom = mINI_ReadInteger(GeneralINI, "Paint", "PaintCustom", 0) != 0;
	DefaultCarConfig.Paint.BasePaint = mINI_ReadInteger(GeneralINI, "Paint", "BasePaint", 1) != 0;
	DefaultCarConfig.Paint.SpoilerPaint = mINI_ReadInteger(GeneralINI, "Paint", "SpoilerPaint", 1) != 0;
	DefaultCarConfig.Paint.RoofScoopPaint = mINI_ReadInteger(GeneralINI, "Paint", "RoofScoopPaint", 1) != 0;
	DefaultCarConfig.Paint.MirrorPaint = mINI_ReadInteger(GeneralINI, "Paint", "MirrorPaint", 1) != 0;
	DefaultCarConfig.Paint.RimPaint = mINI_ReadInteger(GeneralINI, "Paint", "RimPaint", 1) != 0;
	DefaultCarConfig.Paint.SpinnerPaint = mINI_ReadInteger(GeneralINI, "Paint", "SpinnerPaint", 1) != 0;
	DefaultCarConfig.Paint.BrakePaint = mINI_ReadInteger(GeneralINI, "Paint", "BrakePaint", 1) != 0;
	DefaultCarConfig.Paint.EnginePaint = mINI_ReadInteger(GeneralINI, "Paint", "EnginePaint", 1) != 0;
	DefaultCarConfig.Paint.TrunkPaint = mINI_ReadInteger(GeneralINI, "Paint", "TrunkPaint", 1) != 0;
	DefaultCarConfig.Paint.ExhaustPaint = mINI_ReadInteger(GeneralINI, "Paint", "ExhaustPaint", 1) != 0;
	DefaultCarConfig.Paint.Vinyls = mINI_ReadInteger(GeneralINI, "Paint", "Vinyls", 1) != 0;
	DefaultCarConfig.Paint.VinylsCustom = mINI_ReadInteger(GeneralINI, "Paint", "VinylsCustom", 0) != 0;
	DefaultCarConfig.Paint.VinylLayers = mINI_ReadInteger(GeneralINI, "Paint", "VinylLayers", 4) % 5;
	DefaultCarConfig.Paint.Decals = mINI_ReadInteger(GeneralINI, "Paint", "Decals", 1) != 0;
	DefaultCarConfig.Paint.DecalsWindshield = mINI_ReadInteger(GeneralINI, "Paint", "DecalsWindshield", 1) != 0;
	DefaultCarConfig.Paint.DecalsRearWindow = mINI_ReadInteger(GeneralINI, "Paint", "DecalsRearWindow", 1) != 0;
	DefaultCarConfig.Paint.DecalsHood = mINI_ReadInteger(GeneralINI, "Paint", "DecalsHood", 1) != 0;
	DefaultCarConfig.Paint.DecalsLeftDoor = mINI_ReadInteger(GeneralINI, "Paint", "DecalsLeftDoor", 1) != 0;
	DefaultCarConfig.Paint.DecalsRightDoor = mINI_ReadInteger(GeneralINI, "Paint", "DecalsRightDoor", 1) != 0;
	DefaultCarConfig.Paint.DecalsLeftQuarter = mINI_ReadInteger(GeneralINI, "Paint", "DecalsLeftQuarter", 1) != 0;
	DefaultCarConfig.Paint.DecalsRightQuarter = mINI_ReadInteger(GeneralINI, "Paint", "DecalsRightQuarter", 1) != 0;

	// Specialties
	DefaultCarConfig.Specialties.CustomGauges = mINI_ReadInteger(GeneralINI, "Specialties", "CustomGauges", 1) != 0;
	DefaultCarConfig.Specialties.Neon = mINI_ReadInteger(GeneralINI, "Specialties", "Neon", 1) != 0;
	DefaultCarConfig.Specialties.UnderglowNeon = mINI_ReadInteger(GeneralINI, "Specialties", "UnderglowNeon", 1) != 0;
	DefaultCarConfig.Specialties.EngineNeon = mINI_ReadInteger(GeneralINI, "Specialties", "EngineNeon", 1) != 0;
	DefaultCarConfig.Specialties.CabinNeon = mINI_ReadInteger(GeneralINI, "Specialties", "CabinNeon", 1) != 0;
	DefaultCarConfig.Specialties.TrunkNeon = mINI_ReadInteger(GeneralINI, "Specialties", "TrunkNeon", 1) != 0;
	DefaultCarConfig.Specialties.WindowTint = mINI_ReadInteger(GeneralINI, "Specialties", "WindowTint", 1) != 0;
	DefaultCarConfig.Specialties.HeadlightColor = mINI_ReadInteger(GeneralINI, "Specialties", "HeadlightColor", 1) != 0;
	DefaultCarConfig.Specialties.NosPurge = mINI_ReadInteger(GeneralINI, "Specialties", "NosPurge", 1) != 0;
	DefaultCarConfig.Specialties.Hydrualics = mINI_ReadInteger(GeneralINI, "Specialties", "Hydrualics", 1) != 0;
	DefaultCarConfig.Specialties.TrunkAudio = mINI_ReadInteger(GeneralINI, "Specialties", "TrunkAudio", 1) != 0;
	DefaultCarConfig.Specialties.Spinners = mINI_ReadInteger(GeneralINI, "Specialties", "Spinners", 1) != 0;
	DefaultCarConfig.Specialties.SplitHoods = mINI_ReadInteger(GeneralINI, "Specialties", "SplitHoods", 1) != 0;
	DefaultCarConfig.Specialties.Doors = mINI_ReadInteger(GeneralINI, "Specialties", "Doors", 1) != 0;
	DefaultCarConfig.Specialties.LicensePlate = mINI_ReadInteger(GeneralINI, "Specialties", "LicensePlate", 0) != 0;

	// Icons
	DefaultCarConfig.Icons.CategoryBodyShop = mINI_ReadHashS(GeneralINI, "Icons", "CategoryBodyShop", "SHOP_ICON_BODY");
	DefaultCarConfig.Icons.CategoryPerformance = mINI_ReadHashS(GeneralINI, "Icons", "CategoryPerformance", "SHOP_ICON_PERFORMANCE");
	DefaultCarConfig.Icons.CategoryPaint = mINI_ReadHashS(GeneralINI, "Icons", "CategoryPaint", "SHOP_ICON_GRAPHIC");
	DefaultCarConfig.Icons.CategorySpecialties = mINI_ReadHashS(GeneralINI, "Icons", "CategorySpecialties", "SHOP_ICON_INTERIORS");
	DefaultCarConfig.Icons.BodyShop = mINI_ReadHashS(GeneralINI, "Icons", "BodyShop", "SHOP_ICON_BODY");
	DefaultCarConfig.Icons.Performance = mINI_ReadHashS(GeneralINI, "Icons", "Performance", "SHOP_ICON_PERFORMANCE");
	DefaultCarConfig.Icons.Paint = mINI_ReadHashS(GeneralINI, "Icons", "Paint", "SHOP_ICON_GRAPHIC");
	DefaultCarConfig.Icons.Specialties = mINI_ReadHashS(GeneralINI, "Icons", "Specialties", "SHOP_ICON_INTERIORS");
	DefaultCarConfig.Icons.BodyShopFrontBumper = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopFrontBumper", "VISUAL_PART_FRONT_BUMPER");
	DefaultCarConfig.Icons.BodyShopRearBumper = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopRearBumper", "VISUAL_PART_REAR_BUMPER");
	DefaultCarConfig.Icons.BodyShopSkirt = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopSkirt", "VISUAL_PART_SIDE_SKIRT");
	DefaultCarConfig.Icons.BodyShopFender = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopFender", "VISUAL_PART_FRONT_FENDER");
	DefaultCarConfig.Icons.BodyShopQuarter = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopQuarter", "VISUAL_PART_REAR_QUARTER_INTAKES");
	DefaultCarConfig.Icons.BodyShopSpoiler = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopSpoiler", "VISUAL_PART_SPOILER");
	DefaultCarConfig.Icons.BodyShopHood = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopHood", "VISUAL_PART_HOOD");
	DefaultCarConfig.Icons.BodyShopEngine = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopEngine", "VISUAL_PART_ENGINE");
	DefaultCarConfig.Icons.BodyShopTrunk = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopTrunk", "VISUAL_PART_TRUNK");
	DefaultCarConfig.Icons.BodyShopRoofScoops = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopRoofScoops", "VISUAL_PART_ROOF_SCOOP");
	DefaultCarConfig.Icons.BodyShopInterior = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopInterior", "VISUAL_PART_CABIN");
	DefaultCarConfig.Icons.BodyShopRoof = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopRoof", "VISUAL_PART_ROOF");
	DefaultCarConfig.Icons.BodyShopHeadlights = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopHeadlights", "VISUAL_PART_HEAD_LIGHTS");
	DefaultCarConfig.Icons.BodyShopTaillights = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopTaillights", "VISUAL_PART_TAIL_LIGHTS");
	DefaultCarConfig.Icons.BodyShopMirrors = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopMirrors", "VISUAL_PART_SIDE_MIRRORS");
	DefaultCarConfig.Icons.BodyShopExhaust = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopExhaust", "VISUAL_PART_MUFFLERTIP");
	DefaultCarConfig.Icons.BodyShopRims = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopRims", "VISUAL_PART_RIMS");
	DefaultCarConfig.Icons.BodyShopRimsCustom = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopRimsCustom", "VISUAL_RIMS_BRAND_CUSTOM");
	DefaultCarConfig.Icons.BodyShopBrakes = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopBrakes", "VISUAL_PART_BRAKES");
	DefaultCarConfig.Icons.BodyShopCarbonFiber = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiber", "VISUAL_PART_CARBON_FIBRE");
	DefaultCarConfig.Icons.BodyShopCarbonFiberHood = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberHood", "VISUAL_PART_CARBON_FIBRE_HOODS");
	DefaultCarConfig.Icons.BodyShopCarbonFiberSpoiler = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberSpoiler", "VISUAL_PART_CARBON_FIBRE_SPOILERS");
	DefaultCarConfig.Icons.BodyShopCarbonFiberRoofScoop = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberRoofScoop", "VISUAL_PART_CARBON_FIBRE_ROOF_SCOOP");
	DefaultCarConfig.Icons.BodyShopCarbonFiberMirrors = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberMirrors", "VISUAL_PART_CARBON_FIBRE_MIRRORS");
	DefaultCarConfig.Icons.BodyShopCarbonFiberDoor = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberDoor", "VISUAL_PART_CARBON_FIBRE_DOOR");
	DefaultCarConfig.Icons.BodyShopCarbonFiberRevertDoor = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberRevertDoor", "VISUAL_PART_DE_CARBON_FIBRE_DOOR");
	DefaultCarConfig.Icons.BodyShopCarbonFiberTrunk = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberTrunk", "VISUAL_PART_CARBON_FIBRE_TRUNK");
	DefaultCarConfig.Icons.BodyShopCarbonFiberRevertTrunk = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberRevertTrunk", "VISUAL_PART_DE_CARBON_FIBRE_TRUNK");
	DefaultCarConfig.Icons.BodyShopCarbonFiberWidebody = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberWidebody", "VISUAL_PART_CARBON_FIBRE_KIT");
	DefaultCarConfig.Icons.BodyShopCarbonFiberRevertWidebody = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopCarbonFiberRevertWidebody", "VISUAL_PART_DE_CARBON_FIBRE_KIT");
	DefaultCarConfig.Icons.BodyShopWideBodyKits = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopWideBodyKits", "VISUAL_PART_WIDE_BODY_KIT");
	DefaultCarConfig.Icons.BodyShopAttachment0 = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopAttachment0", "VISUAL_PART_ATTACHMENT1");
	DefaultCarConfig.Icons.BodyShopAttachment1 = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopAttachment1", "VISUAL_PART_ATTACHMENT2");
	DefaultCarConfig.Icons.BodyShopAttachment2 = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopAttachment2", "VISUAL_PART_ATTACHMENT3");
	DefaultCarConfig.Icons.BodyShopAttachment3 = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopAttachment3", "VISUAL_PART_ATTACHMENT4");
	DefaultCarConfig.Icons.BodyShopAttachment4 = mINI_ReadHashS(GeneralINI, "Icons", "BodyShopAttachment4", "VISUAL_PART_ATTACHMENT5");
	DefaultCarConfig.Icons.PerformanceEngine = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceEngine", "PERFORMANCE_ENGINE");
	DefaultCarConfig.Icons.PerformanceECU = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceECU", "PERFORMANCE_ECU");
	DefaultCarConfig.Icons.PerformanceTransmission = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTransmission", "PERFORMANCE_TRANSMISSION");
	DefaultCarConfig.Icons.PerformanceChassis = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceChassis", "PERFORMANCE_SUSPENSION");
	DefaultCarConfig.Icons.PerformanceNitrous = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceNitrous", "PERFORMANCE_NITROUS");
	DefaultCarConfig.Icons.PerformanceTires = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTires", "PERFORMANCE_TIRES");
	DefaultCarConfig.Icons.PerformanceBrakes = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceBrakes", "PERFORMANCE_BRAKES");
	DefaultCarConfig.Icons.PerformanceWeightReduction = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceWeightReduction", "PERFORMANCE_WEIGHT_REDUCTION");
	DefaultCarConfig.Icons.PerformanceTurbo = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTurbo", "PERFORMANCE_TURBO");
	DefaultCarConfig.Icons.PerformanceSupercharger = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceSupercharger", "PERFORMANCE_SUPERCHARGER");
	DefaultCarConfig.Icons.PerformanceAerodynamics = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceAerodynamics", "PERFORMANCE_AERODYNAMICS");
	DefaultCarConfig.Icons.PerformanceTestDyno = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTestDyno", "CRIB_ICON_TEST_TRACK");
	DefaultCarConfig.Icons.PaintPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintPaint", "VISUAL_PART_PAINT");
	DefaultCarConfig.Icons.PaintBasePaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintBasePaint", "PAINT_MOD_BASE");
	DefaultCarConfig.Icons.PaintSpoilerPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintSpoilerPaint", "PAINT_MOD_PART_SPOILER");
	DefaultCarConfig.Icons.PaintRoofScoopPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintRoofScoopPaint", "PAINT_MOD_PART_ROOF");
	DefaultCarConfig.Icons.PaintMirrorPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintMirrorPaint", "PAINT_MOD_PART_MIRROR");
	DefaultCarConfig.Icons.PaintRimPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintRimPaint", "PAINT_MOD_PART_RIMS");
	DefaultCarConfig.Icons.PaintSpinnerPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintSpinnerPaint", "PAINT_MOD_PART_INNER_RIM");
	DefaultCarConfig.Icons.PaintBrakePaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintBrakePaint", "PAINT_MOD_PART_BRAKE_CALIPERS");
	DefaultCarConfig.Icons.PaintEnginePaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintEnginePaint", "PAINT_MOD_PART_ENGINE_ACCENT");
	DefaultCarConfig.Icons.PaintTrunkPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintTrunkPaint", "PAINT_MOD_PART_TRUNK_AUDIO");
	DefaultCarConfig.Icons.PaintExhaustPaint = mINI_ReadHashS(GeneralINI, "Icons", "PaintExhaustPaint", "PAINT_MOD_PART_MUFFLER_TIP");
	DefaultCarConfig.Icons.PaintVinyls = mINI_ReadHashS(GeneralINI, "Icons", "PaintVinyls", "VISUAL_PART_VINYL");
	DefaultCarConfig.Icons.PaintVinylsCustom = mINI_ReadHashS(GeneralINI, "Icons", "PaintVinylsCustom", "VINYL_GROUP_CUSTOM");
	DefaultCarConfig.Icons.PaintDecals = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecals", "VISUAL_PART_DECALS");
	DefaultCarConfig.Icons.PaintDecalsWindshield = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield1", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield2", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield3", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield4", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield5", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield6", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield7", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsWindshield8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsWindshield8", "DECAL_ZONE_WINDSHIELD");
	DefaultCarConfig.Icons.PaintDecalsRearWindow = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow1", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow2", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow3", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow4", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow5", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow6", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow7", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsRearWindow8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRearWindow8", "DECAL_ZONE_REARWINDOW");
	DefaultCarConfig.Icons.PaintDecalsHood = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHood", "DECAL_ZONE_HOOD");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1", "DECAL_HOOD_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2", "DECAL_HOOD_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot1", "DECAL_HOOD_L1_SLOT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot2", "DECAL_HOOD_L1_SLOT2");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot3", "DECAL_HOOD_L1_SLOT3");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot4", "DECAL_HOOD_L1_SLOT4");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot5", "DECAL_HOOD_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot6", "DECAL_HOOD_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot7", "DECAL_HOOD_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout1Slot8", "DECAL_HOOD_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot1", "DECAL_HOOD_L2_SLOT1");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot2", "DECAL_HOOD_L2_SLOT2");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot3", "DECAL_HOOD_L2_SLOT3");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot4", "DECAL_HOOD_L2_SLOT4");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot5", "DECAL_HOOD_L2_SLOT5");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot6", "DECAL_HOOD_L2_SLOT6");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot7", "DECAL_HOOD_L2_SLOT7");
	DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsHoodLayout2Slot8", "DECAL_HOOD_L2_SLOT8");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor", "DECAL_ZONE_LEFTDOOR");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor1", "DECAL_LEFTDOOR_SLOT1");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor2", "DECAL_LEFTDOOR_SLOT2");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor3", "DECAL_LEFTDOOR_SLOT3");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor4", "DECAL_LEFTDOOR_SLOT4");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor5", "DECAL_LEFTDOOR_SLOT5");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor6", "DECAL_LEFTDOOR_SLOT6");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor7", "DECAL_LEFT_DOOR_LAYOUT");
	DefaultCarConfig.Icons.PaintDecalsLeftDoor8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftDoor8", "DECAL_LEFT_DOOR_LAYOUT");
	DefaultCarConfig.Icons.PaintDecalsRightDoor = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor", "DECAL_ZONE_RIGHTDOOR");
	DefaultCarConfig.Icons.PaintDecalsRightDoor1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor1", "DECAL_RIGHTDOOR_SLOT1");
	DefaultCarConfig.Icons.PaintDecalsRightDoor2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor2", "DECAL_RIGHTDOOR_SLOT2");
	DefaultCarConfig.Icons.PaintDecalsRightDoor3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor3", "DECAL_RIGHTDOOR_SLOT3");
	DefaultCarConfig.Icons.PaintDecalsRightDoor4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor4", "DECAL_RIGHTDOOR_SLOT4");
	DefaultCarConfig.Icons.PaintDecalsRightDoor5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor5", "DECAL_RIGHTDOOR_SLOT5");
	DefaultCarConfig.Icons.PaintDecalsRightDoor6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor6", "DECAL_RIGHTDOOR_SLOT6");
	DefaultCarConfig.Icons.PaintDecalsRightDoor7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor7", "DECAL_RIGHT_DOOR_LAYOUT");
	DefaultCarConfig.Icons.PaintDecalsRightDoor8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightDoor8", "DECAL_RIGHT_DOOR_LAYOUT");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarter = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarter", "DECAL_ZONE_LEFTPANEL");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2", "DECAL_LREAR_QP_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot1", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot2", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot3", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot4", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot5", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot6", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot7", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot8", "DECAL_LREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot1", "DECAL_LREAR_QP_L1_SLOT1");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot2", "DECAL_LREAR_QP_L1_SLOT2");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot3", "DECAL_LREAR_QP_L1_SLOT3");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot4", "DECAL_LREAR_QP_L1_SLOT4");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot5", "DECAL_LREAR_QP_L1_SLOT5");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot6", "DECAL_LREAR_QP_L1_SLOT6");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot7", "DECAL_LREAR_QP_L1_SLOT7");
	DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot8", "DECAL_LREAR_QP_L1_SLOT8");
	DefaultCarConfig.Icons.PaintDecalsRightQuarter = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarter", "DECAL_ZONE_RIGHTPANEL");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2", "DECAL_RREAR_QP_LAYOUT1");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot1", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot2", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot3", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot4", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot5", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot6", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot7", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout1Slot8", "DECAL_RREAR_QP_LAYOUT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot1 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot1", "DECAL_RREAR_QP_L1_SLOT1");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot2 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot2", "DECAL_RREAR_QP_L1_SLOT2");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot3 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot3", "DECAL_RREAR_QP_L1_SLOT3");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot4 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot4", "DECAL_RREAR_QP_L1_SLOT4");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot5 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot5", "DECAL_RREAR_QP_L1_SLOT5");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot6 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot6", "DECAL_RREAR_QP_L1_SLOT6");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot7 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot7", "DECAL_RREAR_QP_L1_SLOT7");
	DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot8 = mINI_ReadHashS(GeneralINI, "Icons", "PaintDecalsRightQuarterLayout2Slot8", "DECAL_RREAR_QP_L1_SLOT8");
	DefaultCarConfig.Icons.PaintInstallSetup = mINI_ReadHashS(GeneralINI, "Icons", "PaintInstallSetup", "GENERIC_ICON_PURCHASE");
	DefaultCarConfig.Icons.PaintPurchaseSetup = mINI_ReadHashS(GeneralINI, "Icons", "PaintPurchaseSetup", "GENERIC_ICON_PURCHASE");
	DefaultCarConfig.Icons.SpecialtiesCustomGauges = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesCustomGauges", "VISUAL_PART_CUSTOM_HUDS");
	DefaultCarConfig.Icons.SpecialtiesNeon = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesNeon", "VISUAL_PART_NEON");
	DefaultCarConfig.Icons.SpecialtiesUnderglowNeon = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesUnderglowNeon", "NEON_ICON_BASE");
	DefaultCarConfig.Icons.SpecialtiesEngineNeon = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesEngineNeon", "NEON_ICON_ENGINE");
	DefaultCarConfig.Icons.SpecialtiesCabinNeon = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesCabinNeon", "NEON_ICON_CABIN");
	DefaultCarConfig.Icons.SpecialtiesCabinNeonLayout = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesCabinNeonLayout", "NEON_ICON_CABIN_CONFIGURE");
	DefaultCarConfig.Icons.SpecialtiesTrunkNeon = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkNeon", "NEON_ICON_TRUNK");
	DefaultCarConfig.Icons.SpecialtiesWindowTint = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesWindowTint", "VISUAL_PART_WINDOW_TINTING");
	DefaultCarConfig.Icons.SpecialtiesHeadlightColor = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesHeadlightColor", "VISUAL_PART_HEAD_LIGHT_COLOR");
	DefaultCarConfig.Icons.SpecialtiesNosPurge = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesNosPurge", "VISUAL_PART_NOS_PURGE");
	DefaultCarConfig.Icons.SpecialtiesHydraulics = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesHydraulics", "VISUAL_PART_HYDRAULICS");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudio = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudio", "VISUAL_PART_TRUNK_AUDIO");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioLayout = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioLayout", "AUDIO_ICON_CONFIGURE_TRUNK_LAYOUT");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioComponents = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioComponents", "AUDIO_ICON_COMPONENTS");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot1 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot1", "AUDIO_ICON_SLOT1");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot2 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot2", "AUDIO_ICON_SLOT2");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot3 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot3", "AUDIO_ICON_SLOT3");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot4 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot4", "AUDIO_ICON_SLOT4");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot5 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot5", "AUDIO_ICON_SLOT5");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot6 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot6", "AUDIO_ICON_SLOT6");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot7 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot7", "AUDIO_ICON_SLOT7");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot8 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot8", "AUDIO_ICON_SLOT8");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot9 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot9", "AUDIO_ICON_SLOT9");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot10 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot10", "AUDIO_ICON_SLOT10");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot11 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot11", "AUDIO_ICON_SLOT11");
	DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot12 = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesTrunkAudioSlot12", "AUDIO_ICON_SLOT12");
	DefaultCarConfig.Icons.SpecialtiesSpinners = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesSpinners", "VISUAL_PART_SPINNERS");
	DefaultCarConfig.Icons.SpecialtiesSplitHoods = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesSplitHoods", "VISUAL_PART_SPLIT_HOODS");
	DefaultCarConfig.Icons.SpecialtiesDoors = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesDoors", "VISUAL_PART_SCISSOR_DOORS");
	DefaultCarConfig.Icons.SpecialtiesLicensePlate = mINI_ReadHashS(GeneralINI, "Icons", "SpecialtiesLicensePlate", "VISUAL_PART_LICENSE_PLATES");
	DefaultCarConfig.Icons.PerformanceEngineLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceEngineLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceEngineLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceEngineLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceEngineLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceEngineLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceECULevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceECULevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceECULevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceECULevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceECULevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceECULevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceTransmissionLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTransmissionLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceTransmissionLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTransmissionLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceTransmissionLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTransmissionLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceChassisLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceChassisLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceChassisLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceChassisLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceChassisLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceChassisLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceNitrousLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceNitrousLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceNitrousLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceNitrousLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceNitrousLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceNitrousLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceTiresLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTiresLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceTiresLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTiresLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceTiresLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTiresLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceBrakesLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceBrakesLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceBrakesLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceBrakesLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceBrakesLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceBrakesLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceWeightReductionLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceWeightReductionLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceWeightReductionLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceWeightReductionLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceWeightReductionLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceWeightReductionLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceTurboLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTurboLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceTurboLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTurboLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceTurboLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceTurboLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceSuperchargerLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceSuperchargerLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceSuperchargerLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceSuperchargerLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceSuperchargerLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceSuperchargerLevel3", "PERFORMANCE_ICON_PACKAGE_3");
	DefaultCarConfig.Icons.PerformanceAerodynamicsLevel1 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceAerodynamicsLevel1", "PERFORMANCE_ICON_PACKAGE_1");
	DefaultCarConfig.Icons.PerformanceAerodynamicsLevel2 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceAerodynamicsLevel2", "PERFORMANCE_ICON_PACKAGE_2");
	DefaultCarConfig.Icons.PerformanceAerodynamicsLevel3 = mINI_ReadHashS(GeneralINI, "Icons", "PerformanceAerodynamicsLevel3", "PERFORMANCE_ICON_PACKAGE_3");

	// Names
	DefaultCarConfig.Names.CategoryBodyShop = mINI_ReadHashS(GeneralINI, "Names", "CategoryBodyShop", "CUST_CATEGORY_BODY");
	DefaultCarConfig.Names.CategoryPerformance = mINI_ReadHashS(GeneralINI, "Names", "CategoryPerformance", "CUST_CATEGORY_PERFORMANCE");
	DefaultCarConfig.Names.CategoryPaint = mINI_ReadHashS(GeneralINI, "Names", "CategoryPaint", "CUST_CATEGORY_PAINT_VINYL");
	DefaultCarConfig.Names.CategorySpecialties = mINI_ReadHashS(GeneralINI, "Names", "CategorySpecialties", "CUST_CATEGORY_INTERIORS");
	DefaultCarConfig.Names.BodyShop = mINI_ReadHashS(GeneralINI, "Names", "BodyShop", "CUSTOMIZE_TITLE_BODY_SHOP");
	DefaultCarConfig.Names.Performance = mINI_ReadHashS(GeneralINI, "Names", "Performance", "PERFORMANCE_SHOP_TITLE");
	DefaultCarConfig.Names.Paint = mINI_ReadHashS(GeneralINI, "Names", "Paint", "CUSTOMIZE_TITLE_PAINT_SHOP");
	DefaultCarConfig.Names.Specialties = mINI_ReadHashS(GeneralINI, "Names", "Specialties", "CUSTOMIZE_TITLE_INTERIORS_SHOP");
	DefaultCarConfig.Names.BodyShopFrontBumper = mINI_ReadHashS(GeneralINI, "Names", "BodyShopFrontBumper", "VISUAL_PART_FRONT_BUMPER");
	DefaultCarConfig.Names.BodyShopRearBumper = mINI_ReadHashS(GeneralINI, "Names", "BodyShopRearBumper", "VISUAL_PART_REAR_BUMPER");
	DefaultCarConfig.Names.BodyShopSkirt = mINI_ReadHashS(GeneralINI, "Names", "BodyShopSkirt", "VISUAL_PART_SIDE_SKIRT");
	DefaultCarConfig.Names.BodyShopFender = mINI_ReadHashS(GeneralINI, "Names", "BodyShopFender", "VISUAL_PART_FRONT_FENDER");
	DefaultCarConfig.Names.BodyShopQuarter = mINI_ReadHashS(GeneralINI, "Names", "BodyShopQuarter", "VISUAL_PART_REAR_QUARTER");
	DefaultCarConfig.Names.BodyShopSpoiler = mINI_ReadHashS(GeneralINI, "Names", "BodyShopSpoiler", "VISUAL_PART_SPOILER");
	DefaultCarConfig.Names.BodyShopHood = mINI_ReadHashS(GeneralINI, "Names", "BodyShopHood", "VISUAL_PART_HOOD");
	DefaultCarConfig.Names.BodyShopEngine = mINI_ReadHashS(GeneralINI, "Names", "BodyShopEngine", "VISUAL_PART_ENGINE");
	DefaultCarConfig.Names.BodyShopTrunk = mINI_ReadHashS(GeneralINI, "Names", "BodyShopTrunk", "VISUAL_PART_TRUNK");
	DefaultCarConfig.Names.BodyShopRoofScoops = mINI_ReadHashS(GeneralINI, "Names", "BodyShopRoofScoops", "VISUAL_PART_ROOF_SCOOP");
	DefaultCarConfig.Names.BodyShopInterior = mINI_ReadHashS(GeneralINI, "Names", "BodyShopInterior", "VISUAL_PART_CABIN");
	DefaultCarConfig.Names.BodyShopRoof = mINI_ReadHashS(GeneralINI, "Names", "BodyShopRoof", "VISUAL_PART_ROOF");
	DefaultCarConfig.Names.BodyShopHeadlights = mINI_ReadHashS(GeneralINI, "Names", "BodyShopHeadlights", "VISUAL_PART_HEAD_LIGHT");
	DefaultCarConfig.Names.BodyShopTaillights = mINI_ReadHashS(GeneralINI, "Names", "BodyShopTaillights", "VISUAL_PART_TAIL_LIGHT");
	DefaultCarConfig.Names.BodyShopMirrors = mINI_ReadHashS(GeneralINI, "Names", "BodyShopMirrors", "VISUAL_PART_SIDE_MIRROR");
	DefaultCarConfig.Names.BodyShopExhaust = mINI_ReadHashS(GeneralINI, "Names", "BodyShopExhaust", "VISUAL_PART_MUFFLER_TIP");
	DefaultCarConfig.Names.BodyShopRims = mINI_ReadHashS(GeneralINI, "Names", "BodyShopRims", "VISUAL_PART_RIMS");
	DefaultCarConfig.Names.BodyShopRimsCustom = mINI_ReadHashS(GeneralINI, "Names", "BodyShopRimsCustom", "RIMS_BRAND_CUSTOM");
	DefaultCarConfig.Names.BodyShopBrakes = mINI_ReadHashS(GeneralINI, "Names", "BodyShopBrakes", "VISUAL_PART_BRAKES");
	DefaultCarConfig.Names.BodyShopCarbonFiber = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiber", "VISUAL_PART_CARBON_FIBRE");
	DefaultCarConfig.Names.BodyShopCarbonFiberHood = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberHood", "VISUAL_PART_CARBON_FIBRE_HOODS");
	DefaultCarConfig.Names.BodyShopCarbonFiberSpoiler = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberSpoiler", "VISUAL_PART_CARBON_FIBRE_SPOILERS");
	DefaultCarConfig.Names.BodyShopCarbonFiberRoofScoop = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberRoofScoop", "VISUAL_PART_CARBON_FIBRE_ROOF_SCOOP");
	DefaultCarConfig.Names.BodyShopCarbonFiberMirrors = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberMirrors", "VISUAL_PART_CARBON_FIBRE_MIRRORS");
	DefaultCarConfig.Names.BodyShopCarbonFiberDoor = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberDoor", "VISUAL_PART_CARBON_FIBRE_DOOR");
	DefaultCarConfig.Names.BodyShopCarbonFiberRevertDoor = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberRevertDoor", "VISUAL_PART_REVERT_CARBON_FIBRE_DOOR");
	DefaultCarConfig.Names.BodyShopCarbonFiberTrunk = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberTrunk", "VISUAL_PART_CARBON_FIBRE_TRUNK");
	DefaultCarConfig.Names.BodyShopCarbonFiberRevertTrunk = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberRevertTrunk", "VISUAL_PART_REVERT_CARBON_FIBRE_TRUNK");
	DefaultCarConfig.Names.BodyShopCarbonFiberWidebody = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberWidebody", "VISUAL_PART_CARBON_FIBRE_KITS");
	DefaultCarConfig.Names.BodyShopCarbonFiberRevertWidebody = mINI_ReadHashS(GeneralINI, "Names", "BodyShopCarbonFiberRevertWidebody", "VISUAL_PART_REVERT_CARBON_FIBRE_KITS");
	DefaultCarConfig.Names.BodyShopWideBodyKits = mINI_ReadHashS(GeneralINI, "Names", "BodyShopWideBodyKits", "VISUAL_PART_WIDE_BODY_KIT");
	DefaultCarConfig.Names.BodyShopAttachment0 = mINI_ReadHashS(GeneralINI, "Names", "BodyShopAttachment0", "VISUAL_PART_ATTACHMENT1");
	DefaultCarConfig.Names.BodyShopAttachment1 = mINI_ReadHashS(GeneralINI, "Names", "BodyShopAttachment1", "VISUAL_PART_ATTACHMENT2");
	DefaultCarConfig.Names.BodyShopAttachment2 = mINI_ReadHashS(GeneralINI, "Names", "BodyShopAttachment2", "VISUAL_PART_ATTACHMENT3");
	DefaultCarConfig.Names.BodyShopAttachment3 = mINI_ReadHashS(GeneralINI, "Names", "BodyShopAttachment3", "VISUAL_PART_ATTACHMENT4");
	DefaultCarConfig.Names.BodyShopAttachment4 = mINI_ReadHashS(GeneralINI, "Names", "BodyShopAttachment4", "VISUAL_PART_ATTACHMENT5");
	DefaultCarConfig.Names.PerformanceEngine = mINI_ReadHashS(GeneralINI, "Names", "PerformanceEngine", "PERF_CATEGORY_ENGINE");
	DefaultCarConfig.Names.PerformanceECU = mINI_ReadHashS(GeneralINI, "Names", "PerformanceECU", "PERF_CATEGORY_ECU");
	DefaultCarConfig.Names.PerformanceTransmission = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTransmission", "PERF_CATEGORY_TRANSMISSION");
	DefaultCarConfig.Names.PerformanceChassis = mINI_ReadHashS(GeneralINI, "Names", "PerformanceChassis", "PERF_CATEGORY_SUSPENSION");
	DefaultCarConfig.Names.PerformanceNitrous = mINI_ReadHashS(GeneralINI, "Names", "PerformanceNitrous", "PERF_CATEGORY_NOS");
	DefaultCarConfig.Names.PerformanceTires = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTires", "PERF_CATEGORY_TIRES");
	DefaultCarConfig.Names.PerformanceBrakes = mINI_ReadHashS(GeneralINI, "Names", "PerformanceBrakes", "PERF_CATEGORY_BRAKE");
	DefaultCarConfig.Names.PerformanceWeightReduction = mINI_ReadHashS(GeneralINI, "Names", "PerformanceWeightReduction", "PERF_CATEGORY_WEIGHT_REDUCTION");
	DefaultCarConfig.Names.PerformanceTurbo = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTurbo", "PERF_CATEGORY_TURBO");
	DefaultCarConfig.Names.PerformanceSupercharger = mINI_ReadHashS(GeneralINI, "Names", "PerformanceSupercharger", "PERF_CATEGORY_SUPERCHARGER");
	DefaultCarConfig.Names.PerformanceAerodynamics = mINI_ReadHashS(GeneralINI, "Names", "PerformanceAerodynamics", "PERF_CATEGORY_AERODYNAMICS");
	DefaultCarConfig.Names.PerformanceTestDyno = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTestDyno", "UI_CAREER_LOAD_TEST_TRACK");
	DefaultCarConfig.Names.PaintPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintPaint", "PAINT_CATEGORY_PAINTS");
	DefaultCarConfig.Names.PaintPaintCustom = mINI_ReadHashS(GeneralINI, "Names", "PaintPaintCustom", "CUSTOM_PAINT");
	DefaultCarConfig.Names.PaintBasePaint = mINI_ReadHashS(GeneralINI, "Names", "PaintBasePaint", "PAINT_BASE");
	DefaultCarConfig.Names.PaintSpoilerPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintSpoilerPaint", "PAINT_SPOILER");
	DefaultCarConfig.Names.PaintRoofScoopPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintRoofScoopPaint", "PAINT_ROOF");
	DefaultCarConfig.Names.PaintMirrorPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintMirrorPaint", "PAINT_MIRROR");
	DefaultCarConfig.Names.PaintRimPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintRimPaint", "PAINT_RIM");
	DefaultCarConfig.Names.PaintSpinnerPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintSpinnerPaint", "PAINT_SPINNER");
	DefaultCarConfig.Names.PaintBrakePaint = mINI_ReadHashS(GeneralINI, "Names", "PaintBrakePaint", "PAINT_BRAKE");
	DefaultCarConfig.Names.PaintEnginePaint = mINI_ReadHashS(GeneralINI, "Names", "PaintEnginePaint", "PAINT_ENGINE");
	DefaultCarConfig.Names.PaintTrunkPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintTrunkPaint", "PAINT_AUDIO");
	DefaultCarConfig.Names.PaintExhaustPaint = mINI_ReadHashS(GeneralINI, "Names", "PaintExhaustPaint", "PAINT_EXHAUST");
	DefaultCarConfig.Names.PaintVinyls = mINI_ReadHashS(GeneralINI, "Names", "PaintVinyls", "PAINT_CATEGORY_VINYLS");
	DefaultCarConfig.Names.PaintVinylsCustom = mINI_ReadHashS(GeneralINI, "Names", "PaintVinylsCustom", "VINYL_GROUP_CUSTOM");
	DefaultCarConfig.Names.PaintDecals = mINI_ReadHashS(GeneralINI, "Names", "PaintDecals", "PAINT_CATEGORY_DECALS");
	DefaultCarConfig.Names.PaintInstallSetup = mINI_ReadHashS(GeneralINI, "Names", "PaintInstallSetup", "PAINT_CATEGORY_INSTALL_SETUP");
	DefaultCarConfig.Names.PaintPurchaseSetup = mINI_ReadHashS(GeneralINI, "Names", "PaintPurchaseSetup", "PAINT_CATEGORY_PURCHASE_SETUP");
	DefaultCarConfig.Names.SpecialtiesCustomGauges = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesCustomGauges", "UI_ICE_GAUGES");
	DefaultCarConfig.Names.SpecialtiesNeon = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesNeon", "UI_ICE_NEON");
	DefaultCarConfig.Names.SpecialtiesUnderglowNeon = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesUnderglowNeon", "NEON_ICON_BASE");
	DefaultCarConfig.Names.SpecialtiesEngineNeon = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesEngineNeon", "NEON_ICON_ENGINE");
	DefaultCarConfig.Names.SpecialtiesCabinNeon = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesCabinNeon", "NEON_ICON_CABIN");
	DefaultCarConfig.Names.SpecialtiesCabinNeonLayout = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesCabinNeonLayout", "NEON_ICON_CABIN_CONFIGURE");
	DefaultCarConfig.Names.SpecialtiesTrunkNeon = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkNeon", "NEON_ICON_TRUNK");
	DefaultCarConfig.Names.SpecialtiesWindowTint = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesWindowTint", "UI_ICE_TINT");
	DefaultCarConfig.Names.SpecialtiesHeadlightColor = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesHeadlightColor", "UI_ICE_HEADLIGHTS");
	DefaultCarConfig.Names.SpecialtiesNosPurge = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesNosPurge", "UI_ICE_NOS_PURGE");
	DefaultCarConfig.Names.SpecialtiesHydraulics = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesHydraulics", "UI_ICE_HYDRAULICS");
	DefaultCarConfig.Names.SpecialtiesTrunkAudio = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudio", "UI_ICE_TRUNK_AUDIO");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioLayout = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioLayout", "UI_ICE_TRUNK_AUDIO_LAYOUT_SELECTION");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioComponents = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioComponents", "UI_ICE_TRUNK_AUDIO_COMPONENTS");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot1 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot1", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot2 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot2", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot3 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot3", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot4 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot4", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot5 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot5", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot6 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot6", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot7 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot7", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot8 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot8", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot9 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot9", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot10 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot10", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot11 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot11", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot12 = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesTrunkAudioSlot12", "UI_ICE_TRUNK_AUDIO_COMPONENT");
	DefaultCarConfig.Names.SpecialtiesSpinners = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesSpinners", "UI_ICE_SPINNERS");
	DefaultCarConfig.Names.SpecialtiesSplitHoods = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesSplitHoods", "UI_ICE_HOOD");
	DefaultCarConfig.Names.SpecialtiesDoors = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesDoors", "UI_ICE_DOORS");
	DefaultCarConfig.Names.SpecialtiesLicensePlate = mINI_ReadHashS(GeneralINI, "Names", "SpecialtiesLicensePlate", "UI_ICE_LICENSE_PLATES");
	DefaultCarConfig.Names.PerformanceEngineLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceEngineLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceEngineLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceEngineLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceEngineLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceEngineLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceECULevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceECULevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceECULevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceECULevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceECULevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceECULevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceTransmissionLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTransmissionLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceTransmissionLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTransmissionLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceTransmissionLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTransmissionLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceChassisLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceChassisLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceChassisLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceChassisLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceChassisLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceChassisLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceNitrousLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceNitrousLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceNitrousLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceNitrousLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceNitrousLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceNitrousLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceTiresLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTiresLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceTiresLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTiresLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceTiresLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTiresLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceBrakesLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceBrakesLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceBrakesLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceBrakesLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceBrakesLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceBrakesLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceWeightReductionLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceWeightReductionLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceWeightReductionLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceWeightReductionLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceWeightReductionLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceWeightReductionLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceTurboLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTurboLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceTurboLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTurboLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceTurboLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceTurboLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceSuperchargerLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceSuperchargerLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceSuperchargerLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceSuperchargerLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceSuperchargerLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceSuperchargerLevel3", "PERFORMANCE_PACKAGE_3");
	DefaultCarConfig.Names.PerformanceAerodynamicsLevel1 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceAerodynamicsLevel1", "PERFORMANCE_PACKAGE_1");
	DefaultCarConfig.Names.PerformanceAerodynamicsLevel2 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceAerodynamicsLevel2", "PERFORMANCE_PACKAGE_2");
	DefaultCarConfig.Names.PerformanceAerodynamicsLevel3 = mINI_ReadHashS(GeneralINI, "Names", "PerformanceAerodynamicsLevel3", "PERFORMANCE_PACKAGE_3");

	// PerformanceParts
	for (int p = 0; p < DefaultPerformancePartCount; p++)
	{
		DefaultCarConfig.PerformanceParts.ReplacementHashes[p] = mINI_ReadHash(GeneralINI, "PerformanceParts", DefaultPerformancePartNames[p], DefaultPerformancePartHashes[p]);
	}

	// Read other cars' configs
	for (int i = 0; i < CarCount; i++)
	{
		ACarConfig.CarTypeInfo = (DWORD*)((*(DWORD*)_CarTypeInfoArray) + i * SingleCarTypeInfoBlockSize);
		sprintf(CarININame, "%s.ini", (char*)ACarConfig.CarTypeInfo);
		auto CarINIPath = CurrentWorkingDirectory / "UnlimiterData" / CarININame;
		mINI::INIFile CarINIFile(CarINIPath.string());
		mINI::INIStructure CarINI;
		CarINIFile.read(CarINI);

		// Main
		ACarConfig.Main.ForceLODA = mINI_ReadInteger(CarINI, "Main", "ForceLODA", DefaultCarConfig.Main.ForceLODA) != 0;
		ACarConfig.Main.InitiallyUnlocked = mINI_ReadInteger(CarINI, "Main", "InitiallyUnlocked", DefaultCarConfig.Main.InitiallyUnlocked);
		ACarConfig.Main.GameType = mINI_ReadInteger(CarINI, "Main", "GameType", GetDefaultGameType(i));
		ACarConfig.Main.InductionType = mINI_ReadInteger(CarINI, "Main", "InductionType", DefaultCarConfig.Main.InductionType);
		ACarConfig.Main.ScaleBrakesWithRims = mINI_ReadInteger(CarINI, "Main", "ScaleBrakesWithRims", DefaultCarConfig.Main.ScaleBrakesWithRims);
		ACarConfig.Main.SyncVisualPartsWithPhysics = mINI_ReadInteger(CarINI, "Main", "SyncVisualPartsWithPhysics", DefaultCarConfig.Main.SyncVisualPartsWithPhysics);
		ACarConfig.Main.CanBeDrivenByAI = mINI_ReadInteger(CarINI, "Main", "CanBeDrivenByAI", DefaultCarConfig.Main.CanBeDrivenByAI) != 0;

		// CarRenderInfo
		ACarConfig.RenderInfo.LinkLicensePlateToTrunk = mINI_ReadInteger(CarINI, "CarRenderInfo", "LinkLicensePlateToTrunk", GetDefaultLinkLicensePlateToTrunk(i)) != 0;
		ACarConfig.RenderInfo.ShowTrunkUnderInFE = mINI_ReadInteger(CarINI, "CarRenderInfo", "ShowTrunkUnderInFE", GetDefaultShowTrunkUnderInFE(i)) != 0;
		ACarConfig.RenderInfo.RemoveCentreBrakeWithCustomSpoiler = mINI_ReadInteger(CarINI, "CarRenderInfo", "RemoveCentreBrakeWithCustomSpoiler", GetDefaultRemoveCentreBrakeWithCustomSpoiler(i)) != 0;
		ACarConfig.RenderInfo.HasSunroof = mINI_ReadInteger(CarINI, "CarRenderInfo", "HasSunroof", GetDefaultHasSunroof(i)) != 0;

		// Stats
		ACarConfig.Stats.TimingKludgeFactor060 = mINI_ReadFloat(CarINI, "Stats", "TimingKludgeFactor060", DefaultCarConfig.Stats.TimingKludgeFactor060);
		ACarConfig.Stats.TimingKludgeFactor0100 = mINI_ReadFloat(CarINI, "Stats", "TimingKludgeFactor0100", DefaultCarConfig.Stats.TimingKludgeFactor0100);

		// StarGazer
		ACarConfig.StarGazer.StartingRep = mINI_ReadInteger(CarINI, "StarGazer", "StartingRep", DefaultCarConfig.StarGazer.StartingRep);

		// Textures
		ACarConfig.Textures.HeadlightOn = mINI_ReadInteger(CarINI, "Textures", "HeadlightOn", DefaultCarConfig.Textures.HeadlightOn) != 0;
		ACarConfig.Textures.BrakelightOn = mINI_ReadInteger(CarINI, "Textures", "BrakelightOn", DefaultCarConfig.Textures.BrakelightOn) != 0;
		ACarConfig.Textures.CentreBrakelightOn = mINI_ReadInteger(CarINI, "Textures", "CentreBrakelightOn", DefaultCarConfig.Textures.CentreBrakelightOn) != 0;
		ACarConfig.Textures.ReverseOn = mINI_ReadInteger(CarINI, "Textures", "ReverseOn", DefaultCarConfig.Textures.ReverseOn) != 0;
		ACarConfig.Textures.DamageLights = mINI_ReadInteger(CarINI, "Textures", "DamageLights", DefaultCarConfig.Textures.DamageLights) != 0;
		ACarConfig.Textures.CentreBrake = mINI_ReadInteger(CarINI, "Textures", "CentreBrake", DefaultCarConfig.Textures.CentreBrake) != 0;
		ACarConfig.Textures.Reverse = mINI_ReadInteger(CarINI, "Textures", "Reverse", DefaultCarConfig.Textures.Reverse) != 0;
		ACarConfig.Textures.BrakelightOnfInGame = mINI_ReadInteger(CarINI, "Textures", "BrakelightOnfInGame", DefaultCarConfig.Textures.BrakelightOnfInGame) != 0;
		ACarConfig.Textures.TireInnerMask = mINI_ReadInteger(CarINI, "Textures", "TireInnerMask", DefaultCarConfig.Textures.TireInnerMask) != 0;

		// Category
		ACarConfig.Category.BodyShop = mINI_ReadInteger(CarINI, "Category", "BodyShop", DefaultCarConfig.Category.BodyShop) != 0;
		ACarConfig.Category.Performance = mINI_ReadInteger(CarINI, "Category", "Performance", DefaultCarConfig.Category.Performance) != 0;
		ACarConfig.Category.Paint = mINI_ReadInteger(CarINI, "Category", "Paint", DefaultCarConfig.Category.Paint) != 0;
		ACarConfig.Category.Specialties = mINI_ReadInteger(CarINI, "Category", "Specialties", DefaultCarConfig.Category.Specialties) != 0;

		// BodyShop
		ACarConfig.BodyShop.FrontBumper = mINI_ReadInteger(CarINI, "BodyShop", "FrontBumper", DefaultCarConfig.BodyShop.FrontBumper) != 0;
		ACarConfig.BodyShop.RearBumper = mINI_ReadInteger(CarINI, "BodyShop", "RearBumper", DefaultCarConfig.BodyShop.RearBumper) != 0;
		ACarConfig.BodyShop.Skirt = mINI_ReadInteger(CarINI, "BodyShop", "Skirt", DefaultCarConfig.BodyShop.Skirt) != 0;
		ACarConfig.BodyShop.Fender = mINI_ReadInteger(CarINI, "BodyShop", "Fender", DefaultCarConfig.BodyShop.Fender) != 0;
		ACarConfig.BodyShop.Quarter = mINI_ReadInteger(CarINI, "BodyShop", "Quarter", DefaultCarConfig.BodyShop.Quarter) != 0;
		ACarConfig.BodyShop.Spoiler = mINI_ReadInteger(CarINI, "BodyShop", "Spoiler", DefaultCarConfig.BodyShop.Spoiler) != 0;
		ACarConfig.BodyShop.Hood = mINI_ReadInteger(CarINI, "BodyShop", "Hood", DefaultCarConfig.BodyShop.Hood) != 0;
		ACarConfig.BodyShop.Engine = mINI_ReadInteger(CarINI, "BodyShop", "Engine", DefaultCarConfig.BodyShop.Engine) != 0;
		ACarConfig.BodyShop.Trunk = mINI_ReadInteger(CarINI, "BodyShop", "Trunk", DefaultCarConfig.BodyShop.Trunk) != 0;
		ACarConfig.BodyShop.RoofScoops = mINI_ReadInteger(CarINI, "BodyShop", "RoofScoops", DefaultCarConfig.BodyShop.RoofScoops) != 0;
		ACarConfig.BodyShop.Interior = mINI_ReadInteger(CarINI, "BodyShop", "Interior", DefaultCarConfig.BodyShop.Interior) != 0;
		ACarConfig.BodyShop.Roof = mINI_ReadInteger(CarINI, "BodyShop", "Roof", DefaultCarConfig.BodyShop.Roof) != 0;
		ACarConfig.BodyShop.Brakes = mINI_ReadInteger(CarINI, "BodyShop", "Brakes", DefaultCarConfig.BodyShop.Brakes) != 0;
		ACarConfig.BodyShop.Headlights = mINI_ReadInteger(CarINI, "BodyShop", "Headlights", DefaultCarConfig.BodyShop.Headlights) != 0;
		ACarConfig.BodyShop.Taillights = mINI_ReadInteger(CarINI, "BodyShop", "Taillights", DefaultCarConfig.BodyShop.Taillights) != 0;
		ACarConfig.BodyShop.Mirrors = mINI_ReadInteger(CarINI, "BodyShop", "Mirrors", DefaultCarConfig.BodyShop.Mirrors) != 0;
		ACarConfig.BodyShop.Exhaust = mINI_ReadInteger(CarINI, "BodyShop", "Exhaust", DefaultCarConfig.BodyShop.Exhaust) != 0;
		ACarConfig.BodyShop.Rims = mINI_ReadInteger(CarINI, "BodyShop", "Rims", DefaultCarConfig.BodyShop.Rims) != 0;
		ACarConfig.BodyShop.RimsCustom = mINI_ReadInteger(CarINI, "BodyShop", "RimsCustom", DefaultCarConfig.BodyShop.RimsCustom) != 0;
		ACarConfig.BodyShop.CarbonFiber = mINI_ReadInteger(CarINI, "BodyShop", "CarbonFiber", DefaultCarConfig.BodyShop.CarbonFiber) != 0;
		ACarConfig.BodyShop.WideBodyKits = mINI_ReadInteger(CarINI, "BodyShop", "WideBodyKits", DefaultCarConfig.BodyShop.WideBodyKits) != 0;
		ACarConfig.BodyShop.Attachments = mINI_ReadInteger(CarINI, "BodyShop", "Attachments", DefaultCarConfig.BodyShop.Attachments) % 6;
		ACarConfig.BodyShop.Attachment0 = mINI_ReadInteger(CarINI, "BodyShop", "Attachment0", DefaultCarConfig.BodyShop.Attachment0) != 0;
		ACarConfig.BodyShop.Attachment1 = mINI_ReadInteger(CarINI, "BodyShop", "Attachment1", DefaultCarConfig.BodyShop.Attachment1) != 0;
		ACarConfig.BodyShop.Attachment2 = mINI_ReadInteger(CarINI, "BodyShop", "Attachment2", DefaultCarConfig.BodyShop.Attachment2) != 0;
		ACarConfig.BodyShop.Attachment3 = mINI_ReadInteger(CarINI, "BodyShop", "Attachment3", DefaultCarConfig.BodyShop.Attachment3) != 0;
		ACarConfig.BodyShop.Attachment4 = mINI_ReadInteger(CarINI, "BodyShop", "Attachment4", DefaultCarConfig.BodyShop.Attachment4) != 0;

		// Performance
		ACarConfig.Performance.Engine = mINI_ReadInteger(CarINI, "Performance", "Engine", DefaultCarConfig.Performance.Engine) != 0;
		ACarConfig.Performance.ECU = mINI_ReadInteger(CarINI, "Performance", "ECU", DefaultCarConfig.Performance.ECU) != 0;
		ACarConfig.Performance.Transmission = mINI_ReadInteger(CarINI, "Performance", "Transmission", DefaultCarConfig.Performance.Transmission) != 0;
		ACarConfig.Performance.Chassis = mINI_ReadInteger(CarINI, "Performance", "Chassis", DefaultCarConfig.Performance.Chassis) != 0;
		ACarConfig.Performance.Nitrous = mINI_ReadInteger(CarINI, "Performance", "Nitrous", DefaultCarConfig.Performance.Nitrous) != 0;
		ACarConfig.Performance.Tires = mINI_ReadInteger(CarINI, "Performance", "Tires", DefaultCarConfig.Performance.Tires) != 0;
		ACarConfig.Performance.Brakes = mINI_ReadInteger(CarINI, "Performance", "Brakes", DefaultCarConfig.Performance.Brakes) != 0;
		ACarConfig.Performance.WeightReduction = mINI_ReadInteger(CarINI, "Performance", "WeightReduction", DefaultCarConfig.Performance.WeightReduction) != 0;
		ACarConfig.Performance.Induction = mINI_ReadInteger(CarINI, "Performance", "Induction", DefaultCarConfig.Performance.Induction) != 0;
		ACarConfig.Performance.Aerodynamics = mINI_ReadInteger(CarINI, "Performance", "Aerodynamics", DefaultCarConfig.Performance.Aerodynamics) != 0;
		ACarConfig.Performance.TestDyno = mINI_ReadInteger(CarINI, "Performance", "TestDyno", DefaultCarConfig.Performance.TestDyno) != 0;

		// Paint
		ACarConfig.Paint.Paint = mINI_ReadInteger(CarINI, "Paint", "Paint", DefaultCarConfig.Paint.Paint) != 0;
		ACarConfig.Paint.PaintCustom = mINI_ReadInteger(CarINI, "Paint", "PaintCustom", DefaultCarConfig.Paint.PaintCustom) != 0;
		ACarConfig.Paint.BasePaint = mINI_ReadInteger(CarINI, "Paint", "BasePaint", DefaultCarConfig.Paint.BasePaint) != 0;
		ACarConfig.Paint.SpoilerPaint = mINI_ReadInteger(CarINI, "Paint", "SpoilerPaint", DefaultCarConfig.Paint.SpoilerPaint) != 0;
		ACarConfig.Paint.RoofScoopPaint = mINI_ReadInteger(CarINI, "Paint", "RoofScoopPaint", DefaultCarConfig.Paint.RoofScoopPaint) != 0;
		ACarConfig.Paint.MirrorPaint = mINI_ReadInteger(CarINI, "Paint", "MirrorPaint", DefaultCarConfig.Paint.MirrorPaint) != 0;
		ACarConfig.Paint.RimPaint = mINI_ReadInteger(CarINI, "Paint", "RimPaint", DefaultCarConfig.Paint.RimPaint) != 0;
		ACarConfig.Paint.SpinnerPaint = mINI_ReadInteger(CarINI, "Paint", "SpinnerPaint", DefaultCarConfig.Paint.SpinnerPaint) != 0;
		ACarConfig.Paint.BrakePaint = mINI_ReadInteger(CarINI, "Paint", "BrakePaint", DefaultCarConfig.Paint.BrakePaint) != 0;
		ACarConfig.Paint.EnginePaint = mINI_ReadInteger(CarINI, "Paint", "EnginePaint", DefaultCarConfig.Paint.EnginePaint) != 0;
		ACarConfig.Paint.TrunkPaint = mINI_ReadInteger(CarINI, "Paint", "TrunkPaint", DefaultCarConfig.Paint.TrunkPaint) != 0;
		ACarConfig.Paint.ExhaustPaint = mINI_ReadInteger(CarINI, "Paint", "ExhaustPaint", DefaultCarConfig.Paint.ExhaustPaint) != 0;
		ACarConfig.Paint.Vinyls = mINI_ReadInteger(CarINI, "Paint", "Vinyls", DefaultCarConfig.Paint.Vinyls) != 0;
		ACarConfig.Paint.VinylsCustom = mINI_ReadInteger(CarINI, "Paint", "VinylsCustom", DefaultCarConfig.Paint.VinylsCustom) != 0;
		ACarConfig.Paint.VinylLayers = mINI_ReadInteger(CarINI, "Paint", "VinylLayers", DefaultCarConfig.Paint.VinylLayers) % 5;
		ACarConfig.Paint.Decals = mINI_ReadInteger(CarINI, "Paint", "Decals", DefaultCarConfig.Paint.Decals) != 0;
		ACarConfig.Paint.DecalsWindshield = mINI_ReadInteger(CarINI, "Paint", "DecalsWindshield", DefaultCarConfig.Paint.DecalsWindshield) != 0;
		ACarConfig.Paint.DecalsRearWindow = mINI_ReadInteger(CarINI, "Paint", "DecalsRearWindow", DefaultCarConfig.Paint.DecalsRearWindow) != 0;
		ACarConfig.Paint.DecalsHood = mINI_ReadInteger(CarINI, "Paint", "DecalsHood", DefaultCarConfig.Paint.DecalsHood) != 0;
		ACarConfig.Paint.DecalsLeftDoor = mINI_ReadInteger(CarINI, "Paint", "DecalsLeftDoor", DefaultCarConfig.Paint.DecalsLeftDoor) != 0;
		ACarConfig.Paint.DecalsRightDoor = mINI_ReadInteger(CarINI, "Paint", "DecalsRightDoor", DefaultCarConfig.Paint.DecalsRightDoor) != 0;
		ACarConfig.Paint.DecalsLeftQuarter = mINI_ReadInteger(CarINI, "Paint", "DecalsLeftQuarter", DefaultCarConfig.Paint.DecalsLeftQuarter) != 0;
		ACarConfig.Paint.DecalsRightQuarter = mINI_ReadInteger(CarINI, "Paint", "DecalsRightQuarter", DefaultCarConfig.Paint.DecalsRightQuarter) != 0;

		// Specialties
		ACarConfig.Specialties.CustomGauges = mINI_ReadInteger(CarINI, "Specialties", "CustomGauges", DefaultCarConfig.Specialties.CustomGauges) != 0;
		ACarConfig.Specialties.Neon = mINI_ReadInteger(CarINI, "Specialties", "Neon", DefaultCarConfig.Specialties.Neon) != 0;
		ACarConfig.Specialties.UnderglowNeon = mINI_ReadInteger(CarINI, "Specialties", "UnderglowNeon", DefaultCarConfig.Specialties.UnderglowNeon) != 0;
		ACarConfig.Specialties.EngineNeon = mINI_ReadInteger(CarINI, "Specialties", "EngineNeon", DefaultCarConfig.Specialties.EngineNeon) != 0;
		ACarConfig.Specialties.CabinNeon = mINI_ReadInteger(CarINI, "Specialties", "CabinNeon", DefaultCarConfig.Specialties.CabinNeon) != 0;
		ACarConfig.Specialties.TrunkNeon = mINI_ReadInteger(CarINI, "Specialties", "TrunkNeon", DefaultCarConfig.Specialties.TrunkNeon) != 0;
		ACarConfig.Specialties.WindowTint = mINI_ReadInteger(CarINI, "Specialties", "WindowTint", DefaultCarConfig.Specialties.WindowTint) != 0;
		ACarConfig.Specialties.HeadlightColor = mINI_ReadInteger(CarINI, "Specialties", "HeadlightColor", DefaultCarConfig.Specialties.HeadlightColor) != 0;
		ACarConfig.Specialties.NosPurge = mINI_ReadInteger(CarINI, "Specialties", "NosPurge", DefaultCarConfig.Specialties.NosPurge) != 0;
		ACarConfig.Specialties.Hydrualics = mINI_ReadInteger(CarINI, "Specialties", "Hydrualics", DefaultCarConfig.Specialties.Hydrualics) != 0;
		ACarConfig.Specialties.TrunkAudio = mINI_ReadInteger(CarINI, "Specialties", "TrunkAudio", DefaultCarConfig.Specialties.TrunkAudio) != 0;
		ACarConfig.Specialties.Spinners = mINI_ReadInteger(CarINI, "Specialties", "Spinners", DefaultCarConfig.Specialties.Spinners) != 0;
		ACarConfig.Specialties.SplitHoods = mINI_ReadInteger(CarINI, "Specialties", "SplitHoods", DefaultCarConfig.Specialties.SplitHoods) != 0;
		ACarConfig.Specialties.Doors = mINI_ReadInteger(CarINI, "Specialties", "Doors", DefaultCarConfig.Specialties.Doors) != 0;
		ACarConfig.Specialties.LicensePlate = mINI_ReadInteger(CarINI, "Specialties", "LicensePlate", DefaultCarConfig.Specialties.LicensePlate) != 0;
		
		// Icons
		ACarConfig.Icons.CategoryBodyShop = mINI_ReadHash(CarINI, "Icons", "CategoryBodyShop", DefaultCarConfig.Icons.CategoryBodyShop);
		ACarConfig.Icons.CategoryPerformance = mINI_ReadHash(CarINI, "Icons", "CategoryPerformance", DefaultCarConfig.Icons.CategoryPerformance);
		ACarConfig.Icons.CategoryPaint = mINI_ReadHash(CarINI, "Icons", "CategoryPaint", DefaultCarConfig.Icons.CategoryPaint);
		ACarConfig.Icons.CategorySpecialties = mINI_ReadHash(CarINI, "Icons", "CategorySpecialties", DefaultCarConfig.Icons.CategorySpecialties);
		ACarConfig.Icons.BodyShop = mINI_ReadHash(CarINI, "Icons", "BodyShop", DefaultCarConfig.Icons.BodyShop);
		ACarConfig.Icons.Performance = mINI_ReadHash(CarINI, "Icons", "Performance", DefaultCarConfig.Icons.Performance);
		ACarConfig.Icons.Paint = mINI_ReadHash(CarINI, "Icons", "Paint", DefaultCarConfig.Icons.Paint);
		ACarConfig.Icons.Specialties = mINI_ReadHash(CarINI, "Icons", "Specialties", DefaultCarConfig.Icons.Specialties);
		ACarConfig.Icons.BodyShopFrontBumper = mINI_ReadHash(CarINI, "Icons", "BodyShopFrontBumper", DefaultCarConfig.Icons.BodyShopFrontBumper);
		ACarConfig.Icons.BodyShopRearBumper = mINI_ReadHash(CarINI, "Icons", "BodyShopRearBumper", DefaultCarConfig.Icons.BodyShopRearBumper);
		ACarConfig.Icons.BodyShopSkirt = mINI_ReadHash(CarINI, "Icons", "BodyShopSkirt", DefaultCarConfig.Icons.BodyShopSkirt);
		ACarConfig.Icons.BodyShopFender = mINI_ReadHash(CarINI, "Icons", "BodyShopFender", DefaultCarConfig.Icons.BodyShopFender);
		ACarConfig.Icons.BodyShopQuarter = mINI_ReadHash(CarINI, "Icons", "BodyShopQuarter", DefaultCarConfig.Icons.BodyShopQuarter);
		ACarConfig.Icons.BodyShopSpoiler = mINI_ReadHash(CarINI, "Icons", "BodyShopSpoiler", DefaultCarConfig.Icons.BodyShopSpoiler);
		ACarConfig.Icons.BodyShopHood = mINI_ReadHash(CarINI, "Icons", "BodyShopHood", DefaultCarConfig.Icons.BodyShopHood);
		ACarConfig.Icons.BodyShopEngine = mINI_ReadHash(CarINI, "Icons", "BodyShopEngine", DefaultCarConfig.Icons.BodyShopEngine);
		ACarConfig.Icons.BodyShopTrunk = mINI_ReadHash(CarINI, "Icons", "BodyShopTrunk", DefaultCarConfig.Icons.BodyShopTrunk);
		ACarConfig.Icons.BodyShopRoofScoops = mINI_ReadHash(CarINI, "Icons", "BodyShopRoofScoops", DefaultCarConfig.Icons.BodyShopRoofScoops);
		ACarConfig.Icons.BodyShopInterior = mINI_ReadHash(CarINI, "Icons", "BodyShopInterior", DefaultCarConfig.Icons.BodyShopInterior);
		ACarConfig.Icons.BodyShopRoof = mINI_ReadHash(CarINI, "Icons", "BodyShopRoof", DefaultCarConfig.Icons.BodyShopRoof);
		ACarConfig.Icons.BodyShopHeadlights = mINI_ReadHash(CarINI, "Icons", "BodyShopHeadlights", DefaultCarConfig.Icons.BodyShopHeadlights);
		ACarConfig.Icons.BodyShopTaillights = mINI_ReadHash(CarINI, "Icons", "BodyShopTaillights", DefaultCarConfig.Icons.BodyShopTaillights);
		ACarConfig.Icons.BodyShopMirrors = mINI_ReadHash(CarINI, "Icons", "BodyShopMirrors", DefaultCarConfig.Icons.BodyShopMirrors);
		ACarConfig.Icons.BodyShopExhaust = mINI_ReadHash(CarINI, "Icons", "BodyShopExhaust", DefaultCarConfig.Icons.BodyShopExhaust);
		ACarConfig.Icons.BodyShopRims = mINI_ReadHash(CarINI, "Icons", "BodyShopRims", DefaultCarConfig.Icons.BodyShopRims);
		ACarConfig.Icons.BodyShopRimsCustom = mINI_ReadHash(CarINI, "Icons", "BodyShopRimsCustom", DefaultCarConfig.Icons.BodyShopRimsCustom);
		ACarConfig.Icons.BodyShopBrakes = mINI_ReadHash(CarINI, "Icons", "BodyShopBrakes", DefaultCarConfig.Icons.BodyShopBrakes);
		ACarConfig.Icons.BodyShopCarbonFiber = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiber", DefaultCarConfig.Icons.BodyShopCarbonFiber);
		ACarConfig.Icons.BodyShopCarbonFiberHood = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberHood", DefaultCarConfig.Icons.BodyShopCarbonFiberHood);
		ACarConfig.Icons.BodyShopCarbonFiberSpoiler = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberSpoiler", DefaultCarConfig.Icons.BodyShopCarbonFiberSpoiler);
		ACarConfig.Icons.BodyShopCarbonFiberRoofScoop = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberRoofScoop", DefaultCarConfig.Icons.BodyShopCarbonFiberRoofScoop);
		ACarConfig.Icons.BodyShopCarbonFiberMirrors = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberMirrors", DefaultCarConfig.Icons.BodyShopCarbonFiberMirrors);
		ACarConfig.Icons.BodyShopCarbonFiberDoor = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberDoor", DefaultCarConfig.Icons.BodyShopCarbonFiberDoor);
		ACarConfig.Icons.BodyShopCarbonFiberRevertDoor = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberRevertDoor", DefaultCarConfig.Icons.BodyShopCarbonFiberRevertDoor);
		ACarConfig.Icons.BodyShopCarbonFiberTrunk = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberTrunk", DefaultCarConfig.Icons.BodyShopCarbonFiberTrunk);
		ACarConfig.Icons.BodyShopCarbonFiberRevertTrunk = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberRevertTrunk", DefaultCarConfig.Icons.BodyShopCarbonFiberRevertTrunk);
		ACarConfig.Icons.BodyShopCarbonFiberWidebody = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberWidebody", DefaultCarConfig.Icons.BodyShopCarbonFiberWidebody);
		ACarConfig.Icons.BodyShopCarbonFiberRevertWidebody = mINI_ReadHash(CarINI, "Icons", "BodyShopCarbonFiberRevertWidebody", DefaultCarConfig.Icons.BodyShopCarbonFiberRevertWidebody);
		ACarConfig.Icons.BodyShopWideBodyKits = mINI_ReadHash(CarINI, "Icons", "BodyShopWideBodyKits", DefaultCarConfig.Icons.BodyShopWideBodyKits);
		ACarConfig.Icons.BodyShopAttachment0 = mINI_ReadHash(CarINI, "Icons", "BodyShopAttachment0", DefaultCarConfig.Icons.BodyShopAttachment0);
		ACarConfig.Icons.BodyShopAttachment1 = mINI_ReadHash(CarINI, "Icons", "BodyShopAttachment1", DefaultCarConfig.Icons.BodyShopAttachment1);
		ACarConfig.Icons.BodyShopAttachment2 = mINI_ReadHash(CarINI, "Icons", "BodyShopAttachment2", DefaultCarConfig.Icons.BodyShopAttachment2);
		ACarConfig.Icons.BodyShopAttachment3 = mINI_ReadHash(CarINI, "Icons", "BodyShopAttachment3", DefaultCarConfig.Icons.BodyShopAttachment3);
		ACarConfig.Icons.BodyShopAttachment4 = mINI_ReadHash(CarINI, "Icons", "BodyShopAttachment4", DefaultCarConfig.Icons.BodyShopAttachment4);
		ACarConfig.Icons.PerformanceEngine = mINI_ReadHash(CarINI, "Icons", "PerformanceEngine", DefaultCarConfig.Icons.PerformanceEngine);
		ACarConfig.Icons.PerformanceECU = mINI_ReadHash(CarINI, "Icons", "PerformanceECU", DefaultCarConfig.Icons.PerformanceECU);
		ACarConfig.Icons.PerformanceTransmission = mINI_ReadHash(CarINI, "Icons", "PerformanceTransmission", DefaultCarConfig.Icons.PerformanceTransmission);
		ACarConfig.Icons.PerformanceChassis = mINI_ReadHash(CarINI, "Icons", "PerformanceChassis", DefaultCarConfig.Icons.PerformanceChassis);
		ACarConfig.Icons.PerformanceNitrous = mINI_ReadHash(CarINI, "Icons", "PerformanceNitrous", DefaultCarConfig.Icons.PerformanceNitrous);
		ACarConfig.Icons.PerformanceTires = mINI_ReadHash(CarINI, "Icons", "PerformanceTires", DefaultCarConfig.Icons.PerformanceTires);
		ACarConfig.Icons.PerformanceBrakes = mINI_ReadHash(CarINI, "Icons", "PerformanceBrakes", DefaultCarConfig.Icons.PerformanceBrakes);
		ACarConfig.Icons.PerformanceWeightReduction = mINI_ReadHash(CarINI, "Icons", "PerformanceWeightReduction", DefaultCarConfig.Icons.PerformanceWeightReduction);
		ACarConfig.Icons.PerformanceTurbo = mINI_ReadHash(CarINI, "Icons", "PerformanceTurbo", DefaultCarConfig.Icons.PerformanceTurbo);
		ACarConfig.Icons.PerformanceSupercharger = mINI_ReadHash(CarINI, "Icons", "PerformanceSupercharger", DefaultCarConfig.Icons.PerformanceSupercharger);
		ACarConfig.Icons.PerformanceAerodynamics = mINI_ReadHash(CarINI, "Icons", "PerformanceAerodynamics", DefaultCarConfig.Icons.PerformanceAerodynamics);
		ACarConfig.Icons.PerformanceTestDyno = mINI_ReadHash(CarINI, "Icons", "PerformanceTestDyno", DefaultCarConfig.Icons.PerformanceTestDyno);
		ACarConfig.Icons.PaintPaint = mINI_ReadHash(CarINI, "Icons", "PaintPaint", DefaultCarConfig.Icons.PaintPaint);
		ACarConfig.Icons.PaintBasePaint = mINI_ReadHash(CarINI, "Icons", "PaintBasePaint", DefaultCarConfig.Icons.PaintBasePaint);
		ACarConfig.Icons.PaintSpoilerPaint = mINI_ReadHash(CarINI, "Icons", "PaintSpoilerPaint", DefaultCarConfig.Icons.PaintSpoilerPaint);
		ACarConfig.Icons.PaintRoofScoopPaint = mINI_ReadHash(CarINI, "Icons", "PaintRoofScoopPaint", DefaultCarConfig.Icons.PaintRoofScoopPaint);
		ACarConfig.Icons.PaintMirrorPaint = mINI_ReadHash(CarINI, "Icons", "PaintMirrorPaint", DefaultCarConfig.Icons.PaintMirrorPaint);
		ACarConfig.Icons.PaintRimPaint = mINI_ReadHash(CarINI, "Icons", "PaintRimPaint", DefaultCarConfig.Icons.PaintRimPaint);
		ACarConfig.Icons.PaintSpinnerPaint = mINI_ReadHash(CarINI, "Icons", "PaintSpinnerPaint", DefaultCarConfig.Icons.PaintSpinnerPaint);
		ACarConfig.Icons.PaintBrakePaint = mINI_ReadHash(CarINI, "Icons", "PaintBrakePaint", DefaultCarConfig.Icons.PaintBrakePaint);
		ACarConfig.Icons.PaintEnginePaint = mINI_ReadHash(CarINI, "Icons", "PaintEnginePaint", DefaultCarConfig.Icons.PaintEnginePaint);
		ACarConfig.Icons.PaintTrunkPaint = mINI_ReadHash(CarINI, "Icons", "PaintTrunkPaint", DefaultCarConfig.Icons.PaintTrunkPaint);
		ACarConfig.Icons.PaintExhaustPaint = mINI_ReadHash(CarINI, "Icons", "PaintExhaustPaint", DefaultCarConfig.Icons.PaintExhaustPaint);
		ACarConfig.Icons.PaintVinyls = mINI_ReadHash(CarINI, "Icons", "PaintVinyls", DefaultCarConfig.Icons.PaintVinyls);
		ACarConfig.Icons.PaintVinylsCustom = mINI_ReadHash(CarINI, "Icons", "PaintVinylsCustom", DefaultCarConfig.Icons.PaintVinylsCustom);
		ACarConfig.Icons.PaintDecals = mINI_ReadHash(CarINI, "Icons", "PaintDecals", DefaultCarConfig.Icons.PaintDecals);
		ACarConfig.Icons.PaintDecalsWindshield = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield", DefaultCarConfig.Icons.PaintDecalsWindshield);
		ACarConfig.Icons.PaintDecalsWindshield1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield1", DefaultCarConfig.Icons.PaintDecalsWindshield1);
		ACarConfig.Icons.PaintDecalsWindshield2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield2", DefaultCarConfig.Icons.PaintDecalsWindshield2);
		ACarConfig.Icons.PaintDecalsWindshield3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield3", DefaultCarConfig.Icons.PaintDecalsWindshield3);
		ACarConfig.Icons.PaintDecalsWindshield4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield4", DefaultCarConfig.Icons.PaintDecalsWindshield4);
		ACarConfig.Icons.PaintDecalsWindshield5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield5", DefaultCarConfig.Icons.PaintDecalsWindshield5);
		ACarConfig.Icons.PaintDecalsWindshield6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield6", DefaultCarConfig.Icons.PaintDecalsWindshield6);
		ACarConfig.Icons.PaintDecalsWindshield7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield7", DefaultCarConfig.Icons.PaintDecalsWindshield7);
		ACarConfig.Icons.PaintDecalsWindshield8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsWindshield8", DefaultCarConfig.Icons.PaintDecalsWindshield8);
		ACarConfig.Icons.PaintDecalsRearWindow = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow", DefaultCarConfig.Icons.PaintDecalsRearWindow);
		ACarConfig.Icons.PaintDecalsRearWindow1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow1", DefaultCarConfig.Icons.PaintDecalsRearWindow1);
		ACarConfig.Icons.PaintDecalsRearWindow2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow2", DefaultCarConfig.Icons.PaintDecalsRearWindow2);
		ACarConfig.Icons.PaintDecalsRearWindow3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow3", DefaultCarConfig.Icons.PaintDecalsRearWindow3);
		ACarConfig.Icons.PaintDecalsRearWindow4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow4", DefaultCarConfig.Icons.PaintDecalsRearWindow4);
		ACarConfig.Icons.PaintDecalsRearWindow5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow5", DefaultCarConfig.Icons.PaintDecalsRearWindow5);
		ACarConfig.Icons.PaintDecalsRearWindow6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow6", DefaultCarConfig.Icons.PaintDecalsRearWindow6);
		ACarConfig.Icons.PaintDecalsRearWindow7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow7", DefaultCarConfig.Icons.PaintDecalsRearWindow7);
		ACarConfig.Icons.PaintDecalsRearWindow8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRearWindow8", DefaultCarConfig.Icons.PaintDecalsRearWindow8);
		ACarConfig.Icons.PaintDecalsHood = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHood", DefaultCarConfig.Icons.PaintDecalsHood);
		ACarConfig.Icons.PaintDecalsHoodLayout1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1", DefaultCarConfig.Icons.PaintDecalsHoodLayout1);
		ACarConfig.Icons.PaintDecalsHoodLayout2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2", DefaultCarConfig.Icons.PaintDecalsHoodLayout2);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot1", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot1);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot2", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot2);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot3", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot3);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot4", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot4);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot5", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot5);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot6", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot6);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot7", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot7);
		ACarConfig.Icons.PaintDecalsHoodLayout1Slot8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout1Slot8", DefaultCarConfig.Icons.PaintDecalsHoodLayout1Slot8);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot1", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot1);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot2", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot2);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot3", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot3);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot4", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot4);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot5", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot5);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot6", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot6);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot7", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot7);
		ACarConfig.Icons.PaintDecalsHoodLayout2Slot8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsHoodLayout2Slot8", DefaultCarConfig.Icons.PaintDecalsHoodLayout2Slot8);
		ACarConfig.Icons.PaintDecalsLeftDoor = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor", DefaultCarConfig.Icons.PaintDecalsLeftDoor);
		ACarConfig.Icons.PaintDecalsLeftDoor1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor1", DefaultCarConfig.Icons.PaintDecalsLeftDoor1);
		ACarConfig.Icons.PaintDecalsLeftDoor2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor2", DefaultCarConfig.Icons.PaintDecalsLeftDoor2);
		ACarConfig.Icons.PaintDecalsLeftDoor3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor3", DefaultCarConfig.Icons.PaintDecalsLeftDoor3);
		ACarConfig.Icons.PaintDecalsLeftDoor4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor4", DefaultCarConfig.Icons.PaintDecalsLeftDoor4);
		ACarConfig.Icons.PaintDecalsLeftDoor5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor5", DefaultCarConfig.Icons.PaintDecalsLeftDoor5);
		ACarConfig.Icons.PaintDecalsLeftDoor6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor6", DefaultCarConfig.Icons.PaintDecalsLeftDoor6);
		ACarConfig.Icons.PaintDecalsLeftDoor7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor7", DefaultCarConfig.Icons.PaintDecalsLeftDoor7);
		ACarConfig.Icons.PaintDecalsLeftDoor8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftDoor8", DefaultCarConfig.Icons.PaintDecalsLeftDoor8);
		ACarConfig.Icons.PaintDecalsRightDoor = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor", DefaultCarConfig.Icons.PaintDecalsRightDoor);
		ACarConfig.Icons.PaintDecalsRightDoor1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor1", DefaultCarConfig.Icons.PaintDecalsRightDoor1);
		ACarConfig.Icons.PaintDecalsRightDoor2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor2", DefaultCarConfig.Icons.PaintDecalsRightDoor2);
		ACarConfig.Icons.PaintDecalsRightDoor3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor3", DefaultCarConfig.Icons.PaintDecalsRightDoor3);
		ACarConfig.Icons.PaintDecalsRightDoor4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor4", DefaultCarConfig.Icons.PaintDecalsRightDoor4);
		ACarConfig.Icons.PaintDecalsRightDoor5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor5", DefaultCarConfig.Icons.PaintDecalsRightDoor5);
		ACarConfig.Icons.PaintDecalsRightDoor6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor6", DefaultCarConfig.Icons.PaintDecalsRightDoor6);
		ACarConfig.Icons.PaintDecalsRightDoor7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor7", DefaultCarConfig.Icons.PaintDecalsRightDoor7);
		ACarConfig.Icons.PaintDecalsRightDoor8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightDoor8", DefaultCarConfig.Icons.PaintDecalsRightDoor8);
		ACarConfig.Icons.PaintDecalsLeftQuarter = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarter", DefaultCarConfig.Icons.PaintDecalsLeftQuarter);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot1", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot1);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot2", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot2);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot3", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot3);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot4", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot4);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot5", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot5);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot6", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot6);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot7", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot7);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout1Slot8", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout1Slot8);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot1", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot1);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot2", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot2);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot3", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot3);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot4", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot4);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot5", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot5);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot6", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot6);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot7", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot7);
		ACarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsLeftQuarterLayout2Slot8", DefaultCarConfig.Icons.PaintDecalsLeftQuarterLayout2Slot8);
		ACarConfig.Icons.PaintDecalsRightQuarter = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarter", DefaultCarConfig.Icons.PaintDecalsRightQuarter);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot1", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot1);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot2", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot2);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot3", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot3);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot4", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot4);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot5", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot5);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot6", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot6);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot7", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot7);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout1Slot8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout1Slot8", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout1Slot8);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot1 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot1", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot1);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot2 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot2", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot2);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot3 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot3", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot3);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot4 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot4", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot4);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot5 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot5", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot5);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot6 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot6", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot6);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot7 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot7", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot7);
		ACarConfig.Icons.PaintDecalsRightQuarterLayout2Slot8 = mINI_ReadHash(CarINI, "Icons", "PaintDecalsRightQuarterLayout2Slot8", DefaultCarConfig.Icons.PaintDecalsRightQuarterLayout2Slot8);
		ACarConfig.Icons.PaintInstallSetup = mINI_ReadHash(CarINI, "Icons", "PaintInstallSetup", DefaultCarConfig.Icons.PaintInstallSetup);
		ACarConfig.Icons.PaintPurchaseSetup = mINI_ReadHash(CarINI, "Icons", "PaintPurchaseSetup", DefaultCarConfig.Icons.PaintPurchaseSetup);
		ACarConfig.Icons.SpecialtiesCustomGauges = mINI_ReadHash(CarINI, "Icons", "SpecialtiesCustomGauges", DefaultCarConfig.Icons.SpecialtiesCustomGauges);
		ACarConfig.Icons.SpecialtiesNeon = mINI_ReadHash(CarINI, "Icons", "SpecialtiesNeon", DefaultCarConfig.Icons.SpecialtiesNeon);
		ACarConfig.Icons.SpecialtiesUnderglowNeon = mINI_ReadHash(CarINI, "Icons", "SpecialtiesUnderglowNeon", DefaultCarConfig.Icons.SpecialtiesUnderglowNeon);
		ACarConfig.Icons.SpecialtiesEngineNeon = mINI_ReadHash(CarINI, "Icons", "SpecialtiesEngineNeon", DefaultCarConfig.Icons.SpecialtiesEngineNeon);
		ACarConfig.Icons.SpecialtiesCabinNeon = mINI_ReadHash(CarINI, "Icons", "SpecialtiesCabinNeon", DefaultCarConfig.Icons.SpecialtiesCabinNeon);
		ACarConfig.Icons.SpecialtiesCabinNeonLayout = mINI_ReadHash(CarINI, "Icons", "SpecialtiesCabinNeonLayout", DefaultCarConfig.Icons.SpecialtiesCabinNeonLayout);
		ACarConfig.Icons.SpecialtiesTrunkNeon = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkNeon", DefaultCarConfig.Icons.SpecialtiesTrunkNeon);
		ACarConfig.Icons.SpecialtiesWindowTint = mINI_ReadHash(CarINI, "Icons", "SpecialtiesWindowTint", DefaultCarConfig.Icons.SpecialtiesWindowTint);
		ACarConfig.Icons.SpecialtiesHeadlightColor = mINI_ReadHash(CarINI, "Icons", "SpecialtiesHeadlightColor", DefaultCarConfig.Icons.SpecialtiesHeadlightColor);
		ACarConfig.Icons.SpecialtiesNosPurge = mINI_ReadHash(CarINI, "Icons", "SpecialtiesNosPurge", DefaultCarConfig.Icons.SpecialtiesNosPurge);
		ACarConfig.Icons.SpecialtiesHydraulics = mINI_ReadHash(CarINI, "Icons", "SpecialtiesHydraulics", DefaultCarConfig.Icons.SpecialtiesHydraulics);
		ACarConfig.Icons.SpecialtiesTrunkAudio = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudio", DefaultCarConfig.Icons.SpecialtiesTrunkAudio);
		ACarConfig.Icons.SpecialtiesTrunkAudioLayout = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioLayout", DefaultCarConfig.Icons.SpecialtiesTrunkAudioLayout);
		ACarConfig.Icons.SpecialtiesTrunkAudioComponents = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioComponents", DefaultCarConfig.Icons.SpecialtiesTrunkAudioComponents);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot1 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot1", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot1);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot2 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot2", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot2);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot3 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot3", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot3);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot4 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot4", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot4);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot5 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot5", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot5);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot6 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot6", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot6);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot7 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot7", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot7);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot8 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot8", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot8);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot9 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot9", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot9);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot10 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot10", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot10);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot11 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot11", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot11);
		ACarConfig.Icons.SpecialtiesTrunkAudioSlot12 = mINI_ReadHash(CarINI, "Icons", "SpecialtiesTrunkAudioSlot12", DefaultCarConfig.Icons.SpecialtiesTrunkAudioSlot12);
		ACarConfig.Icons.SpecialtiesSpinners = mINI_ReadHash(CarINI, "Icons", "SpecialtiesSpinners", DefaultCarConfig.Icons.SpecialtiesSpinners);
		ACarConfig.Icons.SpecialtiesSplitHoods = mINI_ReadHash(CarINI, "Icons", "SpecialtiesSplitHoods", DefaultCarConfig.Icons.SpecialtiesSplitHoods);
		ACarConfig.Icons.SpecialtiesDoors = mINI_ReadHash(CarINI, "Icons", "SpecialtiesDoors", DefaultCarConfig.Icons.SpecialtiesDoors);
		ACarConfig.Icons.SpecialtiesLicensePlate = mINI_ReadHash(CarINI, "Icons", "SpecialtiesLicensePlate", DefaultCarConfig.Icons.SpecialtiesLicensePlate);
		ACarConfig.Icons.PerformanceEngineLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceEngineLevel1", DefaultCarConfig.Icons.PerformanceEngineLevel1);
		ACarConfig.Icons.PerformanceEngineLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceEngineLevel2", DefaultCarConfig.Icons.PerformanceEngineLevel2);
		ACarConfig.Icons.PerformanceEngineLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceEngineLevel3", DefaultCarConfig.Icons.PerformanceEngineLevel3);
		ACarConfig.Icons.PerformanceECULevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceECULevel1", DefaultCarConfig.Icons.PerformanceECULevel1);
		ACarConfig.Icons.PerformanceECULevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceECULevel2", DefaultCarConfig.Icons.PerformanceECULevel2);
		ACarConfig.Icons.PerformanceECULevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceECULevel3", DefaultCarConfig.Icons.PerformanceECULevel3);
		ACarConfig.Icons.PerformanceTransmissionLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceTransmissionLevel1", DefaultCarConfig.Icons.PerformanceTransmissionLevel1);
		ACarConfig.Icons.PerformanceTransmissionLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceTransmissionLevel2", DefaultCarConfig.Icons.PerformanceTransmissionLevel2);
		ACarConfig.Icons.PerformanceTransmissionLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceTransmissionLevel3", DefaultCarConfig.Icons.PerformanceTransmissionLevel3);
		ACarConfig.Icons.PerformanceChassisLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceChassisLevel1", DefaultCarConfig.Icons.PerformanceChassisLevel1);
		ACarConfig.Icons.PerformanceChassisLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceChassisLevel2", DefaultCarConfig.Icons.PerformanceChassisLevel2);
		ACarConfig.Icons.PerformanceChassisLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceChassisLevel3", DefaultCarConfig.Icons.PerformanceChassisLevel3);
		ACarConfig.Icons.PerformanceNitrousLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceNitrousLevel1", DefaultCarConfig.Icons.PerformanceNitrousLevel1);
		ACarConfig.Icons.PerformanceNitrousLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceNitrousLevel2", DefaultCarConfig.Icons.PerformanceNitrousLevel2);
		ACarConfig.Icons.PerformanceNitrousLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceNitrousLevel3", DefaultCarConfig.Icons.PerformanceNitrousLevel3);
		ACarConfig.Icons.PerformanceTiresLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceTiresLevel1", DefaultCarConfig.Icons.PerformanceTiresLevel1);
		ACarConfig.Icons.PerformanceTiresLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceTiresLevel2", DefaultCarConfig.Icons.PerformanceTiresLevel2);
		ACarConfig.Icons.PerformanceTiresLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceTiresLevel3", DefaultCarConfig.Icons.PerformanceTiresLevel3);
		ACarConfig.Icons.PerformanceBrakesLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceBrakesLevel1", DefaultCarConfig.Icons.PerformanceBrakesLevel1);
		ACarConfig.Icons.PerformanceBrakesLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceBrakesLevel2", DefaultCarConfig.Icons.PerformanceBrakesLevel2);
		ACarConfig.Icons.PerformanceBrakesLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceBrakesLevel3", DefaultCarConfig.Icons.PerformanceBrakesLevel3);
		ACarConfig.Icons.PerformanceWeightReductionLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceWeightReductionLevel1", DefaultCarConfig.Icons.PerformanceWeightReductionLevel1);
		ACarConfig.Icons.PerformanceWeightReductionLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceWeightReductionLevel2", DefaultCarConfig.Icons.PerformanceWeightReductionLevel2);
		ACarConfig.Icons.PerformanceWeightReductionLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceWeightReductionLevel3", DefaultCarConfig.Icons.PerformanceWeightReductionLevel3);
		ACarConfig.Icons.PerformanceTurboLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceTurboLevel1", DefaultCarConfig.Icons.PerformanceTurboLevel1);
		ACarConfig.Icons.PerformanceTurboLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceTurboLevel2", DefaultCarConfig.Icons.PerformanceTurboLevel2);
		ACarConfig.Icons.PerformanceTurboLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceTurboLevel3", DefaultCarConfig.Icons.PerformanceTurboLevel3);
		ACarConfig.Icons.PerformanceSuperchargerLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceSuperchargerLevel1", DefaultCarConfig.Icons.PerformanceSuperchargerLevel1);
		ACarConfig.Icons.PerformanceSuperchargerLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceSuperchargerLevel2", DefaultCarConfig.Icons.PerformanceSuperchargerLevel2);
		ACarConfig.Icons.PerformanceSuperchargerLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceSuperchargerLevel3", DefaultCarConfig.Icons.PerformanceSuperchargerLevel3);
		ACarConfig.Icons.PerformanceAerodynamicsLevel1 = mINI_ReadHash(CarINI, "Icons", "PerformanceAerodynamicsLevel1", DefaultCarConfig.Icons.PerformanceAerodynamicsLevel1);
		ACarConfig.Icons.PerformanceAerodynamicsLevel2 = mINI_ReadHash(CarINI, "Icons", "PerformanceAerodynamicsLevel2", DefaultCarConfig.Icons.PerformanceAerodynamicsLevel2);
		ACarConfig.Icons.PerformanceAerodynamicsLevel3 = mINI_ReadHash(CarINI, "Icons", "PerformanceAerodynamicsLevel3", DefaultCarConfig.Icons.PerformanceAerodynamicsLevel3);

		// Names
		ACarConfig.Names.CategoryBodyShop = mINI_ReadHash(CarINI, "Names", "CategoryBodyShop", DefaultCarConfig.Names.CategoryBodyShop);
		ACarConfig.Names.CategoryPerformance = mINI_ReadHash(CarINI, "Names", "CategoryPerformance", DefaultCarConfig.Names.CategoryPerformance);
		ACarConfig.Names.CategoryPaint = mINI_ReadHash(CarINI, "Names", "CategoryPaint", DefaultCarConfig.Names.CategoryPaint);
		ACarConfig.Names.CategorySpecialties = mINI_ReadHash(CarINI, "Names", "CategorySpecialties", DefaultCarConfig.Names.CategorySpecialties);
		ACarConfig.Names.BodyShop = mINI_ReadHash(CarINI, "Names", "BodyShop", DefaultCarConfig.Names.BodyShop);
		ACarConfig.Names.Performance = mINI_ReadHash(CarINI, "Names", "Performance", DefaultCarConfig.Names.Performance);
		ACarConfig.Names.Paint = mINI_ReadHash(CarINI, "Names", "Paint", DefaultCarConfig.Names.Paint);
		ACarConfig.Names.Specialties = mINI_ReadHash(CarINI, "Names", "Specialties", DefaultCarConfig.Names.Specialties);
		ACarConfig.Names.BodyShopFrontBumper = mINI_ReadHash(CarINI, "Names", "BodyShopFrontBumper", DefaultCarConfig.Names.BodyShopFrontBumper);
		ACarConfig.Names.BodyShopRearBumper = mINI_ReadHash(CarINI, "Names", "BodyShopRearBumper", DefaultCarConfig.Names.BodyShopRearBumper);
		ACarConfig.Names.BodyShopSkirt = mINI_ReadHash(CarINI, "Names", "BodyShopSkirt", DefaultCarConfig.Names.BodyShopSkirt);
		ACarConfig.Names.BodyShopFender = mINI_ReadHash(CarINI, "Names", "BodyShopFender", DefaultCarConfig.Names.BodyShopFender);
		ACarConfig.Names.BodyShopQuarter = mINI_ReadHash(CarINI, "Names", "BodyShopQuarter", DefaultCarConfig.Names.BodyShopQuarter);
		ACarConfig.Names.BodyShopSpoiler = mINI_ReadHash(CarINI, "Names", "BodyShopSpoiler", DefaultCarConfig.Names.BodyShopSpoiler);
		ACarConfig.Names.BodyShopHood = mINI_ReadHash(CarINI, "Names", "BodyShopHood", DefaultCarConfig.Names.BodyShopHood);
		ACarConfig.Names.BodyShopEngine = mINI_ReadHash(CarINI, "Names", "BodyShopEngine", DefaultCarConfig.Names.BodyShopEngine);
		ACarConfig.Names.BodyShopTrunk = mINI_ReadHash(CarINI, "Names", "BodyShopTrunk", DefaultCarConfig.Names.BodyShopTrunk);
		ACarConfig.Names.BodyShopRoofScoops = mINI_ReadHash(CarINI, "Names", "BodyShopRoofScoops", DefaultCarConfig.Names.BodyShopRoofScoops);
		ACarConfig.Names.BodyShopInterior = mINI_ReadHash(CarINI, "Names", "BodyShopInterior", DefaultCarConfig.Names.BodyShopInterior);
		ACarConfig.Names.BodyShopRoof = mINI_ReadHash(CarINI, "Names", "BodyShopRoof", DefaultCarConfig.Names.BodyShopRoof);
		ACarConfig.Names.BodyShopHeadlights = mINI_ReadHash(CarINI, "Names", "BodyShopHeadlights", DefaultCarConfig.Names.BodyShopHeadlights);
		ACarConfig.Names.BodyShopTaillights = mINI_ReadHash(CarINI, "Names", "BodyShopTaillights", DefaultCarConfig.Names.BodyShopTaillights);
		ACarConfig.Names.BodyShopMirrors = mINI_ReadHash(CarINI, "Names", "BodyShopMirrors", DefaultCarConfig.Names.BodyShopMirrors);
		ACarConfig.Names.BodyShopExhaust = mINI_ReadHash(CarINI, "Names", "BodyShopExhaust", DefaultCarConfig.Names.BodyShopExhaust);
		ACarConfig.Names.BodyShopRims = mINI_ReadHash(CarINI, "Names", "BodyShopRims", DefaultCarConfig.Names.BodyShopRims);
		ACarConfig.Names.BodyShopRimsCustom = mINI_ReadHash(CarINI, "Names", "BodyShopRimsCustom", DefaultCarConfig.Names.BodyShopRimsCustom);
		ACarConfig.Names.BodyShopBrakes = mINI_ReadHash(CarINI, "Names", "BodyShopBrakes", DefaultCarConfig.Names.BodyShopBrakes);
		ACarConfig.Names.BodyShopCarbonFiber = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiber", DefaultCarConfig.Names.BodyShopCarbonFiber);
		ACarConfig.Names.BodyShopCarbonFiberHood = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberHood", DefaultCarConfig.Names.BodyShopCarbonFiberHood);
		ACarConfig.Names.BodyShopCarbonFiberSpoiler = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberSpoiler", DefaultCarConfig.Names.BodyShopCarbonFiberSpoiler);
		ACarConfig.Names.BodyShopCarbonFiberRoofScoop = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberRoofScoop", DefaultCarConfig.Names.BodyShopCarbonFiberRoofScoop);
		ACarConfig.Names.BodyShopCarbonFiberMirrors = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberMirrors", DefaultCarConfig.Names.BodyShopCarbonFiberMirrors);
		ACarConfig.Names.BodyShopCarbonFiberDoor = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberDoor", DefaultCarConfig.Names.BodyShopCarbonFiberDoor);
		ACarConfig.Names.BodyShopCarbonFiberRevertDoor = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberRevertDoor", DefaultCarConfig.Names.BodyShopCarbonFiberRevertDoor);
		ACarConfig.Names.BodyShopCarbonFiberTrunk = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberTrunk", DefaultCarConfig.Names.BodyShopCarbonFiberTrunk);
		ACarConfig.Names.BodyShopCarbonFiberRevertTrunk = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberRevertTrunk", DefaultCarConfig.Names.BodyShopCarbonFiberRevertTrunk);
		ACarConfig.Names.BodyShopCarbonFiberWidebody = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberWidebody", DefaultCarConfig.Names.BodyShopCarbonFiberWidebody);
		ACarConfig.Names.BodyShopCarbonFiberRevertWidebody = mINI_ReadHash(CarINI, "Names", "BodyShopCarbonFiberRevertWidebody", DefaultCarConfig.Names.BodyShopCarbonFiberRevertWidebody);
		ACarConfig.Names.BodyShopWideBodyKits = mINI_ReadHash(CarINI, "Names", "BodyShopWideBodyKits", DefaultCarConfig.Names.BodyShopWideBodyKits);
		ACarConfig.Names.BodyShopAttachment0 = mINI_ReadHash(CarINI, "Names", "BodyShopAttachment0", DefaultCarConfig.Names.BodyShopAttachment0);
		ACarConfig.Names.BodyShopAttachment1 = mINI_ReadHash(CarINI, "Names", "BodyShopAttachment1", DefaultCarConfig.Names.BodyShopAttachment1);
		ACarConfig.Names.BodyShopAttachment2 = mINI_ReadHash(CarINI, "Names", "BodyShopAttachment2", DefaultCarConfig.Names.BodyShopAttachment2);
		ACarConfig.Names.BodyShopAttachment3 = mINI_ReadHash(CarINI, "Names", "BodyShopAttachment3", DefaultCarConfig.Names.BodyShopAttachment3);
		ACarConfig.Names.BodyShopAttachment4 = mINI_ReadHash(CarINI, "Names", "BodyShopAttachment4", DefaultCarConfig.Names.BodyShopAttachment4);
		ACarConfig.Names.PerformanceEngine = mINI_ReadHash(CarINI, "Names", "PerformanceEngine", DefaultCarConfig.Names.PerformanceEngine);
		ACarConfig.Names.PerformanceECU = mINI_ReadHash(CarINI, "Names", "PerformanceECU", DefaultCarConfig.Names.PerformanceECU);
		ACarConfig.Names.PerformanceTransmission = mINI_ReadHash(CarINI, "Names", "PerformanceTransmission", DefaultCarConfig.Names.PerformanceTransmission);
		ACarConfig.Names.PerformanceChassis = mINI_ReadHash(CarINI, "Names", "PerformanceChassis", DefaultCarConfig.Names.PerformanceChassis);
		ACarConfig.Names.PerformanceNitrous = mINI_ReadHash(CarINI, "Names", "PerformanceNitrous", DefaultCarConfig.Names.PerformanceNitrous);
		ACarConfig.Names.PerformanceTires = mINI_ReadHash(CarINI, "Names", "PerformanceTires", DefaultCarConfig.Names.PerformanceTires);
		ACarConfig.Names.PerformanceBrakes = mINI_ReadHash(CarINI, "Names", "PerformanceBrakes", DefaultCarConfig.Names.PerformanceBrakes);
		ACarConfig.Names.PerformanceWeightReduction = mINI_ReadHash(CarINI, "Names", "PerformanceWeightReduction", DefaultCarConfig.Names.PerformanceWeightReduction);
		ACarConfig.Names.PerformanceTurbo = mINI_ReadHash(CarINI, "Names", "PerformanceTurbo", DefaultCarConfig.Names.PerformanceTurbo);
		ACarConfig.Names.PerformanceSupercharger = mINI_ReadHash(CarINI, "Names", "PerformanceSupercharger", DefaultCarConfig.Names.PerformanceSupercharger);
		ACarConfig.Names.PerformanceAerodynamics = mINI_ReadHash(CarINI, "Names", "PerformanceAerodynamics", DefaultCarConfig.Names.PerformanceAerodynamics);
		ACarConfig.Names.PerformanceTestDyno = mINI_ReadHash(CarINI, "Names", "PerformanceTestDyno", DefaultCarConfig.Names.PerformanceTestDyno);
		ACarConfig.Names.PaintPaint = mINI_ReadHash(CarINI, "Names", "PaintPaint", DefaultCarConfig.Names.PaintPaint);
		ACarConfig.Names.PaintPaintCustom = mINI_ReadHash(CarINI, "Names", "PaintPaintCustom", DefaultCarConfig.Names.PaintPaintCustom);
		ACarConfig.Names.PaintBasePaint = mINI_ReadHash(CarINI, "Names", "PaintBasePaint", DefaultCarConfig.Names.PaintBasePaint);
		ACarConfig.Names.PaintSpoilerPaint = mINI_ReadHash(CarINI, "Names", "PaintSpoilerPaint", DefaultCarConfig.Names.PaintSpoilerPaint);
		ACarConfig.Names.PaintRoofScoopPaint = mINI_ReadHash(CarINI, "Names", "PaintRoofScoopPaint", DefaultCarConfig.Names.PaintRoofScoopPaint);
		ACarConfig.Names.PaintMirrorPaint = mINI_ReadHash(CarINI, "Names", "PaintMirrorPaint", DefaultCarConfig.Names.PaintMirrorPaint);
		ACarConfig.Names.PaintRimPaint = mINI_ReadHash(CarINI, "Names", "PaintRimPaint", DefaultCarConfig.Names.PaintRimPaint);
		ACarConfig.Names.PaintSpinnerPaint = mINI_ReadHash(CarINI, "Names", "PaintSpinnerPaint", DefaultCarConfig.Names.PaintSpinnerPaint);
		ACarConfig.Names.PaintBrakePaint = mINI_ReadHash(CarINI, "Names", "PaintBrakePaint", DefaultCarConfig.Names.PaintBrakePaint);
		ACarConfig.Names.PaintEnginePaint = mINI_ReadHash(CarINI, "Names", "PaintEnginePaint", DefaultCarConfig.Names.PaintEnginePaint);
		ACarConfig.Names.PaintTrunkPaint = mINI_ReadHash(CarINI, "Names", "PaintTrunkPaint", DefaultCarConfig.Names.PaintTrunkPaint);
		ACarConfig.Names.PaintExhaustPaint = mINI_ReadHash(CarINI, "Names", "PaintExhaustPaint", DefaultCarConfig.Names.PaintExhaustPaint);
		ACarConfig.Names.PaintVinyls = mINI_ReadHash(CarINI, "Names", "PaintVinyls", DefaultCarConfig.Names.PaintVinyls);
		ACarConfig.Names.PaintVinylsCustom = mINI_ReadHash(CarINI, "Names", "PaintVinylsCustom", DefaultCarConfig.Names.PaintVinylsCustom);
		ACarConfig.Names.PaintDecals = mINI_ReadHash(CarINI, "Names", "PaintDecals", DefaultCarConfig.Names.PaintDecals);
		ACarConfig.Names.PaintDecalsWindshield = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield", DefaultCarConfig.Names.PaintDecalsWindshield);
		ACarConfig.Names.PaintDecalsWindshield1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield1", DefaultCarConfig.Names.PaintDecalsWindshield1);
		ACarConfig.Names.PaintDecalsWindshield2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield2", DefaultCarConfig.Names.PaintDecalsWindshield2);
		ACarConfig.Names.PaintDecalsWindshield3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield3", DefaultCarConfig.Names.PaintDecalsWindshield3);
		ACarConfig.Names.PaintDecalsWindshield4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield4", DefaultCarConfig.Names.PaintDecalsWindshield4);
		ACarConfig.Names.PaintDecalsWindshield5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield5", DefaultCarConfig.Names.PaintDecalsWindshield5);
		ACarConfig.Names.PaintDecalsWindshield6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield6", DefaultCarConfig.Names.PaintDecalsWindshield6);
		ACarConfig.Names.PaintDecalsWindshield7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield7", DefaultCarConfig.Names.PaintDecalsWindshield7);
		ACarConfig.Names.PaintDecalsWindshield8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsWindshield8", DefaultCarConfig.Names.PaintDecalsWindshield8);
		ACarConfig.Names.PaintDecalsRearWindow = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow", DefaultCarConfig.Names.PaintDecalsRearWindow);
		ACarConfig.Names.PaintDecalsRearWindow1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow1", DefaultCarConfig.Names.PaintDecalsRearWindow1);
		ACarConfig.Names.PaintDecalsRearWindow2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow2", DefaultCarConfig.Names.PaintDecalsRearWindow2);
		ACarConfig.Names.PaintDecalsRearWindow3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow3", DefaultCarConfig.Names.PaintDecalsRearWindow3);
		ACarConfig.Names.PaintDecalsRearWindow4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow4", DefaultCarConfig.Names.PaintDecalsRearWindow4);
		ACarConfig.Names.PaintDecalsRearWindow5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow5", DefaultCarConfig.Names.PaintDecalsRearWindow5);
		ACarConfig.Names.PaintDecalsRearWindow6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow6", DefaultCarConfig.Names.PaintDecalsRearWindow6);
		ACarConfig.Names.PaintDecalsRearWindow7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow7", DefaultCarConfig.Names.PaintDecalsRearWindow7);
		ACarConfig.Names.PaintDecalsRearWindow8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRearWindow8", DefaultCarConfig.Names.PaintDecalsRearWindow8);
		ACarConfig.Names.PaintDecalsHood = mINI_ReadHash(CarINI, "Names", "PaintDecalsHood", DefaultCarConfig.Names.PaintDecalsHood);
		ACarConfig.Names.PaintDecalsHoodLayout1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1", DefaultCarConfig.Names.PaintDecalsHoodLayout1);
		ACarConfig.Names.PaintDecalsHoodLayout2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2", DefaultCarConfig.Names.PaintDecalsHoodLayout2);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot1", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot1);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot2", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot2);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot3", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot3);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot4", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot4);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot5", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot5);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot6", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot6);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot7", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot7);
		ACarConfig.Names.PaintDecalsHoodLayout1Slot8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout1Slot8", DefaultCarConfig.Names.PaintDecalsHoodLayout1Slot8);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot1", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot1);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot2", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot2);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot3", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot3);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot4", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot4);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot5", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot5);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot6", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot6);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot7", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot7);
		ACarConfig.Names.PaintDecalsHoodLayout2Slot8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsHoodLayout2Slot8", DefaultCarConfig.Names.PaintDecalsHoodLayout2Slot8);
		ACarConfig.Names.PaintDecalsLeftDoor = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor", DefaultCarConfig.Names.PaintDecalsLeftDoor);
		ACarConfig.Names.PaintDecalsLeftDoor1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor1", DefaultCarConfig.Names.PaintDecalsLeftDoor1);
		ACarConfig.Names.PaintDecalsLeftDoor2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor2", DefaultCarConfig.Names.PaintDecalsLeftDoor2);
		ACarConfig.Names.PaintDecalsLeftDoor3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor3", DefaultCarConfig.Names.PaintDecalsLeftDoor3);
		ACarConfig.Names.PaintDecalsLeftDoor4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor4", DefaultCarConfig.Names.PaintDecalsLeftDoor4);
		ACarConfig.Names.PaintDecalsLeftDoor5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor5", DefaultCarConfig.Names.PaintDecalsLeftDoor5);
		ACarConfig.Names.PaintDecalsLeftDoor6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor6", DefaultCarConfig.Names.PaintDecalsLeftDoor6);
		ACarConfig.Names.PaintDecalsLeftDoor7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor7", DefaultCarConfig.Names.PaintDecalsLeftDoor7);
		ACarConfig.Names.PaintDecalsLeftDoor8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftDoor8", DefaultCarConfig.Names.PaintDecalsLeftDoor8);
		ACarConfig.Names.PaintDecalsRightDoor = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor", DefaultCarConfig.Names.PaintDecalsRightDoor);
		ACarConfig.Names.PaintDecalsRightDoor1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor1", DefaultCarConfig.Names.PaintDecalsRightDoor1);
		ACarConfig.Names.PaintDecalsRightDoor2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor2", DefaultCarConfig.Names.PaintDecalsRightDoor2);
		ACarConfig.Names.PaintDecalsRightDoor3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor3", DefaultCarConfig.Names.PaintDecalsRightDoor3);
		ACarConfig.Names.PaintDecalsRightDoor4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor4", DefaultCarConfig.Names.PaintDecalsRightDoor4);
		ACarConfig.Names.PaintDecalsRightDoor5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor5", DefaultCarConfig.Names.PaintDecalsRightDoor5);
		ACarConfig.Names.PaintDecalsRightDoor6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor6", DefaultCarConfig.Names.PaintDecalsRightDoor6);
		ACarConfig.Names.PaintDecalsRightDoor7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor7", DefaultCarConfig.Names.PaintDecalsRightDoor7);
		ACarConfig.Names.PaintDecalsRightDoor8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightDoor8", DefaultCarConfig.Names.PaintDecalsRightDoor8);
		ACarConfig.Names.PaintDecalsLeftQuarter = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarter", DefaultCarConfig.Names.PaintDecalsLeftQuarter);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot1", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot1);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot2", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot2);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot3", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot3);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot4", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot4);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot5", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot5);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot6", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot6);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot7", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot7);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout1Slot8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout1Slot8", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout1Slot8);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot1", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot1);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot2", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot2);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot3", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot3);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot4", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot4);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot5", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot5);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot6", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot6);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot7", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot7);
		ACarConfig.Names.PaintDecalsLeftQuarterLayout2Slot8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsLeftQuarterLayout2Slot8", DefaultCarConfig.Names.PaintDecalsLeftQuarterLayout2Slot8);
		ACarConfig.Names.PaintDecalsRightQuarter = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarter", DefaultCarConfig.Names.PaintDecalsRightQuarter);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot1", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot1);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot2", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot2);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot3", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot3);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot4", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot4);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot5", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot5);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot6", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot6);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot7", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot7);
		ACarConfig.Names.PaintDecalsRightQuarterLayout1Slot8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout1Slot8", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout1Slot8);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot1 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot1", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot1);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot2 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot2", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot2);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot3 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot3", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot3);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot4 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot4", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot4);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot5 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot5", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot5);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot6 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot6", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot6);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot7 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot7", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot7);
		ACarConfig.Names.PaintDecalsRightQuarterLayout2Slot8 = mINI_ReadHash(CarINI, "Names", "PaintDecalsRightQuarterLayout2Slot8", DefaultCarConfig.Names.PaintDecalsRightQuarterLayout2Slot8);
		ACarConfig.Names.PaintInstallSetup = mINI_ReadHash(CarINI, "Names", "PaintInstallSetup", DefaultCarConfig.Names.PaintInstallSetup);
		ACarConfig.Names.PaintPurchaseSetup = mINI_ReadHash(CarINI, "Names", "PaintPurchaseSetup", DefaultCarConfig.Names.PaintPurchaseSetup);
		ACarConfig.Names.SpecialtiesCustomGauges = mINI_ReadHash(CarINI, "Names", "SpecialtiesCustomGauges", DefaultCarConfig.Names.SpecialtiesCustomGauges);
		ACarConfig.Names.SpecialtiesNeon = mINI_ReadHash(CarINI, "Names", "SpecialtiesNeon", DefaultCarConfig.Names.SpecialtiesNeon);
		ACarConfig.Names.SpecialtiesUnderglowNeon = mINI_ReadHash(CarINI, "Names", "SpecialtiesUnderglowNeon", DefaultCarConfig.Names.SpecialtiesUnderglowNeon);
		ACarConfig.Names.SpecialtiesEngineNeon = mINI_ReadHash(CarINI, "Names", "SpecialtiesEngineNeon", DefaultCarConfig.Names.SpecialtiesEngineNeon);
		ACarConfig.Names.SpecialtiesCabinNeon = mINI_ReadHash(CarINI, "Names", "SpecialtiesCabinNeon", DefaultCarConfig.Names.SpecialtiesCabinNeon);
		ACarConfig.Names.SpecialtiesCabinNeonLayout = mINI_ReadHash(CarINI, "Names", "SpecialtiesCabinNeonLayout", DefaultCarConfig.Names.SpecialtiesCabinNeonLayout);
		ACarConfig.Names.SpecialtiesTrunkNeon = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkNeon", DefaultCarConfig.Names.SpecialtiesTrunkNeon);
		ACarConfig.Names.SpecialtiesWindowTint = mINI_ReadHash(CarINI, "Names", "SpecialtiesWindowTint", DefaultCarConfig.Names.SpecialtiesWindowTint);
		ACarConfig.Names.SpecialtiesHeadlightColor = mINI_ReadHash(CarINI, "Names", "SpecialtiesHeadlightColor", DefaultCarConfig.Names.SpecialtiesHeadlightColor);
		ACarConfig.Names.SpecialtiesNosPurge = mINI_ReadHash(CarINI, "Names", "SpecialtiesNosPurge", DefaultCarConfig.Names.SpecialtiesNosPurge);
		ACarConfig.Names.SpecialtiesHydraulics = mINI_ReadHash(CarINI, "Names", "SpecialtiesHydraulics", DefaultCarConfig.Names.SpecialtiesHydraulics);
		ACarConfig.Names.SpecialtiesTrunkAudio = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudio", DefaultCarConfig.Names.SpecialtiesTrunkAudio);
		ACarConfig.Names.SpecialtiesTrunkAudioLayout = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioLayout", DefaultCarConfig.Names.SpecialtiesTrunkAudioLayout);
		ACarConfig.Names.SpecialtiesTrunkAudioComponents = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioComponents", DefaultCarConfig.Names.SpecialtiesTrunkAudioComponents);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot1 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot1", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot1);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot2 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot2", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot2);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot3 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot3", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot3);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot4 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot4", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot4);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot5 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot5", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot5);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot6 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot6", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot6);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot7 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot7", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot7);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot8 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot8", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot8);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot9 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot9", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot9);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot10 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot10", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot10);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot11 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot11", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot11);
		ACarConfig.Names.SpecialtiesTrunkAudioSlot12 = mINI_ReadHash(CarINI, "Names", "SpecialtiesTrunkAudioSlot12", DefaultCarConfig.Names.SpecialtiesTrunkAudioSlot12);
		ACarConfig.Names.SpecialtiesSpinners = mINI_ReadHash(CarINI, "Names", "SpecialtiesSpinners", DefaultCarConfig.Names.SpecialtiesSpinners);
		ACarConfig.Names.SpecialtiesSplitHoods = mINI_ReadHash(CarINI, "Names", "SpecialtiesSplitHoods", DefaultCarConfig.Names.SpecialtiesSplitHoods);
		ACarConfig.Names.SpecialtiesDoors = mINI_ReadHash(CarINI, "Names", "SpecialtiesDoors", DefaultCarConfig.Names.SpecialtiesDoors);
		ACarConfig.Names.SpecialtiesLicensePlate = mINI_ReadHash(CarINI, "Names", "SpecialtiesLicensePlate", DefaultCarConfig.Names.SpecialtiesLicensePlate);
		ACarConfig.Names.PerformanceEngineLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceEngineLevel1", DefaultCarConfig.Names.PerformanceEngineLevel1);
		ACarConfig.Names.PerformanceEngineLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceEngineLevel2", DefaultCarConfig.Names.PerformanceEngineLevel2);
		ACarConfig.Names.PerformanceEngineLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceEngineLevel3", DefaultCarConfig.Names.PerformanceEngineLevel3);
		ACarConfig.Names.PerformanceECULevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceECULevel1", DefaultCarConfig.Names.PerformanceECULevel1);
		ACarConfig.Names.PerformanceECULevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceECULevel2", DefaultCarConfig.Names.PerformanceECULevel2);
		ACarConfig.Names.PerformanceECULevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceECULevel3", DefaultCarConfig.Names.PerformanceECULevel3);
		ACarConfig.Names.PerformanceTransmissionLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceTransmissionLevel1", DefaultCarConfig.Names.PerformanceTransmissionLevel1);
		ACarConfig.Names.PerformanceTransmissionLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceTransmissionLevel2", DefaultCarConfig.Names.PerformanceTransmissionLevel2);
		ACarConfig.Names.PerformanceTransmissionLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceTransmissionLevel3", DefaultCarConfig.Names.PerformanceTransmissionLevel3);
		ACarConfig.Names.PerformanceChassisLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceChassisLevel1", DefaultCarConfig.Names.PerformanceChassisLevel1);
		ACarConfig.Names.PerformanceChassisLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceChassisLevel2", DefaultCarConfig.Names.PerformanceChassisLevel2);
		ACarConfig.Names.PerformanceChassisLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceChassisLevel3", DefaultCarConfig.Names.PerformanceChassisLevel3);
		ACarConfig.Names.PerformanceNitrousLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceNitrousLevel1", DefaultCarConfig.Names.PerformanceNitrousLevel1);
		ACarConfig.Names.PerformanceNitrousLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceNitrousLevel2", DefaultCarConfig.Names.PerformanceNitrousLevel2);
		ACarConfig.Names.PerformanceNitrousLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceNitrousLevel3", DefaultCarConfig.Names.PerformanceNitrousLevel3);
		ACarConfig.Names.PerformanceTiresLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceTiresLevel1", DefaultCarConfig.Names.PerformanceTiresLevel1);
		ACarConfig.Names.PerformanceTiresLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceTiresLevel2", DefaultCarConfig.Names.PerformanceTiresLevel2);
		ACarConfig.Names.PerformanceTiresLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceTiresLevel3", DefaultCarConfig.Names.PerformanceTiresLevel3);
		ACarConfig.Names.PerformanceBrakesLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceBrakesLevel1", DefaultCarConfig.Names.PerformanceBrakesLevel1);
		ACarConfig.Names.PerformanceBrakesLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceBrakesLevel2", DefaultCarConfig.Names.PerformanceBrakesLevel2);
		ACarConfig.Names.PerformanceBrakesLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceBrakesLevel3", DefaultCarConfig.Names.PerformanceBrakesLevel3);
		ACarConfig.Names.PerformanceWeightReductionLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceWeightReductionLevel1", DefaultCarConfig.Names.PerformanceWeightReductionLevel1);
		ACarConfig.Names.PerformanceWeightReductionLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceWeightReductionLevel2", DefaultCarConfig.Names.PerformanceWeightReductionLevel2);
		ACarConfig.Names.PerformanceWeightReductionLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceWeightReductionLevel3", DefaultCarConfig.Names.PerformanceWeightReductionLevel3);
		ACarConfig.Names.PerformanceTurboLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceTurboLevel1", DefaultCarConfig.Names.PerformanceTurboLevel1);
		ACarConfig.Names.PerformanceTurboLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceTurboLevel2", DefaultCarConfig.Names.PerformanceTurboLevel2);
		ACarConfig.Names.PerformanceTurboLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceTurboLevel3", DefaultCarConfig.Names.PerformanceTurboLevel3);
		ACarConfig.Names.PerformanceSuperchargerLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceSuperchargerLevel1", DefaultCarConfig.Names.PerformanceSuperchargerLevel1);
		ACarConfig.Names.PerformanceSuperchargerLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceSuperchargerLevel2", DefaultCarConfig.Names.PerformanceSuperchargerLevel2);
		ACarConfig.Names.PerformanceSuperchargerLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceSuperchargerLevel3", DefaultCarConfig.Names.PerformanceSuperchargerLevel3);
		ACarConfig.Names.PerformanceAerodynamicsLevel1 = mINI_ReadHash(CarINI, "Names", "PerformanceAerodynamicsLevel1", DefaultCarConfig.Names.PerformanceAerodynamicsLevel1);
		ACarConfig.Names.PerformanceAerodynamicsLevel2 = mINI_ReadHash(CarINI, "Names", "PerformanceAerodynamicsLevel2", DefaultCarConfig.Names.PerformanceAerodynamicsLevel2);
		ACarConfig.Names.PerformanceAerodynamicsLevel3 = mINI_ReadHash(CarINI, "Names", "PerformanceAerodynamicsLevel3", DefaultCarConfig.Names.PerformanceAerodynamicsLevel3);

		// PerformanceParts
		for (int p = 0; p < DefaultPerformancePartCount; p++)
		{
			ACarConfig.PerformanceParts.ReplacementHashes[p] = mINI_ReadHash(CarINI, "PerformanceParts", DefaultPerformancePartNames[p], DefaultCarConfig.PerformanceParts.ReplacementHashes[p]);
		}

		CarConfigs_temp.push_back(ACarConfig); // Add to temp list
	}

	CarConfigs = std::move(CarConfigs_temp); // Replace global list with temp
}

void LoadRimBrands()
{
	RimBrand ARimBrand;
	std::vector<RimBrand> RimBrands_temp;

	auto RimBrandsINIPath = CurrentWorkingDirectory / "UnlimiterData" / "_RimBrands.ini";
	mINI::INIFile RimBrandsINIFile(RimBrandsINIPath.string());
	mINI::INIStructure RimBrandsINI;
	RimBrandsINIFile.read(RimBrandsINI);

	char RimBrandID[15];

	int NumRimBrands = mINI_ReadInteger(RimBrandsINI, "RimBrands", "NumberOfRimBrands", DefaultRimBrandCount);
	RemoveRimSizeRestrictions = mINI_ReadInteger(RimBrandsINI, "RimBrands", "RemoveRimSizeRestrictions", 0) != 0;

	for (int i = 0; i <= NumRimBrands; i++)
	{
		sprintf(RimBrandID, "Brand%d", i);

		ARimBrand.BrandNameHash = mINI_ReadHashS(RimBrandsINI, RimBrandID, "BrandName", GetDefaultRimBrandName(i));
		ARimBrand.StringHash = mINI_ReadHashS(RimBrandsINI, RimBrandID, "String", GetDefaultRimBrandString(i));
		ARimBrand.TextureHash = mINI_ReadHashS(RimBrandsINI, RimBrandID, "Texture", GetDefaultRimBrandTexture(i));
		ARimBrand.NoRimSize = mINI_ReadInteger(RimBrandsINI, RimBrandID, "NoRimSize", i ? 0 : 1) != 0;
		ARimBrand.AvailableForRegularCars = mINI_ReadInteger(RimBrandsINI, RimBrandID, "AvailableForRegularCars", GetDefaultRimBrandAvailableForRegularCars(i)) != 0;
		ARimBrand.AvailableForSUVs = mINI_ReadInteger(RimBrandsINI, RimBrandID, "AvailableForSUVs", GetDefaultRimBrandAvailableForSUVs(i)) != 0;

		RimBrands_temp.push_back(ARimBrand); // Add to temp list
	}

	RimBrands = std::move(RimBrands_temp); // Replace global list with temp

	NumRimBrands += 0x702;
	injector::WriteMemory<short>(0x7A587D, NumRimBrands, true); // TranslateCustomizeCatToMarker
	injector::WriteMemory<short>(0x7AFEAD, NumRimBrands, true); // GetMarkerNameFromCategory
	injector::WriteMemory<short>(0x7B6098, NumRimBrands, true); // CarCustomizeManager::GetUnlockHash
	injector::WriteMemory<short>(0x7BAD74, NumRimBrands, true); // CarCustomizeManager::IsCategoryLocked
	injector::WriteMemory<short>(0x7BAF96, NumRimBrands, true); // CarCustomizeManager::IsCategoryLocked
}

void LoadVinylGroups()
{
	VinylGroup AVinylGroup;
	std::vector<VinylGroup> VinylGroups_temp;

	auto VinylGroupsINIPath = CurrentWorkingDirectory / "UnlimiterData" / "_VinylGroups.ini";
	mINI::INIFile VinylGroupsINIFile(VinylGroupsINIPath.string());
	mINI::INIStructure VinylGroupsINI;
	VinylGroupsINIFile.read(VinylGroupsINI);

	char VinylBrandID[15];

	int NumVinylGroups = mINI_ReadInteger(VinylGroupsINI, "VinylGroups", "NumberOfVinylGroups", DefaultVinylGroupCount);

	for (int i = 0; i <= NumVinylGroups; i++)
	{
		sprintf(VinylBrandID, "Group%d", i);

		AVinylGroup.Index = mINI_ReadInteger(VinylGroupsINI, VinylBrandID, "Index", GetDefaultVinylGroupIndex(i));
		AVinylGroup.TextureHash = mINI_ReadHashS(VinylGroupsINI, VinylBrandID, "Texture", GetDefaultVinylGroupTexture(i));
		AVinylGroup.StringHash = mINI_ReadHashS(VinylGroupsINI, VinylBrandID, "String", GetDefaultVinylGroupString(i));
		sprintf(AVinylGroup.CameraInfoName, mINI_ReadString(VinylGroupsINI, VinylBrandID, "CameraInfo"));

		VinylGroups_temp.push_back(AVinylGroup); // Add to temp list
	}

	VinylGroups = std::move(VinylGroups_temp); // Replace global list with temp

	NumVinylGroups += 0x402;
	injector::WriteMemory<short>(0x7A5823, NumVinylGroups, true); // TranslateCustomizeCatToMarker
	injector::WriteMemory<short>(0x7AFE90, NumVinylGroups, true); // GetMarkerNameFromCategory
	injector::WriteMemory<short>(0x7B600C, NumVinylGroups, true); // CarCustomizeManager::GetUnlockHash
	injector::WriteMemory<short>(0x7BAEC3, NumVinylGroups, true); // CarCustomizeManager::IsCategoryLocked
	injector::WriteMemory<short>(0x7BAF10, NumVinylGroups, true); // CarCustomizeManager::IsCategoryLocked
	injector::WriteMemory<short>(0x7c0ae7, NumVinylGroups, true); // CustomizePaint::Setup
}

void LoadPaintGroups()
{
	PaintGroup APaintGroup;
	std::vector<PaintGroup> PaintGroups_temp;

	auto PaintGroupsINIPath = CurrentWorkingDirectory / "UnlimiterData" / "_PaintGroups.ini";
	mINI::INIFile PaintGroupsINIFile(PaintGroupsINIPath.string());
	mINI::INIStructure PaintGroupsINI;
	PaintGroupsINIFile.read(PaintGroupsINI);

	char PaintBrandID[15];

	int NumPaintGroups = mINI_ReadInteger(PaintGroupsINI, "PaintGroups", "NumberOfPaintGroups", DefaultPaintBrandCount);
	if (NumPaintGroups > MaximumPaintBrandCount) NumPaintGroups = MaximumPaintBrandCount;

	UnifyPaints = mINI_ReadInteger(PaintGroupsINI, "PaintGroups", "UnifyPaints", 0) != 0;

	for (int i = 0; i <= NumPaintGroups; i++)
	{
		sprintf(PaintBrandID, "Group%d", i);

		APaintGroup.BrandNameHash = mINI_ReadHashS(PaintGroupsINI, PaintBrandID, "BrandName", GetDefaultPaintBrandName(i));
		APaintGroup.StringHash = mINI_ReadHashS(PaintGroupsINI, PaintBrandID, "String", GetDefaultPaintBrandString(i));
		APaintGroup.AvailableForBody = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForBody", GetDefaultPaintBrandAvailableForBody(i)) != 0;
		APaintGroup.AvailableForRims = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForRims", GetDefaultPaintBrandAvailableForRims(i)) != 0;
		APaintGroup.AvailableForBrakes = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForBrakes", GetDefaultPaintBrandAvailableForBrakes(i)) != 0;
		APaintGroup.AvailableForHoses = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForHoses", GetDefaultPaintBrandAvailableForHoses(i)) != 0;
		APaintGroup.AvailableForTrunk = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForTrunk", GetDefaultPaintBrandAvailableForTrunk(i)) != 0;
		APaintGroup.AvailableForMuffler = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForMuffler", GetDefaultPaintBrandAvailableForMuffler(i)) != 0;
		APaintGroup.AvailableForVinyl = mINI_ReadInteger(PaintGroupsINI, PaintBrandID, "AvailableForVinyl", GetDefaultPaintBrandAvailableForVinyl(i)) != 0;

		PaintGroups_temp.push_back(APaintGroup); // Add to temp list
	}

	PaintGroups = std::move(PaintGroups_temp);
}

void LoadStarGazer()
{
	auto StarGazerINIPath = CurrentWorkingDirectory / "UnlimiterData" / "_StarGazer.ini";
	mINI::INIFile StarGazerINIFile(StarGazerINIPath.string());
	mINI::INIStructure StarGazerINI;
	StarGazerINIFile.read(StarGazerINI);

	TheStarGazer.Rep[0] = 0; // Default reputation for no stars
	int i;
	for (i = 1; i <= StarGazerMaxStars; i++)
	{
		if (i <= DefaultStarGazerMaxStars || StarGazerINI["StarGazer"].has(std::to_string(i)))
			TheStarGazer.Rep[i] = mINI_ReadInteger(StarGazerINI, "StarGazer", std::to_string(i), DefaultRepForStars[i]);
		else
		{
			TheStarGazer.Rep[i] = DefaultRepForStars[DefaultStarGazerMaxStars] + 1;
			break;
		}
	}

	TheStarGazer.MaxStars = i - 1;
	TheStarGazer.MaxRep = TheStarGazer.Rep[i - 1];
	TheStarGazer.ForceRep = mINI_ReadInteger(StarGazerINI, "StarGazer", "ForceRep", -1);
}

void LoadCameraInfo()
{
	CameraInfoFile ACameraInfoFile;
	StaticCameraInfoFile AStaticCameraInfoFile;

	std::vector<CameraInfoFile> PartCameraInfos_temp;
	std::vector<CameraInfoFile> VinylCameraInfos_temp;
	std::vector<StaticCameraInfoFile> StaticCameraInfos_temp;

	UseCameraInfo = false; // Default to false

	// Part Camera Infos
	auto it = std::filesystem::directory_iterator(CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Part");

	for (const auto & entry : it)
	{
		if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
		{
			auto PartCameraInfoINIPath = entry.path();
			mINI::INIFile PartCameraInfoINIFile(PartCameraInfoINIPath.string());
			mINI::INIStructure PartCameraInfoINI;
			PartCameraInfoINIFile.read(PartCameraInfoINI);

			// Read camera info
			sprintf(ACameraInfoFile.Name, PartCameraInfoINIPath.stem().string().c_str());
			sprintf(ACameraInfoFile.InfoName, mINI_ReadString(PartCameraInfoINI, "CameraInfo", "Name"));
			ACameraInfoFile.Info.IsAnim = mINI_ReadInteger(PartCameraInfoINI, "CameraInfo", "IsAnim") != 0;
			ACameraInfoFile.Info.CarSlotID = mINI_ReadInteger(PartCameraInfoINI, "CameraInfo", "CarSlotID");
			ACameraInfoFile.Info.TurntableRot = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "TurntableRot");
			ACameraInfoFile.Info.FrontWheelRot = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "FrontWheelRot");
			ACameraInfoFile.Info.FoV = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "FoV");
			ACameraInfoFile.Info.XEye = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "XEye");
			ACameraInfoFile.Info.XLook = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "XLook");
			ACameraInfoFile.Info.YEye = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "YEye");
			ACameraInfoFile.Info.XRot = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "XRot");
			ACameraInfoFile.Info.Zoom = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "Zoom");
			ACameraInfoFile.Info.YRot = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "YRot");
			ACameraInfoFile.Info.AnimTime = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "AnimTime");
			ACameraInfoFile.Info.Focus = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "Focus");
			ACameraInfoFile.Info.UnkF = mINI_ReadFloat(PartCameraInfoINI, "CameraInfo", "UnkF");
			PartCameraInfos_temp.push_back(ACameraInfoFile);
			
		}
	}

	TheCameraInfoStuff.PartCameraInfos = std::move(PartCameraInfos_temp);
	// Fix offsets
	for (int i = 0; i < TheCameraInfoStuff.PartCameraInfos.size(); i++)
	{
		TheCameraInfoStuff.PartCameraInfos[i].Info.CameraInfoName = TheCameraInfoStuff.PartCameraInfos[i].InfoName;
	}

	// Vinyl Camera Infos
	it = std::filesystem::directory_iterator(CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Vinyl");

	for (const auto& entry : it)
	{
		if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
		{
			auto VinylCameraInfoINIPath = entry.path();
			mINI::INIFile VinylCameraInfoINIFile(VinylCameraInfoINIPath.string());
			mINI::INIStructure VinylCameraInfoINI;
			VinylCameraInfoINIFile.read(VinylCameraInfoINI);

			// Read camera info
			sprintf(ACameraInfoFile.Name, VinylCameraInfoINIPath.stem().string().c_str());
			sprintf(ACameraInfoFile.InfoName, mINI_ReadString(VinylCameraInfoINI, "CameraInfo", "Name"));
			ACameraInfoFile.Info.IsAnim = mINI_ReadInteger(VinylCameraInfoINI, "CameraInfo", "IsAnim") != 0;
			ACameraInfoFile.Info.CarSlotID = mINI_ReadInteger(VinylCameraInfoINI, "CameraInfo", "CarSlotID");
			ACameraInfoFile.Info.TurntableRot = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "TurntableRot");
			ACameraInfoFile.Info.FrontWheelRot = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "FrontWheelRot");
			ACameraInfoFile.Info.FoV = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "FoV");
			ACameraInfoFile.Info.XEye = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "XEye");
			ACameraInfoFile.Info.XLook = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "XLook");
			ACameraInfoFile.Info.YEye = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "YEye");
			ACameraInfoFile.Info.XRot = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "XRot");
			ACameraInfoFile.Info.Zoom = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "Zoom");
			ACameraInfoFile.Info.YRot = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "YRot");
			ACameraInfoFile.Info.AnimTime = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "AnimTime");
			ACameraInfoFile.Info.Focus = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "Focus");
			ACameraInfoFile.Info.UnkF = mINI_ReadFloat(VinylCameraInfoINI, "CameraInfo", "UnkF");
			VinylCameraInfos_temp.push_back(ACameraInfoFile);
		}
	}

	TheCameraInfoStuff.VinylCameraInfos = std::move(VinylCameraInfos_temp);
	// Fix offsets
	for (int i = 0; i < TheCameraInfoStuff.VinylCameraInfos.size(); i++)
	{
		TheCameraInfoStuff.VinylCameraInfos[i].Info.CameraInfoName = TheCameraInfoStuff.VinylCameraInfos[i].InfoName;
	}

	// Static Camera Infos
	it = std::filesystem::directory_iterator(CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Static");

	for (const auto& entry : it)
	{
		if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
		{
			auto StaticCameraInfoINIPath = entry.path();
			mINI::INIFile StaticCameraInfoINIFile(StaticCameraInfoINIPath.string());
			mINI::INIStructure StaticCameraInfoINI;
			StaticCameraInfoINIFile.read(StaticCameraInfoINI);

			// Read camera info
			sprintf(AStaticCameraInfoFile.Name, StaticCameraInfoINIPath.stem().string().c_str());
			sprintf(AStaticCameraInfoFile.InfoName, mINI_ReadString(StaticCameraInfoINI, "CameraInfo", "Name"));
			AStaticCameraInfoFile.Info.TurntableRot = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "TurntableRot");
			AStaticCameraInfoFile.Info.FrontWheelRot = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "FrontWheelRot");
			AStaticCameraInfoFile.Info.FoV = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "FoV");
			AStaticCameraInfoFile.Info.XEye = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "XEye");
			AStaticCameraInfoFile.Info.XLook = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "XLook");
			AStaticCameraInfoFile.Info.YEye = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "YEye");
			AStaticCameraInfoFile.Info.XRot = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "XRot");
			AStaticCameraInfoFile.Info.Zoom = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "Zoom");
			AStaticCameraInfoFile.Info.YRot = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "YRot");
			AStaticCameraInfoFile.Info.AnimTime = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "AnimTime");
			AStaticCameraInfoFile.Info.Focus = mINI_ReadFloat(StaticCameraInfoINI, "CameraInfo", "Focus");
			StaticCameraInfos_temp.push_back(AStaticCameraInfoFile);
		}
	}

	TheCameraInfoStuff.StaticCameraInfos = std::move(StaticCameraInfos_temp);

	// Fix offsets
	for (int i = 0; i < TheCameraInfoStuff.StaticCameraInfos.size(); i++)
	{
		TheCameraInfoStuff.StaticCameraInfos[i].Info.CameraInfoName = TheCameraInfoStuff.StaticCameraInfos[i].InfoName;
	}

	if (!TheCameraInfoStuff.PartCameraInfos.empty() 
		&& !TheCameraInfoStuff.VinylCameraInfos.empty() 
		&& !TheCameraInfoStuff.StaticCameraInfos.empty())
	{
		UseCameraInfo = true; // Use the info we just read
	}
	else
	{
		UseCameraInfo = false; // At least one of them is empty, use vanilla camera info
	}
}