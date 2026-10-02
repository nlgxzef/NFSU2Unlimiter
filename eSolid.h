#pragma once

struct eStripEntry // Taken from MW, doesn't match with U2
{
	unsigned int DataOffset;
	unsigned __int16 DataSize;
	unsigned __int16 Flags;
	unsigned __int8 NumVerts;
	unsigned __int8 PolyGroupNumber;
	char TextureNumber;
	char LightMaterialIndex;
	unsigned __int8 VertexDescription;
	unsigned __int8 VertexFormat;
	unsigned __int16 DataDisplayListSize;
};

struct eSolidPlatInfo
{
	unsigned __int16 Version;
	unsigned __int16 StripFlags;
	unsigned __int16 NumStrips;
	unsigned __int16 NumIdxClrTable;
	unsigned int SizeofStripData;
	unsigned int DataOffset0;
	unsigned int DataOffset1;
	unsigned int DataOffset2;
	unsigned int DataOffset3;
	eStripEntry* StripEntryTable;
	unsigned __int8* StripDataStart;
};


struct eSolidPlatInterface
{
	eSolidPlatInfo* PlatInfo;
};

struct eModel; // shut up C2027

struct eSolid : eSolidPlatInterface, bTNode<eSolid>
{
	unsigned __int8 Version;
	unsigned __int8 EndianSwapped;
	unsigned __int16 Flags;
	unsigned int NameHash;
	short NumPolys;
	short NumVerts;
	char NumBones;
	char NumTextureTableEntries;
	char NumLightMaterials;
	char NumPositionMarkerTableEntries;
	int ReferencedFrameCounter;
	float AABBMinX;
	float AABBMinY;
	float AABBMinZ;
	DWORD/*eTextureEntry*/* pTextureTable;
	float AABBMaxX;
	float AABBMaxY;
	float AABBMaxZ;
	DWORD/*eLightMaterialEntry*/* LightMaterialTable;
	bMatrix4 PivotMatrix;
	ePositionMarker* PositionMarkerTable;
	DWORD/*eNormalSmoother*/* NormalSmoother;
	bTList<eModel> ModelList;
	DWORD/*eDamageVertex*/* DamageVertexTable;
	DWORD/*eConnectivityData*/* ConnectivityData;
	float Volume;
	float Density;
	float field_0xA0;
	char Name[28];
};
