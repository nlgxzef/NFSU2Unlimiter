#pragma once

#include "CarPart.h"

struct CarPartIndex
{
	CarPart* Part;
	int NumParts;
};

struct CarPartPack
{
	int Padding_1605[2];
	unsigned int Version;
	const char* StringTable;
	unsigned int StringTableSize;
	/*CarPartAttributeTable*/ DWORD* AttributeTableTable;
	unsigned int NumAttributeTables;
	/*CarPartAttribute*/ DWORD* AttributesTable;
	unsigned int NumAttributes;
	unsigned int* TypeNameTable;
	unsigned int NumTypeNames;
	/*CarPartModelTable*/ DWORD* ModelTable;
	unsigned int NumModelTables;
	CarPart* PartsTable;
	unsigned int NumParts;
};

struct CarPartDatabase
{
	bTList<CarPartPack> CarPartPackList;
	int NumPacks;
	int NumParts;
	int NumBytes;
	CarPartIndex PaintPart_Gloss[3];
	CarPartIndex PaintPart_Metallic[3];
	CarPartIndex PaintPart_Pearl[3];
	CarPartIndex PaintPart_Vinyl[3];
	CarPartIndex PaintPart_Rims[3];
	CarPartIndex PaintPart_Caliper[3];
	CarPartIndex VinylPart_All[3];
	CarPartIndex VinylPart_Body[3];
	CarPartIndex VinylPart_Hood[3];
	CarPartIndex VinylPart_Side[3];
	CarPartIndex VinylPart_Manufacturer[3];
};

CarPartDatabase* DBCarPart;