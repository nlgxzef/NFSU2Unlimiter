#pragma once

inline bool DoesFileExist(char const* path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool IsSUV(BYTE CarTypeID)
{
	if (CarTypeID >= CarCount) return 0;
	return *(bool*)((*(DWORD*)_CarTypeInfoArray) + CarTypeID * 0x890 + 0x88A);
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

bool IsInitiallyUnlocked(BYTE CarTypeID)
{
	auto config = GetCarConfigByType(CarTypeID);
	if (config && config->InitiallyUnlocked != ConfigState_Default)
	{
		return config->InitiallyUnlocked == ConfigState_Enabled;
	}

	return AllNewCarsInitiallyUnlocked;
}

bool CanCarBeDrivenByAI(BYTE CarTypeID)
{
	auto config = GetCarConfigByType(CarTypeID);
	if (config && config->CanBeDrivenByAI != ConfigState_Default)
	{
		return config->CanBeDrivenByAI == ConfigState_Enabled;
	}

	return AllNewCarsCanBeDrivenByAI;
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