#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"

struct CameraInfo
{
	int IsAnim;
	char const* CameraInfoName = "";
	int CarSlotID;
	float TurntableRot;
	float FrontWheelRot;
	float FoV;
	float XEye;
	float XLook;
	float YEye;
	float XRot;
	float Zoom;
	float YRot;
	float AnimTime;
	float Focus;
	float UnkF;
};

struct StaticCameraInfo
{
	char const* CameraInfoName = "";
	float TurntableRot;
	float FrontWheelRot;
	float FoV;
	float XEye;
	float XLook;
	float YEye;
	float XRot;
	float Zoom;
	float YRot;
	float AnimTime;
	float Focus;
};

typedef struct
{
	char Name[MAX_PATH];
	char InfoName[MAX_PATH];
	CameraInfo Info;
} CameraInfoFile;

typedef struct
{
	char Name[MAX_PATH];
	char InfoName[MAX_PATH];
	StaticCameraInfo Info;
} StaticCameraInfoFile;

typedef struct
{
	std::vector<CameraInfoFile> PartCameraInfos;
	std::vector<CameraInfoFile> VinylCameraInfos;
	std::vector<StaticCameraInfoFile> StaticCameraInfos;
} CameraInfoStuff;

CameraInfoStuff TheCameraInfoStuff;

CameraInfo* PartCameraInfoTable = (CameraInfo*)0x7F4608;
CameraInfo* HummerEngineCameraInfo = (CameraInfo*)0x7F5D00;
CameraInfo* VinylCameraInfo = (CameraInfo*)0x7F5D40;
StaticCameraInfo* StaticCameraInfoTable = (StaticCameraInfo*)0x7F6088;

void ExportCameraInfo()
{
	const int result = MessageBox(NULL, "Are you sure you want to export camera info from the game to ini files?\r\nThis will overwrite previous files.\r\n\r\nAfter this dialog box is closed, ExportCameraInfo variable in the settings will get set back to 0.", "NFSU2 Unlimiter", MB_YESNO);
	bool Export = 0;

	switch (result)
	{
	case IDYES:
		Export = 1;
		break;
	case IDNO:
		// Do something
		break;
	}

	if (Export)
	{
		char FileNameBuf[MAX_PATH];

		for (int i = 0; i < 49 * 2; i++)
		{
			auto& CameraInfo = PartCameraInfoTable[i];

			if (i < 49)
			{
				if (CameraInfo.IsAnim) sprintf(FileNameBuf, "_Sedans_ANIM%d.ini", CameraInfo.CarSlotID - 0x40302010);
				else sprintf(FileNameBuf, "_Sedans_%s.ini", GetCarSlotIDName(CameraInfo.CarSlotID));
			}
			else
			{
				if (CameraInfo.IsAnim) sprintf(FileNameBuf, "_SUVs_ANIM%d.ini", CameraInfo.CarSlotID - 0x40302010);
				else sprintf(FileNameBuf, "_SUVs_%s.ini", GetCarSlotIDName(CameraInfo.CarSlotID));
			}

			auto CameraInfoPath = CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Part" / FileNameBuf;
			mINI::INIFile CameraInfoINIFile(CameraInfoPath.string());
			mINI::INIStructure CameraInfoINI;
			CameraInfoINIFile.read(CameraInfoINI);

			CameraInfoINI["CameraInfo"]["Name"] = CameraInfo.CameraInfoName;
			CameraInfoINI["CameraInfo"]["IsAnim"] = std::to_string(CameraInfo.IsAnim);
			CameraInfoINI["CameraInfo"]["CarSlotID"] = std::to_string(CameraInfo.CarSlotID);
			CameraInfoINI["CameraInfo"]["TurntableRot"] = std::to_string(CameraInfo.TurntableRot);
			CameraInfoINI["CameraInfo"]["FrontWheelRot"] = std::to_string(CameraInfo.FrontWheelRot);
			CameraInfoINI["CameraInfo"]["FoV"] = std::to_string(CameraInfo.FoV);
			CameraInfoINI["CameraInfo"]["XEye"] = std::to_string(CameraInfo.XEye);
			CameraInfoINI["CameraInfo"]["XLook"] = std::to_string(CameraInfo.XLook);
			CameraInfoINI["CameraInfo"]["YEye"] = std::to_string(CameraInfo.YEye);
			CameraInfoINI["CameraInfo"]["XRot"] = std::to_string(CameraInfo.XRot);
			CameraInfoINI["CameraInfo"]["Zoom"] = std::to_string(CameraInfo.Zoom);
			CameraInfoINI["CameraInfo"]["YRot"] = std::to_string(CameraInfo.YRot);
			CameraInfoINI["CameraInfo"]["AnimTime"] = std::to_string(CameraInfo.AnimTime);
			CameraInfoINI["CameraInfo"]["Focus"] = std::to_string(CameraInfo.Focus);

			CameraInfoINIFile.write(CameraInfoINI, true);
		}

		for (int i = 0; i < 1; i++)
		{
			auto& CameraInfo = HummerEngineCameraInfo[i];

			sprintf(FileNameBuf, "HUMMER_%s.ini", GetCarSlotIDName(CameraInfo.CarSlotID));
			auto CameraInfoPath = CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Part" / FileNameBuf;
			mINI::INIFile CameraInfoINIFile(CameraInfoPath.string());
			mINI::INIStructure CameraInfoINI;
			CameraInfoINIFile.read(CameraInfoINI);

			CameraInfoINI["CameraInfo"]["Name"] = CameraInfo.CameraInfoName;
			CameraInfoINI["CameraInfo"]["IsAnim"] = std::to_string(CameraInfo.IsAnim);
			CameraInfoINI["CameraInfo"]["CarSlotID"] = std::to_string(CameraInfo.CarSlotID);
			CameraInfoINI["CameraInfo"]["TurntableRot"] = std::to_string(CameraInfo.TurntableRot);
			CameraInfoINI["CameraInfo"]["FrontWheelRot"] = std::to_string(CameraInfo.FrontWheelRot);
			CameraInfoINI["CameraInfo"]["FoV"] = std::to_string(CameraInfo.FoV);
			CameraInfoINI["CameraInfo"]["XEye"] = std::to_string(CameraInfo.XEye);
			CameraInfoINI["CameraInfo"]["XLook"] = std::to_string(CameraInfo.XLook);
			CameraInfoINI["CameraInfo"]["YEye"] = std::to_string(CameraInfo.YEye);
			CameraInfoINI["CameraInfo"]["XRot"] = std::to_string(CameraInfo.XRot);
			CameraInfoINI["CameraInfo"]["Zoom"] = std::to_string(CameraInfo.Zoom);
			CameraInfoINI["CameraInfo"]["YRot"] = std::to_string(CameraInfo.YRot);
			CameraInfoINI["CameraInfo"]["AnimTime"] = std::to_string(CameraInfo.AnimTime);
			CameraInfoINI["CameraInfo"]["Focus"] = std::to_string(CameraInfo.Focus);

			CameraInfoINIFile.write(CameraInfoINI, true);
		}

		for (int i = 0; i < 14; i++)
		{
			auto& CameraInfo = VinylCameraInfo[i];

			if (i < 7)
				sprintf(FileNameBuf, "_Sedans_%s_%d.ini", GetCarSlotIDName(CameraInfo.CarSlotID), i ? i + 21 : i + 20);
			else
				sprintf(FileNameBuf, "_SUVs_%s_%d.ini", GetCarSlotIDName(CameraInfo.CarSlotID), i - 7 ? i + 21 - 7 : i + 20 - 7);

			auto CameraInfoPath = CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Vinyl" / FileNameBuf;
			mINI::INIFile CameraInfoINIFile(CameraInfoPath.string());
			mINI::INIStructure CameraInfoINI;
			CameraInfoINIFile.read(CameraInfoINI);

			CameraInfoINI["CameraInfo"]["Name"] = CameraInfo.CameraInfoName;
			CameraInfoINI["CameraInfo"]["IsAnim"] = std::to_string(CameraInfo.IsAnim);
			CameraInfoINI["CameraInfo"]["CarSlotID"] = std::to_string(CameraInfo.CarSlotID);
			CameraInfoINI["CameraInfo"]["TurntableRot"] = std::to_string(CameraInfo.TurntableRot);
			CameraInfoINI["CameraInfo"]["FrontWheelRot"] = std::to_string(CameraInfo.FrontWheelRot);
			CameraInfoINI["CameraInfo"]["FoV"] = std::to_string(CameraInfo.FoV);
			CameraInfoINI["CameraInfo"]["XEye"] = std::to_string(CameraInfo.XEye);
			CameraInfoINI["CameraInfo"]["XLook"] = std::to_string(CameraInfo.XLook);
			CameraInfoINI["CameraInfo"]["YEye"] = std::to_string(CameraInfo.YEye);
			CameraInfoINI["CameraInfo"]["XRot"] = std::to_string(CameraInfo.XRot);
			CameraInfoINI["CameraInfo"]["Zoom"] = std::to_string(CameraInfo.Zoom);
			CameraInfoINI["CameraInfo"]["YRot"] = std::to_string(CameraInfo.YRot);
			CameraInfoINI["CameraInfo"]["AnimTime"] = std::to_string(CameraInfo.AnimTime);
			CameraInfoINI["CameraInfo"]["Focus"] = std::to_string(CameraInfo.Focus);

			CameraInfoINIFile.write(CameraInfoINI, true);
		}

		for (int i = 0; i < 2; i++)
		{
			auto& CameraInfo = StaticCameraInfoTable[i];

			if (!i)
				sprintf(FileNameBuf, "_Sedans.ini");
			else
				sprintf(FileNameBuf, "_SUVs.ini");
			auto CameraInfoPath = CurrentWorkingDirectory / "UnlimiterData" / "CameraInfo" / "Static" / FileNameBuf;
			mINI::INIFile CameraInfoINIFile(CameraInfoPath.string());
			mINI::INIStructure CameraInfoINI;
			CameraInfoINIFile.read(CameraInfoINI);

			CameraInfoINI["StaticCameraInfo"]["Name"] = CameraInfo.CameraInfoName;
			CameraInfoINI["StaticCameraInfo"]["TurntableRot"] = std::to_string(CameraInfo.TurntableRot);
			CameraInfoINI["StaticCameraInfo"]["FrontWheelRot"] = std::to_string(CameraInfo.FrontWheelRot);
			CameraInfoINI["StaticCameraInfo"]["FoV"] = std::to_string(CameraInfo.FoV);
			CameraInfoINI["StaticCameraInfo"]["XEye"] = std::to_string(CameraInfo.XEye);
			CameraInfoINI["StaticCameraInfo"]["XLook"] = std::to_string(CameraInfo.XLook);
			CameraInfoINI["StaticCameraInfo"]["YEye"] = std::to_string(CameraInfo.YEye);
			CameraInfoINI["StaticCameraInfo"]["XRot"] = std::to_string(CameraInfo.XRot);
			CameraInfoINI["StaticCameraInfo"]["Zoom"] = std::to_string(CameraInfo.Zoom);
			CameraInfoINI["StaticCameraInfo"]["YRot"] = std::to_string(CameraInfo.YRot);
			CameraInfoINI["StaticCameraInfo"]["AnimTime"] = std::to_string(CameraInfo.AnimTime);
			CameraInfoINI["StaticCameraInfo"]["Focus"] = std::to_string(CameraInfo.Focus);

			CameraInfoINIFile.write(CameraInfoINI, true);
		}
	}
	
	//ExitTheGameFlag
	*(int*)0x864F4C = 1;
}

char const* GetCameraInfoForVinylGroup(unsigned int group)
{
	char const* result = "";
	int NumVinylGroups = VinylGroups.size();

	if (group > NumVinylGroups) return "";

	for (int i = 0; i < NumVinylGroups; i++)
	{
		if (VinylGroups[i].Index == group)
		{
			result = VinylGroups[i].CameraInfoName;
			break;
		}
	}
	return result;
}

CameraInfo* GetPartCameraInfo(int CarTypeID, int CarSlotID)
{
	if (!UseCameraInfo) return 0;

	// Change car slot ID if we use a common camera info for this part
	switch (CarSlotID)
	{
	case CAR_SLOT_ID::NEON_ENGINE:
	case CAR_SLOT_ID::PAINT_ENGINE:
		CarSlotID = CAR_SLOT_ID::ENGINE;
		break;

	case CAR_SLOT_ID::TRUNK:
	case CAR_SLOT_ID::PAINT_AUDIO:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_0:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_1:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_2:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_3:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_4:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_5:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_6:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_7:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_8:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_9:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_10:
	case CAR_SLOT_ID::TRUNK_AUDIO_COMP_11:
		CarSlotID = CAR_SLOT_ID::TRUNK_AUDIO;
		break;

	case CAR_SLOT_ID::FENDER:
		CarSlotID = CAR_SLOT_ID::DECAL_LEFT_DOOR;
		break;

	case CAR_SLOT_ID::QUARTER:
		CarSlotID = CAR_SLOT_ID::DECAL_RIGHT_QUARTER;
		break;

	case CAR_SLOT_ID::LICENSE_PLATE:
		CarSlotID = CAR_SLOT_ID::BRAKELIGHT;
		break;

	case CAR_SLOT_ID::TOP:
		CarSlotID = CAR_SLOT_ID::ROOF;
		break;
	}


	// Check if we have car-specific part camera info for this part
	char CameraInfoName[MAX_PATH];
	sprintf(CameraInfoName, "%s_%s", GetCarTypeName(CarTypeID), GetCarSlotIDName(CarSlotID));

	for (int i = 0; i < TheCameraInfoStuff.PartCameraInfos.size(); i++)
	{
		if (!strcmp(TheCameraInfoStuff.PartCameraInfos[i].Name, CameraInfoName))
			return &TheCameraInfoStuff.PartCameraInfos[i].Info;
	}

	// If not, check if we have sedans or SUVs part camera info
	sprintf(CameraInfoName, "%s_%s", IsSUV(CarTypeID) ? "_SUVs" : "_Sedans", GetCarSlotIDName(CarSlotID));

	for (int i = 0; i < TheCameraInfoStuff.PartCameraInfos.size(); i++)
	{
		if (!strcmp(TheCameraInfoStuff.PartCameraInfos[i].Name, CameraInfoName))
			return &TheCameraInfoStuff.PartCameraInfos[i].Info;
	}

	// If we still don't have it, use the default camera info
	return 0;
}

CameraInfo* GetVinylCameraInfo(int CarTypeID, DWORD* TheCarPart)
{
	if (!UseCameraInfo) return 0;

	int CarSlotID = CAR_SLOT_ID::VINYL_LAYER0; // Get the CarSlotID from the part data
	int VinylGroupIndex = *((BYTE*)TheCarPart + 5) & 0x1F; // Get the CarSlotID from the part data
	
	// Check if we have car-specific Vinyl camera info for this Vinyl
	char const *VinylGroupCameraInfoName = GetCameraInfoForVinylGroup(VinylGroupIndex);

	if (VinylGroupCameraInfoName[0] != '\0') // We have specific camera info for this Vinyl group
	{
		char CameraInfoName[MAX_PATH];
		sprintf(CameraInfoName, "%s_%s", GetCarTypeName(CarTypeID), VinylGroupCameraInfoName);

		for (int i = 0; i < TheCameraInfoStuff.VinylCameraInfos.size(); i++)
		{
			if (!strcmp(TheCameraInfoStuff.VinylCameraInfos[i].Name, CameraInfoName))
				return &TheCameraInfoStuff.VinylCameraInfos[i].Info;
		}

		// If not, check if we have sedans or SUVs Vinyl camera info
		sprintf(CameraInfoName, "%s_%s", IsSUV(CarTypeID) ? "_SUVs" : "_Sedans", VinylGroupCameraInfoName);

		for (int i = 0; i < TheCameraInfoStuff.VinylCameraInfos.size(); i++)
		{
			if (!strcmp(TheCameraInfoStuff.VinylCameraInfos[i].Name, CameraInfoName))
				return &TheCameraInfoStuff.VinylCameraInfos[i].Info;
		}
	}
	else // No specific camera info for this Vinyl group
	{
		return GetPartCameraInfo(CarTypeID, CarSlotID);
	}

	// If we still don't have it, use the default camera info
	return 0;
}

StaticCameraInfo* GetStaticCameraInfo(int CarTypeID)
{
	if (!UseCameraInfo) return 0;

	// Check if we have car-specific static camera info
	char CameraInfoName[MAX_PATH];
	sprintf(CameraInfoName, "%s", GetCarTypeName(CarTypeID));

	for (int i = 0; i < TheCameraInfoStuff.StaticCameraInfos.size(); i++)
	{
		if (!strcmp(TheCameraInfoStuff.StaticCameraInfos[i].Name, CameraInfoName))
			return &TheCameraInfoStuff.StaticCameraInfos[i].Info;
	}

	// If not, check if we have sedans or SUVs static camera info
	sprintf(CameraInfoName, "%s", IsSUV(CarTypeID) ? "_SUVs" : "_Sedans");

	for (int i = 0; i < TheCameraInfoStuff.StaticCameraInfos.size(); i++)
	{
		if (!strcmp(TheCameraInfoStuff.StaticCameraInfos[i].Name, CameraInfoName))
			return &TheCameraInfoStuff.StaticCameraInfos[i].Info;
	}

	// If we still don't have it, use the default static camera info
	return 0;
}

CameraInfo* __cdecl FindPartCameraInfo(int CarSlotID, int IsSUV, int IsHummer, DWORD* TheCarPart)
{
	int CameraInfoID; // edx
	int i; // ecx

	// Read Part Options for the car
	DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
	int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);

	// Get camera info from config files
	CameraInfo* result = (CarSlotID >= 64 && CarSlotID <= 67) ? GetVinylCameraInfo(CarTypeID, TheCarPart) : GetPartCameraInfo(CarTypeID, CarSlotID);
	if (result) return result;

	// If not found, use vanilla implementation:
	if (IsSUV && IsHummer && (CarSlotID == 13 || CarSlotID == 155 || CarSlotID == 68))// ENGINE, NEON_ENGINE, PAINT_ENGINE
		return HummerEngineCameraInfo;
	if (CarSlotID < 34 || CarSlotID > 46) // Not TRUNK_AUDIO(_COMP_x)
	{
		if (CarSlotID >= 64 && CarSlotID <= 67)   // VINYL_LAYER0-3
		{
			if ((*((BYTE*)TheCarPart + 5) & 0x1F) == 20)
				return &VinylCameraInfo[7 * IsSUV];
			if ((*((BYTE*)TheCarPart + 5) & 0x1Fu) >= 0x16 && (*((BYTE*)TheCarPart + 5) & 0x1Fu) <= 0x1B)
				return &VinylCameraInfo[7 * IsSUV - 21 + (*((BYTE*)TheCarPart + 5) & 0x1F)];
			CarSlotID = 64;
		}
	}
	else
	{
		CarSlotID = 34;                             // TRUNK_AUDIO
	}

	if (CarSlotID == 10 || CarSlotID == 72) CarSlotID = 34; // TRUNK, PAINT_AUDIO
	if (CarSlotID == 23) CarSlotID = 55; // FENDER
	if (CarSlotID == 24) CarSlotID = 58; // QUARTER
	if (CarSlotID == 33) CarSlotID = 15; // LICENSE_PLATE
	if (CarSlotID == 8) CarSlotID = 7; // TOP
	//if (CarSlotID == 0) CarSlotID = 53; // BASE

	CameraInfoID = 0;
	for (i = (int)&PartCameraInfoTable[49 * IsSUV].CarSlotID; *(DWORD*)i != CarSlotID; i += 60)
	{
		if (++CameraInfoID >= 49)
			return 0;
	}
	return &PartCameraInfoTable[49 * IsSUV + CameraInfoID];
}