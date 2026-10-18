#pragma once

inline bool DoesFileExist(char const* path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

int Clamp(int value, int min, int max)
{
	if (value < min) value = min;
	else if (value > max) value = max;

	return value;
}

bool IsUG1(int CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarConfigs[CarTypeID].Main.GameType == 1;
}

bool IsUG2(int CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarConfigs[CarTypeID].Main.GameType == 2;
}

bool IsSUV(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarTypeInfoArray[CarTypeID].GetIsSUV() || CarConfigs[CarTypeID].Main.GameType == 3;
}

bool IsRacer(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarTypeInfoArray[CarTypeID].IsRacer();
}

bool IsCop(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarTypeInfoArray[CarTypeID].IsCop();
}

bool IsTraffic(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarTypeInfoArray[CarTypeID].IsTraffic();
}

bool IsSkinnable(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarTypeInfoArray[CarTypeID].IsSkinnable();
}

bool IsInitiallyUnlocked(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarConfigs[CarTypeID].Main.InitiallyUnlocked == 1 || AllNewCarsInitiallyUnlocked;
}

bool CanCarBeDrivenByAI(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return CarConfigs[CarTypeID].Main.CanBeDrivenByAI || AllNewCarsCanBeDrivenByAI;
}

bool UsesSupercharger(BYTE CarTypeID)
{
	if (CarConfigs[CarTypeID].Main.InductionType == 1) return 1;
	return 0;
}

DWORD* GetCarTypeInfo(int CarTypeID)
{
	if (CarTypeID < 0 || CarTypeID >= CarCount)
		CarTypeID = (ReplacementCar >= 0 && ReplacementCar < CarCount) ? ReplacementCar : 0;

	return GetCarTypeInfo_Game(CarTypeID);
}

char* GetCarTypeName(int CarTypeID)
{
	return (char*)GetCarTypeInfo(CarTypeID);
}

DWORD* GetCarTypeInfoFromHash(DWORD CarTypeNameHash)
{
	for (int i = 0; i < CarCount; i++)
	{
		if (CarTypeInfoArray[i].CarTypeNameHash == CarTypeNameHash)
			return GetCarTypeInfo(i);
	}

	return GetCarTypeInfo(ReplacementCar);
}

int GetCarTypeIDFromHash(DWORD CarTypeNameHash)
{
	for (int i = 0; i < CarCount; i++)
	{
		if (CarTypeInfoArray[i].CarTypeNameHash == CarTypeNameHash)
			return i;
	}
	return ReplacementCar;
}

void FillUpPerformanceConfig()
{
	int PerfConfigFilledSize = 0;

	for (int i = 0; i < 512; i++)
	{
		if (PerfConfigTables[i]) PerfConfigFilledSize++;
		else
		{
			PerfConfigTables[i] = PerfConfigTables[i - PerfConfigFilledSize];
		}
	}
}

bool IsMenuEmpty_BodyShop(int CarTypeID)
{
	if (CarConfigs[CarTypeID].BodyShop.FrontBumper != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.RearBumper != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Skirt != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Fender != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Quarter != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Spoiler != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Hood != 0) return 0;
	if (!CarConfigs[CarTypeID].Main.SyncVisualPartsWithPhysics && CarConfigs[CarTypeID].BodyShop.Engine != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Trunk != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.RoofScoops != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Interior != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Roof != 0) return 0;
	if (!CarConfigs[CarTypeID].Main.SyncBrakesWithPhysics && CarConfigs[CarTypeID].BodyShop.Brakes != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Headlights != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Taillights != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Mirrors != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Exhaust != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Rims != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.CarbonFiber != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.WideBodyKits != 0) return 0;
	if (CarConfigs[CarTypeID].BodyShop.Attachments != 0) return 0;

	return 1;
}

bool IsMenuEmpty_Performance(int CarTypeID)
{
	if (CarConfigs[CarTypeID].Performance.Engine != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.ECU != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.Transmission != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.Chassis != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.Nitrous != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.Tires != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.Brakes != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.WeightReduction != 0) return 0;
	if (CarConfigs[CarTypeID].Performance.Induction != 0) return 0;
	if (!CarConfigs[CarTypeID].Main.SyncVisualPartsWithPhysics && CarConfigs[CarTypeID].Performance.Aerodynamics != 0) return 0;
	if ((*(int*)_FEDatabase == 1 && !*(int*)0x83898C) && CarConfigs[CarTypeID].Performance.TestDyno != 0) return 0;

	return 1;
}

bool IsMenuEmpty_Paint(int CarTypeID)
{
	if (CarConfigs[CarTypeID].Paint.Paint != 0) return 0;
	if (CarConfigs[CarTypeID].Paint.Vinyls != 0) return 0;
	if (CarConfigs[CarTypeID].Paint.Decals != 0) return 0;

	return 1;
}

#include "Specialties.h"
bool IsMenuEmpty_Specialties(int CarTypeID)
{
	auto& S = CarConfigs[CarTypeID].Specialties;

	// The game's own garage exclusion, at 0x559727
	if (!IsCustomizingFromGarage() && S.CustomGauges != 0) return 0;

	if (IsSpecialtyOffered(SPEC_Neon, S.Neon)) return 0;
	if (IsSpecialtyOffered(SPEC_WindowTint, S.WindowTint)) return 0;
	if (IsSpecialtyOffered(SPEC_HeadlightColor, S.HeadlightColor)) return 0;
	if (IsSpecialtyOffered(SPEC_NosPurge, S.NosPurge)) return 0;
	if (IsSpecialtyOffered(SPEC_ExhaustFlame, S.ExhaustFlame)) return 0;
	if (IsSpecialtyOffered(SPEC_TireSmoke, S.TireSmoke)) return 0;
	if (IsSpecialtyOffered(SPEC_Hydraulics, S.Hydrualics)) return 0;
	if (IsSpecialtyOffered(SPEC_TrunkAudio, S.TrunkAudio)) return 0;
	if (IsSpecialtyOffered(SPEC_Spinners, S.Spinners)) return 0;
	if (IsSpecialtyOffered(SPEC_SplitHoods, S.SplitHoods)) return 0;
	if (IsSpecialtyOffered(SPEC_Doors, S.Doors)) return 0;
	if (IsSpecialtyOffered(SPEC_LicensePlate, S.LicensePlate)) return 0;

	return 1;
}

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

void SetRimBrandName(RimBrand& Brand, const char* Name)
{
	Brand.BrandNameHash = bStringHash((char*)Name);

	strncpy(Brand.BrandName, Name ? Name : "", sizeof(Brand.BrandName) - 1);
	Brand.BrandName[sizeof(Brand.BrandName) - 1] = 0;
}

void SetFileName(char *File, const char* Name, size_t size = 64)
{
	const char* src = Name ? Name : "";
	size_t len = strlen(src);

	// Find first dot
	const char* dot = strchr(src, '.');

	// If dot exists, trim everything after it
	if (dot && dot != src)
	{
		len = (size_t)(dot - src);
	}

	// Copy trimmed name safely
	if (len >= size)
		len = size - 1;

	memcpy(File, src, len);
	File[len] = '\0';
}

char ProfileNameBuf[32];
char* GetProfileName()
{
	char* result = (char*)0x83A9E0;
	if (!result[0]) return (char*)"UNDRGND2";

	strcpy(ProfileNameBuf, result);
	return ProfileNameBuf;
}
char* GetProfileNameUpper()
{
	return strupr(GetProfileName());
}

void SetExitGameFlag()
{
	injector::WriteMemory<int>(0x864F4C, 1, true); // ExitTheGameFlag
}

// mINI stuff

int mINI_ReadInteger(mINI::INIStructure& ini, std::string Section, std::string Key, int DefaultValue = 0)
{
	int result = DefaultValue;

	if (ini.has(Section) && ini[Section].has(Key))
	{
		try
		{
			if (ini[Section][Key].starts_with("0x") || ini[Section][Key].starts_with("-0x")) result = std::stol(ini[Section][Key], 0, 16);
			else result = std::stol(ini[Section][Key]);
		}
		catch (const std::exception& ex)
		{
			result = DefaultValue;
		}
	}

	return result;
}

float mINI_ReadFloat(mINI::INIStructure& ini, std::string Section, std::string Key, float DefaultValue = 0.0f)
{
	float result = DefaultValue;

	if (ini.has(Section) && ini[Section].has(Key))
	{
		try
		{
			result = std::stof(ini[Section][Key]);
		}
		catch (const std::exception& ex)
		{
			result = DefaultValue;
		}
	}

	return result;
}

char* mINI_ReadString(mINI::INIStructure& ini, std::string Section, std::string Key, const char* DefaultValue = "")
{
	char* result = (char*)DefaultValue;

	if (ini.has(Section) && ini[Section].has(Key))
	{
		try
		{
			result = (char*)ini[Section][Key].c_str();
		}
		catch (const std::exception& ex)
		{
			result = (char*)DefaultValue;
		}
	}

	return result;
}

DWORD mINI_ReadHash(mINI::INIStructure& ini, std::string Section, std::string Key, DWORD DefaultValue = -1)
{
	DWORD result = DefaultValue;

	if (ini.has(Section) && ini[Section].has(Key))
	{
		try
		{
			result = bStringHash((char*)ini[Section][Key].c_str());
		}
		catch (const std::exception& ex)
		{
			result = DefaultValue;
		}
	}

	return result;
}

DWORD mINI_ReadHashS(mINI::INIStructure& ini, std::string Section, std::string Key, const char* DefaultValue = "")
{
	DWORD result = bStringHash((char*)DefaultValue);

	if (ini.has(Section) && ini[Section].has(Key))
	{
		try
		{
			result = bStringHash((char*)ini[Section][Key].c_str());
		}
		catch (const std::exception& ex)
		{
			result = bStringHash((char*)DefaultValue);
		}
	}

	return result;
}
