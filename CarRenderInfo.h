#include "stdio.h"
#include "InGameFunctions.h"
#include "includes\IniReader.h"
#include "Config.h"

// CarRenderInfo::Render

DWORD DoLinkLPToTrunk = 0x625401;
DWORD DontLinkLPToTrunk = 0x6259FE;

int LinkLicensePlateToTrunk_Game(int CarTypeID)
{
	switch (CarTypeID)
	{
	case 0:
	case 1:
	case 5:
	case 8:
	case 9:
	case 12:
	case 13:
	case 14:
	case 17:
	case 25:
		return 1;
	default:
		return 0;
	}

	return 0;
}

int LinkLicensePlateToTrunk(int CarTypeID)
{
	auto config = GetCarConfigByType(CarTypeID);
	if (config && config->LinkLicensePlateToTrunk != ConfigState_Default)
	{
		return config->LinkLicensePlateToTrunk == ConfigState_Enabled;
	}

	return LinkLicensePlateToTrunk_Game(CarTypeID);
}

void __declspec(naked) LinkLicensePlateToTrunkCodeCave()
{
	_asm mov CarTypeID_Temp, eax;
	_asm pushad;

	if (LinkLicensePlateToTrunk(CarTypeID_Temp))
	{
		_asm popad;
		_asm jmp DoLinkLPToTrunk;
	}
	else
	{
		_asm popad;
		_asm jmp DontLinkLPToTrunk;
	}
}

int ShowTrunkUnderInFE(int CarTypeID)
{
	auto config = GetCarConfigByType(CarTypeID);
	if (config && config->ShowTrunkUnderInFE != ConfigState_Default)
	{
		return config->ShowTrunkUnderInFE == ConfigState_Enabled;
	}

	return  ShowTrunkUnderInFE_Game(CarTypeID);
}

DWORD DoRemoveCentreBrakeWithCustomSpoiler = 0x615823;
DWORD DontRemoveCentreBrakeWithCustomSpoiler = 0x61582B;

int RemoveCentreBrakeWithCustomSpoiler(int CarTypeID)
{
	auto config = GetCarConfigByType(CarTypeID);
	if (config && config->RemoveCentreBrakeWithCustomSpoiler != ConfigState_Default)
	{
		return config->RemoveCentreBrakeWithCustomSpoiler == ConfigState_Enabled;
	}

	return  RemoveCentreBrakeWithCustomSpoiler_Game(CarTypeID);
}

void __declspec(naked) RemoveCentreBrakeWithCustomSpoilerCodeCave()
{
	_asm mov CarTypeID_Temp, eax;
	_asm pushad;

	if (RemoveCentreBrakeWithCustomSpoiler(CarTypeID_Temp))
	{
		_asm popad;
		_asm add esp, 4
		_asm jmp DoRemoveCentreBrakeWithCustomSpoiler;
	}
	else
	{
		_asm popad;
		_asm add esp, 4
		_asm jmp DontRemoveCentreBrakeWithCustomSpoiler;
	}
}

int HasSunroof_Game(int CarTypeID)
{
	switch (CarTypeID)
	{
	case 3:
	case 5:
	case 18:
	case 23:
		return 1;
	default:
		return 0;
	}

	return 0;
}

DWORD DoesHaveSunroof = 0x60C86A;
DWORD DoesNotHaveSunroof = 0x60C870;

int HasSunroof(int CarTypeID)
{
	auto config = GetCarConfigByType(CarTypeID);
	if (config && config->HasSunroof != ConfigState_Default)
	{
		return config->HasSunroof == ConfigState_Enabled;
	}

	return HasSunroof_Game(CarTypeID);
}

void __declspec(naked) HasSunroofCodeCave()
{
	_asm mov CarTypeID_Temp, eax;
	_asm pushad;

	if (HasSunroof(CarTypeID_Temp))
	{
		_asm popad;
		_asm jmp DoesHaveSunroof;
	}
	else
	{
		_asm popad;
		_asm jmp DoesNotHaveSunroof;
	}
}

void __declspec(naked) CabinNeonFixCodeCave1()
{
	_asm
	{
		jg origcode
		mov eax, [esi + 0x04]
		mov eax, [eax + 0x800]
		mov cl, [eax + 0x05]
		shr cl, 05
		test cl, cl
		jng loc_622D2F

		origcode :
		push 0x6227CE
			retn

			loc_622D2F :
		push 0x622D2F
			retn
	}
}

void __declspec(naked) CabinNeonFixCodeCave2()
{
	_asm
	{
		movzx eax, byte ptr ds : [esi + 0x360 + 0xC]
		mov[esp + 0x5C], eax
		fild dword ptr ds : [esp + 0x5C]
		fmul st(0), st(1)
		call _ftol2
		movzx edx, byte ptr ds : [esi + 0x361 + 0xC]
		movzx ecx, byte ptr ds : [esi + 0x35C + 0xC]
		mov[esp + 0x5C], edx
		add eax, ecx
		mov[esp + 0xD8], eax
		fild dword ptr ds : [esp + 0x5C]
		fmul st(0), st(1)
		call _ftol2
		movzx edx, byte ptr ds : [esi + 0x362 + 0xC]
		movzx ecx, byte ptr ds : [esi + 0x35D + 0xC]
		mov[esp + 0x5C], edx
		add eax, ecx
		mov[esp + 0x190], eax
		fild dword ptr ds : [esp + 0x5C]
		fmul st(0), st(1)
		call _ftol2
		movzx edx, byte ptr ds : [esi + 0x363 + 0xC]
		movzx ecx, byte ptr ds : [esi + 0x35E + 0xC]
		mov[esp + 0x5C], edx
		add eax, ecx
		mov[esp + 0x1F0], eax
		fild dword ptr ds : [esp + 0x5C]
		fmul st(0), st(1)
		call _ftol2
		movzx ecx, byte ptr ds : [esi + 0x35F + 0xC]
		mov edx, [esp + 0xD4]
		add edx, eax
		mov eax, [esp + 0xD8]
		add ecx, edx
		test eax, eax
		jle p1
		cmp eax, 0xFE
		mov[esp + 0xC0], 0xFE
		jg p2
		p5 :
		mov[esp + 0xC0], eax
			p2 :
		mov eax, [esp + 0x190]
			test eax, eax
			jg p3
			xor eax, eax
			jmp p4
			p1 :
		xor eax, eax
			jmp p5
			p3 :
		cmp eax, 0xFE
			mov[esp + 0xD8], 0xFE
			jg p6
			p4 :
		mov[esp + 0xD8], eax
			p6 :
		mov eax, [esp + 0x1F0]
			test eax, eax
			jg p7
			xor eax, eax
			jmp p8
			p7 :
		cmp eax, 0xFE
			mov edx, 0xFE
			jg p9
			p8 :
		mov edx, eax
			p9 :
		test ecx, ecx
			jg p10
			xor ecx, ecx
			jmp p11
			p10 :
		cmp ecx, 0xFE
			mov eax, 0xFE
			jg p12
			p11 :
		mov eax, ecx
			p12 :
		mov ecx, [esp + 0xD8]
			shl eax, 8
			or eax, edx
			shl eax, 8
			or eax, ecx
			mov ecx, [esp + 0xC0]
			shl eax, 8
			or eax, ecx
			mov[esi + 0x364 + 0xC], eax

			originalcode :
		mov eax, [esp + 0xEC]

			push 0x622BE0
			retn
	}
}

// 0x6304C4
void __declspec(naked) ShowEngineAttrCodeCave()
{
	_asm
	{
		mov eax, [esp + 0x30C]
		mov esi, [esp + 0x58]
		//pushad
	}

	DWORD* CarRenderInfo, * RideInfo, * partPtr;
	DWORD ShowEngine;

	_asm mov CarRenderInfo, esi;
	_asm mov ShowEngine, eax;

	if (!ShowEngine)
	{
		if (CarRenderInfo)
		{
			RideInfo = (DWORD*)CarRenderInfo[1]; // RideInfo
			if (RideInfo)
			{
				partPtr = (DWORD*)RideInfo[356 + 9]; // HOOD
				if (partPtr)
				{
					ShowEngine = CarPart_GetAppliedAttributeUParam(partPtr, bStringHash("SHOWENGINE"), 0);
				}

			}
		}
	}

	_asm
	{
		//popad
		mov eax, ShowEngine
		//mov dword ptr ds: [esp+0x30C], eax
		push 0x6304CB
		retn
	}
}

// 0x61548A
void __declspec(naked) CarRenderInfoAttributesCodeCave()
{
	_asm
	{
		pop edi
		pop esi
		pop ebx
		pop ecx
		pushad
	}

	DWORD* CarRenderInfo, *RideInfo, *partPtr;
	DWORD AttrVal;
	float fAttrVal;

	_asm mov dword ptr CarRenderInfo, esi;

	if (CarRenderInfo)
	{
		RideInfo = (DWORD*)CarRenderInfo[1]; // RideInfo
		if (RideInfo)
		{
			partPtr = (DWORD*)RideInfo[356 + 23]; // FENDER
			if (partPtr)
			{
				fAttrVal = (float)CarPart_GetAppliedAttributeUParam(partPtr, bStringHash("FRONT_TRACK_WIDTH"), 0);
				if (fAttrVal)
				{
					// increase track width
				}
			}

			partPtr = (DWORD*)RideInfo[356 + 24]; // QUARTER
			if (partPtr)
			{
				fAttrVal = (float)CarPart_GetAppliedAttributeUParam(partPtr, bStringHash("REAR_TRACK_WIDTH"), 0);
				if (fAttrVal)
				{
					// increase track width
				}
			}
		}
	}


	_asm
	{
		popad
		retn
	}
}

void __fastcall CarRenderInfo_UpdateWheelYRenderOffset(DWORD* CarRenderInfo, void* EDX_Unused)
{
	DWORD* RideInfo; // eax
	DWORD* FrontWheelPart; // ecx MAPDST
	DWORD* RearWheelPart; // eax
	DWORD* FenderPart;
	DWORD* QuarterPart;
	DWORD* WideBodyPart;
	int i; // edi
	int WheelID;
	float* WheelWidthOffsets; // ebp
	float* WheelOffsets; // ebx
	DWORD* CarTypeInfo; // esi
	float* CurrWheel; // esi
	int SpinnerOffsetHash; // eax
	float* SpinnerOffsetAttr; // eax
	float SpinnerOffset; // st7
	float FrontTireOffset;
	float RearTireOffset;
	int UseCustomWidth; // eax
	float CurrWheelWidth; // st7
	int FrontWheelSomething; // [esp+Ch] [ebp-10h]
	int RearWheelSomething; // [esp+10h] [ebp-Ch]
	float WheelWidth; // [esp+18h] [ebp-4h]
	DWORD AttrVal;

	CarTypeInfo = (DWORD*)CarRenderInfo[2];
	if (CarTypeInfo)
	{
		RideInfo = (DWORD*)CarRenderInfo[1];
		if (RideInfo)
		{
			FrontWheelPart = (DWORD*)RideInfo[356 + 29]; // FRONT_WHEEL
			RearWheelPart = (DWORD*)RideInfo[356 + 30]; // REAR_WHEEL
			FenderPart = (DWORD*)RideInfo[356 + 23]; // FENDER
			QuarterPart = (DWORD*)RideInfo[356 + 24]; // QUARTER
			WideBodyPart = (DWORD*)RideInfo[356 + 6]; // WIDE_BODY
		}
		else
		{
			FrontWheelPart = 0;
			RearWheelPart = 0;
			FenderPart = 0;
			QuarterPart = 0;
			WideBodyPart = 0;
		}

		// Check our custom attributes for track width
		FrontTireOffset = 0;
		RearTireOffset = 0;
		if (WideBodyPart && (*((BYTE*)RideInfo + 2104 + 6) == 1)) // check has WIDE_BODY and its visibility
		{
			AttrVal = CarPart_GetAppliedAttributeUParam(WideBodyPart, bStringHash("FRONT_TIRE_OFFSET"), 0);
			FrontTireOffset = *(float*)&AttrVal;
			AttrVal =  CarPart_GetAppliedAttributeUParam(WideBodyPart, bStringHash("REAR_TIRE_OFFSET"), 0);
			RearTireOffset = *(float*)&AttrVal;
		}
		else
		{
			if (FenderPart && (*((BYTE*)RideInfo + 2104 + 23) == 1)) // check has FENDER and its visibility
			{
				AttrVal = CarPart_GetAppliedAttributeUParam(FenderPart, bStringHash("FRONT_TIRE_OFFSET"), 0);
				FrontTireOffset = *(float*)&AttrVal;
			}
			if (QuarterPart && (*((BYTE*)RideInfo + 2104 + 24) == 1)) // check has QUARTER and its visibility
			{
				AttrVal = CarPart_GetAppliedAttributeUParam(QuarterPart, bStringHash("REAR_TIRE_OFFSET"), 0);
				RearTireOffset = *(float*)&AttrVal;
			}
		}

		if (FrontWheelPart)
			FrontWheelSomething = *((BYTE*)FrontWheelPart + 5) >> 5;
		else
			FrontWheelSomething = 0;

		if (RearWheelPart)
			RearWheelSomething = *((BYTE*)RearWheelPart + 5) >> 5;
		else
			RearWheelSomething = 0;

		WheelWidth = *(float*)_WheelStandardWidth;

		WheelWidthOffsets = (float*)(CarRenderInfo + 264); // [4]
		WheelOffsets = (float*)(CarRenderInfo + 256); // [4][2] + [4] + [2] + [4]??

		for (i = 72; i < 120; i += 12) // 72, 84, 96, 108
		{
			CurrWheel = (float*)(CarTypeInfo + i);

			WheelOffsets[0] = -CurrWheel[7]; // Y
			WheelOffsets[1] = -CurrWheel[8]; // WidebodyY

			WheelID = CarTypeInfo[i + 6];
			switch (WheelID)
			{
			case 0: // Front Left
				WheelOffsets[0] -= FrontTireOffset;
				WheelOffsets[1] -= FrontTireOffset;
				break;
			case 1: // Front Right
				WheelOffsets[0] += FrontTireOffset;
				WheelOffsets[1] += FrontTireOffset;
				break;
			case 2: // Rear Right
				WheelOffsets[0] += RearTireOffset;
				WheelOffsets[1] += RearTireOffset;
				break;
			case 3: // Rear Left
				WheelOffsets[0] -= RearTireOffset;
				WheelOffsets[1] -= RearTireOffset;
				break;
			}

			if (FrontWheelPart)
			{
				AttrVal = CarPart_GetAppliedAttributeUParam(FrontWheelPart, bStringHash("SPINNEROFFSET"), 0);
				SpinnerOffset = *(float*)&AttrVal;
				WheelOffsets[14] = SpinnerOffset;
				WheelOffsets[15] = SpinnerOffset;
			}
			UseCustomWidth = FrontWheelSomething;
			if (i >= 96)
				UseCustomWidth = RearWheelSomething;
			if (UseCustomWidth)
				CurrWheelWidth = CurrWheel[5]; // TireSkidWidth
			else
				CurrWheelWidth = WheelWidth;

			WheelWidthOffsets[0] = CurrWheelWidth;
			++WheelWidthOffsets;

			WheelOffsets += 2;
		}
	}
	else
	{
		memset(CarRenderInfo + 256, 0, 0x20u);
	}
}

void __fastcall CarRenderInfo_RenderNeon(DWORD* CarRenderInfo, void* EDX_Unused, int eView, float* bV3, float* bM4)
{
	DWORD* RideInfo; // edi MAPDST
	DWORD* AnimationThing; // eax MAPDST
	DWORD* HoodPart;
	DWORD* TrunkPart;
	DWORD ShowEngineThruHood;
	DWORD ShowAudioThruTrunk;
	int HoodLocationState; // eax
	int TrunkLocationState; // eax
	float a8; // [esp+0h] [ebp-28h]
	float a8a; // [esp+0h] [ebp-28h]
	bool IsHoodOpenOrAnimating; // [esp+20h] [ebp-8h]
	bool IsTrunkOpenOrAnimating; // [esp+24h] [ebp-4h]
	float AnimationTime; // [esp+34h] [ebp+Ch] MAPDST

	// In-Game Neon controller floats
	float flt_7A6734 = *(float*)0x7A6734;
	float flt_7A6738 = *(float*)0x7A6738;
	float flt_7A673C = *(float*)0x7A673C;
	float flt_7A6740 = *(float*)0x7A6740;
	float flt_7A65E8 = *(float*)0x7A65E8;
	float flt_7A65EC = *(float*)0x7A65EC;
	float flt_7A65F8 = *(float*)0x7A65F8;
	float flt_7A65FC = *(float*)0x7A65FC;

	RideInfo = (DWORD*)CarRenderInfo[1];
	if (RideInfo)
	{
		AnimationThing = (DWORD*)CarRenderInfo[2897];
		if (AnimationThing)
		{
			HoodLocationState = CarAnimManager_GetLocationState(*(DWORD*)(AnimationThing[2] + 208), 0);
			TrunkLocationState = CarAnimManager_GetLocationState(*(DWORD*)(AnimationThing[2] + 208), 1);
		}
		else
		{
			HoodLocationState = 1;
			TrunkLocationState = 1;
		}
		IsHoodOpenOrAnimating = HoodLocationState != 1;
		IsTrunkOpenOrAnimating = TrunkLocationState != 1;

		// Underglow Neon
		if (RideInfo[510]) // NEON
		{
			CarRenderInfo_RenderNeon2(
				CarRenderInfo,
				eView,
				bV3,
				bM4,
				CarRenderInfo[2821],
				CarRenderInfo + 2822,
				CarRenderInfo[214],
				flt_7A6734,
				1,
				1.0);
		}

		// Engine Neon
		if (RideInfo[511]) // NEON_ENGINE
		{
			AnimationTime = 0.0;
			if (IsHoodOpenOrAnimating)
				AnimationTime = CarAnimManager_GetAnimationTime(*(DWORD*)(AnimationThing[2] + 208), 0, 0);

			// Show engine neon if our custom attribute is present
			HoodPart = (DWORD*)RideInfo[356 + 9];
			if (HoodPart)
			{
				ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, bStringHash("SHOWENGINE"), 0);
				if (ShowEngineThruHood) AnimationTime = 1.0;
			}

			CarRenderInfo_RenderNeon2(
				CarRenderInfo,
				eView,
				bV3,
				bM4,
				CarRenderInfo[2840],
				CarRenderInfo + 2841,
				CarRenderInfo[217],
				flt_7A6738,
				0,
				AnimationTime);
			a8 = flt_7A65F8 * flt_7A6738;

			CarRenderInfo_RenderNeon2(
				CarRenderInfo,
				eView,
				bV3,
				bM4,
				CarRenderInfo[2840],
				CarRenderInfo + 2841,
				CarRenderInfo[217] & 0xFFFFFF | ((unsigned int)(unsigned __int64)((double)*((unsigned __int8*)CarRenderInfo
					+ 871)
					* flt_7A65FC) << 24),
				a8,
				0,
				AnimationTime);
		}

		// Cabin Neon
		if (RideInfo[512]) // NEON_CABIN
		{
			CarRenderInfo_RenderNeon2(
				CarRenderInfo,
				eView,
				bV3,
				bM4,
				CarRenderInfo[2859],
				CarRenderInfo + 2860,
				CarRenderInfo[220],
				flt_7A673C,
				0,
				1.0);
		}

		// Trunk Neon
		if (RideInfo[513])// NEON_TRUNK
		{
			AnimationTime = 0.0;
			if (IsTrunkOpenOrAnimating)
				AnimationTime = CarAnimManager_GetAnimationTime(*(DWORD*)(AnimationThing[2] + 208), 1, 0);

			// Show trunk neon if our custom attribute is present
			TrunkPart = (DWORD*)RideInfo[356 + 10];
			if (TrunkPart)
			{
				ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, bStringHash("SHOWTRUNK"), 0);
				if (ShowAudioThruTrunk) AnimationTime = 1.0;
			}

			CarRenderInfo_RenderNeon2(
				CarRenderInfo,
				eView,
				bV3,
				bM4,
				CarRenderInfo[2878],
				CarRenderInfo + 2879,
				CarRenderInfo[223],
				flt_7A6740,
				0,
				AnimationTime);
			a8a = flt_7A65E8 * flt_7A6740;
			CarRenderInfo_RenderNeon2(
				CarRenderInfo,
				eView,
				bV3,
				bM4,
				CarRenderInfo[2878],
				CarRenderInfo + 2879,
				CarRenderInfo[223] & 0xFFFFFF | ((unsigned int)(unsigned __int64)((double)*((unsigned __int8*)CarRenderInfo
					+ 895)
					* flt_7A65EC) << 24),
				a8a,
				0,
				AnimationTime);
		}
	}
}