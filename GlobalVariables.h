#pragma once

#define SingleCarTypeInfoBlockSize 0x890
#define SingleCarPartSize 0xE

#define _FECarConfigRef 0x8389AC
#define _FEDatabase 0x850078
#define _FEDatabase_0 0x83A9D0
#define _CarPartDB 0x8A2B68
#define _CarTypeInfoArray 0x8A1CCC
#define _CarPartPartsTable 0x8A1CC0
#define _TheGameFlowManager 0x8654A4
#define _CarSlotIDNames 0x802DE8
#define _CarPartIDNames 0x803338
#define _WheelStandardWidth 0x8026C4
#define gTheRideInfo 0x8392C0
#define gCarCustomizeManager 0x838980
#define gIceSideBarOverlayName 0x7FA204
#define TopOrFullScreenRide 0x8371B0
#define ThePlayerCareer 0x85AD40
#define _DrawTrunkAudio 0x8A1BEC
#define _CareerStringTable 0x838428
#define _PerformanceClassRange 0x7A1A84
#define _UnlockAllThings 0x838464
#define _g_pEAXSound 0x82B884
#define ForceHeadlightsOn 0x8A1BF0
#define ForceBrakelightsOn 0x8A1BF4
#define ForceReverselightsOn 0x8A1BF8
#define StreamingTexturePackLoader 0x83579C

#define _FEGroup_vtbl 0x79B8E8
#define _CategoryNode_vtbl 0x79BE34


#define _gCC_CHOOSE_CUSTOMIZE_CATEGORY 0x7FA1B0
#define _gCC_CHOOSE_PERF_CATEGORY 0x7FA1B4
#define _gCC_GENERIC_PARTS_BROWSER 0x7FA1B8
#define _gCC_CHOOSE_PAINT_CATEGORY 0x7FA1BC
#define _gCC_CHOOSE_ICE_CATEGORY 0x7FA1C0
#define _gCC_NEON_MAIN 0x7FA1C4
#define _gCC_CHOOSE_PERF_PACKAGE 0x7FA1C8
#define _gCC_BUY_PERF_PARTS 0x7FA1CC
#define _gCC_DEBUG_CUSTOMIZE 0x7FA1D0
#define _gCC_CHOOSE_BRANDS 0x7FA1D4
#define _gCC_PAINT_BROWSER 0x7FA1D8
#define _gCC_CHOOSE_VINYL_LAYER 0x7FA1DC
#define _gCC_DECALS_MAIN 0x7FA1E0
#define _gCC_DECALS_OVERLAY 0x7FA1E4
#define _gCC_ONLINE_CAR_LOT 0x7FA1E8
#define _gCC_CHOOSE_UNIQUE_PART 0x7FA1EC
#define _gCC_CHOOSE_RIM_BRAND 0x7FA1F0
#define _gCC_RIMS_BROWSER 0x7FA1F4
#define _gCC_CHOOSE_SPINNERS 0x7FA1F8
#define aGenericDialog_SMALL_fng 0x78FA24

#define TheGameFlowManager 0x8654A4
#define PlayerCash 0x861E74
#define __PaintMenuState 0x8389A0 // unofficial name

char AttachmentString[64];
char AttachmentDefaultString[64];
char CarTypeName[15];
char CarININame[MAX_PATH];
char RimBrandID[15];
char RimBrandString[64];
char RimBrandIcon[64];
char VinylBrandID[15];
char VinylBrandString[64];
char VinylBrandIcon[64];
char DecalMenuTexture[64];
char DecalMenuString[64];
char DecalMenuDefaultTexture[64];
char DecalMenuDefaultString[64];

//int RenderFECarFlares = -1;
bool RemoveRimSizeRestrictions = 0;
int RimsToCustomize = 0; // -1 = Rear, 0 = All, 1 = Front

// Default values for Rim Brands if _RimBrands.ini is N/A
#define DefaultRimBrandCount 23

char* DefaultRimBrandNames[]
{
	(char*)"",
	(char*)"5ZIGEN",
	(char*)"ADVAN",
	(char*)"AVUS",
	(char*)"BBS",
	(char*)"DONZ",
	(char*)"ENKEI",
	(char*)"FOXX",
	(char*)"GIANELLE",
	(char*)"GIOVANNA",
	(char*)"KAIZER",
	(char*)"KONIG",
	(char*)"LEXANI",
	(char*)"LOWENHART",
	(char*)"MOMO",
	(char*)"NFSU",
	(char*)"OASIS",
	(char*)"OZ",
	(char*)"RACINGHART",
	(char*)"ROTA",
	(char*)"DAVIN",
	(char*)"VOLK",
	(char*)"WELDWHEEL",
	(char*)"WORK"
};

char* DefaultRimBrandStrings[]
{
	(char*)"RIMS_BRAND_CUSTOM",
	(char*)"RIMS_BRAND_5ZIGEN",
	(char*)"RIMS_BRAND_ADVAN",
	(char*)"RIMS_BRAND_AVUS",
	(char*)"RIMS_BRAND_BBS",
	(char*)"RIMS_BRAND_DONZ",
	(char*)"RIMS_BRAND_ENKEI",
	(char*)"RIMS_BRAND_FOXX",
	(char*)"RIMS_BRAND_GIANELLE",
	(char*)"RIMS_BRAND_GIOVANNA",
	(char*)"RIMS_BRAND_KAIZER",
	(char*)"RIMS_BRAND_KONIG",
	(char*)"RIMS_BRAND_LEXANI",
	(char*)"RIMS_BRAND_LOWENHART",
	(char*)"RIMS_BRAND_MOMO",
	(char*)"RIMS_BRAND_NFSU",
	(char*)"RIMS_BRAND_OASIS",
	(char*)"RIMS_BRAND_OZ",
	(char*)"RIMS_BRAND_RACINGHART",
	(char*)"RIMS_BRAND_ROTA",
	(char*)"RIMS_BRAND_STREETSPIN",
	(char*)"RIMS_BRAND_VOLK",
	(char*)"RIMS_BRAND_WELD_RACING",
	(char*)"RIMS_BRAND_WORK"
};

char* DefaultRimBrandTextures[]
{
	(char*)"VISUAL_RIMS_BRAND_CUSTOM",
	(char*)"DECAL_ICON_5ZIGEN",
	(char*)"VISUAL_RIMS_BRAND_ADVAN",
	(char*)"DECAL_ICON_AVUS",
	(char*)"VISUAL_RIMS_BRAND_BBS",
	(char*)"DECAL_ICON_DONZ",
	(char*)"VISUAL_RIMS_BRAND_ENKEI",
	(char*)"DECAL_ICON_FOXX",
	(char*)"VISUAL_RIMS_BRAND_GIANELLE",
	(char*)"DECAL_ICON_GIOVANNA",
	(char*)"DECAL_ICON_KAIZER",
	(char*)"VISUAL_RIMS_BRAND_KONIG",
	(char*)"VISUAL_RIMS_BRAND_LEXANI",
	(char*)"DECAL_ICON_LOWENHART",
	(char*)"VISUAL_RIMS_BRAND_MOMO",
	(char*)"DECAL_ICON_NFSU2",
	(char*)"VISUAL_RIMS_BRAND_OASIS",
	(char*)"VISUAL_RIMS_BRAND_OZ",
	(char*)"VISUAL_RIMS_BRAND_RACINGHART",
	(char*)"DECAL_ICON_ROTA",
	(char*)"DECAL_ICON_STREETSPIN",
	(char*)"DECAL_ICON_VOLK",
	(char*)"DECAL_ICON_WELD_RACING",
	(char*)"DECAL_ICON_WORK"
};

bool DefaultRimBrandAvailableForRegularCars[]
{ 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1 };

bool DefaultRimBrandAvailableForSUVs[]
{ 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0 };

// Default values for Vinyl Groups if _VinylGroups.ini is N/A
#define DefaultVinylGroupCount 29

int DefaultVinylGroupIndexes[]
{
	31, 1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 20, 21, 23, 24, 25, 26, 27, 22, 28
};

char* DefaultVinylGroupStrings[]
{
	(char*)"VINYL_CATEGORY_CUSTOM",
	(char*)"VINYL_CATEGORY_TEARSET",
	(char*)"VINYL_CATEGORY_TEAR",
	(char*)"VINYL_CATEGORY_STRIPESET",
	(char*)"VINYL_CATEGORY_STRIPE",
	(char*)"VINYL_CATEGORY_SPLASHSET",
	(char*)"VINYL_CATEGORY_SPLASH",
	(char*)"VINYL_CATEGORY_MODERNSET",
	(char*)"VINYL_CATEGORY_MODERN",
	(char*)"VINYL_CATEGORY_FLAMESET",
	(char*)"VINYL_CATEGORY_FLAMES",
	(char*)"VINYL_CATEGORY_LIGHTNINGSET",
	(char*)"VINYL_CATEGORY_LIGHTNING",
	(char*)"VINYL_CATEGORY_RACINGSET",
	(char*)"VINYL_CATEGORY_RACING",
	(char*)"VINYL_CATEGORY_FLAGSET",
	(char*)"VINYL_CATEGORY_FLAG",
	(char*)"VINYL_CATEGORY_TRIBALSET",
	(char*)"VINYL_CATEGORY_TRIBAL",
	(char*)"VINYL_CATEGORY_WILDSET",
	(char*)"VINYL_CATEGORY_WILD",
	(char*)"VINYL_CATEGORY_HOODS",
	(char*)"VINYL_CATEGORY_AFTERMARKET",
	(char*)"VINYL_CATEGORY_BODY",
	(char*)"VINYL_CATEGORY_UNIQUE",
	(char*)"VINYL_CATEGORY_TOP_LAYER",
	(char*)"VINYL_CATEGORY_ARTFACTORY",
	(char*)"VINYL_CATEGORY_CONTEST",
	(char*)"VINYL_CATEGORY_MANUFACTURER",
	(char*)"VINYL_CATEGORY_SPECIAL"
};

char* DefaultVinylGroupTextures[]
{
	(char*)"VINYL_GROUP_CUSTOM",
	(char*)"VINYL_GROUP_TEAR_SET",
	(char*)"VINYL_GROUP_TEAR",
	(char*)"VINYL_GROUP_STRIPES_SET",
	(char*)"VINYL_GROUP_STRIPES",
	(char*)"VINYL_GROUP_SPLASH_SET",
	(char*)"VINYL_GROUP_SPLASH",
	(char*)"VINYL_GROUP_MODERN_SET",
	(char*)"VINYL_GROUP_MODERN",
	(char*)"VINYL_GROUP_FLAMES_SET",
	(char*)"VINYL_GROUP_FLAMES",
	(char*)"VINYL_GROUP_LIGHTNING_SET",
	(char*)"VINYL_GROUP_LIGHTNING",
	(char*)"VINYL_GROUP_RACING_SET",
	(char*)"VINYL_GROUP_RACING",
	(char*)"VINYL_GROUP_FLAG_SET",
	(char*)"VINYL_GROUP_FLAG",
	(char*)"VINYL_GROUP_TRIBAL_SET",
	(char*)"VINYL_GROUP_TRIBAL",
	(char*)"VINYL_GROUP_WILD_SET",
	(char*)"VINYL_GROUP_WILD",
	(char*)"VINYL_GROUP_HOOD",
	(char*)"VINYL_GROUP_AFTER_MARKET",
	(char*)"VINYL_GROUP_BODY",
	(char*)"VINYL_GROUP_UNIQUE",
	(char*)"VINYL_GROUP_TOP_LAYER",
	(char*)"VINYL_GROUP_ART_FACTORY",
	(char*)"VINYL_GROUP_CONTEST",
	(char*)"VINYL_GROUP_MANUFACTURER",
	(char*)"VINYL_GROUP_SPECIAL"
};

// Default values for Paint Groups if _PaintGroups.ini is N/A
#define DefaultPaintBrandCount 8
#define MaximumPaintBrandCount 64

#define NumPaintSwatchObjects 60

bool UnifyPaints = 0;
int DatumOffset = 0;

char* DefaultPaintBrandNames[]
{
	(char*)"",
	(char*)"GLOSS",
	(char*)"METAL",
	(char*)"PEARL",
	(char*)"HOSE",
	(char*)"CALIPER",
	(char*)"MUFFLER",
	(char*)"RIM",
	(char*)"VINYL"
};

char* DefaultPaintBrandStrings[]
{
	(char*)"CUSTOM_PAINT",
	(char*)"GLOSS_PAINT",
	(char*)"METAL_PAINT",
	(char*)"PEARL_PAINT",
	(char*)"HOSE_PAINT",
	(char*)"CALIPER_PAINT",
	(char*)"MUFFLER_PAINT",
	(char*)"RIM_PAINT",
	(char*)"VINYL_PAINT"
};

bool DefaultPaintBrandAvailableForBody[]
{ 1, 1, 1, 1, 1, 0, 1, 0, 0 };

bool DefaultPaintBrandAvailableForRims[]
{ 0, 0, 0, 0, 0, 0, 0, 1, 0 };

bool DefaultPaintBrandAvailableForBrakes[]
{ 0, 0, 0, 0, 0, 1, 0, 0, 0 };

bool DefaultPaintBrandAvailableForHoses[]
{ 1, 1, 1, 1, 1, 0, 1, 0, 0 };

bool DefaultPaintBrandAvailableForTrunk[]
{ 1, 1, 1, 1, 1, 0, 1, 0, 0 };

bool DefaultPaintBrandAvailableForMuffler[]
{ 1, 1, 1, 1, 1, 0, 1, 0, 0 };

bool DefaultPaintBrandAvailableForVinyl[]
{ 0, 0, 0, 0, 0, 0, 0, 0, 1 };

int PaintCarSlotIDsToLookFor[]
{
	63, // BASE_PAINT
	70, // PAINT_BRAKE
	73, // PAINT_RIM
	77, // VINYL_COLOUR0_0
	164 // HUD_BACKING_COLOUR
};

// Default values for Star Gazer if _StarGazer.ini is N/A
#define DefaultStarGazerMaxStars 10
#define StarGazerMaxStars 100

int DefaultRepForStars[]
{
	0,
	500,
	1000,
	1500,
	2000,
	2500,
	3000,
	3500,
	4000,
	4500,
	5000
};

// _General.ini & CARNAME.ini
typedef struct
{
	int InitiallyUnlocked;
	bool ForceLODA;
	int GameType; // 0 = Default, 1 = UG1, 2 = UG2, 3 = SUV, 4+ = None
	int InductionType; // 0 = Turbo, 1 = Supercharger, 2+ = Twincharger??
	bool ScaleBrakesWithRims;
	bool SyncVisualPartsWithPhysics;
	bool CanBeDrivenByAI;
} MainSection;

typedef struct
{
	bool HeadlightOn;
	bool BrakelightOn;
	bool CentreBrakelightOn;
	bool ReverseOn;
	bool DamageLights;
	bool CentreBrake;
	bool Reverse;
	bool BrakelightOnfInGame;
	bool TireInnerMask;
} TextureSection;

typedef struct
{
	bool LinkLicensePlateToTrunk;
	bool ShowTrunkUnderInFE;
	bool RemoveCentreBrakeWithCustomSpoiler;
	bool HasSunroof;
} CarRenderInfoSection;

typedef struct
{
	float TimingKludgeFactor060;
	float TimingKludgeFactor0100;
} StatsSection;

typedef struct
{
	int StartingRep;
} StarGazerSection;

typedef struct
{
	bool BodyShop;
	bool Performance;
	bool Paint;
	bool Specialties;
} CategorySection;

typedef struct
{
	bool FrontBumper;
	bool RearBumper;
	bool Skirt;
	bool Fender;
	bool Quarter;
	bool Spoiler;
	bool Hood;
	bool Engine;
	bool Trunk;
	bool RoofScoops;
	bool Interior;
	bool Roof;
	bool Brakes;
	bool Headlights;
	bool Taillights;
	bool Mirrors;
	bool Exhaust;
	bool Rims;
	bool RimsCustom;
	bool CarbonFiber;
	bool WideBodyKits;
	int Attachments;
	bool Attachment0;
	bool Attachment1;
	bool Attachment2;
	bool Attachment3;
	bool Attachment4;
} BodyShopSection;

typedef struct
{
	bool Engine;
	bool ECU;
	bool Transmission;
	bool Chassis;
	bool Nitrous;
	bool Tires;
	bool Brakes;
	bool WeightReduction;
	bool Induction;
	bool Aerodynamics;
	bool TestDyno;
} PerformanceSection;

typedef struct
{
	bool Paint;
	bool PaintCustom;
	bool BasePaint;
	bool SpoilerPaint;
	bool RoofScoopPaint;
	bool MirrorPaint;
	bool RimPaint;
	bool SpinnerPaint;
	bool BrakePaint;
	bool EnginePaint;
	bool TrunkPaint;
	bool ExhaustPaint;
	bool Vinyls;
	bool VinylsCustom;
	bool VinylLayers;
	bool Decals;
	bool DecalsWindshield;
	bool DecalsRearWindow;
	bool DecalsHood;
	bool DecalsLeftDoor;
	bool DecalsRightDoor;
	bool DecalsLeftQuarter;
	bool DecalsRightQuarter;
} PaintSection;

typedef struct
{
	bool CustomGauges;
	bool Neon;
	bool UnderglowNeon;
	bool EngineNeon;
	bool CabinNeon;
	bool TrunkNeon;
	bool WindowTint;
	bool HeadlightColor;
	bool NosPurge;
	bool Hydrualics;
	bool TrunkAudio;
	bool Spinners;
	bool SplitHoods;
	bool Doors;
	bool LicensePlate;
} SpecialtiesSection;

typedef struct
{
	DWORD CategoryBodyShop;
	DWORD CategoryPerformance;
	DWORD CategoryPaint;
	DWORD CategorySpecialties;
	DWORD BodyShop;
	DWORD Performance;
	DWORD Paint;
	DWORD Specialties;
	DWORD BodyShopFrontBumper;
	DWORD BodyShopRearBumper;
	DWORD BodyShopSkirt;
	DWORD BodyShopFender;
	DWORD BodyShopQuarter;
	DWORD BodyShopSpoiler;
	DWORD BodyShopHood;
	DWORD BodyShopEngine;
	DWORD BodyShopTrunk;
	DWORD BodyShopRoofScoops;
	DWORD BodyShopInterior;
	DWORD BodyShopRoof;
	DWORD BodyShopHeadlights;
	DWORD BodyShopTaillights;
	DWORD BodyShopMirrors;
	DWORD BodyShopExhaust;
	DWORD BodyShopRims;
	DWORD BodyShopRimsCustom;
	DWORD BodyShopBrakes;
	DWORD BodyShopCarbonFiber;
	DWORD BodyShopCarbonFiberHood;
	DWORD BodyShopCarbonFiberSpoiler;
	DWORD BodyShopCarbonFiberRoofScoop;
	DWORD BodyShopCarbonFiberMirrors;
	DWORD BodyShopCarbonFiberDoor;
	DWORD BodyShopCarbonFiberRevertDoor;
	DWORD BodyShopCarbonFiberTrunk;
	DWORD BodyShopCarbonFiberRevertTrunk;
	DWORD BodyShopCarbonFiberWidebody;
	DWORD BodyShopCarbonFiberRevertWidebody;
	DWORD BodyShopWideBodyKits;
	DWORD BodyShopAttachment0;
	DWORD BodyShopAttachment1;
	DWORD BodyShopAttachment2;
	DWORD BodyShopAttachment3;
	DWORD BodyShopAttachment4;
	DWORD PerformanceEngine;
	DWORD PerformanceECU;
	DWORD PerformanceTransmission;
	DWORD PerformanceChassis;
	DWORD PerformanceNitrous;
	DWORD PerformanceTires;
	DWORD PerformanceBrakes;
	DWORD PerformanceWeightReduction;
	DWORD PerformanceTurbo;
	DWORD PerformanceSupercharger;
	DWORD PerformanceAerodynamics;
	DWORD PerformanceTestDyno;
	DWORD PaintPaint;
	DWORD PaintBasePaint;
	DWORD PaintSpoilerPaint;
	DWORD PaintRoofScoopPaint;
	DWORD PaintMirrorPaint;
	DWORD PaintRimPaint;
	DWORD PaintSpinnerPaint;
	DWORD PaintBrakePaint;
	DWORD PaintEnginePaint;
	DWORD PaintTrunkPaint;
	DWORD PaintExhaustPaint;
	DWORD PaintVinyls;
	DWORD PaintVinylsCustom;
	DWORD PaintDecals;
	DWORD PaintDecalsWindshield;
	DWORD PaintDecalsWindshield1;
	DWORD PaintDecalsWindshield2;
	DWORD PaintDecalsWindshield3;
	DWORD PaintDecalsWindshield4;
	DWORD PaintDecalsWindshield5;
	DWORD PaintDecalsWindshield6;
	DWORD PaintDecalsWindshield7;
	DWORD PaintDecalsWindshield8;
	DWORD PaintDecalsRearWindow;
	DWORD PaintDecalsRearWindow1;
	DWORD PaintDecalsRearWindow2;
	DWORD PaintDecalsRearWindow3;
	DWORD PaintDecalsRearWindow4;
	DWORD PaintDecalsRearWindow5;
	DWORD PaintDecalsRearWindow6;
	DWORD PaintDecalsRearWindow7;
	DWORD PaintDecalsRearWindow8;
	DWORD PaintDecalsHood;
	DWORD PaintDecalsHoodLayout1;
	DWORD PaintDecalsHoodLayout2;
	DWORD PaintDecalsHoodLayout1Slot1;
	DWORD PaintDecalsHoodLayout1Slot2;
	DWORD PaintDecalsHoodLayout1Slot3;
	DWORD PaintDecalsHoodLayout1Slot4;
	DWORD PaintDecalsHoodLayout1Slot5;
	DWORD PaintDecalsHoodLayout1Slot6;
	DWORD PaintDecalsHoodLayout1Slot7;
	DWORD PaintDecalsHoodLayout1Slot8;
	DWORD PaintDecalsHoodLayout2Slot1;
	DWORD PaintDecalsHoodLayout2Slot2;
	DWORD PaintDecalsHoodLayout2Slot3;
	DWORD PaintDecalsHoodLayout2Slot4;
	DWORD PaintDecalsHoodLayout2Slot5;
	DWORD PaintDecalsHoodLayout2Slot6;
	DWORD PaintDecalsHoodLayout2Slot7;
	DWORD PaintDecalsHoodLayout2Slot8;
	DWORD PaintDecalsLeftDoor;
	DWORD PaintDecalsLeftDoor1;
	DWORD PaintDecalsLeftDoor2;
	DWORD PaintDecalsLeftDoor3;
	DWORD PaintDecalsLeftDoor4;
	DWORD PaintDecalsLeftDoor5;
	DWORD PaintDecalsLeftDoor6;
	DWORD PaintDecalsLeftDoor7;
	DWORD PaintDecalsLeftDoor8;
	DWORD PaintDecalsRightDoor;
	DWORD PaintDecalsRightDoor1;
	DWORD PaintDecalsRightDoor2;
	DWORD PaintDecalsRightDoor3;
	DWORD PaintDecalsRightDoor4;
	DWORD PaintDecalsRightDoor5;
	DWORD PaintDecalsRightDoor6;
	DWORD PaintDecalsRightDoor7;
	DWORD PaintDecalsRightDoor8;
	DWORD PaintDecalsLeftQuarter;
	DWORD PaintDecalsLeftQuarterLayout1;
	DWORD PaintDecalsLeftQuarterLayout2;
	DWORD PaintDecalsLeftQuarterLayout1Slot1;
	DWORD PaintDecalsLeftQuarterLayout1Slot2;
	DWORD PaintDecalsLeftQuarterLayout1Slot3;
	DWORD PaintDecalsLeftQuarterLayout1Slot4;
	DWORD PaintDecalsLeftQuarterLayout1Slot5;
	DWORD PaintDecalsLeftQuarterLayout1Slot6;
	DWORD PaintDecalsLeftQuarterLayout1Slot7;
	DWORD PaintDecalsLeftQuarterLayout1Slot8;
	DWORD PaintDecalsLeftQuarterLayout2Slot1;
	DWORD PaintDecalsLeftQuarterLayout2Slot2;
	DWORD PaintDecalsLeftQuarterLayout2Slot3;
	DWORD PaintDecalsLeftQuarterLayout2Slot4;
	DWORD PaintDecalsLeftQuarterLayout2Slot5;
	DWORD PaintDecalsLeftQuarterLayout2Slot6;
	DWORD PaintDecalsLeftQuarterLayout2Slot7;
	DWORD PaintDecalsLeftQuarterLayout2Slot8;
	DWORD PaintDecalsRightQuarter;
	DWORD PaintDecalsRightQuarterLayout1;
	DWORD PaintDecalsRightQuarterLayout2;
	DWORD PaintDecalsRightQuarterLayout1Slot1;
	DWORD PaintDecalsRightQuarterLayout1Slot2;
	DWORD PaintDecalsRightQuarterLayout1Slot3;
	DWORD PaintDecalsRightQuarterLayout1Slot4;
	DWORD PaintDecalsRightQuarterLayout1Slot5;
	DWORD PaintDecalsRightQuarterLayout1Slot6;
	DWORD PaintDecalsRightQuarterLayout1Slot7;
	DWORD PaintDecalsRightQuarterLayout1Slot8;
	DWORD PaintDecalsRightQuarterLayout2Slot1;
	DWORD PaintDecalsRightQuarterLayout2Slot2;
	DWORD PaintDecalsRightQuarterLayout2Slot3;
	DWORD PaintDecalsRightQuarterLayout2Slot4;
	DWORD PaintDecalsRightQuarterLayout2Slot5;
	DWORD PaintDecalsRightQuarterLayout2Slot6;
	DWORD PaintDecalsRightQuarterLayout2Slot7;
	DWORD PaintDecalsRightQuarterLayout2Slot8;
	DWORD PaintInstallSetup;
	DWORD PaintPurchaseSetup;
	DWORD SpecialtiesCustomGauges;
	DWORD SpecialtiesNeon;
	DWORD SpecialtiesUnderglowNeon;
	DWORD SpecialtiesEngineNeon;
	DWORD SpecialtiesCabinNeon;
	DWORD SpecialtiesCabinNeonLayout;
	DWORD SpecialtiesTrunkNeon;
	DWORD SpecialtiesWindowTint;
	DWORD SpecialtiesHeadlightColor;
	DWORD SpecialtiesNosPurge;
	DWORD SpecialtiesHydraulics;
	DWORD SpecialtiesTrunkAudio;
	DWORD SpecialtiesTrunkAudioLayout;
	DWORD SpecialtiesTrunkAudioComponents;
	DWORD SpecialtiesTrunkAudioSlot1;
	DWORD SpecialtiesTrunkAudioSlot2;
	DWORD SpecialtiesTrunkAudioSlot3;
	DWORD SpecialtiesTrunkAudioSlot4;
	DWORD SpecialtiesTrunkAudioSlot5;
	DWORD SpecialtiesTrunkAudioSlot6;
	DWORD SpecialtiesTrunkAudioSlot7;
	DWORD SpecialtiesTrunkAudioSlot8;
	DWORD SpecialtiesTrunkAudioSlot9;
	DWORD SpecialtiesTrunkAudioSlot10;
	DWORD SpecialtiesTrunkAudioSlot11;
	DWORD SpecialtiesTrunkAudioSlot12;
	DWORD SpecialtiesSpinners;
	DWORD SpecialtiesSplitHoods;
	DWORD SpecialtiesDoors;
	DWORD SpecialtiesLicensePlate;
	DWORD PerformanceEngineLevel1;
	DWORD PerformanceEngineLevel2;
	DWORD PerformanceEngineLevel3;
	DWORD PerformanceECULevel1;
	DWORD PerformanceECULevel2;
	DWORD PerformanceECULevel3;
	DWORD PerformanceTransmissionLevel1;
	DWORD PerformanceTransmissionLevel2;
	DWORD PerformanceTransmissionLevel3;
	DWORD PerformanceChassisLevel1;
	DWORD PerformanceChassisLevel2;
	DWORD PerformanceChassisLevel3;
	DWORD PerformanceNitrousLevel1;
	DWORD PerformanceNitrousLevel2;
	DWORD PerformanceNitrousLevel3;
	DWORD PerformanceTiresLevel1;
	DWORD PerformanceTiresLevel2;
	DWORD PerformanceTiresLevel3;
	DWORD PerformanceBrakesLevel1;
	DWORD PerformanceBrakesLevel2;
	DWORD PerformanceBrakesLevel3;
	DWORD PerformanceWeightReductionLevel1;
	DWORD PerformanceWeightReductionLevel2;
	DWORD PerformanceWeightReductionLevel3;
	DWORD PerformanceTurboLevel1;
	DWORD PerformanceTurboLevel2;
	DWORD PerformanceTurboLevel3;
	DWORD PerformanceSuperchargerLevel1;
	DWORD PerformanceSuperchargerLevel2;
	DWORD PerformanceSuperchargerLevel3;
	DWORD PerformanceAerodynamicsLevel1;
	DWORD PerformanceAerodynamicsLevel2;
	DWORD PerformanceAerodynamicsLevel3;
} IconsSection;

typedef struct
{
	DWORD CategoryBodyShop;
	DWORD CategoryPerformance;
	DWORD CategoryPaint;
	DWORD CategorySpecialties;
	DWORD BodyShop;
	DWORD Performance;
	DWORD Paint;
	DWORD Specialties;
	DWORD BodyShopFrontBumper;
	DWORD BodyShopRearBumper;
	DWORD BodyShopSkirt;
	DWORD BodyShopFender;
	DWORD BodyShopQuarter;
	DWORD BodyShopSpoiler;
	DWORD BodyShopHood;
	DWORD BodyShopEngine;
	DWORD BodyShopTrunk;
	DWORD BodyShopRoofScoops;
	DWORD BodyShopInterior;
	DWORD BodyShopRoof;
	DWORD BodyShopHeadlights;
	DWORD BodyShopTaillights;
	DWORD BodyShopMirrors;
	DWORD BodyShopExhaust;
	DWORD BodyShopRims;
	DWORD BodyShopRimsCustom;
	DWORD BodyShopBrakes;
	DWORD BodyShopCarbonFiber;
	DWORD BodyShopCarbonFiberHood;
	DWORD BodyShopCarbonFiberSpoiler;
	DWORD BodyShopCarbonFiberRoofScoop;
	DWORD BodyShopCarbonFiberMirrors;
	DWORD BodyShopCarbonFiberDoor;
	DWORD BodyShopCarbonFiberRevertDoor;
	DWORD BodyShopCarbonFiberTrunk;
	DWORD BodyShopCarbonFiberRevertTrunk;
	DWORD BodyShopCarbonFiberWidebody;
	DWORD BodyShopCarbonFiberRevertWidebody;
	DWORD BodyShopWideBodyKits;
	DWORD BodyShopAttachment0;
	DWORD BodyShopAttachment1;
	DWORD BodyShopAttachment2;
	DWORD BodyShopAttachment3;
	DWORD BodyShopAttachment4;
	DWORD PerformanceEngine;
	DWORD PerformanceECU;
	DWORD PerformanceTransmission;
	DWORD PerformanceChassis;
	DWORD PerformanceNitrous;
	DWORD PerformanceTires;
	DWORD PerformanceBrakes;
	DWORD PerformanceWeightReduction;
	DWORD PerformanceTurbo;
	DWORD PerformanceSupercharger;
	DWORD PerformanceAerodynamics;
	DWORD PerformanceTestDyno;
	DWORD PaintPaint;
	DWORD PaintPaintCustom;
	DWORD PaintBasePaint;
	DWORD PaintSpoilerPaint;
	DWORD PaintRoofScoopPaint;
	DWORD PaintMirrorPaint;
	DWORD PaintRimPaint;
	DWORD PaintSpinnerPaint;
	DWORD PaintBrakePaint;
	DWORD PaintEnginePaint;
	DWORD PaintTrunkPaint;
	DWORD PaintExhaustPaint;
	DWORD PaintVinyls;
	DWORD PaintVinylsCustom;
	DWORD PaintDecals;
	DWORD PaintDecalsWindshield;
	DWORD PaintDecalsWindshield1;
	DWORD PaintDecalsWindshield2;
	DWORD PaintDecalsWindshield3;
	DWORD PaintDecalsWindshield4;
	DWORD PaintDecalsWindshield5;
	DWORD PaintDecalsWindshield6;
	DWORD PaintDecalsWindshield7;
	DWORD PaintDecalsWindshield8;
	DWORD PaintDecalsRearWindow;
	DWORD PaintDecalsRearWindow1;
	DWORD PaintDecalsRearWindow2;
	DWORD PaintDecalsRearWindow3;
	DWORD PaintDecalsRearWindow4;
	DWORD PaintDecalsRearWindow5;
	DWORD PaintDecalsRearWindow6;
	DWORD PaintDecalsRearWindow7;
	DWORD PaintDecalsRearWindow8;
	DWORD PaintDecalsHood;
	DWORD PaintDecalsHoodLayout1;
	DWORD PaintDecalsHoodLayout2;
	DWORD PaintDecalsHoodLayout1Slot1;
	DWORD PaintDecalsHoodLayout1Slot2;
	DWORD PaintDecalsHoodLayout1Slot3;
	DWORD PaintDecalsHoodLayout1Slot4;
	DWORD PaintDecalsHoodLayout1Slot5;
	DWORD PaintDecalsHoodLayout1Slot6;
	DWORD PaintDecalsHoodLayout1Slot7;
	DWORD PaintDecalsHoodLayout1Slot8;
	DWORD PaintDecalsHoodLayout2Slot1;
	DWORD PaintDecalsHoodLayout2Slot2;
	DWORD PaintDecalsHoodLayout2Slot3;
	DWORD PaintDecalsHoodLayout2Slot4;
	DWORD PaintDecalsHoodLayout2Slot5;
	DWORD PaintDecalsHoodLayout2Slot6;
	DWORD PaintDecalsHoodLayout2Slot7;
	DWORD PaintDecalsHoodLayout2Slot8;
	DWORD PaintDecalsLeftDoor;
	DWORD PaintDecalsLeftDoor1;
	DWORD PaintDecalsLeftDoor2;
	DWORD PaintDecalsLeftDoor3;
	DWORD PaintDecalsLeftDoor4;
	DWORD PaintDecalsLeftDoor5;
	DWORD PaintDecalsLeftDoor6;
	DWORD PaintDecalsLeftDoor7;
	DWORD PaintDecalsLeftDoor8;
	DWORD PaintDecalsRightDoor;
	DWORD PaintDecalsRightDoor1;
	DWORD PaintDecalsRightDoor2;
	DWORD PaintDecalsRightDoor3;
	DWORD PaintDecalsRightDoor4;
	DWORD PaintDecalsRightDoor5;
	DWORD PaintDecalsRightDoor6;
	DWORD PaintDecalsRightDoor7;
	DWORD PaintDecalsRightDoor8;
	DWORD PaintDecalsLeftQuarter;
	DWORD PaintDecalsLeftQuarterLayout1;
	DWORD PaintDecalsLeftQuarterLayout2;
	DWORD PaintDecalsLeftQuarterLayout1Slot1;
	DWORD PaintDecalsLeftQuarterLayout1Slot2;
	DWORD PaintDecalsLeftQuarterLayout1Slot3;
	DWORD PaintDecalsLeftQuarterLayout1Slot4;
	DWORD PaintDecalsLeftQuarterLayout1Slot5;
	DWORD PaintDecalsLeftQuarterLayout1Slot6;
	DWORD PaintDecalsLeftQuarterLayout1Slot7;
	DWORD PaintDecalsLeftQuarterLayout1Slot8;
	DWORD PaintDecalsLeftQuarterLayout2Slot1;
	DWORD PaintDecalsLeftQuarterLayout2Slot2;
	DWORD PaintDecalsLeftQuarterLayout2Slot3;
	DWORD PaintDecalsLeftQuarterLayout2Slot4;
	DWORD PaintDecalsLeftQuarterLayout2Slot5;
	DWORD PaintDecalsLeftQuarterLayout2Slot6;
	DWORD PaintDecalsLeftQuarterLayout2Slot7;
	DWORD PaintDecalsLeftQuarterLayout2Slot8;
	DWORD PaintDecalsRightQuarter;
	DWORD PaintDecalsRightQuarterLayout1;
	DWORD PaintDecalsRightQuarterLayout2;
	DWORD PaintDecalsRightQuarterLayout1Slot1;
	DWORD PaintDecalsRightQuarterLayout1Slot2;
	DWORD PaintDecalsRightQuarterLayout1Slot3;
	DWORD PaintDecalsRightQuarterLayout1Slot4;
	DWORD PaintDecalsRightQuarterLayout1Slot5;
	DWORD PaintDecalsRightQuarterLayout1Slot6;
	DWORD PaintDecalsRightQuarterLayout1Slot7;
	DWORD PaintDecalsRightQuarterLayout1Slot8;
	DWORD PaintDecalsRightQuarterLayout2Slot1;
	DWORD PaintDecalsRightQuarterLayout2Slot2;
	DWORD PaintDecalsRightQuarterLayout2Slot3;
	DWORD PaintDecalsRightQuarterLayout2Slot4;
	DWORD PaintDecalsRightQuarterLayout2Slot5;
	DWORD PaintDecalsRightQuarterLayout2Slot6;
	DWORD PaintDecalsRightQuarterLayout2Slot7;
	DWORD PaintDecalsRightQuarterLayout2Slot8;
	DWORD PaintInstallSetup;
	DWORD PaintPurchaseSetup;
	DWORD SpecialtiesCustomGauges;
	DWORD SpecialtiesNeon;
	DWORD SpecialtiesUnderglowNeon;
	DWORD SpecialtiesEngineNeon;
	DWORD SpecialtiesCabinNeon;
	DWORD SpecialtiesCabinNeonLayout;
	DWORD SpecialtiesTrunkNeon;
	DWORD SpecialtiesWindowTint;
	DWORD SpecialtiesHeadlightColor;
	DWORD SpecialtiesNosPurge;
	DWORD SpecialtiesHydraulics;
	DWORD SpecialtiesTrunkAudio;
	DWORD SpecialtiesTrunkAudioLayout;
	DWORD SpecialtiesTrunkAudioComponents;
	DWORD SpecialtiesTrunkAudioSlot1;
	DWORD SpecialtiesTrunkAudioSlot2;
	DWORD SpecialtiesTrunkAudioSlot3;
	DWORD SpecialtiesTrunkAudioSlot4;
	DWORD SpecialtiesTrunkAudioSlot5;
	DWORD SpecialtiesTrunkAudioSlot6;
	DWORD SpecialtiesTrunkAudioSlot7;
	DWORD SpecialtiesTrunkAudioSlot8;
	DWORD SpecialtiesTrunkAudioSlot9;
	DWORD SpecialtiesTrunkAudioSlot10;
	DWORD SpecialtiesTrunkAudioSlot11;
	DWORD SpecialtiesTrunkAudioSlot12;
	DWORD SpecialtiesSpinners;
	DWORD SpecialtiesSplitHoods;
	DWORD SpecialtiesDoors;
	DWORD SpecialtiesLicensePlate;
	DWORD PerformanceEngineLevel1;
	DWORD PerformanceEngineLevel2;
	DWORD PerformanceEngineLevel3;
	DWORD PerformanceECULevel1;
	DWORD PerformanceECULevel2;
	DWORD PerformanceECULevel3;
	DWORD PerformanceTransmissionLevel1;
	DWORD PerformanceTransmissionLevel2;
	DWORD PerformanceTransmissionLevel3;
	DWORD PerformanceChassisLevel1;
	DWORD PerformanceChassisLevel2;
	DWORD PerformanceChassisLevel3;
	DWORD PerformanceNitrousLevel1;
	DWORD PerformanceNitrousLevel2;
	DWORD PerformanceNitrousLevel3;
	DWORD PerformanceTiresLevel1;
	DWORD PerformanceTiresLevel2;
	DWORD PerformanceTiresLevel3;
	DWORD PerformanceBrakesLevel1;
	DWORD PerformanceBrakesLevel2;
	DWORD PerformanceBrakesLevel3;
	DWORD PerformanceWeightReductionLevel1;
	DWORD PerformanceWeightReductionLevel2;
	DWORD PerformanceWeightReductionLevel3;
	DWORD PerformanceTurboLevel1;
	DWORD PerformanceTurboLevel2;
	DWORD PerformanceTurboLevel3;
	DWORD PerformanceSuperchargerLevel1;
	DWORD PerformanceSuperchargerLevel2;
	DWORD PerformanceSuperchargerLevel3;
	DWORD PerformanceAerodynamicsLevel1;
	DWORD PerformanceAerodynamicsLevel2;
	DWORD PerformanceAerodynamicsLevel3;
} NamesSection;

typedef struct
{
	int AnimationType[CAR_SLOT_ID::__NUM];
} AnimationSection;


#define DefaultPerformancePartCount 71

typedef struct
{
	DWORD ReplacementHashes[DefaultPerformancePartCount];
} PerformancePartsSection;

typedef struct
{
	DWORD* CarTypeInfo;
	MainSection Main;
	TextureSection Textures;
	CarRenderInfoSection RenderInfo;
	StatsSection Stats;
	StarGazerSection StarGazer;
	CategorySection Category;
	BodyShopSection BodyShop;
	PerformanceSection Performance;
	PaintSection Paint;
	SpecialtiesSection Specialties;
	IconsSection Icons;
	NamesSection Names;
	AnimationSection Animations;
	PerformancePartsSection PerformanceParts;
	//StockPartsSection StockParts;
	//RandomPartsSection RandomParts;
} CarConfig;

CarConfig DefaultCarConfig;
std::vector<CarConfig> CarConfigs;

// _FNGFixes.ini
typedef struct
{
	char Prefix[64];
} Child;

typedef struct
{
	char FNGName[64];
	char ObjectPrefix[64];
	int NumberOfObjects;
	std::vector<Child> Children;
} FNGFix;

std::vector<FNGFix> FNGFixes;

// _RimBrands.ini
typedef struct
{
	DWORD BrandNameHash;
	DWORD StringHash;
	DWORD TextureHash;
	bool NoRimSize;
	bool HideBrandName;
	bool AvailableForRegularCars;
	bool AvailableForSUVs;
} RimBrand;

std::vector<RimBrand> RimBrands;

// _VinylGroups.ini
typedef struct
{
	int Index;
	DWORD StringHash;
	DWORD TextureHash;
	char CameraInfoName[MAX_PATH];
} VinylGroup;

std::vector<VinylGroup> VinylGroups;

// _PaintGroups.ini
typedef struct
{
	DWORD BrandNameHash;
	DWORD StringHash;
	bool AvailableForBody;
	bool AvailableForRims;
	bool AvailableForBrakes;
	bool AvailableForHoses;
	bool AvailableForTrunk;
	bool AvailableForMuffler;
	bool AvailableForVinyl;
} PaintGroup;

std::vector<PaintGroup> PaintGroups;

int NumAvailablePaintGroups = 0;
int AvailablePaintGroups[MaximumPaintBrandCount];

// _StarGazer.ini
typedef struct
{
	int MaxStars;
	int MaxRep;
	int ForceRep;
	int Rep[StarGazerMaxStars + 1];
} StarGazer;

StarGazer TheStarGazer;

// Camera Info Stuff is in CameraInfo.h
bool UseCameraInfo = false;

float CarSelectTireSteerAngle_Backup;

std::filesystem::path CurrentWorkingDirectory;

DWORD ColorHashes[3][4] = {
	{0x0000D99A, 0x02DDC8F0, 0x00136707, 0x026E1AC5}, // RED, GREEN, BLUE, ALPHA
	{0x001C0D0C, 0x5E96E722, 0x02804819, 0x50317397}, // RED2, GREEN2, BLUE2, ALPHA2
	{0x368A1A6A, 0x00BA7DC0, 0x07C4C1D7, 0x004ACF95} // DISPRED, DISPGREEN, DISPBLUE, DISPALPHA
};

char* GetDefaultRimBrandName(int ID)
{
	if (ID > DefaultRimBrandCount) return (char*)"";
	else return DefaultRimBrandNames[ID];
}

char* GetDefaultRimBrandTexture(int ID)
{
	if (ID > DefaultRimBrandCount) return (char*)"";
	else return DefaultRimBrandTextures[ID];
}

char* GetDefaultRimBrandString(int ID)
{
	if (ID > DefaultRimBrandCount) return (char*)"";
	else return DefaultRimBrandStrings[ID];
}

int GetDefaultRimBrandAvailableForRegularCars(int ID)
{
	if (ID > DefaultRimBrandCount) return 1;
	else return DefaultRimBrandAvailableForRegularCars[ID];
}

int GetDefaultRimBrandAvailableForSUVs(int ID)
{
	if (ID > DefaultRimBrandCount) return 1;
	else return DefaultRimBrandAvailableForSUVs[ID];
}

int GetDefaultVinylGroupIndex(int ID)
{
	if (ID > DefaultVinylGroupCount) return 1;
	else return DefaultVinylGroupIndexes[ID];
}

char* GetDefaultVinylGroupTexture(int ID)
{
	if (ID > DefaultVinylGroupCount) return (char*)"";
	else return DefaultVinylGroupTextures[ID];
}

char* GetDefaultVinylGroupString(int ID)
{
	if (ID > DefaultVinylGroupCount) return (char*)"";
	else return DefaultVinylGroupStrings[ID];
}

char* GetDefaultPaintBrandName(int ID)
{
	if (ID > DefaultPaintBrandCount) return (char*)"";
	else return DefaultPaintBrandNames[ID];
}

char* GetDefaultPaintBrandString(int ID)
{
	if (ID > DefaultPaintBrandCount) return (char*)"";
	else return DefaultPaintBrandStrings[ID];
}

int GetDefaultLinkLicensePlateToTrunk(int CarTypeID)
{
	switch (CarTypeID)
	{
		case 0:	// PEUGOT
		case 1:	// FOCUS
		case 5:	// CIVIC
		case 8:	// HUMMER
		case 9:	// NAVIGATOR
		case 12:// SENTRA
		case 13:// CELICA
		case 14:// IS300
		case 17:// A3
		case 25:// GTO
			return 1;
	}

	return 0;
}

int GetDefaultShowTrunkUnderInFE(int CarType)
{
	switch (CarType)
	{
		case 0:	// PEUGOT
		case 1:	// FOCUS
		case 2:	// COROLLA
		case 3:	// 240SX
		case 6:	// PEUGOT106
		case 7:	// CORSA
		case 8:	// HUMMER
		case 9:	// NAVIGATOR
		case 10:// ESCALADE
		case 11:// TIBURON
		case 13:// CELICA
		case 15:// SUPRA
		case 16:// GOLF
		case 17:// A3
		case 18:// RSX
		case 19:// ECLIPSE
		case 20:// TT
		case 22:// 350Z
		case 24:// 3000GT
		case 29:// RX7
			return 1;
	}

	return 0;
}

int GetDefaultRemoveCentreBrakeWithCustomSpoiler(int CarType)
{
	switch (CarType)
	{
		case 5:	 // CIVIC
		case 11: // TIBURON
		case 12: // SENTRA
		case 24: // 3000GT
		case 25: // GTO
		case 30: // IMPREZAWRX
			return 1;
	}

	return 0;
}

int GetDefaultHasSunroof(int CarTypeID)
{
	switch (CarTypeID)
	{
		case 3:	 // 240SX
		case 5:	 // CIVIC
		case 18: // RSX
		case 23: // G35
			return 1;
	}

	return 0;
}

int GetDefaultGameType(int CarTypeID)
{
	switch (CarTypeID)
	{
		case 0: // PEUGOT
		case 1: // FOCUS
		case 3: // 240SX
		case 4: // MIATA
		case 5: // CIVIC
		case 11: // TIBURON
		case 12: // SENTRA
		case 13: // CELICA
		case 15: // SUPRA
		case 16: // GOLF
		case 18: // RSX
		case 19: // ECLIPSE
		case 22: // 350Z
		case 27: // SKYLINE
		case 29: // RX7
			return 1; // UG1

		case 2: // COROLLA
		case 6: // PEUGOT106
		case 7: // CORSA
		case 14: // IS300
		case 17: // A3
		case 20: // TT
		case 21: // RX8
		case 23: // G35
		case 24: // 3000GT
		case 25: // GTO
		case 26: // MUSTANGGT
		case 28: // LANCEREVO8
		case 30: // IMPREZAWRX
			return 2; // UG2

		case 8: // HUMMER
		case 9: // NAVIGATOR
		case 10: // ESCALADE
			return 3; // SUV
	}

	return 0;
}

int GetDefaultPaintBrandAvailableForBody(int ID)
{
	if (ID > DefaultPaintBrandCount) return 1;
	else return DefaultPaintBrandAvailableForBody[ID];
}

int GetDefaultPaintBrandAvailableForRims(int ID)
{
	if (ID > DefaultPaintBrandCount) return 0;
	else return DefaultPaintBrandAvailableForRims[ID];
}

int GetDefaultPaintBrandAvailableForBrakes(int ID)
{
	if (ID > DefaultPaintBrandCount) return 0;
	else return DefaultPaintBrandAvailableForBrakes[ID];
}

int GetDefaultPaintBrandAvailableForHoses(int ID)
{
	if (ID > DefaultPaintBrandCount) return 1;
	else return DefaultPaintBrandAvailableForHoses[ID];
}

int GetDefaultPaintBrandAvailableForTrunk(int ID)
{
	if (ID > DefaultPaintBrandCount) return 1;
	else return DefaultPaintBrandAvailableForTrunk[ID];
}

int GetDefaultPaintBrandAvailableForMuffler(int ID)
{
	if (ID > DefaultPaintBrandCount) return 1;
	else return DefaultPaintBrandAvailableForMuffler[ID];
}

int GetDefaultPaintBrandAvailableForVinyl(int ID)
{
	if (ID > DefaultPaintBrandCount) return 1;
	else return DefaultPaintBrandAvailableForVinyl[ID];
}

char* DefaultPerformancePartNames[]
{
	(char*)"WT_REMOVE_REAR_SEATS",
	(char*)"WT_REMOVE_INTERIOR_PANELS",
	(char*)"WT_LIGHTWEIGHT_WINDOWS",
	(char*)"WT_LIGHTWEIGHT_SEATS",
	(char*)"WT_LIGHTWEIGHT_DOORS",
	(char*)"WT_FOAM_FILLED_INTERIOR",
	(char*)"TR_SHORT_THROW_SHIFT_KIT",
	(char*)"TR_LIGHT_FLYWHEEL",
	(char*)"TR_DIFFERENTIAL",
	(char*)"TR_LIMITED_SLIP_DIFFERENTIAL",
	(char*)"TR_HIGH_PERFORMANCE_CLUTCH",
	(char*)"TR_6_SPEED_TRANSMISSION",
	(char*)"NO_DRY_SHOT_OF_NITROUS",
	(char*)"NO_WET_SHOT_OF_NITROUS",
	(char*)"NO_DIRECT_PORT_NITROUS_OXIDE",
	(char*)"EN_MUFFLER",
	(char*)"EN_COLD_AIR_INTAKE_SYSTEM",
	(char*)"EN_REPLACE_HEADERS",
	(char*)"EN_MILD_CAMSHAFT_AND_CAM_GEARS",
	(char*)"EN_CAT_BACK_EXHAUST_SYSTEM",
	(char*)"EN_HIGH_FLOW_INTAKE_MANIFOLD",
	(char*)"EN_LARGER_DIAMETER_DOWNPIPE",
	(char*)"EN_RACING_CAMSHAFT_AND_GEARS",
	(char*)"EN_PORT_AND_POLISH_HEADS",
	(char*)"EN_BLUEPRINT_THE_BLOCK",
	(char*)"EN_HIGH_FLOW_HEADERS",
	(char*)"TU_STAGE_1_TURBO_KIT",
	(char*)"TU_STAGE_2_TURBO_KIT",
	(char*)"TU_STAGE_3_TURBO_KIT",
	(char*)"SU_SPORT_SPRINGS_AND_SHOCKS",
	(char*)"SU_STRUT_TOWER_BAR",
	(char*)"SU_PERFORMANCE_SPRINGS_AND_SHOCKS",
	(char*)"SU_FRONT_AND_REAR_SWAY_BARS",
	(char*)"SU_FRONT_AND_REAR_TIE_BARS",
	(char*)"SU_UPGRADE_BUSHINGS",
	(char*)"SU_COIL_OVER_SUSPENSION_SYSTEM",
	(char*)"SU_CAMBER_KIT",
	(char*)"SU_LARGE_DIAMETER_SWAY_BARS",
	(char*)"BR_STREET_COMPOUND_BRAKE_PADS",
	(char*)"BR_STEEL_BRAIDED_BRAKE_LINES",
	(char*)"BR_CROSS_DRILLED_ROTORS",
	(char*)"BR_LARGE_DIAMETER_ROTORS",
	(char*)"BR_RACE_COMPOUND_BRAKE_PADS",
	(char*)"BR_CROSS_DRILLED_AND_SLOTTED_ROTORS",
	(char*)"BR_6_PISTON_RACING_CALIPERS",
	(char*)"EC_REMOVE_SPEED_LIMITER",
	(char*)"EC_PERFORMANCE_CHIP",
	(char*)"EC_HIGH_FLOW_FUEL_PUMP",
	(char*)"EC_FUEL_PRESSURE_REGULATOR",
	(char*)"EC_FUEL_RAIL",
	(char*)"EC_FUEL_FILTER",
	(char*)"EC_ENGINE_MANAGEMENT_UNIT",
	(char*)"EC_FUEL_INJECTORS",
	(char*)"TI_STREET_PERFORMANCE_TIRES",
	(char*)"TI_PRO_PERFORMANCE_TIRES",
	(char*)"TI_EXTREME_PERFORMANCE_TIRES",
	(char*)"AE_FRONT_BUMPER_L1",
	(char*)"AE_FRONT_BUMPER_L2",
	(char*)"AE_FRONT_BUMPER_L3",
	(char*)"AE_SIDESKIRTS_L1",
	(char*)"AE_SIDESKIRTS_L2",
	(char*)"AE_SIDESKIRTS_L3",
	(char*)"AE_REAR_BUMPER_L1",
	(char*)"AE_REAR_BUMPER_L2",
	(char*)"AE_REAR_BUMPER_L3",
	(char*)"AE_SPOILER_L1",
	(char*)"AE_SPOILER_L2",
	(char*)"AE_SPOILER_L3",
	(char*)"SC_STAGE_1_SUPERCHARGER_KIT",
	(char*)"SC_STAGE_2_SUPERCHARGER_KIT",
	(char*)"SC_STAGE_3_SUPERCHARGER_KIT"
};

DWORD DefaultPerformancePartHashes[]
{
	CT_bStringHash("WT_REMOVE_REAR_SEATS"),
	CT_bStringHash("WT_REMOVE_INTERIOR_PANELS"),
	CT_bStringHash("WT_LIGHTWEIGHT_WINDOWS"),
	CT_bStringHash("WT_LIGHTWEIGHT_SEATS"),
	CT_bStringHash("WT_LIGHTWEIGHT_DOORS"),
	CT_bStringHash("WT_FOAM_FILLED_INTERIOR"),
	CT_bStringHash("TR_SHORT_THROW_SHIFT_KIT"),
	CT_bStringHash("TR_LIGHT_FLYWHEEL"),
	CT_bStringHash("TR_DIFFERENTIAL"),
	CT_bStringHash("TR_LIMITED_SLIP_DIFFERENTIAL"),
	CT_bStringHash("TR_HIGH_PERFORMANCE_CLUTCH"),
	CT_bStringHash("TR_6_SPEED_TRANSMISSION"),
	CT_bStringHash("NO_DRY_SHOT_OF_NITROUS"),
	CT_bStringHash("NO_WET_SHOT_OF_NITROUS"),
	CT_bStringHash("NO_DIRECT_PORT_NITROUS_OXIDE"),
	CT_bStringHash("EN_MUFFLER"),
	CT_bStringHash("EN_COLD_AIR_INTAKE_SYSTEM"),
	CT_bStringHash("EN_REPLACE_HEADERS"),
	CT_bStringHash("EN_MILD_CAMSHAFT_AND_CAM_GEARS"),
	CT_bStringHash("EN_CAT_BACK_EXHAUST_SYSTEM"),
	CT_bStringHash("EN_HIGH_FLOW_INTAKE_MANIFOLD"),
	CT_bStringHash("EN_LARGER_DIAMETER_DOWNPIPE"),
	CT_bStringHash("EN_RACING_CAMSHAFT_AND_GEARS"),
	CT_bStringHash("EN_PORT_AND_POLISH_HEADS"),
	CT_bStringHash("EN_BLUEPRINT_THE_BLOCK"),
	CT_bStringHash("EN_HIGH_FLOW_HEADERS"),
	CT_bStringHash("TU_STAGE_1_TURBO_KIT"),
	CT_bStringHash("TU_STAGE_2_TURBO_KIT"),
	CT_bStringHash("TU_STAGE_3_TURBO_KIT"),
	CT_bStringHash("SU_SPORT_SPRINGS_AND_SHOCKS"),
	CT_bStringHash("SU_STRUT_TOWER_BAR"),
	CT_bStringHash("SU_PERFORMANCE_SPRINGS_AND_SHOCKS"),
	CT_bStringHash("SU_FRONT_AND_REAR_SWAY_BARS"),
	CT_bStringHash("SU_FRONT_AND_REAR_TIE_BARS"),
	CT_bStringHash("SU_UPGRADE_BUSHINGS"),
	CT_bStringHash("SU_COIL_OVER_SUSPENSION_SYSTEM"),
	CT_bStringHash("SU_CAMBER_KIT"),
	CT_bStringHash("SU_LARGE_DIAMETER_SWAY_BARS"),
	CT_bStringHash("BR_STREET_COMPOUND_BRAKE_PADS"),
	CT_bStringHash("BR_STEEL_BRAIDED_BRAKE_LINES"),
	CT_bStringHash("BR_CROSS_DRILLED_ROTORS"),
	CT_bStringHash("BR_LARGE_DIAMETER_ROTORS"),
	CT_bStringHash("BR_RACE_COMPOUND_BRAKE_PADS"),
	CT_bStringHash("BR_CROSS_DRILLED_AND_SLOTTED_ROTORS"),
	CT_bStringHash("BR_6_PISTON_RACING_CALIPERS"),
	CT_bStringHash("EC_REMOVE_SPEED_LIMITER"),
	CT_bStringHash("EC_PERFORMANCE_CHIP"),
	CT_bStringHash("EC_HIGH_FLOW_FUEL_PUMP"),
	CT_bStringHash("EC_FUEL_PRESSURE_REGULATOR"),
	CT_bStringHash("EC_FUEL_RAIL"),
	CT_bStringHash("EC_FUEL_FILTER"),
	CT_bStringHash("EC_ENGINE_MANAGEMENT_UNIT"),
	CT_bStringHash("EC_FUEL_INJECTORS"),
	CT_bStringHash("TI_STREET_PERFORMANCE_TIRES"),
	CT_bStringHash("TI_PRO_PERFORMANCE_TIRES"),
	CT_bStringHash("TI_EXTREME_PERFORMANCE_TIRES"),
	CT_bStringHash("AE_FRONT_BUMPER_L1"),
	CT_bStringHash("AE_FRONT_BUMPER_L2"),
	CT_bStringHash("AE_FRONT_BUMPER_L3"),
	CT_bStringHash("AE_SIDESKIRTS_L1"),
	CT_bStringHash("AE_SIDESKIRTS_L2"),
	CT_bStringHash("AE_SIDESKIRTS_L3"),
	CT_bStringHash("AE_REAR_BUMPER_L1"),
	CT_bStringHash("AE_REAR_BUMPER_L2"),
	CT_bStringHash("AE_REAR_BUMPER_L3"),
	CT_bStringHash("AE_SPOILER_L1"),
	CT_bStringHash("AE_SPOILER_L2"),
	CT_bStringHash("AE_SPOILER_L3"),
	CT_bStringHash("SC_STAGE_1_SUPERCHARGER_KIT"),
	CT_bStringHash("SC_STAGE_2_SUPERCHARGER_KIT"),
	CT_bStringHash("SC_STAGE_3_SUPERCHARGER_KIT")
};

int EUExclusiveCars[]
{
	7, // CORSA
	6 // PEUGOT106
};

int USExclusiveCars[]
{
	5, // CIVIC
	18 // RSX
};