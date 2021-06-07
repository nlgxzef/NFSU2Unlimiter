#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"

typedef struct
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
}CameraInfo;

typedef struct
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
}StaticCameraInfo;

CameraInfo* PartCameraInfoTable = (CameraInfo*)0x7F4608;
CameraInfo* HummerEngineCameraInfo = (CameraInfo*)0x7F5D00;
CameraInfo* VinylCameraInfo = (CameraInfo*)0x7F5D40;
StaticCameraInfo* StaticCameraInfoTable = (StaticCameraInfo*)0x7F6088;

CameraInfo* __cdecl FindPartCameraInfo(int CarSlotID, int IsSUV, int IsHummer, DWORD* TheCarPart)
{
	int CameraInfoID; // edx
	int i; // ecx

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