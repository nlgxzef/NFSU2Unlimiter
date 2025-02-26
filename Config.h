#pragma once
#include "InGameFunctions.h"

enum ConfigState : int
{
	ConfigState_Enabled = 1,
	ConfigState_Disabled = 0,
	ConfigState_Default = -1,
};

struct ShadedConfig
{
	ConfigState EngineNeon;
	ConfigState CabinNeon;
	ConfigState TrunkNeon;
};

struct CarConfig : ShadedConfig
{
	ConfigState LinkLicensePlateToTrunk;
	ConfigState ShowTrunkUnderInFE;
	ConfigState RemoveCentreBrakeWithCustomSpoiler;
	ConfigState HasSunroof;
	ConfigState InitiallyUnlocked;
	ConfigState CanBeDrivenByAI;
};

struct RimBrandConfig
{
	DWORD Name;
	DWORD Icon;
	DWORD String;
	bool NoRimSize;
	bool HideBrandName;
	bool Suv;
	bool Car;
};

struct GeneralConfig : ShadedConfig
{
	std::vector<RimBrandConfig*> RimBrands;
};

std::map<DWORD, CarConfig*> g_CarConfigs;
GeneralConfig g_Config;

CarConfig* GetCarConfigByType(int type)
{
	auto name = GetCarTypeName(type);
	auto hash = bStringHash(name);
	if (g_CarConfigs.count(hash) > 0)
	{
		return g_CarConfigs[hash];
	}

	return nullptr;
}

void InitSharedConfig(ShadedConfig* shared, CIniReader& iniReader)
{
	shared->EngineNeon = (ConfigState)iniReader.ReadInteger("Specialties", "EngineNeon", ConfigState_Default);
	shared->CabinNeon = (ConfigState)iniReader.ReadInteger("Specialties", "CabinNeon", ConfigState_Default);
	shared->TrunkNeon = (ConfigState)iniReader.ReadInteger("Specialties", "TrunkNeon", ConfigState_Default);
}

void InitCarConfig(char* fileName)
{
	sprintf(CarININame, "UnlimiterData\\%s", fileName);
	CIniReader iniReader(CarININame);

	auto carConfig = new CarConfig();

	carConfig->LinkLicensePlateToTrunk = (ConfigState)iniReader.ReadInteger("CarRenderInfo", "LinkLicensePlateToTrunk", ConfigState_Default);
	carConfig->ShowTrunkUnderInFE = (ConfigState)iniReader.ReadInteger("CarRenderInfo", "ShowTrunkUnderInFE", ConfigState_Default);
	carConfig->RemoveCentreBrakeWithCustomSpoiler = (ConfigState)iniReader.ReadInteger("CarRenderInfo", "RemoveCentreBrakeWithCustomSpoiler", ConfigState_Default);
	carConfig->HasSunroof = (ConfigState)iniReader.ReadInteger("CarRenderInfo", "HasSunroof", ConfigState_Default);

	carConfig->InitiallyUnlocked = (ConfigState)iniReader.ReadInteger("Main", "InitiallyUnlocked", ConfigState_Default);
	carConfig->CanBeDrivenByAI = (ConfigState)iniReader.ReadInteger("Main", "CanBeDrivenByAI", ConfigState_Default);

	InitSharedConfig(carConfig, iniReader);

	int size = strlen(fileName) - 4;
	memcpy(CarININame, fileName, size);
	CarININame[size] = 0;
	g_CarConfigs[bStringHash(CarININame)] = carConfig;
}

void InitCarConfigs()
{
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile("UnlimiterData\\*", &findFileData);

	do
	{
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			InitCarConfig(findFileData.cFileName);
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
}

void InitRimBrands()
{
	CIniReader RimBrandsINI("UnlimiterData\\_RimBrands.ini");
	int RimBrandsCount = RimBrandsINI.ReadInteger("RimBrands", "NumberOfRimBrands", -1);
	for (int i = 0; i <= RimBrandsCount; i++)
	{
		auto rimBrand = new RimBrandConfig();
		char RimBrandID[15];
		sprintf(RimBrandID, "Brand%d", i);

		rimBrand->Name = bStringHash(RimBrandsINI.ReadString(RimBrandID, "BrandName", ""));
		rimBrand->Icon = bStringHash(RimBrandsINI.ReadString(RimBrandID, "Texture", ""));
		rimBrand->String = bStringHash(RimBrandsINI.ReadString(RimBrandID, "String", ""));
		rimBrand->NoRimSize = RimBrandsINI.ReadInteger(RimBrandID, "NoRimSize", 0) == 1;
		rimBrand->HideBrandName = RimBrandsINI.ReadInteger(RimBrandID, "HideBrandName", 0) == 1;
		rimBrand->Suv = RimBrandsINI.ReadInteger(RimBrandID, "Suv", 0) == 1;
		rimBrand->Car = RimBrandsINI.ReadInteger(RimBrandID, "Car", 0) == 1;

		g_Config.RimBrands.push_back(rimBrand);
	}
}

void InitConfig()
{
	InitCarConfigs();

	CIniReader GeneralINI("UnlimiterData\\_General.ini");

	InitSharedConfig(&g_Config, GeneralINI);
	InitRimBrands();
}