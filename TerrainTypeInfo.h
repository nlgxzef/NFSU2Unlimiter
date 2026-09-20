#pragma once

enum TerrainType : __int32
{
	TERRAIN_TYPE_NONE = 0x0,
	TERRAIN_TYPE_ROAD = 0x1,
	TERRAIN_TYPE_ROAD_WET = 0x2,
	TERRAIN_TYPE_ROAD_DRIFT = 0x3,
	TERRAIN_TYPE_ROAD_SMOKE_1 = 0x4,
	TERRAIN_TYPE_ROAD_SMOKE_2 = 0x5,
	TERRAIN_TYPE_ROAD_SMOKE_3 = 0x6,
	TERRAIN_TYPE_BRIDGE = 0x7,
	TERRAIN_TYPE_DIRT = 0x8,
	TERRAIN_TYPE_GRAVEL = 0x9,
	TERRAIN_TYPE_ROUGH_ROAD = 0xA,
	TERRAIN_TYPE_COBBLESTONE = 0xB,
	TERRAIN_TYPE_STAIRS = 0xC,
	TERRAIN_TYPE_PUDDLE = 0xD,
	TERRAIN_TYPE_DEEP_WATER = 0xE,
	TERRAIN_TYPE_GRASS = 0xF,
	TERRAIN_TYPE_SIDEWALK = 0x10,
	TERRAIN_TYPE_WOOD = 0x11,
	TERRAIN_TYPE_PLASTIC = 0x12,
	TERRAIN_TYPE_GLASS = 0x13,
	TERRAIN_TYPE_SOLID_WALL = 0x14,
	TERRAIN_TYPE_SEE_THROUGH_WALL = 0x15,
	TERRAIN_TYPE_PLANT = 0x16,
	TERRAIN_TYPE_POST = 0x17,
	TERRAIN_TYPE_PILLAR = 0x18,
	TERRAIN_TYPE_METAL_GRATE = 0x19,
	TERRAIN_TYPE_METAL = 0x1A,
	TERRAIN_TYPE_CHAINLINK = 0x1B,
	TERRAIN_TYPE_CAR = 0x1C,
	NUM_TERRAIN_TYPES = 0x1D,
};

struct TerrainTypeInfo
{
	char* Name;
	unsigned int NameHash;
	TerrainType Type;
	char ShouldTireMakeRoadSound;
	char ShouldCameraGoThroughIt;
	char* SkidTextureName;
	float SkidWidthMultiplier;
	float field_0x18;
	float RoadNoiseGridSpacingInverse;
	float RoadNoiseScale;
	float ElevationOffset;
	float StaticFrictionMultiplier;
	float DynamicFrictionMultiplier;
	float RollingFrictionMultiplier;
	float CollisionPriority;
	float ForceMagnitude;
	float ForceFrequency;
	unsigned int DebugTopologyColour;
	const char* DriveOverEmitterGroup;
	float DriveOverMinSpeed;
	float DriveOverMaxSpeed;
	const char* DriveOver2EmitterGroup;
	float DriveOver2MinSpeed;
	float DriveOver2MaxSpeed;
	const char* SkidSmokeEmitterGroup;
	float SkidSmokeMinSpeed;
	float SkidSmokeMaxSpeed;
	const char* TireSpewEmitterGroup;
	float TireSpewMinSpeed;
	float TireSpewMaxSpeed;
	const char* BottomOutEmitterGroup;
	float BottomOutMinImpact;
	float BottomOutMaxImpact;
	const char* CollisionImpactEmitterGroup;
	float CollisionImpactMinForce;
	float CollisionImpactMaxForce;
	const char* CollisionScrapeEmitterGroup;
	float CollisionScrapeMinSpeed;
	float CollisionScrapeMaxSpeed;
};

TerrainTypeInfo* TerrainTypeInfoTable = (TerrainTypeInfo*)0x800EE0;