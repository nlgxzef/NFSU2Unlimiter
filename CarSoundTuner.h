#pragma once

bool CarSoundTunerEnabled, SkipLegacyCSTCheck, ConvertFromLegacyCST, ForceUpgradeFromLegacyCST, ExportCarSoundData;
int BigFileVFSHandlePoolSize = 64;

#define MAX_FILES 128

#define SIZE_OF_g_SND_ENGINE_DATA 0x7ED8F8
#define SIZE_OF_g_SND_DUAL_GINSU 0x7ED8FC
#define SIZE_OF_g_SND_CARDATAMAPPING 0x7ED900
#define SIZE_OF_g_SND_SHIFTPATTERNS 0x7ED904
#define SIZE_OF_g_SND_SWEETNERDATASET 0x7ED90C
#define SIZE_OF_g_SND_ACCELFROMIDLE 0x7ED910
#define SIZE_OF_g_SND_TURBODATASET 0x7ED914

#define g_SND_ENGINE_DATA 0x7F0DE0
#define g_SND_DUAL_GINSU 0x7F1AA8
#define g_SND_CARDATAMAPPING 0x7F2230
#define g_SND_SHIFTPATTERNS 0x7F2860
#define g_SND_SWEETNERDATASET 0x7F3040
#define g_SND_ACCELFROMIDLE 0x7F3248
#define g_SND_TURBODATASET 0x7F3338

// One entry per car, four upgrade levels of three ids each. The game indexes it as a flat array
// of the 12 byte triples (mapping * 4 + level), which is why the shift pattern and turbo call
// sites further down are pointed at g_CDM + 1 and g_CDM + 2 rather than at g_CDM itself.
struct CarDataMapping
{
	int StockEngineData;
	int StockShiftPatternData;
	int StockTurboData;

	int StreetEngineData;
	int StreetShiftPatternData;
	int StreetTurboData;

	int ProEngineData;
	int ProShiftPatternData;
	int ProTurboData;

	int ExtremeEngineData;
	int ExtremeShiftPatternData;
	int ExtremeTurboData;
};

// No structure needed for CarTypeMapping, it is a single integer.

struct EngineData
{
	char* MainRAMBankName = (char*)" ";
	char* AuxRAMBankName = (char*)" ";
	int CarID;
	char* GinsuAccel = (char*)" ";
	int UseDualGinsu; // 0 on Hummer and Skyline, 1 on the rest
	int GinsuDecelID;
	int AccelFromIdleID;
	float MaxRPM;
	float MinRPM;
	int AEMSVol;
	int GinsuAccelVol;
	float AccelDeltaRPMThreshold;
	float AEMSMixLRPM;
	float GinsuMixLRPM;
	float AEMSMixSRPM;
	float GinsuMixSRPM;
	float AEMSMixNRPM; // UnkFloat1
	float GinsuMixNRPM; // UnkFloat2
	int GinsuLowPassCutoff; // Unk2
};

struct DualGinsu
{
	char* GinsuDecelFileName = (char*)" ";
	int AEMSDecelVol;
	int GinsuDecelVol;
	int MinRPM;
	int MaxRPM;
	float FadeIn;
	float FadeOut;
	float DecelDeltaRPMThreshold;
	int DecelPitchOffset; // Unk0
	float DecelAEMSMixLRPM;
	float DecelGinsuMixLRPM;
	float DecelAEMSMixSRPM;
	float DecelGinsuMixSRPM;
};

struct ShiftPattern // unsure
{
	char* BankName = (char*)" ";
	int UpVolShift; // Unk1
	float UpShiftSoundDelay; // UnkFloat1
	int UpDisengageFallRPM; // Unk2
	int UpDisengageFallT; // Unk3
	int UpEngagingRiseRPM; // Unk4
	int UpEngagingRiseT; // Unk5
	int UpEngagingAttackT; // Unk6
	float UpEngagingAttackVol; // UnkFloat2
	int LFOVolAmp; // Unk7
	int LFOVolFreq; // Unk8
	int LFOVolDecayTime; // Unk9
	int LFORPMAmp; // Unk10
	int LFORPMFreq; // Unk11
	int LFORPMDecayTime; // Unk12
	int DownVolShift; // Unk13
	float DownShiftSoundDelay; // UnkFloat3
	int DownDisengageFallRPM; // Unk14
	int DownDisengageFallT; // Unk15
	int DownEngagingRiseRPM; // Unk16
	int DownEngagingRiseT; // Unk17
	int DownEngagingFallRPM; // Unk18
	int DownEngagingFallT; // Unk19
	float DownReattachScale; // UnkFloat4
};

struct SweetnerDataSet
{
	char* SweetBank = (char*)" ";
	int ShiftSweetsVol; // SputterVol
	int SputterVol; // ShiftSweetsVol
};

struct TurboDataSet // Done
{
	char* TurboBank = (char*)" ";
	int SpoolVol; // Unk1
	int BlowoffVol1; // Unk2
	int BlowoffVol2; // Unk3
	float ChargeTime; // UnkFloat1
	float LeakRate; // ChargeTime
};

struct AccelFromIdle
{
	float PeakVol; // UnkFloat1
	int PeakRPM; // Unk1
	int PeakT; // Unk2
	int ResumeT; // Unk3
	int InteruptT; // Unk4
};

// Allocate memory for our stuff
CarDataMapping g_CDM[MAX_FILES];
EngineData g_ED[MAX_FILES];
DualGinsu g_DG[MAX_FILES];
ShiftPattern g_SH[MAX_FILES];
SweetnerDataSet g_SDS[MAX_FILES];
TurboDataSet g_TDS[MAX_FILES];
AccelFromIdle g_AFI[MAX_FILES];

// Counts
int CarDataMappingCount = 0;
int EngineDataCount = 0;
int DualGinsuCount = 0;
int ShiftPatternCount = 0;
int SweetnerDataCount = 0;
int TurboDataCount = 0;
int AccelFromIdleCount = 0;

// Keep file names in order they got read, parallel with arrays to link all the data properly.
struct FileName
{
	char _n[64];
};

// Resolved once, before the hook goes in, by calling the game's own function. The standalone mod
// carried a hand written copy of the 31 case switch instead, which had to be kept in sync by hand.
constexpr int VanillaCarTypeMappingCount = 31; // the switch at 0x459240 covers CarTypeID 0 to 30
int VanillaCarTypeMapping[VanillaCarTypeMappingCount];
int VanillaCarTypeMappingDefault;

// Filled at load for every car, so the hook itself only does an array lookup.
std::vector<int> CarTypeMapping;

// The game holds on to the bank name pointers for the rest of the process, so the strings have to
// outlive the INIStructure they came out of. std::list never moves its elements.
std::list<std::string> CarSoundStringPool;

char* PersistCarSoundString(const char* Value)
{
	CarSoundStringPool.emplace_back(Value ? Value : "");
	return (char*)CarSoundStringPool.back().c_str();
}

// Leaves the current pointer alone when the key is absent. Vanilla strings live in the executable
// and never move, so keeping them costs nothing.
void ReadCarSoundString(mINI::INIStructure& Ini, std::string Section, std::string Key, char*& Value)
{
	if (Ini.has(Section) && Ini[Section].has(Key)) Value = PersistCarSoundString(Ini[Section][Key].c_str());
}

// Gets the index of the file name read from the key in the given list
int FindMatchingCarSoundFileName(mINI::INIStructure& Ini, std::string Section, std::string Key, std::vector<FileName> &FileList)
{
	int result = -1;

	FileName toMatch;
	SetFileName(toMatch._n, mINI_ReadString(Ini, Section, Key));

	for (int i = 0; i < FileList.size(); i++)
	{
		if (!strcmp(toMatch._n, FileList[i]._n)) return i;
	}

	return result;
}

char* GetNameFromIndex(int index, std::vector<FileName>& FileList)
{
	if (index == -1 || index > MAX_FILES) return (char*)"";
	return FileList[index]._n;
}

void SnapshotVanillaCarTypeMapping()
{
	for (int i = 0; i < VanillaCarTypeMappingCount; i++)
	{
		VanillaCarTypeMapping[i] = GetCarTypeMapping_Game(i);
	}

	// Anything past the switch takes the default case, which is what add-on cars get
	VanillaCarTypeMappingDefault = GetCarTypeMapping_Game(VanillaCarTypeMappingCount);
}
int GetVanillaCarTypeMapping(int CarTypeID)
{
	if (CarTypeID >= 0 && CarTypeID < VanillaCarTypeMappingCount) return VanillaCarTypeMapping[CarTypeID];
	return VanillaCarTypeMappingDefault;
}

int GetCarTypeMapping(int CarTypeID)
{
	if (CarTypeID >= 0 && CarTypeID < (int)CarTypeMapping.size()) return CarTypeMapping[CarTypeID];
	return GetVanillaCarTypeMapping(CarTypeID);
}

void SnapshotCarTypeMapping()
{
	for (int i = 0; i < CarCount; i++)
	{
		CarTypeMapping.push_back(GetCarTypeMapping(i));
	}
}

// Returns the first car name using the given mapping
char* GetCarTypeNameFromMapping(int mapping)
{
	for (int i = 0; i < CarCount; i++)
	{
		if (GetCarTypeMapping(i) == mapping) return GetCarTypeName(i);
	}

	return (char*)"";
}

int CarTypeMapping_Temp;

void __declspec(naked) GetCarTypeMappingCodeCave()
{
	_asm
	{
		mov eax, [esp + 4]
		mov CarTypeMapping_Temp, eax
		pushad
	}

	CarTypeMapping_Temp = GetCarTypeMapping(CarTypeMapping_Temp);

	_asm
	{
		popad
		mov eax, CarTypeMapping_Temp
		retn
	}
}

void LoadVanillaCarSoundData()
{
	// Start from the vanilla data. Our arrays are longer than the game's, so wrap around to fill
	// the tail rather than leave it zeroed: an id the game clamps into that range then still
	// produces a real sound instead of a null bank name.
	CarDataMappingCount = injector::ReadMemory<int>(SIZE_OF_g_SND_CARDATAMAPPING, true) / sizeof(CarDataMapping);
	EngineDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_ENGINE_DATA, true) / sizeof(EngineData);
	DualGinsuCount = injector::ReadMemory<int>(SIZE_OF_g_SND_DUAL_GINSU, true) / sizeof(DualGinsu);
	ShiftPatternCount = injector::ReadMemory<int>(SIZE_OF_g_SND_SHIFTPATTERNS, true) / sizeof(ShiftPattern);
	SweetnerDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_SWEETNERDATASET, true) / sizeof(SweetnerDataSet);
	TurboDataCount = injector::ReadMemory<int>(SIZE_OF_g_SND_TURBODATASET, true) / sizeof(TurboDataSet);
	AccelFromIdleCount = injector::ReadMemory<int>(SIZE_OF_g_SND_ACCELFROMIDLE, true) / sizeof(AccelFromIdle);

	if (CarDataMappingCount < 1 || EngineDataCount < 1 || DualGinsuCount < 1 || ShiftPatternCount < 1
		|| SweetnerDataCount < 1 || TurboDataCount < 1 || AccelFromIdleCount < 1) return;

	for (int i = 0; i < MAX_FILES; i++)
	{
		g_CDM[i] = injector::ReadMemory<CarDataMapping>(g_SND_CARDATAMAPPING + (i % CarDataMappingCount) * sizeof(CarDataMapping), true);
		g_ED[i] = injector::ReadMemory<EngineData>(g_SND_ENGINE_DATA + (i % EngineDataCount) * sizeof(EngineData), true);
		g_DG[i] = injector::ReadMemory<DualGinsu>(g_SND_DUAL_GINSU + (i % DualGinsuCount) * sizeof(DualGinsu), true);
		g_SH[i] = injector::ReadMemory<ShiftPattern>(g_SND_SHIFTPATTERNS + (i % ShiftPatternCount) * sizeof(ShiftPattern), true);
		g_SDS[i] = injector::ReadMemory<SweetnerDataSet>(g_SND_SWEETNERDATASET + (i % SweetnerDataCount) * sizeof(SweetnerDataSet), true);
		g_TDS[i] = injector::ReadMemory<TurboDataSet>(g_SND_TURBODATASET + (i % TurboDataCount) * sizeof(TurboDataSet), true);
		g_AFI[i] = injector::ReadMemory<AccelFromIdle>(g_SND_ACCELFROMIDLE + (i % AccelFromIdleCount) * sizeof(AccelFromIdle), true);
	}
}

void LoadLegacyCarSoundData()
{
	char FileNameBuf[MAX_PATH];

	for (int i = 0; i < MAX_FILES; i++)
	{
		sprintf(FileNameBuf, "%02d.ini", i);

		// CarDataMapping
		auto CarDataMappingINIPath = CurrentWorkingDirectory / "CarSoundData" / "CarDataMapping" / FileNameBuf;
		mINI::INIFile CarDataMappingINIFile(CarDataMappingINIPath.string());
		mINI::INIStructure CarDataMappingINI;

		if (CarDataMappingINIFile.read(CarDataMappingINI))
		{
			g_CDM[i].StockEngineData = mINI_ReadInteger(CarDataMappingINI, "Stock", "EngineData", g_CDM[i].StockEngineData);
			g_CDM[i].StockShiftPatternData = mINI_ReadInteger(CarDataMappingINI, "Stock", "ShiftPattern", g_CDM[i].StockShiftPatternData);
			g_CDM[i].StockTurboData = mINI_ReadInteger(CarDataMappingINI, "Stock", "TurboDataSet", g_CDM[i].StockTurboData);

			g_CDM[i].StreetEngineData = mINI_ReadInteger(CarDataMappingINI, "Street", "EngineData", g_CDM[i].StreetEngineData);
			g_CDM[i].StreetShiftPatternData = mINI_ReadInteger(CarDataMappingINI, "Street", "ShiftPattern", g_CDM[i].StreetShiftPatternData);
			g_CDM[i].StreetTurboData = mINI_ReadInteger(CarDataMappingINI, "Street", "TurboDataSet", g_CDM[i].StreetTurboData);

			g_CDM[i].ProEngineData = mINI_ReadInteger(CarDataMappingINI, "Pro", "EngineData", g_CDM[i].ProEngineData);
			g_CDM[i].ProShiftPatternData = mINI_ReadInteger(CarDataMappingINI, "Pro", "ShiftPattern", g_CDM[i].ProShiftPatternData);
			g_CDM[i].ProTurboData = mINI_ReadInteger(CarDataMappingINI, "Pro", "TurboDataSet", g_CDM[i].ProTurboData);

			g_CDM[i].ExtremeEngineData = mINI_ReadInteger(CarDataMappingINI, "Extreme", "EngineData", g_CDM[i].ExtremeEngineData);
			g_CDM[i].ExtremeShiftPatternData = mINI_ReadInteger(CarDataMappingINI, "Extreme", "ShiftPattern", g_CDM[i].ExtremeShiftPatternData);
			g_CDM[i].ExtremeTurboData = mINI_ReadInteger(CarDataMappingINI, "Extreme", "TurboDataSet", g_CDM[i].ExtremeTurboData);

			CarDataMappingCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// EngineData
		auto EngineDataINIPath = CurrentWorkingDirectory / "CarSoundData" / "EngineData" / FileNameBuf;
		mINI::INIFile EngineDataINIFile(EngineDataINIPath.string());
		mINI::INIStructure EngineDataINI;

		if (EngineDataINIFile.read(EngineDataINI))
		{
			ReadCarSoundString(EngineDataINI, "EngineData", "MainRAMBankName", g_ED[i].MainRAMBankName);
			ReadCarSoundString(EngineDataINI, "EngineData", "AuxRAMBankName", g_ED[i].AuxRAMBankName);
			g_ED[i].CarID = mINI_ReadInteger(EngineDataINI, "EngineData", "CarID", g_ED[i].CarID);
			ReadCarSoundString(EngineDataINI, "EngineData", "GinsuAccel", g_ED[i].GinsuAccel);
			g_ED[i].UseDualGinsu = mINI_ReadInteger(EngineDataINI, "EngineData", "UseDualGinsu",
				mINI_ReadInteger(EngineDataINI, "EngineData", "Unk1", g_ED[i].UseDualGinsu));
			g_ED[i].GinsuDecelID = mINI_ReadInteger(EngineDataINI, "EngineData", "GinsuDecelID", g_ED[i].GinsuDecelID);
			g_ED[i].AccelFromIdleID = mINI_ReadInteger(EngineDataINI, "EngineData", "AccelFromIdleID", g_ED[i].AccelFromIdleID);
			g_ED[i].MaxRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "MaxRPM", g_ED[i].MaxRPM);
			g_ED[i].MinRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "MinRPM", g_ED[i].MinRPM);
			g_ED[i].AEMSVol = mINI_ReadInteger(EngineDataINI, "EngineData", "AEMSVol", g_ED[i].AEMSVol);
			g_ED[i].GinsuAccelVol = mINI_ReadInteger(EngineDataINI, "EngineData", "GinsuAccelVol", g_ED[i].GinsuAccelVol);
			g_ED[i].AccelDeltaRPMThreshold = mINI_ReadFloat(EngineDataINI, "EngineData", "AccelDeltaRPMThreshold", g_ED[i].AccelDeltaRPMThreshold);
			g_ED[i].AEMSMixLRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "AEMSMixLRPM", g_ED[i].AEMSMixLRPM);
			g_ED[i].GinsuMixLRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "GinsuMixLRPM", g_ED[i].GinsuMixLRPM);
			g_ED[i].AEMSMixSRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "AEMSMixSRPM", g_ED[i].AEMSMixSRPM);
			g_ED[i].GinsuMixSRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "GinsuMixSRPM", g_ED[i].GinsuMixSRPM);
			g_ED[i].AEMSMixNRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "UnkFloat1", g_ED[i].AEMSMixNRPM); // Was UnkFloat1 on Legacy CST
			g_ED[i].GinsuMixNRPM = mINI_ReadFloat(EngineDataINI, "EngineData", "UnkFloat2", g_ED[i].GinsuMixNRPM); // Was UnkFloat2 on Legacy CST
			g_ED[i].GinsuLowPassCutoff = mINI_ReadInteger(EngineDataINI, "EngineData", "Unk2", g_ED[i].GinsuLowPassCutoff); // Was Unk2 on Legacy CST

			EngineDataCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// DualGinsu
		auto DualGinsuINIPath = CurrentWorkingDirectory / "CarSoundData" / "DualGinsu" / FileNameBuf;
		mINI::INIFile DualGinsuINIFile(DualGinsuINIPath.string());
		mINI::INIStructure DualGinsuINI;

		if (DualGinsuINIFile.read(DualGinsuINI))
		{
			ReadCarSoundString(DualGinsuINI, "DualGinsu", "GinsuDecelFileName", g_DG[i].GinsuDecelFileName);
			g_DG[i].AEMSDecelVol = mINI_ReadInteger(DualGinsuINI, "DualGinsu", "AEMSDecelVol", g_DG[i].AEMSDecelVol);
			g_DG[i].GinsuDecelVol = mINI_ReadInteger(DualGinsuINI, "DualGinsu", "GinsuDecelVol", g_DG[i].GinsuDecelVol);
			g_DG[i].MinRPM = mINI_ReadInteger(DualGinsuINI, "DualGinsu", "MinRPM", g_DG[i].MinRPM);
			g_DG[i].MaxRPM = mINI_ReadInteger(DualGinsuINI, "DualGinsu", "MaxRPM", g_DG[i].MaxRPM);
			g_DG[i].FadeIn = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "FadeIn", g_DG[i].FadeIn);
			g_DG[i].FadeOut = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "FadeOut", g_DG[i].FadeOut);
			g_DG[i].DecelDeltaRPMThreshold = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "DecelDeltaRPMThreshold", g_DG[i].DecelDeltaRPMThreshold);
			g_DG[i].DecelPitchOffset = mINI_ReadInteger(DualGinsuINI, "DualGinsu", "Unk0", g_DG[i].DecelPitchOffset); // Was Unk0 on Legacy CST
			g_DG[i].DecelAEMSMixLRPM = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "DecelAEMSMixLRPM", g_DG[i].DecelAEMSMixLRPM);
			g_DG[i].DecelGinsuMixLRPM = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "DecelGinsuMixLRPM", g_DG[i].DecelGinsuMixLRPM);
			g_DG[i].DecelAEMSMixSRPM = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "DecelAEMSMixSRPM", g_DG[i].DecelAEMSMixSRPM);
			g_DG[i].DecelGinsuMixSRPM = mINI_ReadFloat(DualGinsuINI, "DualGinsu", "DecelGinsuMixSRPM", g_DG[i].DecelGinsuMixSRPM);

			DualGinsuCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// ShiftPattern
		auto ShiftPatternINIPath = CurrentWorkingDirectory / "CarSoundData" / "ShiftPatterns" / FileNameBuf;
		mINI::INIFile ShiftPatternINIFile(ShiftPatternINIPath.string());
		mINI::INIStructure ShiftPatternINI;

		if (ShiftPatternINIFile.read(ShiftPatternINI))
		{
			ReadCarSoundString(ShiftPatternINI, "ShiftPattern", "BankName", g_SH[i].BankName);
			g_SH[i].UpVolShift = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk1", g_SH[i].UpVolShift);
			g_SH[i].UpShiftSoundDelay = mINI_ReadFloat(ShiftPatternINI, "ShiftPattern", "UnkFloat1", g_SH[i].UpShiftSoundDelay);
			g_SH[i].UpDisengageFallRPM = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk2", g_SH[i].UpDisengageFallRPM);
			g_SH[i].UpDisengageFallT = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk3", g_SH[i].UpDisengageFallT);
			g_SH[i].UpEngagingRiseRPM = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk4", g_SH[i].UpEngagingRiseRPM);
			g_SH[i].UpEngagingRiseT = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk5", g_SH[i].UpEngagingRiseT);
			g_SH[i].UpEngagingAttackT = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk6", g_SH[i].UpEngagingAttackT);
			g_SH[i].UpEngagingAttackVol = mINI_ReadFloat(ShiftPatternINI, "ShiftPattern", "UnkFloat2", g_SH[i].UpEngagingAttackVol);
			g_SH[i].LFOVolAmp = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk7", g_SH[i].LFOVolAmp);
			g_SH[i].LFOVolFreq = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk8", g_SH[i].LFOVolFreq);
			g_SH[i].LFOVolDecayTime = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk9", g_SH[i].LFOVolDecayTime);
			g_SH[i].LFORPMAmp = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk10", g_SH[i].LFORPMAmp);
			g_SH[i].LFORPMFreq = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk11", g_SH[i].LFORPMFreq);
			g_SH[i].LFORPMDecayTime = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk12", g_SH[i].LFORPMDecayTime);
			g_SH[i].DownVolShift = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk13", g_SH[i].DownVolShift);
			g_SH[i].DownShiftSoundDelay = mINI_ReadFloat(ShiftPatternINI, "ShiftPattern", "UnkFloat3", g_SH[i].DownShiftSoundDelay);
			g_SH[i].DownDisengageFallRPM = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk14", g_SH[i].DownDisengageFallRPM);
			g_SH[i].DownDisengageFallT = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk15", g_SH[i].DownDisengageFallT);
			g_SH[i].DownEngagingRiseRPM = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk16", g_SH[i].DownEngagingRiseRPM);
			g_SH[i].DownEngagingRiseT = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk17", g_SH[i].DownEngagingRiseT);
			g_SH[i].DownEngagingFallRPM = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk18", g_SH[i].DownEngagingFallRPM);
			g_SH[i].DownEngagingFallT = mINI_ReadInteger(ShiftPatternINI, "ShiftPattern", "Unk19", g_SH[i].DownEngagingFallT);
			g_SH[i].DownReattachScale = mINI_ReadFloat(ShiftPatternINI, "ShiftPattern", "UnkFloat4", g_SH[i].DownReattachScale);

			ShiftPatternCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// Sweetner
		auto SweetnerINIPath = CurrentWorkingDirectory / "CarSoundData" / "SweetnerDataSet" / FileNameBuf;
		mINI::INIFile SweetnerINIFile(SweetnerINIPath.string());
		mINI::INIStructure SweetnerINI;

		if (SweetnerINIFile.read(SweetnerINI))
		{
			ReadCarSoundString(SweetnerINI, "Sweetner", "SweetBank", g_SDS[i].SweetBank);
			g_SDS[i].ShiftSweetsVol = mINI_ReadInteger(SweetnerINI, "Sweetner", "SputterVol", g_SDS[i].ShiftSweetsVol); // Legacy CST had the values swapped
			g_SDS[i].SputterVol = mINI_ReadInteger(SweetnerINI, "Sweetner", "ShiftSweetsVol", g_SDS[i].SputterVol);

			SweetnerDataCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// Turbo
		auto TurboINIPath = CurrentWorkingDirectory / "CarSoundData" / "TurboDataSet" / FileNameBuf;
		mINI::INIFile TurboINIFile(TurboINIPath.string());
		mINI::INIStructure TurboINI;

		if (TurboINIFile.read(TurboINI))
		{
			ReadCarSoundString(TurboINI, "Turbo", "TurboBank", g_TDS[i].TurboBank);
			g_TDS[i].SpoolVol = mINI_ReadInteger(TurboINI, "Turbo", "TurboChargeVol", mINI_ReadInteger(TurboINI, "Turbo", "Unk1", g_TDS[i].SpoolVol));
			g_TDS[i].BlowoffVol1 = mINI_ReadInteger(TurboINI, "Turbo", "LowBoostBovVol", mINI_ReadInteger(TurboINI, "Turbo", "Unk2", g_TDS[i].BlowoffVol1));
			g_TDS[i].BlowoffVol2 = mINI_ReadInteger(TurboINI, "Turbo", "HighBoostBovVol", mINI_ReadInteger(TurboINI, "Turbo", "Unk3", g_TDS[i].BlowoffVol2));
			g_TDS[i].ChargeTime = mINI_ReadFloat(TurboINI, "Turbo", "UnkFloat1", g_TDS[i].ChargeTime);
			g_TDS[i].LeakRate = mINI_ReadFloat(TurboINI, "Turbo", "ChargeTime", g_TDS[i].LeakRate);

			TurboDataCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// AccelTrans (AccelFromIdle)
		auto AccelTransINIPath = CurrentWorkingDirectory / "CarSoundData" / "AccelFromIdle" / FileNameBuf;
		mINI::INIFile AccelTransINIFile(AccelTransINIPath.string());
		mINI::INIStructure AccelTransINI;

		if (AccelTransINIFile.read(AccelTransINI))
		{
			g_AFI[i].PeakVol = mINI_ReadFloat(AccelTransINI, "AccelTrans", "UnkFloat1", g_AFI[i].PeakVol);
			g_AFI[i].PeakRPM = mINI_ReadInteger(AccelTransINI, "AccelTrans", "Unk1", g_AFI[i].PeakRPM);
			g_AFI[i].PeakT = mINI_ReadInteger(AccelTransINI, "AccelTrans", "Unk2", g_AFI[i].PeakT);
			g_AFI[i].ResumeT = mINI_ReadInteger(AccelTransINI, "AccelTrans", "Unk3", g_AFI[i].ResumeT);
			g_AFI[i].InteruptT = mINI_ReadInteger(AccelTransINI, "AccelTrans", "Unk4", g_AFI[i].InteruptT);

			AccelFromIdleCount = i + 1; // Latest successfully read file means we have that many files.
		}

		// One CarTypeMapping per car, by name, so add-on cars can have their own entry. Resolved here
		// rather than in the hook so that no file work happens on the game's stack.
		//CarTypeMapping.assign(CarCount > 0 ? CarCount : 0, 0);

		for (int i = 0; i < CarCount; i++)
		{
			int Mapping = GetVanillaCarTypeMapping(i);

			sprintf(FileNameBuf, "%s.ini", GetCarTypeName(i));
			auto CarTypeMappingINIPath = CurrentWorkingDirectory / "CarSoundData" / "CarTypeMapping" / FileNameBuf;
			mINI::INIFile CarTypeMappingINIFile(CarTypeMappingINIPath.string());
			mINI::INIStructure CarTypeMappingINI;

			if (CarTypeMappingINIFile.read(CarTypeMappingINI))
				Mapping = mINI_ReadInteger(CarTypeMappingINI, "CarTypeMapping", "CarDataMapping", Mapping);

			// A typo in an ini would otherwise index straight past our arrays
			if (Mapping < 0 || Mapping >= MAX_FILES) Mapping = GetVanillaCarTypeMapping(i);

			CarTypeMapping[i] = Mapping;
		}
	}
}

bool LoadCarSoundData()
{
	std::vector<FileName> CarDataFileNames;
	std::vector<FileName> EngineDataFileNames;
	std::vector<FileName> DualGinsuFileNames;
	std::vector<FileName> ShiftPatternFileNames;
	std::vector<FileName> SweetnerDataFileNames;
	std::vector<FileName> TurboDataFileNames;
	std::vector<FileName> AccelFromIdleFileNames;

	int i = 0;

	auto CarSoundDataPath = CurrentWorkingDirectory / "UnlimiterData" / "CarSoundData";

	// Return if the directory doesn't exist
	if (!std::filesystem::exists(CarSoundDataPath)) return 0;

	// Sweetner
	auto SweetnerINIPath = CarSoundDataPath / "SweetnerDataSet";
	FileName AFile;

	if (std::filesystem::exists(SweetnerINIPath))
	{
		auto it = std::filesystem::directory_iterator(SweetnerINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				ReadCarSoundString(itINI, "Sweetner", "SweetBank", g_SDS[i].SweetBank);
				g_SDS[i].ShiftSweetsVol = mINI_ReadInteger(itINI, "Sweetner", "ShiftSweetsVol", g_SDS[i].ShiftSweetsVol);
				g_SDS[i].SputterVol = mINI_ReadInteger(itINI, "Sweetner", "SputterVol", g_SDS[i].SputterVol);

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				SweetnerDataFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	SweetnerDataCount = i;
	i = 0;


	// AccelFromIdle
	auto AccelFromIdleINIPath = CarSoundDataPath / "AccelFromIdle";
	
	if (std::filesystem::exists(AccelFromIdleINIPath))
	{
		auto it = std::filesystem::directory_iterator(AccelFromIdleINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				g_AFI[i].PeakVol = mINI_ReadFloat(itINI, "AccelTrans", "PeakVol", g_AFI[i].PeakVol);
				g_AFI[i].PeakRPM = mINI_ReadInteger(itINI, "AccelTrans", "PeakRPM", g_AFI[i].PeakRPM);
				g_AFI[i].PeakT = mINI_ReadInteger(itINI, "AccelTrans", "PeakT", g_AFI[i].PeakT);
				g_AFI[i].ResumeT = mINI_ReadInteger(itINI, "AccelTrans", "ResumeT", g_AFI[i].ResumeT);
				g_AFI[i].InteruptT = mINI_ReadInteger(itINI, "AccelTrans", "InteruptT", g_AFI[i].InteruptT);

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				AccelFromIdleFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	AccelFromIdleCount = i;
	i = 0;


	// DualGinsu
	auto DualGinsuINIPath = CarSoundDataPath / "DualGinsu";
	
	if (std::filesystem::exists(DualGinsuINIPath))
	{
		auto it = std::filesystem::directory_iterator(DualGinsuINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				ReadCarSoundString(itINI, "DualGinsu", "GinsuDecelFileName", g_DG[i].GinsuDecelFileName);
				g_DG[i].AEMSDecelVol = mINI_ReadInteger(itINI, "DualGinsu", "AEMSDecelVol", g_DG[i].AEMSDecelVol);
				g_DG[i].GinsuDecelVol = mINI_ReadInteger(itINI, "DualGinsu", "GinsuDecelVol", g_DG[i].GinsuDecelVol);
				g_DG[i].MinRPM = mINI_ReadInteger(itINI, "DualGinsu", "MinRPM", g_DG[i].MinRPM);
				g_DG[i].MaxRPM = mINI_ReadInteger(itINI, "DualGinsu", "MaxRPM", g_DG[i].MaxRPM);
				g_DG[i].FadeIn = mINI_ReadFloat(itINI, "DualGinsu", "FadeIn", g_DG[i].FadeIn);
				g_DG[i].FadeOut = mINI_ReadFloat(itINI, "DualGinsu", "FadeOut", g_DG[i].FadeOut);
				g_DG[i].DecelDeltaRPMThreshold = mINI_ReadFloat(itINI, "DualGinsu", "DecelDeltaRPMThreshold", g_DG[i].DecelDeltaRPMThreshold);
				g_DG[i].DecelPitchOffset = mINI_ReadInteger(itINI, "DualGinsu", "DecelPitchOffset", g_DG[i].DecelPitchOffset);
				g_DG[i].DecelAEMSMixLRPM = mINI_ReadFloat(itINI, "DualGinsu", "DecelAEMSMixLRPM", g_DG[i].DecelAEMSMixLRPM);
				g_DG[i].DecelGinsuMixLRPM = mINI_ReadFloat(itINI, "DualGinsu", "DecelGinsuMixLRPM", g_DG[i].DecelGinsuMixLRPM);
				g_DG[i].DecelAEMSMixSRPM = mINI_ReadFloat(itINI, "DualGinsu", "DecelAEMSMixSRPM", g_DG[i].DecelAEMSMixSRPM);
				g_DG[i].DecelGinsuMixSRPM = mINI_ReadFloat(itINI, "DualGinsu", "DecelGinsuMixSRPM", g_DG[i].DecelGinsuMixSRPM);

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				DualGinsuFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	DualGinsuCount = i;
	i = 0;


	// EngineData
	auto EngineDataINIPath = CarSoundDataPath / "EngineData";

	if (std::filesystem::exists(EngineDataINIPath))
	{
		auto it = std::filesystem::directory_iterator(EngineDataINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				ReadCarSoundString(itINI, "EngineData", "MainRAMBankName", g_ED[i].MainRAMBankName);
				ReadCarSoundString(itINI, "EngineData", "AuxRAMBankName", g_ED[i].AuxRAMBankName);
				//g_ED[i].CarID = mINI_ReadInteger(itINI, "EngineData", "CarID", g_ED[i].CarID); // Match with SweetnerDataSet
				ReadCarSoundString(itINI, "EngineData", "GinsuAccel", g_ED[i].GinsuAccel);
				g_ED[i].UseDualGinsu = mINI_ReadInteger(itINI, "EngineData", "UseDualGinsu", g_ED[i].UseDualGinsu);
				//g_ED[i].GinsuDecelID = mINI_ReadInteger(itINI, "EngineData", "GinsuDecelID", g_ED[i].GinsuDecelID); // Match with DualGinsu
				//g_ED[i].AccelFromIdleID = mINI_ReadInteger(itINI, "EngineData", "AccelFromIdleID", g_ED[i].AccelFromIdleID); // Match with AccelFromIdle
				g_ED[i].MaxRPM = mINI_ReadFloat(itINI, "EngineData", "MaxRPM", g_ED[i].MaxRPM);
				g_ED[i].MinRPM = mINI_ReadFloat(itINI, "EngineData", "MinRPM", g_ED[i].MinRPM);
				g_ED[i].AEMSVol = mINI_ReadInteger(itINI, "EngineData", "AEMSVol", g_ED[i].AEMSVol);
				g_ED[i].GinsuAccelVol = mINI_ReadInteger(itINI, "EngineData", "GinsuAccelVol", g_ED[i].GinsuAccelVol);
				g_ED[i].AccelDeltaRPMThreshold = mINI_ReadFloat(itINI, "EngineData", "AccelDeltaRPMThreshold", g_ED[i].AccelDeltaRPMThreshold);
				g_ED[i].AEMSMixLRPM = mINI_ReadFloat(itINI, "EngineData", "AEMSMixLRPM", g_ED[i].AEMSMixLRPM);
				g_ED[i].GinsuMixLRPM = mINI_ReadFloat(itINI, "EngineData", "GinsuMixLRPM", g_ED[i].GinsuMixLRPM);
				g_ED[i].AEMSMixSRPM = mINI_ReadFloat(itINI, "EngineData", "AEMSMixSRPM", g_ED[i].AEMSMixSRPM);
				g_ED[i].GinsuMixSRPM = mINI_ReadFloat(itINI, "EngineData", "GinsuMixSRPM", g_ED[i].GinsuMixSRPM);
				g_ED[i].AEMSMixNRPM = mINI_ReadFloat(itINI, "EngineData", "AEMSMixNRPM", g_ED[i].AEMSMixNRPM);
				g_ED[i].GinsuMixNRPM = mINI_ReadFloat(itINI, "EngineData", "GinsuMixNRPM", g_ED[i].GinsuMixNRPM);
				g_ED[i].GinsuLowPassCutoff = mINI_ReadInteger(itINI, "EngineData", "GinsuLowPassCutoff", g_ED[i].GinsuLowPassCutoff);

				int tmp = g_ED[i].CarID;
				g_ED[i].CarID = FindMatchingCarSoundFileName(itINI, "EngineData", "SweetnerDataSetFileName", SweetnerDataFileNames);
				if (g_ED[i].CarID == -1) g_ED[i].CarID = tmp;

				tmp = g_ED[i].GinsuDecelID;
				g_ED[i].GinsuDecelID = FindMatchingCarSoundFileName(itINI, "EngineData", "DualGinsuFileName", DualGinsuFileNames);
				if (g_ED[i].GinsuDecelID == -1) g_ED[i].GinsuDecelID = tmp;

				tmp = g_ED[i].AccelFromIdleID;
				g_ED[i].AccelFromIdleID = FindMatchingCarSoundFileName(itINI, "EngineData", "AccelFromIdleFileName", AccelFromIdleFileNames);
				if (g_ED[i].AccelFromIdleID == -1) g_ED[i].AccelFromIdleID = tmp;

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				EngineDataFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	EngineDataCount = i;
	i = 0;


	// ShiftPattern
	auto ShiftPatternINIPath = CarSoundDataPath / "ShiftPattern";

	if (std::filesystem::exists(ShiftPatternINIPath))
	{
		auto it = std::filesystem::directory_iterator(ShiftPatternINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				ReadCarSoundString(itINI, "ShiftPattern", "BankName", g_SH[i].BankName);
				g_SH[i].UpVolShift = mINI_ReadInteger(itINI, "ShiftPattern", "UpVolShift", g_SH[i].UpVolShift);
				g_SH[i].UpShiftSoundDelay = mINI_ReadFloat(itINI, "ShiftPattern", "UpShiftSoundDelay", g_SH[i].UpShiftSoundDelay);
				g_SH[i].UpDisengageFallRPM = mINI_ReadInteger(itINI, "ShiftPattern", "UpDisengageFallRPM", g_SH[i].UpDisengageFallRPM);
				g_SH[i].UpDisengageFallT = mINI_ReadInteger(itINI, "ShiftPattern", "UpDisengageFallT", g_SH[i].UpDisengageFallT);
				g_SH[i].UpEngagingRiseRPM = mINI_ReadInteger(itINI, "ShiftPattern", "UpEngagingRiseRPM", g_SH[i].UpEngagingRiseRPM);
				g_SH[i].UpEngagingRiseT = mINI_ReadInteger(itINI, "ShiftPattern", "UpEngagingRiseT", g_SH[i].UpEngagingRiseT);
				g_SH[i].UpEngagingAttackT = mINI_ReadInteger(itINI, "ShiftPattern", "UpEngagingAttackT", g_SH[i].UpEngagingAttackT);
				g_SH[i].UpEngagingAttackVol = mINI_ReadFloat(itINI, "ShiftPattern", "UpEngagingAttackVol", g_SH[i].UpEngagingAttackVol);
				g_SH[i].LFOVolAmp = mINI_ReadInteger(itINI, "ShiftPattern", "LFOVolAmp", g_SH[i].LFOVolAmp);
				g_SH[i].LFOVolFreq = mINI_ReadInteger(itINI, "ShiftPattern", "LFOVolFreq", g_SH[i].LFOVolFreq);
				g_SH[i].LFOVolDecayTime = mINI_ReadInteger(itINI, "ShiftPattern", "LFOVolDecayTime", g_SH[i].LFOVolDecayTime);
				g_SH[i].LFORPMAmp = mINI_ReadInteger(itINI, "ShiftPattern", "LFORPMAmp", g_SH[i].LFORPMAmp);
				g_SH[i].LFORPMFreq = mINI_ReadInteger(itINI, "ShiftPattern", "LFORPMFreq", g_SH[i].LFORPMFreq);
				g_SH[i].LFORPMDecayTime = mINI_ReadInteger(itINI, "ShiftPattern", "LFORPMDecayTime", g_SH[i].LFORPMDecayTime);
				g_SH[i].DownVolShift = mINI_ReadInteger(itINI, "ShiftPattern", "DownVolShift", g_SH[i].DownVolShift);
				g_SH[i].DownShiftSoundDelay = mINI_ReadFloat(itINI, "ShiftPattern", "DownShiftSoundDelay", g_SH[i].DownShiftSoundDelay);
				g_SH[i].DownDisengageFallRPM = mINI_ReadInteger(itINI, "ShiftPattern", "DownDisengageFallRPM", g_SH[i].DownDisengageFallRPM);
				g_SH[i].DownDisengageFallT = mINI_ReadInteger(itINI, "ShiftPattern", "DownDisengageFallT", g_SH[i].DownDisengageFallT);
				g_SH[i].DownEngagingRiseRPM = mINI_ReadInteger(itINI, "ShiftPattern", "DownEngagingRiseRPM", g_SH[i].DownEngagingRiseRPM);
				g_SH[i].DownEngagingRiseT = mINI_ReadInteger(itINI, "ShiftPattern", "DownEngagingRiseT", g_SH[i].DownEngagingRiseT);
				g_SH[i].DownEngagingFallRPM = mINI_ReadInteger(itINI, "ShiftPattern", "DownEngagingFallRPM", g_SH[i].DownEngagingFallRPM);
				g_SH[i].DownEngagingFallT = mINI_ReadInteger(itINI, "ShiftPattern", "DownEngagingFallT", g_SH[i].DownEngagingFallT);
				g_SH[i].DownReattachScale = mINI_ReadFloat(itINI, "ShiftPattern", "DownReattachScale", g_SH[i].DownReattachScale);

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				ShiftPatternFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	ShiftPatternCount = i;
	i = 0;


	// TurboDataSet
	auto TurboDataSetINIPath = CarSoundDataPath / "TurboDataSet";

	if (std::filesystem::exists(TurboDataSetINIPath))
	{
		auto it = std::filesystem::directory_iterator(TurboDataSetINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				ReadCarSoundString(itINI, "Turbo", "TurboBank", g_TDS[i].TurboBank);
				g_TDS[i].SpoolVol = mINI_ReadInteger(itINI, "Turbo", "SpoolVol", mINI_ReadInteger(itINI, "Turbo", "TurboChargeVol", g_TDS[i].SpoolVol));
				g_TDS[i].BlowoffVol1 = mINI_ReadInteger(itINI, "Turbo", "BlowoffVol1", mINI_ReadInteger(itINI, "Turbo", "LowBoostBovVol", g_TDS[i].BlowoffVol1));
				g_TDS[i].BlowoffVol2 = mINI_ReadInteger(itINI, "Turbo", "BlowoffVol2", mINI_ReadInteger(itINI, "Turbo", "HighBoostBovVol", g_TDS[i].BlowoffVol2));
				g_TDS[i].ChargeTime = mINI_ReadFloat(itINI, "Turbo", "ChargeTime", g_TDS[i].ChargeTime);
				g_TDS[i].LeakRate = mINI_ReadFloat(itINI, "Turbo", "LeakRate", g_TDS[i].LeakRate);

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				TurboDataFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	TurboDataCount = i;
	i = 0;


	// CarDataMapping
	auto CarDataMappingINIPath = CarSoundDataPath / "CarDataMapping";

	if (std::filesystem::exists(CarDataMappingINIPath))
	{
		auto it = std::filesystem::directory_iterator(CarDataMappingINIPath);

		for (const auto& entry : it)
		{
			if (i >= MAX_FILES) break;

			if (entry.is_regular_file() && entry.path().extension() == ".ini") // Check if .ini
			{
				auto itINIPath = entry.path();
				mINI::INIFile itINIFile(itINIPath.string());
				mINI::INIStructure itINI;
				itINIFile.read(itINI);

				// Stock
				int tmp = g_CDM[i].StockEngineData;
				g_CDM[i].StockEngineData = FindMatchingCarSoundFileName(itINI, "Stock", "EngineData", EngineDataFileNames);
				if (g_CDM[i].StockEngineData == -1) g_CDM[i].StockEngineData = tmp;

				tmp = g_CDM[i].StockShiftPatternData;
				g_CDM[i].StockShiftPatternData = FindMatchingCarSoundFileName(itINI, "Stock", "ShiftPattern", ShiftPatternFileNames);
				if (g_CDM[i].StockShiftPatternData == -1) g_CDM[i].StockShiftPatternData = tmp;

				tmp = g_CDM[i].StockTurboData;
				g_CDM[i].StockTurboData = FindMatchingCarSoundFileName(itINI, "Stock", "TurboDataSet", TurboDataFileNames);
				if (g_CDM[i].StockTurboData == -1) g_CDM[i].StockTurboData = tmp;

				// Street
				tmp = g_CDM[i].StreetEngineData;
				g_CDM[i].StreetEngineData = FindMatchingCarSoundFileName(itINI, "Street", "EngineData", EngineDataFileNames);
				if (g_CDM[i].StreetEngineData == -1) g_CDM[i].StreetEngineData = tmp;

				tmp = g_CDM[i].StreetShiftPatternData;
				g_CDM[i].StreetShiftPatternData = FindMatchingCarSoundFileName(itINI, "Street", "ShiftPattern", ShiftPatternFileNames);
				if (g_CDM[i].StreetShiftPatternData == -1) g_CDM[i].StreetShiftPatternData = tmp;

				tmp = g_CDM[i].StreetTurboData;
				g_CDM[i].StreetTurboData = FindMatchingCarSoundFileName(itINI, "Street", "TurboDataSet", TurboDataFileNames);
				if (g_CDM[i].StreetTurboData == -1) g_CDM[i].StreetTurboData = tmp;

				// Pro
				tmp = g_CDM[i].ProEngineData;
				g_CDM[i].ProEngineData = FindMatchingCarSoundFileName(itINI, "Pro", "EngineData", EngineDataFileNames);
				if (g_CDM[i].ProEngineData == -1) g_CDM[i].ProEngineData = tmp;

				tmp = g_CDM[i].ProShiftPatternData;
				g_CDM[i].ProShiftPatternData = FindMatchingCarSoundFileName(itINI, "Pro", "ShiftPattern", ShiftPatternFileNames);
				if (g_CDM[i].ProShiftPatternData == -1) g_CDM[i].ProShiftPatternData = tmp;

				tmp = g_CDM[i].ProTurboData;
				g_CDM[i].ProTurboData = FindMatchingCarSoundFileName(itINI, "Pro", "TurboDataSet", TurboDataFileNames);
				if (g_CDM[i].ProTurboData == -1) g_CDM[i].ProTurboData = tmp;

				// Extreme
				tmp = g_CDM[i].ExtremeEngineData;
				g_CDM[i].ExtremeEngineData = FindMatchingCarSoundFileName(itINI, "Extreme", "EngineData", EngineDataFileNames);
				if (g_CDM[i].ExtremeEngineData == -1) g_CDM[i].ExtremeEngineData = tmp;

				tmp = g_CDM[i].ExtremeShiftPatternData;
				g_CDM[i].ExtremeShiftPatternData = FindMatchingCarSoundFileName(itINI, "Extreme", "ShiftPattern", ShiftPatternFileNames);
				if (g_CDM[i].ExtremeShiftPatternData == -1) g_CDM[i].ExtremeShiftPatternData = tmp;

				tmp = g_CDM[i].ExtremeTurboData;
				g_CDM[i].ExtremeTurboData = FindMatchingCarSoundFileName(itINI, "Extreme", "TurboDataSet", TurboDataFileNames);
				if (g_CDM[i].ExtremeTurboData == -1) g_CDM[i].ExtremeTurboData = tmp;

				// Save file name for later
				SetFileName(AFile._n, itINIPath.stem().string().c_str());
				CarDataFileNames.push_back(AFile);

				i++;
			}
		}
	}
	else return 0;
	CarDataMappingCount = i;
	i = 0;


	// CarTypeMapping
	//CarTypeMapping.assign(CarCount > 0 ? CarCount : 0, 0);

	// _General.ini
	auto ctmDefINIPath = CarSoundDataPath / "_General.ini";
	mINI::INIFile ctmDefINIFile(ctmDefINIPath.string());
	mINI::INIStructure ctmDefINI;
	ctmDefINIFile.read(ctmDefINI);

	int defMap = FindMatchingCarSoundFileName(ctmDefINI, "CarTypeMapping", "CarDataMapping", CarDataFileNames);
	if (defMap == -1) defMap = VanillaCarTypeMappingDefault;

	// Car-Specific (CARNAME.ini)
	for (i = 0; i < CarCount; i++)
	{
		sprintf(CarININame, "%s.ini", GetCarTypeName(i));
		auto ctmINIPath = CarSoundDataPath / CarININame;
		mINI::INIFile ctmINIFile(ctmINIPath.string());
		mINI::INIStructure ctmINI;
		ctmINIFile.read(ctmINI);

		CarTypeMapping[i] = FindMatchingCarSoundFileName(ctmDefINI, "CarTypeMapping", "CarDataMapping", CarDataFileNames);
		if (CarTypeMapping[i] == -1) CarTypeMapping[i] = GetVanillaCarTypeMapping(i);
	}

	/*
	* if (NumAccelFromIdle
		&& NumTurboDataSet
		&& NumSweetner
		&& NumShiftPattern
		&& NumDualGinsu
		&& NumEngineData
		&& NumCarDataMapping)
	*/
	
	return 1;
}

void SaveCarSoundData()
{
	std::vector<FileName> CarDataFileNames;
	std::vector<FileName> EngineDataFileNames;
	std::vector<FileName> DualGinsuFileNames;
	std::vector<FileName> ShiftPatternFileNames;
	std::vector<FileName> SweetnerDataFileNames;
	std::vector<FileName> TurboDataFileNames;
	std::vector<FileName> AccelFromIdleFileNames;

	char FileNameBuf[MAX_PATH];
	std::error_code ErrorCode;

	auto CarSoundDataPath = CurrentWorkingDirectory / "UnlimiterData" / "CarSoundData";
	std::filesystem::create_directories(CarSoundDataPath, ErrorCode); // Create dir if it doesn't exist

	// Sweetner
	auto SweetnerINIPath = CarSoundDataPath / "SweetnerDataSet";
	std::filesystem::create_directories(SweetnerINIPath, ErrorCode);
	FileName AFile;

	for (int i = 0; i < SweetnerDataCount; i++)
	{
		// Create file name from SweetBank
		SetFileName(AFile._n, g_SDS[i].SweetBank);
		sprintf(FileNameBuf, "%02d_%s.ini", i, AFile._n);

		auto exINIPath = SweetnerINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["Sweetner"]["SweetBank"] = g_SDS[i].SweetBank;
		exINI["Sweetner"]["ShiftSweetsVol"] = std::to_string(g_SDS[i].ShiftSweetsVol);
		exINI["Sweetner"]["SputterVol"] = std::to_string(g_SDS[i].SputterVol);

		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		SweetnerDataFileNames.push_back(AFile);
	}

	// AccelFromIdle
	auto AccelFromIdleINIPath = CarSoundDataPath / "AccelFromIdle";
	std::filesystem::create_directories(AccelFromIdleINIPath, ErrorCode);

	for (int i = 0; i < AccelFromIdleCount; i++)
	{
		// Create file name
		sprintf(FileNameBuf, "%02d.ini", i);

		auto exINIPath = AccelFromIdleINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["AccelTrans"]["PeakVol"] = std::to_string(g_AFI[i].PeakVol);
		exINI["AccelTrans"]["PeakRPM"] = std::to_string(g_AFI[i].PeakRPM);
		exINI["AccelTrans"]["PeakT"] = std::to_string(g_AFI[i].PeakT);
		exINI["AccelTrans"]["ResumeT"] = std::to_string(g_AFI[i].ResumeT);
		exINI["AccelTrans"]["InteruptT"] = std::to_string(g_AFI[i].InteruptT);

		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		AccelFromIdleFileNames.push_back(AFile);
	}


	// DualGinsu
	auto DualGinsuINIPath = CarSoundDataPath / "DualGinsu";
	std::filesystem::create_directories(DualGinsuINIPath, ErrorCode);

	for (int i = 0; i < DualGinsuCount; i++)
	{
		// Create file name from GinsuDecel
		SetFileName(AFile._n, g_DG[i].GinsuDecelFileName);
		sprintf(FileNameBuf, "%02d_%s.ini", i, AFile._n);

		auto exINIPath = DualGinsuINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["DualGinsu"]["GinsuDecelFileName"] = g_DG[i].GinsuDecelFileName;
		exINI["DualGinsu"]["AEMSDecelVol"] = std::to_string(g_DG[i].AEMSDecelVol);
		exINI["DualGinsu"]["GinsuDecelVol"] = std::to_string(g_DG[i].GinsuDecelVol);
		exINI["DualGinsu"]["MinRPM"] = std::to_string(g_DG[i].MinRPM);
		exINI["DualGinsu"]["MaxRPM"] = std::to_string(g_DG[i].MaxRPM);
		exINI["DualGinsu"]["FadeIn"] = std::to_string(g_DG[i].FadeIn);
		exINI["DualGinsu"]["FadeOut"] = std::to_string(g_DG[i].FadeOut);
		exINI["DualGinsu"]["DecelDeltaRPMThreshold"] = std::to_string(g_DG[i].DecelDeltaRPMThreshold);
		exINI["DualGinsu"]["DecelPitchOffset"] = std::to_string(g_DG[i].DecelPitchOffset);
		exINI["DualGinsu"]["DecelAEMSMixLRPM"] = std::to_string(g_DG[i].DecelAEMSMixLRPM);
		exINI["DualGinsu"]["DecelGinsuMixLRPM"] = std::to_string(g_DG[i].DecelGinsuMixLRPM);
		exINI["DualGinsu"]["DecelAEMSMixSRPM"] = std::to_string(g_DG[i].DecelAEMSMixSRPM);
		exINI["DualGinsu"]["DecelGinsuMixSRPM"] = std::to_string(g_DG[i].DecelGinsuMixSRPM);

		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		DualGinsuFileNames.push_back(AFile);
	}


	// EngineData
	auto EngineDataINIPath = CarSoundDataPath / "EngineData";
	std::filesystem::create_directories(EngineDataINIPath, ErrorCode);

	for (int i = 0; i < EngineDataCount; i++)
	{
		// Create file name from Main (EE) RAM Bank
		SetFileName(AFile._n, g_ED[i].MainRAMBankName);
		sprintf(FileNameBuf, "%02d_%s.ini", i, AFile._n);

		auto exINIPath = EngineDataINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["EngineData"]["MainRAMBankName"] = g_ED[i].MainRAMBankName;
		exINI["EngineData"]["AuxRAMBankName"] = g_ED[i].AuxRAMBankName;
		exINI["EngineData"]["SweetnerDataSetFileName"] = GetNameFromIndex(g_ED[i].CarID, SweetnerDataFileNames);
		exINI["EngineData"]["GinsuAccel"] = g_ED[i].GinsuAccel;
		exINI["EngineData"]["UseDualGinsu"] = std::to_string(g_ED[i].UseDualGinsu);
		exINI["EngineData"]["DualGinsuFileName"] = GetNameFromIndex(g_ED[i].GinsuDecelID, DualGinsuFileNames);
		exINI["EngineData"]["AccelFromIdleFileName"] = GetNameFromIndex(g_ED[i].AccelFromIdleID, AccelFromIdleFileNames);
		exINI["EngineData"]["MaxRPM"] = std::to_string(g_ED[i].MaxRPM);
		exINI["EngineData"]["MinRPM"] = std::to_string(g_ED[i].MinRPM);
		exINI["EngineData"]["AEMSVol"] = std::to_string(g_ED[i].AEMSVol);
		exINI["EngineData"]["GinsuAccelVol"] = std::to_string(g_ED[i].GinsuAccelVol);
		exINI["EngineData"]["AccelDeltaRPMThreshold"] = std::to_string(g_ED[i].AccelDeltaRPMThreshold);
		exINI["EngineData"]["AEMSMixLRPM"] = std::to_string(g_ED[i].AEMSMixLRPM);
		exINI["EngineData"]["GinsuMixLRPM"] = std::to_string(g_ED[i].GinsuMixLRPM);
		exINI["EngineData"]["AEMSMixSRPM"] = std::to_string(g_ED[i].AEMSMixSRPM);
		exINI["EngineData"]["GinsuMixSRPM"] = std::to_string(g_ED[i].GinsuMixSRPM);
		exINI["EngineData"]["AEMSMixNRPM"] = std::to_string(g_ED[i].AEMSMixNRPM);
		exINI["EngineData"]["GinsuMixNRPM"] = std::to_string(g_ED[i].GinsuMixNRPM);
		exINI["EngineData"]["GinsuLowPassCutoff"] = std::to_string(g_ED[i].GinsuLowPassCutoff);

		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		EngineDataFileNames.push_back(AFile);
	}


	// ShiftPattern
	auto ShiftPatternINIPath = CarSoundDataPath / "ShiftPattern";
	std::filesystem::create_directories(ShiftPatternINIPath, ErrorCode);

	for (int i = 0; i < ShiftPatternCount; i++)
	{
		// Create file name from Bank Name
		SetFileName(AFile._n, g_SH[i].BankName);
		sprintf(FileNameBuf, "%02d_%s.ini", i, AFile._n);

		auto exINIPath = ShiftPatternINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["ShiftPattern"]["BankName"] = g_SH[i].BankName;
		exINI["ShiftPattern"]["UpVolShift"] = std::to_string(g_SH[i].UpVolShift);
		exINI["ShiftPattern"]["UpShiftSoundDelay"] = std::to_string(g_SH[i].UpShiftSoundDelay);
		exINI["ShiftPattern"]["UpDisengageFallRPM"] = std::to_string(g_SH[i].UpDisengageFallRPM);
		exINI["ShiftPattern"]["UpDisengageFallT"] = std::to_string(g_SH[i].UpDisengageFallT);
		exINI["ShiftPattern"]["UpEngagingRiseRPM"] = std::to_string(g_SH[i].UpEngagingRiseRPM);
		exINI["ShiftPattern"]["UpEngagingRiseT"] = std::to_string(g_SH[i].UpEngagingRiseT);
		exINI["ShiftPattern"]["UpEngagingAttackT"] = std::to_string(g_SH[i].UpEngagingAttackT);
		exINI["ShiftPattern"]["UpEngagingAttackVol"] = std::to_string(g_SH[i].UpEngagingAttackVol);
		exINI["ShiftPattern"]["LFOVolAmp"] = std::to_string(g_SH[i].LFOVolAmp);
		exINI["ShiftPattern"]["LFOVolFreq"] = std::to_string(g_SH[i].LFOVolFreq);
		exINI["ShiftPattern"]["LFOVolDecayTime"] = std::to_string(g_SH[i].LFOVolDecayTime);
		exINI["ShiftPattern"]["LFORPMAmp"] = std::to_string(g_SH[i].LFORPMAmp);
		exINI["ShiftPattern"]["LFORPMFreq"] = std::to_string(g_SH[i].LFORPMFreq);
		exINI["ShiftPattern"]["LFORPMDecayTime"] = std::to_string(g_SH[i].LFORPMDecayTime);
		exINI["ShiftPattern"]["DownVolShift"] = std::to_string(g_SH[i].DownVolShift);
		exINI["ShiftPattern"]["DownShiftSoundDelay"] = std::to_string(g_SH[i].DownShiftSoundDelay);
		exINI["ShiftPattern"]["DownDisengageFallRPM"] = std::to_string(g_SH[i].DownDisengageFallRPM);
		exINI["ShiftPattern"]["DownDisengageFallT"] = std::to_string(g_SH[i].DownDisengageFallT);
		exINI["ShiftPattern"]["DownEngagingRiseRPM"] = std::to_string(g_SH[i].DownEngagingRiseRPM);
		exINI["ShiftPattern"]["DownEngagingRiseT"] = std::to_string(g_SH[i].DownEngagingRiseT);
		exINI["ShiftPattern"]["DownEngagingFallRPM"] = std::to_string(g_SH[i].DownEngagingFallRPM);
		exINI["ShiftPattern"]["DownEngagingFallT"] = std::to_string(g_SH[i].DownEngagingFallT);
		exINI["ShiftPattern"]["DownReattachScale"] = std::to_string(g_SH[i].DownReattachScale);

		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		ShiftPatternFileNames.push_back(AFile);
	}


	// TurboDataSet
	auto TurboDataSetINIPath = CarSoundDataPath / "TurboDataSet";
	std::filesystem::create_directories(TurboDataSetINIPath, ErrorCode);

	for (int i = 0; i < TurboDataCount; i++)
	{
		// Create file name from Bank Name
		SetFileName(AFile._n, g_TDS[i].TurboBank);
		sprintf(FileNameBuf, "%02d_%s.ini", i, AFile._n);

		auto exINIPath = TurboDataSetINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["Turbo"]["TurboBank"] = g_TDS[i].TurboBank;
		exINI["Turbo"]["SpoolVol"] = std::to_string(g_TDS[i].SpoolVol);
		exINI["Turbo"]["BlowoffVol1"] = std::to_string(g_TDS[i].BlowoffVol1);
		exINI["Turbo"]["BlowoffVol2"] = std::to_string(g_TDS[i].BlowoffVol2);
		exINI["Turbo"]["ChargeTime"] = std::to_string(g_TDS[i].ChargeTime);
		exINI["Turbo"]["LeakRate"] = std::to_string(g_TDS[i].LeakRate);

		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		TurboDataFileNames.push_back(AFile);
	}


	// CarDataMapping
	auto CarDataMappingINIPath = CarSoundDataPath / "CarDataMapping";
	std::filesystem::create_directories(CarDataMappingINIPath, ErrorCode);

	for (int i = 0; i < CarDataMappingCount; i++)
	{
		// Create file name from the first car that uses this mapping
		sprintf(FileNameBuf, "%02d_%s.ini", i, GetCarTypeNameFromMapping(i));

		auto exINIPath = CarDataMappingINIPath / FileNameBuf;
		mINI::INIFile exINIFile(exINIPath.string());
		mINI::INIStructure exINI;
		exINIFile.read(exINI);

		exINI["Stock"]["EngineData"] = GetNameFromIndex(g_CDM[i].StockEngineData, EngineDataFileNames);
		exINI["Stock"]["ShiftPattern"] = GetNameFromIndex(g_CDM[i].StockShiftPatternData, ShiftPatternFileNames);
		exINI["Stock"]["TurboDataSet"] = GetNameFromIndex(g_CDM[i].StockTurboData, TurboDataFileNames);

		exINI["Street"]["EngineData"] = GetNameFromIndex(g_CDM[i].StreetEngineData, EngineDataFileNames);
		exINI["Street"]["ShiftPattern"] = GetNameFromIndex(g_CDM[i].StreetShiftPatternData, ShiftPatternFileNames);
		exINI["Street"]["TurboDataSet"] = GetNameFromIndex(g_CDM[i].StreetTurboData, TurboDataFileNames);

		exINI["Pro"]["EngineData"] = GetNameFromIndex(g_CDM[i].ProEngineData, EngineDataFileNames);
		exINI["Pro"]["ShiftPattern"] = GetNameFromIndex(g_CDM[i].ProShiftPatternData, ShiftPatternFileNames);
		exINI["Pro"]["TurboDataSet"] = GetNameFromIndex(g_CDM[i].ProTurboData, TurboDataFileNames);

		exINI["Extreme"]["EngineData"] = GetNameFromIndex(g_CDM[i].ExtremeEngineData, EngineDataFileNames);
		exINI["Extreme"]["ShiftPattern"] = GetNameFromIndex(g_CDM[i].ExtremeShiftPatternData, ShiftPatternFileNames);
		exINI["Extreme"]["TurboDataSet"] = GetNameFromIndex(g_CDM[i].ExtremeTurboData, TurboDataFileNames);


		exINIFile.write(exINI, true);

		// Save file name for later
		SetFileName(AFile._n, FileNameBuf); // get rid of ini
		CarDataFileNames.push_back(AFile);
	}


	// CarTypeMapping

	// _General.ini
	auto ctmDefINIPath = CarSoundDataPath / "_General.ini";
	mINI::INIFile ctmDefINIFile(ctmDefINIPath.string());
	mINI::INIStructure ctmDefINI;
	ctmDefINIFile.read(ctmDefINI);

	ctmDefINI["CarTypeMapping"]["CarDataMapping"] = GetNameFromIndex(GetCarTypeMapping(-1), CarDataFileNames);

	ctmDefINIFile.write(ctmDefINI, true);

	// Car-Specific (CARNAME.ini)
	for (int i = 0; i < CarCount; i++)
	{
		sprintf(CarININame, "%s.ini", GetCarTypeName(i));
		auto ctmINIPath = CarSoundDataPath / CarININame;
		mINI::INIFile ctmINIFile(ctmINIPath.string());
		mINI::INIStructure ctmINI;
		ctmINIFile.read(ctmINI);

		ctmINI["CarTypeMapping"]["CarDataMapping"] = GetNameFromIndex(GetCarTypeMapping(i), CarDataFileNames);

		ctmINIFile.write(ctmINI, true);
	}
}

bool IsLegacyCarSoundTunerEnabledFromConfig()
{
	auto LegacyCSTSettingsPath = CurrentWorkingDirectory / "NFSU2CarSoundTunerSettings.ini";
	mINI::INIFile cstINIFile(LegacyCSTSettingsPath.string());
	mINI::INIStructure cstINI;
	cstINIFile.read(cstINI);

	return mINI_ReadInteger(cstINI, "Main", "EnableCarSoundTuner", 1) != 0;
}

void DisableLegacyCarSoundTuner()
{
	// Disable via ini
	auto LegacyCSTSettingsPath = CurrentWorkingDirectory / "NFSU2CarSoundTunerSettings.ini";
	mINI::INIFile cstINIFile(LegacyCSTSettingsPath.string());
	mINI::INIStructure cstINI;
	cstINIFile.read(cstINI);

	cstINI["Main"]["EnableCarSoundTuner"] = std::to_string(0);

	cstINIFile.write(cstINI, true);

	// Rename asi
	auto LegacyCSTASIPath = CurrentWorkingDirectory / "NFSU2CarSoundTuner.asi";
	auto LegacyCSTASIPathNew = CurrentWorkingDirectory / "NFSU2CarSoundTuner.asi_disabled";
	if (std::filesystem::exists(LegacyCSTASIPath) && !std::filesystem::exists(LegacyCSTASIPathNew))
	{
		std::filesystem::rename(LegacyCSTASIPath, LegacyCSTASIPathNew);
	}
}

void CheckAndConvertLegacyData()
{
	// Force upgrade without checking if legacy CST is present and active
	if (ForceUpgradeFromLegacyCST)
	{
		ConvertFromLegacyCST = 1;
		return;
	}

	if (!SkipLegacyCSTCheck)
	{
		// Legacy CST
		if (GetModuleHandleA("NFSU2CarSoundTuner.asi")
			&& IsLegacyCarSoundTunerEnabledFromConfig()
			&& std::filesystem::exists(CurrentWorkingDirectory / "CarSoundData"))
		{
			int btn = MessageBoxA(NULL,
				"Legacy Car Sound Tuner detected.\n"
				"Do you want to convert legacy data to Unlimiter data?",
				"NFSU2 Unlimiter",
				MB_ICONQUESTION | MB_YESNO);

			if (btn == IDYES) ConvertFromLegacyCST = 1;
		}
	}
}

void InitCarSoundTuner()
{
	SnapshotVanillaCarTypeMapping();
	SnapshotCarTypeMapping();

	// Convert Legacy Car Sound Tuner data to Unlimiter Car Sound data 
	if (ConvertFromLegacyCST)
	{
		LoadVanillaCarSoundData(); // Load vanilla data first
		LoadLegacyCarSoundData(); // Load all legacy data from CarSoundData over the vanilla ones
		SaveCarSoundData(); // Save new ones into UnlimiterData\\CarSoundData

		DisableLegacyCarSoundTuner(); // Disable Legacy CST

		MessageBoxA(NULL,
			"Converted legacy Car Sound Tuner data to Unlimiter Car Sound data.\n"
			"Also disabled legacy Car Sound Tuner.\n\n"
			"The game will now close.",
			"NFSU2 Unlimiter",
			MB_ICONINFORMATION);

		SetExitGameFlag(); // ExitTheGameFlag
	}

	// Export vanilla hardcoded data to Unlimiter Car Sound data
	if (ExportCarSoundData)
	{
		LoadVanillaCarSoundData(); // Load vanilla data
		SaveCarSoundData(); // Save new ones into UnlimiterData\\CarSoundData

		MessageBoxA(NULL,
			"Exported vanilla data as Unlimiter Car Sound data.\n\n"
			"The game will now close.",
			"NFSU2 Unlimiter",
			MB_ICONINFORMATION);

		SetExitGameFlag(); // ExitTheGameFlag
	}

	// Load Unlimiter Car Sound data
	if (LoadCarSoundData())
	{
		// Loaded successfully, set up all the hooks, replacements and counts
			
		// Point the game at our arrays instead of its own
		injector::WriteMemory(0x4594C7, g_CDM, true); // EAXCar::CarPreSetup
		injector::WriteMemory(0x4796AD, g_CDM, true); // EAXCar::EAXCar

		injector::WriteMemory(0x4594DB, g_ED, true); // EAXCar::CarPreSetup
		injector::WriteMemory(0x4642C3, g_ED, true); // CSTATEMGR_AICar::ResolveEngineBankLoading
		injector::WriteMemory(0x46431A, g_ED, true); // CSTATEMGR_AICar::ResolveEngineBankLoading
		injector::WriteMemory(0x469939, g_ED, true); // EAXAITunerCar::ConnectCar
		injector::WriteMemory(0x469959, g_ED, true); // EAXAITunerCar::ConnectCar
		injector::WriteMemory(0x476CD4, g_ED, true); // CSTATEMGR_AICar::LoadData
		injector::WriteMemory(0x4796A3, g_ED, true); // EAXCar::EAXCar

		injector::WriteMemory(0x459509, g_DG, true); // EAXCar::CarPreSetup
		injector::WriteMemory(0x459526, g_DG, true); // EAXCar::CarPreSetup

		injector::WriteMemory(0x45C384, g_SH, true); // SFXCTL_Shifting::SetupCTL
		injector::WriteMemory(0x45C379, (int*)g_CDM + 1, true); // SFXCTL_Shifting::SetupCTL
		injector::WriteMemory(0x45E6CB, g_SH, true); // CARSFX_Shift::SetupSFX
		injector::WriteMemory(0x45E6BF, (int*)g_CDM + 1, true); // CARSFX_Shift::SetupSFX

		injector::WriteMemory(0x45E6E4, g_SDS, true); // CARSFX_Shift::SetupSFX
		injector::WriteMemory(0x45E6F1, g_SDS, true); // CARSFX_Shift::SetupSFX
		injector::WriteMemory(0x45F735, g_SDS, true); // CARSFX_SparkChatter::SetupSFX
		injector::WriteMemory(0x45F743, g_SDS, true); // CARSFX_SparkChatter::SetupSFX

		injector::WriteMemory(0x45EBA7, g_TDS, true); // CARSFX_Turbo::SetupSFX
		injector::WriteMemory(0x45EB9D, (int*)g_CDM + 2, true); // CARSFX_Turbo::SetupSFX

		injector::WriteMemory(0x45CACE, g_AFI, true); // SFXCTL_AccelTrans::SetupCTL

		// Cave CarTypeMapping
		injector::MakeJMP(0x459240, GetCarTypeMappingCodeCave, true); // GetCarTypeMapping

		// Fix sweetner counts
		injector::WriteMemory<BYTE>(0x45F72C, MAX_FILES - 1, true); // CARSFX_SparkChatter::SetupSFX
		injector::WriteMemory<BYTE>(0x45E6DA, MAX_FILES - 1, true); // CARSFX_Shift::SetupSFX

		injector::WriteMemory<BYTE>(0x46137C, MAX_FILES - 1, true); // CARSFX_AIEngine::InitializeEngine
		injector::WriteMemory<int>(0x461380, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x4721BB, MAX_FILES - 1, true); // CARSFX_Shift::PlayDisengageSnd
		injector::WriteMemory<int>(0x4721BF, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x47239E, MAX_FILES - 1, true); // CARSFX_Shift::PlayEngageSnd
		injector::WriteMemory<int>(0x4723A2, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x47D9E8, MAX_FILES - 1, true); // CARSFX_PlayerEngine::InitializeEngine
		injector::WriteMemory<int>(0x47D9EC, MAX_FILES - 1, true);
		injector::WriteMemory<BYTE>(0x4851D7, MAX_FILES - 1, true); // CARSFX_SparkChatter::InitSFX
		injector::WriteMemory<int>(0x4851DB, MAX_FILES - 1, true);
	}
	else // Cannot load one or more thing(s), error out
	{
		int btn = MessageBoxA(NULL,
			"Cannot load Unlimiter Car Sound data.\n"
			"You can enable ExportCarSoundData option to export required data from hardcoded vanilla values\n\n"
			"Do you want to continue playing without Car Sound Tuner?",
			"NFSU2 Unlimiter",
			MB_ICONQUESTION | MB_YESNO);

		if (btn == IDNO) SetExitGameFlag(); // ExitTheGameFlag
	}
}