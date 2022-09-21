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
	sprintf(CarININame, "UnlimiterData\\%s.ini", GetCarTypeName(TheCounter));

	CIniReader UnlimiterCarData(CarININame);
	return (UnlimiterCarData.ReadInteger("Main", "InitiallyUnlocked", 0) == 1) || AllNewCarsInitiallyUnlocked;
}

bool CanCarBeDrivenByAI(BYTE CarTypeID)
{
	sprintf(CarININame, "UnlimiterData\\%s.ini", GetCarTypeName(TheCounter));

	CIniReader UnlimiterCarData(CarININame);
	return (UnlimiterCarData.ReadInteger("Main", "CanBeDrivenByAI", 1) == 1) || AllNewCarsCanBeDrivenByAI;
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