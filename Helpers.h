#pragma once

inline bool DoesFileExist(char const* path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
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
	return *(bool*)((*(DWORD*)_CarTypeInfoArray) + CarTypeID * 0x890 + 0x88A) || CarConfigs[CarTypeID].Main.GameType == 3;
}

bool IsRacer(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return *(BYTE*)((*(DWORD*)_CarTypeInfoArray) + CarTypeID * 0x890 + 0x844) == 0;
}

bool IsTraffic(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return *(BYTE*)((*(DWORD*)_CarTypeInfoArray) + CarTypeID * 0x890 + 0x844) == 2;
}

int GetCarTypeIDFromHash(DWORD CarTypeNameHash)
{
	for (int i = 0; i < CarCount; i++)
	{
		if (*(DWORD*)((*(DWORD*)_CarTypeInfoArray) + i * 0x890 + 0xD0) == CarTypeNameHash)
			return i;
	}
	return ReplacementCar;
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

	// Not sure
	DWORD SpoolBaseD = *(DWORD*)((*(DWORD*)_CarTypeInfoArray) + CarTypeID * 0x890 + 0x358);
	DWORD SpoolTopD = *(DWORD*)((*(DWORD*)_CarTypeInfoArray) + CarTypeID * 0x890 + 0x638);
	float SpoolBase = *(float*)&SpoolBaseD;
	float SpoolTop = *(float*)&SpoolTopD;

	return (SpoolBase == 0.0f) && (SpoolTop == 0.0f);
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
	if (!CarConfigs[CarTypeID].Main.SyncVisualPartsWithPhysics && CarConfigs[CarTypeID].BodyShop.Brakes != 0) return 0;
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
