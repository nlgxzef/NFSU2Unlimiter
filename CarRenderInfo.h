#include "stdio.h"
#include "InGameFunctions.h"

// CarRenderInfo::Render

DWORD DoLinkLPToTrunk = 0x625401;
DWORD DontLinkLPToTrunk = 0x6259FE;

int LinkLicensePlateToTrunk(int CarTypeID)
{
	return CarConfigs[CarTypeID].RenderInfo.LinkLicensePlateToTrunk;
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
	return CarConfigs[CarTypeID].RenderInfo.ShowTrunkUnderInFE;
}

DWORD DoRemoveCentreBrakeWithCustomSpoiler = 0x615823;
DWORD DontRemoveCentreBrakeWithCustomSpoiler = 0x61582B;

int RemoveCentreBrakeWithCustomSpoiler(int CarTypeID)
{
	return CarConfigs[CarTypeID].RenderInfo.RemoveCentreBrakeWithCustomSpoiler;
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

DWORD DoesHaveSunroof = 0x60C86A;
DWORD DoesNotHaveSunroof = 0x60C870;

int HasSunroof(int CarTypeID)
{
	return CarConfigs[CarTypeID].RenderInfo.HasSunroof;
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
					ShowEngine = CarPart_GetAppliedAttributeUParam(partPtr, CT_bStringHash("SHOWENGINE"), 0);
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
			AttrVal = CarPart_GetAppliedAttributeUParam(WideBodyPart, CT_bStringHash("FRONT_TIRE_OFFSET"), 0);
			FrontTireOffset = *(float*)&AttrVal;
			AttrVal =  CarPart_GetAppliedAttributeUParam(WideBodyPart, CT_bStringHash("REAR_TIRE_OFFSET"), 0);
			RearTireOffset = *(float*)&AttrVal;
		}
		
		if (FenderPart && (*((BYTE*)RideInfo + 2104 + 23) == 1)) // check has FENDER and its visibility
		{
			AttrVal = CarPart_GetAppliedAttributeUParam(FenderPart, CT_bStringHash("FRONT_TIRE_OFFSET"), 0);
			FrontTireOffset += *(float*)&AttrVal;
		}
		if (QuarterPart && (*((BYTE*)RideInfo + 2104 + 24) == 1)) // check has QUARTER and its visibility
		{
			AttrVal = CarPart_GetAppliedAttributeUParam(QuarterPart, CT_bStringHash("REAR_TIRE_OFFSET"), 0);
			RearTireOffset += *(float*)&AttrVal;
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
				AttrVal = CarPart_GetAppliedAttributeUParam(FrontWheelPart, CT_bStringHash("SPINNEROFFSET"), 0);
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

static injector::hook_back<int(*)(DWORD, int, int)> hb_GetTextureInfo;
DWORD GetNeonTextureInfo(DWORD name_hash, int return_default_texture_if_not_found, int include_unloaded_textures)
{
	// Required code is implemented in CarRenderInfo_UpdateCarReplacementTextures
	return hb_GetTextureInfo.fun(name_hash, return_default_texture_if_not_found, include_unloaded_textures);
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
			bool LED = CarPart_GetAppliedAttributeUParam((DWORD*)RideInfo[510], CT_bStringHash("LED"), 0) != 0;
			
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
				LED ? .0f : 1.0f);
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
				ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("SHOWENGINE"), 0);
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
				ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("SHOWTRUNK"), 0);
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

#define CRI_Loc_OnLights 7 // CarRenderInfo + 0x1C (Padding)
#define CRI_Loc_BrokenLights 11 // CarRenderInfo + 0x2C (Padding)
#define CRI_Loc_TimeBaseStart 15 // CarRenderInfo + 0x3C (Padding)

void CarRenderInfo_SetLightState(DWORD* CarRenderInfo, int LightID, bool on)
{
	DWORD* TheCar = (DWORD*)CarRenderInfo[0];

	if (LightID == -1 && TheCar)
	{
		// Headlights
		if (((BYTE*)TheCar)[2365]) CarRenderInfo[CRI_Loc_OnLights] |= 7;
		else CarRenderInfo[CRI_Loc_OnLights] &= 7;

		// Brakelights
		if (((BYTE*)TheCar)[2372]) CarRenderInfo[CRI_Loc_OnLights] |= 56;
		else CarRenderInfo[CRI_Loc_OnLights] &= 56;
	}
	else
	{
		if (on)
			CarRenderInfo[CRI_Loc_OnLights] |= LightID;
		else
			CarRenderInfo[CRI_Loc_OnLights] &= LightID;
	}
}

int CarRenderInfo_GetLightState(DWORD* CarRenderInfo, int LightID)
{
	int result = 0; // OFF
	bool LightsOn = false;
	bool DamageLights = false;

	CarRenderInfo_SetLightState(CarRenderInfo, -1, true); // Read all (HL + BL) from Car struct and write into CRI first

	DWORD* RideInfo = (DWORD*)CarRenderInfo[1];
	if (RideInfo)
	{
		LightsOn = CarConfigs[RideInfo[0]].Textures.HeadlightOn;
		DamageLights = CarConfigs[RideInfo[0]].Textures.DamageLights;
	}

	int OnLights = CarRenderInfo[CRI_Loc_OnLights];
	int BrokenLights = CarRenderInfo[CRI_Loc_BrokenLights];

	if (OnLights & LightID) result = 1; // ON
	if (BrokenLights & LightID) result = DamageLights ? 2 : 0; // DAMAGE0 or OFF

	return result;
}

void __fastcall CarRenderInfo_UpdateLightStateTextures(DWORD* CarRenderInfo, void* EDX_Unused)
{
	// TODO: Update with dynamic light states (also find or save them in CarRenderInfo like MW)
	
	int LeftBrakelightState; // eax
	int RightBrakelightState; // edx

	ReplacementTextureEntry* BodyReplacementTextureTable = (ReplacementTextureEntry*)(CarRenderInfo + 398);
	ReplacementTextureEntry* BodyReplacementTextureTableCarbon = (ReplacementTextureEntry*)(CarRenderInfo + 690);
	ReplacementTextureEntry* BodyReplacementTextureTableCF2 = (ReplacementTextureEntry*)(CarRenderInfo + 982);
	UsedCarTextureInfo* UsedTextureInfos = (UsedCarTextureInfo*)(CarRenderInfo + 16);

	// Headlights
	int LeftHLState = (*(int*)ForceHeadlightsOn) ? 1 : CarRenderInfo_GetLightState(CarRenderInfo, 1);
	int RightHLState = (*(int*)ForceHeadlightsOn) ? 1 : CarRenderInfo_GetLightState(CarRenderInfo, 2);

	DWORD ReplaceHLLeft = UsedTextureInfos->ReplaceHeadlightHash[LeftHLState];
	DWORD ReplaceHLRight = UsedTextureInfos->ReplaceHeadlightHash[RightHLState];
	DWORD ReplaceHLGlassLeft = UsedTextureInfos->ReplaceHeadlightGlassHash[LeftHLState];
	DWORD ReplaceHLGlassRight = UsedTextureInfos->ReplaceHeadlightGlassHash[RightHLState];

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 26, ReplaceHLLeft, 73); // Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 36, ReplaceHLLeft, 73); // Mapped Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 27, ReplaceHLRight, 73); // Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 37, ReplaceHLRight, 73); // Mapped Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 31, ReplaceHLGlassLeft, 73); // Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 41, ReplaceHLGlassLeft, 73); // Mapped Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 32, ReplaceHLGlassRight, 73); // Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 42, ReplaceHLGlassRight, 73); // Mapped Glass Right

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 26, ReplaceHLLeft, 73); // Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 36, ReplaceHLLeft, 73); // Mapped Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 27, ReplaceHLRight, 73); // Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 37, ReplaceHLRight, 73); // Mapped Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 31, ReplaceHLGlassLeft, 73); // Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 41, ReplaceHLGlassLeft, 73); // Mapped Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 32, ReplaceHLGlassRight, 73); // Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 42, ReplaceHLGlassRight, 73); // Mapped Glass Right

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 26, ReplaceHLLeft, 73); // Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 36, ReplaceHLLeft, 73); // Mapped Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 27, ReplaceHLRight, 73); // Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 37, ReplaceHLRight, 73); // Mapped Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 31, ReplaceHLGlassLeft, 73); // Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 41, ReplaceHLGlassLeft, 73); // Mapped Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 32, ReplaceHLGlassRight, 73); // Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 42, ReplaceHLGlassRight, 73); // Mapped Glass Right

	LeftBrakelightState = 0;
	RightBrakelightState = 0;
	if (*CarRenderInfo && *(BYTE*)(*CarRenderInfo + 2372))
	{
		LeftBrakelightState = 1;
		RightBrakelightState = 1;
	}
	if (*(int*)ForceBrakelightsOn)
	{
		LeftBrakelightState = 1;
		RightBrakelightState = 1;
	}

	// Brakelights
	int LeftBLState = LeftBrakelightState;
	int RightBLState = RightBrakelightState;
	int CentreBLState = 0;

	DWORD ReplaceBLLeft = UsedTextureInfos->ReplaceBrakelightHash[LeftBLState];
	DWORD ReplaceBLRight = UsedTextureInfos->ReplaceBrakelightHash[RightBLState];
	DWORD ReplaceBLCentre = UsedTextureInfos->ReplaceCentreBrakelightHash[CentreBLState];
	DWORD ReplaceBLGlassLeft = UsedTextureInfos->ReplaceBrakelightGlassHash[LeftBLState];
	DWORD ReplaceBLGlassRight = UsedTextureInfos->ReplaceBrakelightGlassHash[RightBLState];
	DWORD ReplaceBLGlassCentre = UsedTextureInfos->ReplaceBrakelightGlassHash[CentreBLState];

	// Normal table
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 28, ReplaceBLLeft, 73); // Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 38, ReplaceBLLeft, 73); // Mapped Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 29, ReplaceBLRight, 73); // Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 39, ReplaceBLRight, 73); // Mapped Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 30, ReplaceBLCentre, 73); // Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 40, ReplaceBLCentre, 73); // Mapped Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 33, ReplaceBLGlassLeft, 73); // Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 43, ReplaceBLGlassLeft, 73); // Mapped Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 34, ReplaceBLGlassRight, 73); // Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 44, ReplaceBLGlassRight, 73); // Mapped Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 35, ReplaceBLGlassCentre, 73); // Glass Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 45, ReplaceBLGlassCentre, 73); // Mapped Glass Centre

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 28, ReplaceBLLeft, 73); // Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 38, ReplaceBLLeft, 73); // Mapped Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 29, ReplaceBLRight, 73); // Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 39, ReplaceBLRight, 73); // Mapped Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 30, ReplaceBLCentre, 73); // Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 40, ReplaceBLCentre, 73); // Mapped Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 33, ReplaceBLGlassLeft, 73); // Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 43, ReplaceBLGlassLeft, 73); // Mapped Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 34, ReplaceBLGlassRight, 73); // Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 44, ReplaceBLGlassRight, 73); // Mapped Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 35, ReplaceBLGlassCentre, 73); // Glass Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 45, ReplaceBLGlassCentre, 73); // Mapped Glass Centre

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 28, ReplaceBLLeft, 73); // Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 38, ReplaceBLLeft, 73); // Mapped Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 29, ReplaceBLRight, 73); // Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 39, ReplaceBLRight, 73); // Mapped Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 30, ReplaceBLCentre, 73); // Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 40, ReplaceBLCentre, 73); // Mapped Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 33, ReplaceBLGlassLeft, 73); // Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 43, ReplaceBLGlassLeft, 73); // Mapped Glass Left
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 34, ReplaceBLGlassRight, 73); // Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 44, ReplaceBLGlassRight, 73); // Mapped Glass Right
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 35, ReplaceBLGlassCentre, 73); // Glass Centre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 45, ReplaceBLGlassCentre, 73); // Mapped Glass Centre
}

void __fastcall CarRenderInfo_UpdateCarReplacementTextures(DWORD* CarRenderInfo, void* EDX_Unused)
{
	ReplacementTextureEntry* BodyReplacementTextureTable = (ReplacementTextureEntry*)(CarRenderInfo + 398);
	ReplacementTextureEntry* BodyReplacementTextureTableCarbon = (ReplacementTextureEntry*)(CarRenderInfo + 690);
	ReplacementTextureEntry* BodyReplacementTextureTableCF2 = (ReplacementTextureEntry*)(CarRenderInfo + 982);
	UsedCarTextureInfo* UsedTextureInfos = (UsedCarTextureInfo*)(CarRenderInfo + 16);

	DWORD* CarDamage = 0;
	if (CarRenderInfo[0]) CarDamage = *(DWORD**)(CarRenderInfo[0] + 2392); // CarDamage

	DWORD* DamagedWindowHashes = (DWORD*)0x802740;
	DWORD* DamagedWindow2Hashes = (DWORD*)0x802750;

	int DamageAmount = 0; // 0 - no damage, 1 - light damage, 2 - medium damage

	// Window Damage
	for (int i = 0; i < 6; i++)
	{
		if (CarDamage) DamageAmount = (CarDamage[56] >> (2 * i + 16)) & 3;

		ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 13 + i, DamagedWindowHashes[DamageAmount], 73); // WINDOW
		ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 20 + i, DamagedWindow2Hashes[DamageAmount], 73); // WINDOW2
		
		if (i == 1)
		{
			ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTable, 19, (DamageAmount <= 0) ? 0xD195BE56 : DamagedWindowHashes[DamageAmount], 73); // REAR_DEFROSTER or REAR_WINDOW damage
		}
	}
	
	char srcTextureNameBuf[64];
	DWORD srcTextureHash = 0;
	DWORD TextureHash = 0;

	// Do Extra Customization texture replacements
	if (!DisableTextureReplacement)
	{
		DWORD* RideInfo = (DWORD*)CarRenderInfo[1];

		if (RideInfo)
		{
			// INTERIOR
			DWORD* Part = (DWORD*)RideInfo[356 + CARSLOTID_BASE];
			sprintf(srcTextureNameBuf, "%s_INTERIOR", GetCarTypeName(RideInfo[0]));
			srcTextureHash = bStringHash(srcTextureNameBuf);

			if (Part)
			{
				TextureHash = CarPart_GetTextureName(Part);
				if (TextureHash)
				{
					ReplacementTextureTable_WriteEntry(BodyReplacementTextureTable, 49, srcTextureHash, TextureHash, 73); // INTERIOR
				}
			}

			// LICENSEPLATE
			Part = (DWORD*)RideInfo[356 + CARSLOTID_LICENSE_PLATE];

			if (Part)
			{
				TextureHash = CarPart_GetTextureName(Part);
				if (TextureHash)
				{
					ReplacementTextureTable_WriteEntry(BodyReplacementTextureTable, 50, CT_bStringHash("LICENSE_PLATE"), TextureHash, 73); // LICENSE_PLATE
				}
			}

			// Attachments

			int CurrTexReplSlot = 51;

			for (int i = 0; i < CARSLOTID_ATTACHMENT_MODEL_NUM; i++)
			{
				Part = (DWORD*)RideInfo[356 + CARSLOTID_DAMAGE_FRONT + i];

				if (Part)
				{
					srcTextureHash = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME_OLD"), 0);
					TextureHash = CarPart_GetTextureName(Part);

					if (srcTextureHash && TextureHash)
					{
						ReplacementTextureTable_WriteEntry(BodyReplacementTextureTable, CurrTexReplSlot++, srcTextureHash, TextureHash, 73); // Add replacement texture
					}
				}
			}

			// Neon Texture Stuff
			Part = (DWORD*)RideInfo[356 + CARSLOTID_NEON];
			sprintf(srcTextureNameBuf, "%s_NEON", GetCarTypeName(RideInfo[0]));
			srcTextureHash = bStringHash(srcTextureNameBuf);

			if (Part)
			{
				TextureHash = CarPart_GetTextureName(Part);
				CarRenderInfo[209] = GetNeonTextureInfo(TextureHash ? TextureHash : srcTextureHash, 0, 0); // Neon Texture Info
			}
		}
	}

	// Copy main table to CF table
	memcpy(BodyReplacementTextureTableCarbon, BodyReplacementTextureTable, 0x490);
	memcpy(BodyReplacementTextureTableCF2, BodyReplacementTextureTable, 0x490);

	// Do CF Stuff
	DWORD Carbonfibre = CT_bStringHash("CARBONFIBRE");

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 0, UsedTextureInfos->ReplaceGlobalHash, 73); // Skin1
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 1, UsedTextureInfos->ReplaceGlobalHash, 73); // Skin1b
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 2, UsedTextureInfos->ReplaceGlobalHash, 73); // Global_Skin1
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 3, Carbonfibre, 73); // Carskin_Carbonfibre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCarbon, 4, Carbonfibre, 73); // U2 Carskin 0x3C84D757

	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 0, UsedTextureInfos->ReplaceGlobalHash, 73); // Skin1
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 1, UsedTextureInfos->ReplaceGlobalHash, 73); // Skin1b
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 2, UsedTextureInfos->ReplaceGlobalHash, 73); // Global_Skin1
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 3, Carbonfibre, 73); // Carskin_Carbonfibre
	ReplacementTextureTable_UpdateEntry(BodyReplacementTextureTableCF2, 4, UsedTextureInfos->ReplaceGlobalHash, 73); // U2 Carskin 0x3C84D757
}

DWORD GetWheelTextureHash(DWORD* _RideInfo, int index)
{
	DWORD* RimPart = (DWORD*)_RideInfo[356 + (index == 0 ? CARSLOTID_FRONT_WHEEL : CARSLOTID_REAR_WHEEL)];
	if (!RimPart) return 0;

	DWORD TextureHash = CarPart_GetTextureName(RimPart);
	if (TextureHash) return bStringHash2((char*)"_WHEEL", TextureHash);

	return CarConfigs[_RideInfo[0]].Textures.TireInnerMask
		? bStringHash2((char*)"_TIRE", CarPart_GetCarTypeNameHash(RimPart))
		: 0;
}

DWORD GetWheelTextureMaskHash(DWORD* _RideInfo, int index)
{
	DWORD* RimPart = (DWORD*)_RideInfo[356 + (index == 0 ? CARSLOTID_FRONT_WHEEL : CARSLOTID_REAR_WHEEL)];
	if (!RimPart) return 0;

	DWORD TextureHash = CarPart_GetTextureName(RimPart);
	if (TextureHash) return bStringHash2((char*)"_WHEEL_INNER_MASK", TextureHash);
	return CarConfigs[_RideInfo[0]].Textures.TireInnerMask
		? bStringHash2((char*)"_TIRE_INNER_MASK", CarPart_GetCarTypeNameHash(RimPart))
		: 0;
}

DWORD GetSpinnerTextureHash(DWORD* _RideInfo, int index)
{
	DWORD* RimPart = (DWORD*)_RideInfo[356 + (index == 0 ? CARSLOTID_FRONT_WHEEL : CARSLOTID_REAR_WHEEL)];
	if (!RimPart) return 0;

	DWORD TextureHash = CarPart_GetAppliedAttributeUParam(RimPart, CT_bStringHash("SPINNER_TEXTURE"), 0);
	return TextureHash;
}

DWORD GetSpinnerTextureMaskHash(DWORD* _RideInfo, int index)
{
	DWORD* RimPart = (DWORD*)_RideInfo[356 + (index == 0 ? CARSLOTID_FRONT_WHEEL : CARSLOTID_REAR_WHEEL)];
	if (!RimPart) return 0;

	DWORD TextureHash = CarPart_GetAppliedAttributeUParam(RimPart, CT_bStringHash("SPINNER_TEXTURE"), 0);
	return TextureHash ? bStringHash2((char*)"_MASK", TextureHash) : 0;
}

int BodyPartsToLookForDoorline[] = {
	CARSLOTID_WIDE_BODY,
	CARSLOTID_FRONT_BUMPER,
	CARSLOTID_REAR_BUMPER,
	CARSLOTID_SKIRT,
	CARSLOTID_BODY
};

DWORD GetDoorlineHash(DWORD* _RideInfo)
{
	char* CarTypeName = GetCarTypeName(_RideInfo[0]);
	DWORD result = bStringHash2("_DOORLINE", bStringHash(CarTypeName)); //GetDoorlineHash_Game(_RideInfo);
	DWORD resultKit = bStringHash2("_KIT", result); //GetDoorlineHash_Game(_RideInfo);
	DWORD resultWide = bStringHash2("_WIDEBODY", result); //GetDoorlineHash_Game(_RideInfo);
	
	// Look for body parts to check if any of them have a custom doorline
	for (int i = 0; i < 5; i++)
	{
		DWORD* part = (DWORD*)_RideInfo[356 + BodyPartsToLookForDoorline[i]];

		if (part) 
		{
			if (*(BYTE*)(part + 5) >> 5) // non-stock
				return i == 0
				? CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("DOORLINE"), resultWide)
				: CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("DOORLINE"), resultKit);

			else if (i < 4) continue;
			else return CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("DOORLINE"), result); // stock
		}
	}

	return result;
}

DWORD GetDoorlineMaskHash(DWORD* _RideInfo)
{
	char* CarTypeName = GetCarTypeName(_RideInfo[0]);
	DWORD CarTypeNameHash = bStringHash(CarTypeName);
	DWORD result = bStringHash2("_DOORLINE_MASK", CarTypeNameHash);
	DWORD resultKit = bStringHash2("_DOORLINE_KIT_MASK", CarTypeNameHash);
	DWORD resultWide = bStringHash2("_DOORLINE_WIDEBODY_MASK", CarTypeNameHash);

	// Look for body parts to check if any of them have a custom doorline mask
	for (int i = 0; i < 5; i++)
	{
		DWORD* part = (DWORD*)_RideInfo[356 + BodyPartsToLookForDoorline[i]];

		if (part)
		{
			if (*(BYTE*)(part + 5) >> 5) // non-stock
				return i == 0
				? CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("DOORLINE_MASK"), resultWide)
				: CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("DOORLINE_MASK"), resultKit);
			else if (i < 4) continue;
			else return CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("DOORLINE_MASK"), result); // stock
		}
	}

	return result;
}

void CompositeRim(DWORD* _RideInfo)
{
	DWORD CompositeWheelHash; // ebx
	DWORD WheelTexHash; // edi
	DWORD TextureNameHash; // eax MAPDST
	DWORD WheelInnerMaskTexHash; // eax

	for (int i = 0; i < 2; i++)
	{
		CompositeWheelHash = _RideInfo[354 + i]; // DUMMY_WHEELx, DUMMY_WHEEL_REARx?? or DUMMY_SPINNERx
		WheelTexHash = GetWheelTextureHash(_RideInfo, i);
		WheelInnerMaskTexHash = GetWheelTextureMaskHash(_RideInfo, i);
		CompositeWheel(_RideInfo, CompositeWheelHash, WheelTexHash, WheelInnerMaskTexHash, CARSLOTID_PAINT_RIM/* + i*/);
	}
}

int GetTempCarSkinTextures(DWORD* textures_to_load, int num_textures, int max_textures, DWORD* ride)
{
	// Vinyls
	for (int i = 0; i < 4; ++i) // VINYL_LAYER0-3
	{
		DWORD* VinylPart = (DWORD*)ride[356 + CARSLOTID_VINYL_LAYER0 + i];
		if (VinylPart)
		{
			DWORD VinylLayerHash = GetVinylLayerHash_Game(ride, i);
			DWORD VinylLayerMaskHash = GetVinylLayerMaskHash_Game(ride, i);
			num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, VinylLayerHash);
			num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, VinylLayerMaskHash);
		}
	}

	// DOORLINE
	DWORD DoorlineTextureHash = GetDoorlineHash(ride);
	DWORD DoorlineTextureMaskHash = GetDoorlineMaskHash(ride);
	if (DoorlineTextureHash && DoorlineTextureMaskHash)
	{
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, DoorlineTextureHash);
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, DoorlineTextureMaskHash);
	}

	// HOOD_SPOILER
	/*
	DWORD HoodSpoilerTextureHash = GetHoodSpoilerTextureHash(ride);
	DWORD HoodSpoilerTextureMaskHash = GetHoodSpoilerTextureMaskHash(ride);
	if (HoodSpoilerTextureHash && HoodSpoilerTextureMaskHash)
	{
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, HoodSpoilerTextureHash);
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, HoodSpoilerTextureMaskHash);
	}
	*/

	// FRONT_WHEEL
	DWORD WheelTextureHash = GetWheelTextureHash(ride, 0);
	DWORD WheelTextureMaskHash = GetWheelTextureMaskHash(ride, 0);
	if (WheelTextureHash && WheelTextureMaskHash)
	{
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, WheelTextureHash);
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, WheelTextureMaskHash);
	}

	// REAR_WHEEL
	DWORD RearWheelTextureHash = GetWheelTextureHash(ride, 1);
	DWORD RearWheelTextureMaskHash = GetWheelTextureMaskHash(ride, 1);
	if (RearWheelTextureHash && RearWheelTextureMaskHash)
	{
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, RearWheelTextureHash);
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, RearWheelTextureMaskHash);

		//textures_to_load[180 - 87] = WheelTextureHash; // REAR WHEEL PAINT??
	}

	// SPINNER
	DWORD SpinnerTextureHash = GetSpinnerTextureHash(ride, 0);
	DWORD SpinnerTextureMaskHash = GetSpinnerTextureMaskHash(ride, 0);
	if (SpinnerTextureHash && SpinnerTextureMaskHash)
	{
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, SpinnerTextureHash);
		num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, SpinnerTextureMaskHash);
	}
	
	return num_textures;
}

void GetUsedCarTextureInfo(UsedCarTextureInfo* info, DWORD* ride_info, int front_end_only, int engine)
{
	/*
	GetUsedCarTextureInfo_Game(info, ride_info, front_end_only, engine);
	char TextureNameBu2f[64];

	sprintf(TextureNameBu2f, "%s_SKIN1", CarTypeName); // SKIN1
	printf("%s", TextureNameBu2f); // SKIN1
	return;
	*/

	char* CarTypeName = GetCarTypeName(ride_info[0]);
	int NumUsedCarTextures = 0;
	int NumTempUsedCarTextures = 0;

	bool IsInFE = *(int*)TheGameFlowManager == 3; // TheGameFlowManager->mCurrentState

	memset(info, 0, sizeof(UsedCarTextureInfo));

	char TextureNameBuf[64];

	sprintf(TextureNameBuf, "%s_SKIN1", CarTypeName); // SKIN1
	info->MappedSkinHash = bStringHash(TextureNameBuf);

	sprintf(TextureNameBuf, "%s_SKIN1B", CarTypeName); // SKIN1B
	info->MappedSkinBHash = bStringHash(TextureNameBuf);

	info->MappedGlobalHash = CT_bStringHash("GLOBAL_SKIN1"); // Global Skin1

	DWORD* Part = (DWORD*)ride_info[356 + CARSLOTID_FRONT_WHEEL]; // Front wheel
	DWORD TextureHash = 0;
	
	if (Part)
	{
		TextureHash = CarPart_GetTextureName(Part);
		if (TextureHash)
		{
			info->MappedWheelHash = bStringHash2((char*)"_WHEEL", TextureHash);
		}
		else
		{
			sprintf(TextureNameBuf, "%s_TIRE", CarTypeName);
			info->MappedWheelHash = CarConfigs[ride_info[0]].Textures.TireInnerMask ? bStringHash(TextureNameBuf) : 0;
		}
		info->MappedSpinnerHash = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("SPINNER_TEXTURE"), 0);
	}
	else
	{
		info->MappedWheelHash = 0; // No front wheel
	}
	
	DWORD *RPart = (DWORD*)ride_info[356 + CARSLOTID_REAR_WHEEL]; // Rear wheel
	if (RPart && (Part[0] != RPart[0])) // Check if rear wheel is different from front wheel
	{
		TextureHash = CarPart_GetTextureName(RPart);
		if (TextureHash);// info->MappedWheelHash = bStringHash2((char*)"_WHEEL", TextureHash);
		else
		{
			sprintf(TextureNameBuf, "%s_TIRE", CarTypeName);
			//info->MappedWheelHash = CarConfigs[ride_info[0]].Textures.TireInnerMask ? bStringHash(TextureNameBuf) : 0;
		}
	}

	info->MappedSpoilerHash = CT_bStringHash("SPOILER_SKIN"); // Spoiler
	info->MappedRoofScoopHash = CT_bStringHash("ROOF_SKIN"); // Roof scoop

	TextureHash = ride_info[353]; // Replace skin hashes
	if (TextureHash)
	{
		info->ReplaceSkinHash = TextureHash; // mCompositeSkinHash
		info->ReplaceWheelHash = ride_info[354]; // mCompositeWheelHash
		info->ReplaceSpinnerHash = ride_info[355]; // mCompositeSpinnerHash
		info->ReplaceSpoilerHash = TextureHash;
	}
	else // non-skinnable cars
	{
		Part = (DWORD*)ride_info[356 + CARSLOTID_BASE_PAINT]; // paint

		TextureHash = info->MappedSkinHash;
		info->ReplaceSkinHash = TextureHash;
		info->ReplaceSpoilerHash = TextureHash;
		info->ReplaceWheelHash = 0;
		info->ReplaceSpinnerHash = 0;
	}

	info->ReplaceRoofScoopHash = TextureHash;

	info->ReplaceSkinBHash = 0;
	info->ReplaceGlobalHash = info->ReplaceSkinHash;

	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceSkinHash); // Add replace skin hash
	if (!ride_info[353] && info->ReplaceSkinBHash) NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceSkinBHash);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceWheelHash); // Add replace wheel hash
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceSpinnerHash); // Add replace spinner hash

	NumTempUsedCarTextures = GetTempCarSkinTextures(info->TexturesToLoadTemp, NumTempUsedCarTextures, MaxTempTex, ride_info);

	sprintf(TextureNameBuf, "%s_SKIN2", CarTypeName); // SKIN2
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf)); // Add Skin2

	sprintf(TextureNameBuf, "%s_SKIN3", CarTypeName); // SKIN3
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf)); // Add Skin3

	sprintf(TextureNameBuf, "%s_SKIN4", CarTypeName); // SKIN4
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf)); // Add Skin4

	// Light textures
	sprintf(TextureNameBuf, "%s_KIT00_HEADLIGHT", CarTypeName); // Headlight texture prefix
	DWORD HLTex = bStringHash(TextureNameBuf);

	sprintf(TextureNameBuf, "%s_KIT00_BRAKELIGHT", CarTypeName); // Brakelight texture prefix
	DWORD BLTex = bStringHash(TextureNameBuf);

	bool OwnCentre = CarConfigs[ride_info[0]].Textures.CentreBrake;
	bool OwnReverse = CarConfigs[ride_info[0]].Textures.Reverse;

	DWORD CentreBLTex = BLTex;
	DWORD RevLTex = BLTex;

	sprintf(TextureNameBuf, "%s_CENTRE_BRAKELIGHT", CarTypeName); // Centre brakelight texture prefix
	CentreBLTex = bStringHash(TextureNameBuf);

	if (OwnReverse)
	{
		sprintf(TextureNameBuf, "%s_REVERSE", CarTypeName); // Reverse light texture prefix
		RevLTex = bStringHash(TextureNameBuf);
	}

	// Check texture name from attributes
	Part = (DWORD*)ride_info[356 + CARSLOTID_HEADLIGHT]; // Headlights
	if (Part)
	{
		HLTex = CarPart_GetAppliedAttributeUParam(Part, 0x10C98090, HLTex); // TEXTURE_NAME
	}

	Part = (DWORD*)ride_info[356 + CARSLOTID_BRAKELIGHT]; // Brakelights
	if (Part)
	{
		BLTex = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME"), BLTex);
		CentreBLTex = OwnCentre ? CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME_CENTRE"), CentreBLTex) : CentreBLTex;
		RevLTex = OwnReverse ? CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME_REVERSE"), RevLTex) : BLTex;
	}

	info->MappedLightHash[0] = CT_bStringHash("HEADLIGHT_LEFT");
	info->MappedLightHash[1] = CT_bStringHash("HEADLIGHT_RIGHT");
	info->MappedLightHash[5] = CT_bStringHash("HEADLIGHT_GLASS_LEFT");
	info->MappedLightHash[6] = CT_bStringHash("HEADLIGHT_GLASS_RIGHT");
	info->MappedLightHash[2] = CT_bStringHash("BRAKELIGHT_LEFT");
	info->MappedLightHash[3] = CT_bStringHash("BRAKELIGHT_RIGHT");
	info->MappedLightHash[4] = CT_bStringHash("BRAKELIGHT_CENTRE");
	info->MappedLightHash[7] = CT_bStringHash("BRAKELIGHT_GLASS_LEFT");
	info->MappedLightHash[8] = CT_bStringHash("BRAKELIGHT_GLASS_RIGHT");
	info->MappedLightHash[9] = CT_bStringHash("BRAKELIGHT_GLASS_CENTRE");
	//info->MappedLightHash[10] = CT_bStringHash("REVERSE_LIGHT"); // New dynamic name for reverse lights

	bool Onf = !IsInFE && CarConfigs[ride_info[0]].Textures.BrakelightOnfInGame;

	info->ReplaceHeadlightHash[0] = bStringHash2((char*)"_OFF", HLTex);
	info->ReplaceHeadlightHash[1] = CarConfigs[ride_info[0]].Textures.HeadlightOn ? bStringHash2((char*)"_ON", HLTex) : info->ReplaceHeadlightHash[0];
	info->ReplaceHeadlightHash[2] = CarConfigs[ride_info[0]].Textures.DamageLights ? bStringHash2((char*)"_DAMAGE0", HLTex) : info->ReplaceHeadlightHash[0];
	info->ReplaceHeadlightGlassHash[0] = bStringHash2((char*)"_GLASS_OFF", HLTex);
	info->ReplaceHeadlightGlassHash[1] = CarConfigs[ride_info[0]].Textures.HeadlightOn ? bStringHash2((char*)"_GLASS_ON", HLTex) : info->ReplaceHeadlightGlassHash[0];
	info->ReplaceHeadlightGlassHash[2] = CarConfigs[ride_info[0]].Textures.DamageLights ? bStringHash2((char*)"_GLASS_DAMAGE0", HLTex) : bStringHash("HEADLIGHT_GLASS_DAMAGE0");;
	info->ReplaceBrakelightHash[0] = bStringHash2(Onf ? (char*)"_ONF" : (char*)"_OFF", BLTex);
	info->ReplaceBrakelightHash[1] = CarConfigs[ride_info[0]].Textures.BrakelightOn ? bStringHash2((char*)"_ON", BLTex) : info->ReplaceBrakelightHash[0];
	info->ReplaceBrakelightHash[2] = CarConfigs[ride_info[0]].Textures.DamageLights ? bStringHash2((char*)"_DAMAGE0", BLTex) : info->ReplaceBrakelightHash[0];
	info->ReplaceBrakelightGlassHash[0] = bStringHash2((char*)"_GLASS_OFF", BLTex);
	info->ReplaceBrakelightGlassHash[1] = CarConfigs[ride_info[0]].Textures.BrakelightOn ? bStringHash2((char*)"_GLASS_ON", BLTex) : info->ReplaceBrakelightGlassHash[0];
	info->ReplaceBrakelightGlassHash[2] = CarConfigs[ride_info[0]].Textures.DamageLights ? bStringHash2((char*)"_GLASS_DAMAGE0", BLTex) : bStringHash("BRAKELIGHT_GLASS_DAMAGE0");
	info->ReplaceCentreBrakelightHash[0] = bStringHash2((char*)"_OFF", CentreBLTex);
	info->ReplaceCentreBrakelightHash[1] = CarConfigs[ride_info[0]].Textures.CentreBrakelightOn ? bStringHash2((char*)"_ON", CentreBLTex) : info->ReplaceCentreBrakelightHash[0];
	info->ReplaceCentreBrakelightHash[2] = CarConfigs[ride_info[0]].Textures.DamageLights ? bStringHash2((char*)"_DAMAGE0", CentreBLTex) : info->ReplaceCentreBrakelightHash[0];
	info->ReplaceReverselightHash[0] = OwnReverse ? bStringHash2((char*)"_OFF", RevLTex) : 0;
	info->ReplaceReverselightHash[1] = OwnReverse ? (CarConfigs[ride_info[0]].Textures.ReverseOn ? bStringHash2((char*)"_ON", RevLTex) : info->ReplaceReverselightHash[0]) : 0;
	info->ReplaceReverselightHash[2] = OwnReverse ? (CarConfigs[ride_info[0]].Textures.DamageLights ? bStringHash2((char*)"_DAMAGE0", RevLTex) : info->ReplaceReverselightHash[0]) : 0;

	// Add light textures to the table
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceHeadlightHash[0]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceHeadlightHash[1]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceHeadlightHash[2]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceHeadlightGlassHash[0]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceHeadlightGlassHash[1]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceHeadlightGlassHash[2]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceBrakelightHash[0]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceBrakelightHash[1]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceBrakelightHash[2]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceBrakelightGlassHash[0]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceBrakelightGlassHash[1]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceBrakelightGlassHash[2]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceCentreBrakelightHash[0]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceCentreBrakelightHash[1]);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceCentreBrakelightHash[2]);
	
	if (OwnReverse)
	{
		NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceReverselightHash[0]);
		NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceReverselightHash[1]);
		NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, info->ReplaceReverselightHash[2]);
	}

	// Add other car textures
	sprintf(TextureNameBuf, "%s_MISC", CarTypeName); // MISC
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	sprintf(TextureNameBuf, "%s_INTERIOR", CarTypeName); // INTERIOR
	TextureHash = bStringHash(TextureNameBuf);
	Part = (DWORD*)ride_info[356 + CARSLOTID_BASE]; // Interior
	if (Part)
	{
		TextureHash = CarPart_GetAppliedAttributeUParam(Part, 0x10C98090, TextureHash); // TEXTURE_NAME
	}
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	
	sprintf(TextureNameBuf, "%s_BADGING", CarTypeName); // BADGING
	TextureHash = bStringHash(TextureNameBuf);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	info->MappedBadging = TextureHash;

	sprintf(TextureNameBuf, "%s_BADGING_EU", CarTypeName); // BADGING_EU
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	sprintf(TextureNameBuf, "%s_LICENSE_PLATE", CarTypeName); // LICENSE_PLATE
	TextureHash = bStringHash(TextureNameBuf);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	info->MappedLicensePlateHash = TextureHash;

	sprintf(TextureNameBuf, "%s_SIDELIGHT", CarTypeName); // SIDELIGHT
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	sprintf(TextureNameBuf, "%s_DOOR_HANDLE", CarTypeName); // DOOR_HANDLE
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	sprintf(TextureNameBuf, "%s_LOGO", CarTypeName); // LOGO
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	sprintf(TextureNameBuf, "AUDIO_SKIN");
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	char* ShadowSuffix = IsInFE || front_end_only ? (char*)"FE" : (char*)"IG";
	sprintf(TextureNameBuf, "%s_SHADOW%s", CarTypeName, ShadowSuffix); // SHADOWFE/IG
	TextureHash = bStringHash(TextureNameBuf);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	info->ShadowHash = TextureHash;

	sprintf(TextureNameBuf, "%s_NEON", CarTypeName); // NEON
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, bStringHash(TextureNameBuf));

	sprintf(TextureNameBuf, "%s_TIRE", CarTypeName); // TIRE
	TextureHash = bStringHash(TextureNameBuf);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	info->MappedTireHash = TextureHash;

	sprintf(TextureNameBuf, "%s_RIM", CarTypeName); // RIM
	TextureHash = bStringHash(TextureNameBuf);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	info->MappedRimHash = TextureHash;

	sprintf(TextureNameBuf, "%s_RIM_BLUR", CarTypeName); // RIM_BLUR
	TextureHash = bStringHash(TextureNameBuf);
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	info->MappedRimBlurHash = TextureHash;

	if (front_end_only || engine)
	{
		Part = (DWORD*)ride_info[356 + CARSLOTID_ENGINE];
		TextureHash = Part ? CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME"), 0) : 0; // TEXTURE_NAME
		NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, TextureHash);
	}
	
	// Attachments
	DWORD srcTextureHash = 0;

	for (int i = 0; i < CARSLOTID_ATTACHMENT_MODEL_NUM; i++)
	{
		Part = (DWORD*)ride_info[356 + CARSLOTID_DAMAGE_FRONT + i];

		if (Part)
		{
			srcTextureHash = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME_OLD"), 0);
			TextureHash = CarPart_GetTextureName(Part);

			if (srcTextureHash && TextureHash)
			{
				UsedCarTextureReplace(info->TexturesToLoadPerm, NumUsedCarTextures, srcTextureHash, TextureHash); // Add replacement texture
			}
		}
	}

	// Load decals
	DWORD Size;
	DWORD Shape;
	DWORD DecalNameHash;

	int DecalSlot1 = CARSLOTID_DECAL_HOOD_TEX0;
	int DecalSlot8 = CARSLOTID_DECAL_HOOD_TEX7;

	for (int i = CARSLOTID_DECAL_MODEL_FIRST; i <= CARSLOTID_DECAL_MODEL_LAST; i++)
	{
		Part = (DWORD*)ride_info[356 + i]; // Decal dummy part
		if (Part)
		{
			Size = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("SIZE"), 0); // SIZE
			Shape = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("SHAPE"), 0); // SHAPE

			if (Shape && Size)
			{
				for (int j = DecalSlot1; j <= DecalSlot8; j++)
				{
					Part = (DWORD*)ride_info[356 + j]; // Decal texture part
					if (Part)
					{
						DecalNameHash = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("NAME"), 0); // NAME

						if (DecalNameHash)
						{
							switch (Shape)
							{
							case 0x7A8B6BD0: // SQUARE
								DecalNameHash = bStringHash2((char*)"_SQUARE", DecalNameHash);
								break;
							case 0x001C0D0D: // RECT
								DecalNameHash = bStringHash2((char*)"_RECT", DecalNameHash);
								break;
							case 0x001EDC08: // WIDE
								DecalNameHash = bStringHash2((char*)"_WIDE", DecalNameHash);
								break;
							}

							NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, DecalNameHash);
						}
					}
				}
			}
		}

		DecalSlot1 += 8;
		DecalSlot8 += 8;
	}

	info->NumTexturesToLoadPerm = NumUsedCarTextures;
	info->NumTexturesToLoadTemp = NumTempUsedCarTextures;
}

// Rear wheel light material replacement
// 0x6277FD
DWORD ecx_wheel_backup;
void __declspec(naked) RearWheelLightMaterialCodeCave()
{
	_asm
	{
		mov ecx_wheel_backup, ecx
		mov ecx, dword ptr ds : [esp + eax + 0x190]
		test ecx, ecx // wheel model != 0
		jz loc_62787A
		mov ecx, dword ptr ds : [esp + eax + 0x1F0]
		test ecx, ecx // light material != 0
		jz loc_62787A
		mov eax, dword ptr ds : [esp + 0xE0]
		mov ecx, dword ptr ds : [esp + eax + 0x190]
		mov eax, dword ptr ds : [esp + eax + 0x1F0]
		push eax
		push 0x22719FA9 // MAGSILVER
		call eModel_ReplaceLightMaterial_Game
		mov eax, dword ptr ds : [esp + 0xE0]
		mov ecx, dword ptr ds : [esp + eax + 0x1F0]
		push ecx
		mov ecx, dword ptr ds : [esp + eax + 0x194]
		push 0xFD102A92 // MAGCHROME
		call eModel_ReplaceLightMaterial_Game
		mov eax, dword ptr ds : [esp + 0xE0]
		mov ecx, dword ptr ds : [esp + eax + 0x1F0]
		push ecx
		mov ecx, dword ptr ds : [esp + eax + 0x194]
		push 0x12049251 // MAGGUNMETAL
		call eModel_ReplaceLightMaterial_Game
		mov eax, dword ptr ds : [esp + 0xE0]

		loc_62787A :
		mov ecx, ecx_wheel_backup
			push 0x62787A
			retn
	}
}

// Light Flare Color Stuff

Color __fastcall CarRenderInfo_GetColor(DWORD* CarRenderInfo, void* EDX_Unused, int CarSlotID, int ColorID, Color original, int AlphaType)
{
	Color color = 0;

	DWORD* TheRideInfo = (DWORD*)CarRenderInfo[1];

	if (TheRideInfo)
	{
		DWORD* Part = RideInfo_GetPart(TheRideInfo, CarSlotID); // BODY
		if (Part)
		{
			color.r = CarPart_GetAppliedAttributeUParam(Part, ColorHashes[ColorID][0], original.r) % 256;
			color.g = CarPart_GetAppliedAttributeUParam(Part, ColorHashes[ColorID][1], original.g) % 256;
			color.b = CarPart_GetAppliedAttributeUParam(Part, ColorHashes[ColorID][2], original.b) % 256;
			if (AlphaType) color.a = CarPart_GetAppliedAttributeUParam(Part, ColorHashes[ColorID][3], original.a) % (64 * AlphaType % 5);
		}
	}

	return color;
}

float cpr = 1.0f;
float cpb = 1.0f;
float cpw = 1.0f;
float copm = 12.0f;
float copt = 1.5f;
int copModulo = 19;
float copWhiteMul = 3.0f;
float dword_903544 = 4.0f;
int counter_32323 = 0;
int counter_32327 = 0;
int dword_9B0974 = 0;
float flt_9B3504 = 0.2617994f;
float flt_9B0BD0 = 0.78539819f;

static injector::hook_back<void(*)(DWORD*, eLightFlare*, bMatrix4*, float, int, int, float, Color)> hb_eRenderLightFlare;
DWORD* _CarRenderInfo_Backup = 0;

void eRenderLightFlare_Hook(DWORD* view, eLightFlare* light_flare, bMatrix4* local_world, float intensity_scale, int a5, int a6, float a7, Color ColourOverRide, float sizescale)
{
	DWORD* CarRenderInfo = _CarRenderInfo_Backup;
	/*
	if (light_flare)
	{
		switch (light_flare->NameHash) // eLightFlare->NameHash
		{
		case 0xD09091C6: // RIGHT_HEADLIGHT
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* RHeadlightPart = RideInfo_GetPart(RideInfo, 40); // RIGHT_HEADLIGHT
					if (RHeadlightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(RHeadlightPart, bStringHash((char*)"RED"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(RHeadlightPart, bStringHash((char*)"GREEN"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(RHeadlightPart, bStringHash((char*)"BLUE"), 0) % 256;

						// If no color attribute is present, use the values from LEFT_HEADLIGHT instead
						if (r == 0 && g == 0 && b == 0)
						{
							DWORD* HeadlightPart = RideInfo_GetPart(RideInfo, 31); // LEFT_HEADLIGHT
							if (HeadlightPart)
							{
								r = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"RED"), 0) % 256;
								g = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"GREEN"), 0) % 256;
								b = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"BLUE"), 0) % 256;
							}
						}

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0x7A5BCF69: // CENTRE_HEADLIGHT
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* HeadlightPart = RideInfo_GetPart(RideInfo, 31); // LEFT_HEADLIGHT
					if (HeadlightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"DISPRED"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"DISPGREEN"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"DISPBLUE"), 0) % 256;

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0x9DB90133: // LEFT_HEADLIGHT
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* HeadlightPart = RideInfo_GetPart(RideInfo, 31); // LEFT_HEADLIGHT
					if (HeadlightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"RED"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"GREEN"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(HeadlightPart, bStringHash((char*)"BLUE"), 0) % 256;

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0xBF700A79: // RIGHT_BRAKELIGHT
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* RBrakelightPart = RideInfo_GetPart(RideInfo, 38); // RIGHT_BRAKELIGHT
					if (RBrakelightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(RBrakelightPart, bStringHash((char*)"RED"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(RBrakelightPart, bStringHash((char*)"GREEN"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(RBrakelightPart, bStringHash((char*)"BLUE"), 0) % 256;

						// If no color attribute is present, use the values from LEFT_BRAKELIGHT instead
						if (r == 0 && g == 0 && b == 0)
						{
							DWORD* BrakelightPart = RideInfo_GetPart(RideInfo, 29); // LEFT_BRAKELIGHT
							if (BrakelightPart)
							{
								r = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"RED"), 0) % 256;
								g = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"GREEN"), 0) % 256;
								b = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"BLUE"), 0) % 256;
							}
						}

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0xA2A2FC7C: // CENTRE_BRAKELIGHT
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* BrakelightPart = RideInfo_GetPart(RideInfo, 29); // LEFT_BRAKELIGHT
					if (BrakelightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"DISPRED"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"DISPGREEN"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"DISPBLUE"), 0) % 256;

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0x31A66786: // LEFT_BRAKELIGHT
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* BrakelightPart = RideInfo_GetPart(RideInfo, 29); // LEFT_BRAKELIGHT
					if (BrakelightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"RED"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"GREEN"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"BLUE"), 0) % 256;

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0x7ADF7EF8: // RIGHT_REVERSE
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* RBrakelightPart = RideInfo_GetPart(RideInfo, 38); // RIGHT_BRAKELIGHT
					if (RBrakelightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(RBrakelightPart, bStringHash((char*)"RED2"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(RBrakelightPart, bStringHash((char*)"GREEN2"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(RBrakelightPart, bStringHash((char*)"BLUE2"), 0) % 256;

						// If no color attribute is present, use the values from LEFT_BRAKELIGHT instead
						if (r == 0 && g == 0 && b == 0)
						{
							DWORD* BrakelightPart = RideInfo_GetPart(RideInfo, 29); // LEFT_BRAKELIGHT
							if (BrakelightPart)
							{
								r = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"RED2"), 0) % 256;
								g = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"GREEN2"), 0) % 256;
								b = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"BLUE2"), 0) % 256;
							}
						}

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;

		case 0x7A5B2F25: // LEFT_REVERSE
			if (CarRenderInfo)
			{
				DWORD* RideInfo = (DWORD*)CarRenderInfo[33];
				if (RideInfo)
				{
					DWORD* BrakelightPart = RideInfo_GetPart(RideInfo, 29); // LEFT_BRAKELIGHT
					if (BrakelightPart)
					{
						int r = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"RED2"), 0) % 256;
						int g = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"GREEN2"), 0) % 256;
						int b = CarPart_GetAppliedAttributeIParam(BrakelightPart, bStringHash((char*)"BLUE2"), 0) % 256;

						// eLightFlare->ColourTint (normally unused in MW)
						light_flare->ColourTint = r + (g << 8) + (b << 16);
						ColourOverRide = light_flare->ColourTint;
					}
				}
			}
			break;
		}
	}
	*/
	// Call the original function
	hb_eRenderLightFlare.fun(view, light_flare, local_world, intensity_scale, a5, a6, a7, ColourOverRide);
}

// 0x6159DE
void __declspec(naked) RenderLightFlareCodeCave()
{
	_asm
	{
		mov edi, dword ptr ds : [esp + 0x60] // CarRenderInfo
		mov _CarRenderInfo_Backup, edi
		call eRenderLightFlare_Hook

		push 0x6159E3
		retn
	}
}

float coplightflicker(float time, int offset)
{
	float a1; // [esp+0h] [ebp-4h]

	a1 = ((float)offset * copt + time) * copm + 1.5707964f;
	counter_32323 = (counter_32323 + 1) % copModulo;
	return bSin(a1);
}

float coplightflicker2(float time, int whichcolor, int flarecount)
{
	int v3; // esi
	double v4; // st7
	float a1a; // [esp+0h] [ebp-Ch]
	float v7; // [esp+4h] [ebp-8h]
	float v8; // [esp+4h] [ebp-8h]
	float v9; // [esp+8h] [ebp-4h]

	v3 = whichcolor;
	counter_32327 = (counter_32327 + 1) % copModulo;
	if (*(float*)&whichcolor == 0.0)
	{
		whichcolor = dword_9B0974;
	}
	else if (whichcolor == 1)
	{
		whichcolor = 3.1415927f; // pi
	}
	else if (whichcolor == 2)
	{
		*(float*)&whichcolor = dword_903544;
	}
	a1a = 6.2831855 / flt_9B3504 * time + 1.5707964f;
	v4 = bSin(a1a);
	v9 = v4 * v4;
	if (v3 == 2)
	{
		v7 = ((float)flarecount * copt + time) * copm + 1.5707964;
		counter_32323 = (counter_32323 + 1) % copModulo;
		return bSin(v7) * v9;
	}
	else
	{
		v8 = 6.2831855 / flt_9B0BD0 * time + *(float*)&whichcolor + 1.5707964;
		if (bSin(v8) <= 0.2)
			return 0.0;
		else
			return v9;
	}
}


float signalflicker(float time, float rate)
{
	return (fmod(time, rate) > (rate / 2.0f)) ? 1.0f : 0.0f;
}

int signalsequence(float time, float rate, int num)
{
	return floor(fmod(time, rate) / (rate / (num + 1)));
}

void __fastcall CarRenderInfo_CreateCarLightFlares(DWORD* CarRenderInfo, void* EDX_Unused)
{
	DWORD* Model;
	ePositionMarker* Marker;
	eLightFlare* Flare;
	int FlareType = 0;

	*(float*)(CarRenderInfo + CRI_Loc_TimeBaseStart) = bRandomF(1.0f); // Random start time offset

	if (CarRenderInfo[2]) // CarTypeInfo
	{
		for (int i = CARSLOTID_MODEL_NUM * 2 - 1; i >= 0; i--) // 63 slots, 2 models for 4 LODs
		{
			Marker = 0;
			Model = (DWORD*)(CarRenderInfo[1514 + i * 4]);

			if (Model)
			{
				while (1)
				{
					Marker = eModel_GetPositionMarker(Model, Marker);
					if (!Marker) break;

					Flare = (eLightFlare*)j__malloc(0x30);
					if (!Flare) break;

					switch (Marker->NameHash)
					{
					case CT_bStringHash("CENTRE_HEADLIGHT"):
						FlareType = 0; // ELF_CAR_HEADLIGHT
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_HEADLIGHT_BULB, 0, CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_HEADLIGHT, 2, 0, 0), 0);
						if (!Flare->ColourTint) goto _LeftHeadlight;
						break;
					case CT_bStringHash("RIGHT_HEADLIGHT"):
						FlareType = 0; // ELF_CAR_HEADLIGHT
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_HEADLIGHT_BULB, 0, CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_HEADLIGHT, 1, 0, 0), 0);
						if (!Flare->ColourTint) goto _LeftHeadlight;
						break;
					case CT_bStringHash("LEFT_HEADLIGHT"):
						FlareType = 0; // ELF_CAR_HEADLIGHT
					_LeftHeadlight:
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_HEADLIGHT_BULB, 0, CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_HEADLIGHT, 0, 0, 0), 0);
						break;


					case CT_bStringHash("CENTRE_BRAKELIGHT"):
						FlareType = 1; // ELF_CAR_BRAKELIGHT
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_BRAKELIGHT, 2, 0, 0);
						if (!Flare->ColourTint) goto _LeftBrakelight;
						break;
					case CT_bStringHash("RIGHT_BRAKELIGHT"):
						FlareType = 1; // ELF_CAR_BRAKELIGHT
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_BRAKELIGHT, 1, 0, 0);
						if (!Flare->ColourTint) goto _LeftBrakelight;
						break;
					case CT_bStringHash("LEFT_BRAKELIGHT"):
						FlareType = 1; // ELF_CAR_BRAKELIGHT
					_LeftBrakelight:
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_BRAKELIGHT, 0, 0, 0);
						break;


					case CT_bStringHash("RIGHT_REVERSE"):
						FlareType = 3; // ELF_CAR_REVERSELIGHT
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_BRAKELIGHT, 4, 0, 0);
						if (!Flare->ColourTint) goto _LeftReverse;
						break;
					case CT_bStringHash("LEFT_REVERSE"):
						FlareType = 3; // ELF_CAR_REVERSELIGHT
					_LeftReverse:
						Flare->ColourTint = CarRenderInfo_GetColor(CarRenderInfo, 0, CARSLOTID_BRAKELIGHT, 3, 0, 0);
						break;


					case CT_bStringHash("COPLIGHTRED"):
						Flare->ColourTint = 0x9300007F;
						FlareType = 5; // ELF_CAR_COPLIGHTRED
						break;

					case CT_bStringHash("COPLIGHTBLUE"):
						Flare->ColourTint = 0x937F0000;
						FlareType = 6; // ELF_CAR_COPLIGHTBLUE
						break;

					case CT_bStringHash("COPLIGHTWHITE"):
						Flare->ColourTint = 0x7F7F7F7F;
						FlareType = 7; // ELF_CAR_COPLIGHTWHITE
						break;

					case CT_bStringHash("COPLIGHTBRIGHTRED"):
						Flare->ColourTint = 0x660000AA;
						FlareType = 10; // ELF_CAR_COPLIGHTBRIGHTRED
						break;

					case CT_bStringHash("COPLIGHTBRIGHTBLUE"):
						Flare->ColourTint = 0x66870300;
						FlareType = 11; // ELF_CAR_COPLIGHTBRIGHTBLUE
						break;

					case CT_bStringHash("COPLIGHTORANGE"):
						Flare->ColourTint = 0x662684FF;
						FlareType = 12; // ELF_CAR_COPLIGHTORANGE
						break;


					case CT_bStringHash("FRONT_LEFT_SIGNAL"):
					case CT_bStringHash("FRONT_RIGHT_SIGNAL"):
					case CT_bStringHash("REAR_LEFT_SIGNAL"):
					case CT_bStringHash("REAR_RIGHT_SIGNAL"):
						Flare->ColourTint = 0x9300496D;
						FlareType = 11; //17; // ELF_BLINKING_AMBER
						break;
					}

					Flare->NameHash = Marker->NameHash;
					Flare->Type = FlareType;

					switch (FlareType)
					{
					case 5: // ELF_CAR_COPLIGHTRED
					case 6: // ELF_CAR_COPLIGHTBLUE
					case 7: // ELF_CAR_COPLIGHTWHITE
					case 10: // ELF_CAR_COPLIGHTBRIGHTRED
					case 11: // ELF_CAR_COPLIGHTBRIGHTBLUE
					case 12: // ELF_CAR_COPLIGHTORANGE
						Flare->Flags = 2; // n-Directional
						break;
					default:
						Flare->Flags = 4; // Uni-Directional
						break;
					}
					Flare->PositionX = Marker->Matrix.v3.x - *((float*)CarRenderInfo + 12);
					Flare->PositionY = Marker->Matrix.v3.y - *((float*)CarRenderInfo + 13);
					Flare->PositionZ = Marker->Matrix.v3.z - *((float*)CarRenderInfo + 14);
					Flare->ReflectPosZ = 0;
					Flare->DirectionX = Marker->Matrix.v2.x;
					Flare->DirectionY = Marker->Matrix.v2.y;
					Flare->DirectionZ = Marker->Matrix.v2.z;

					//bTNode<eLightFlare>_AddAfter((bTNode<eLightFlare>*)CarRenderInfo[65], CarRenderInfo[66]);

					eLightFlare* Last = (eLightFlare*)CarRenderInfo[253];
					Last->Next = Flare;
					CarRenderInfo[253] = (DWORD)Flare;
					Flare->Prev = Last;
					Flare->Next = (eLightFlare*)(CarRenderInfo + 252);

				}
			}
		}
	}
}

void __fastcall CarRenderInfo_RenderFlaresOnCar(DWORD* CarRenderInfo, void* EDX_Unused, DWORD* view, bVector3* position, bMatrix4* body_matrix, int force_light_state, int reflection, float scale)
{
	float time = *(float*)(CarRenderInfo + CRI_Loc_TimeBaseStart) + *(float*)0x7FB718; //WorldTimeSeconds + this->CarTimebaseStart
	bMatrix4* LocalWorld = eFrameMallocMatrix(1);

	DWORD* TheCar = (DWORD*)CarRenderInfo[0]; // this

	if (LocalWorld)
	{
		bCopy(LocalWorld, body_matrix);
		LocalWorld->v3.x = position->x;
		LocalWorld->v3.y = position->y;
		LocalWorld->v3.z = position->z;
		LocalWorld->v3.w = 1.0f;

		//if (reflection) CarRenderInfo_RenderTextureHeadlights_Hook(CarRenderInfo, 0, view, (float*)LocalWorld, 0);
		// Debug cop lights
		CarRenderInfo_SetLightState(CarRenderInfo, ForceLightFlaresOn, true);

		DWORD* RideInfo = (DWORD*)CarRenderInfo[1]; // this->pRideInfo
		if (IsCop(RideInfo[0]) && (CarRenderInfo[CRI_Loc_OnLights] & 0x1000) != 0)// UsageType = Cop && mOnLights = Cop Lights
			++view[21];

		// Check for US parking lights
		bool USParkingLights = 0;
		DWORD* HeadlightPart = RideInfo_GetPart(RideInfo, CARSLOTID_HEADLIGHT);
		if (HeadlightPart) USParkingLights = CarPart_GetAppliedAttributeUParam(HeadlightPart, bStringHash((char*)"US_PARKING_LIGHTS"), 0) != 0;

		int PixelSize = eView_GetPixelSize(view, position, 3.0f);

		if (eGetCurrentViewMode() == 3) PixelSize = (int)(PixelSize * *(float*)0x7A6700);

		if (PixelSize >= 4/*view[9]*/ && eView_GetVisibleState(view, (bVector3*)(CarRenderInfo + 4), (bVector3*)(CarRenderInfo + 8), LocalWorld)) // this->AABBMin, this->AABBMax
		{
			// define default intensity values
			float IntsLeftHeadlight = 0.0f;
			float IntsRightHeadlight = 0.0f;
			float IntsCentreHeadlight = 0.0f;
			float IntsLeftBrakelight = 0.85f;
			float IntsRightBrakelight = 0.85f;
			float IntsCentreBrakelight = 0.0f;

			if (!(TheCar && *(DWORD*)(TheCar[5] + 4) == 3))
			{
				IntsLeftHeadlight = 1.0f;
				IntsRightHeadlight = 1.0f;
				IntsCentreHeadlight = 1.0f;
			}
			
			float IntsLeftReverse = 0.0f;
			float IntsRightReverse = 0.0f;
			float IntsCopRed = 0.0f;
			float IntsCopBlue = 0.0f;
			float IntsCopWhite = 0.0f;
			float IntsFrontLeftSignal = 0.0f;
			float IntsFrontRightSignal = 0.0f;
			float IntsRearLeftSignal = 0.0f;
			float IntsRearRightSignal = 0.0f;

			bool FlickerHeadlights = 0;

			if (*(bool*)ForceHeadlightsOn) force_light_state |= 1;
			if (*(bool*)ForceBrakelightsOn) force_light_state |= 2;
			if (*(bool*)ForceReverselightsOn) force_light_state |= 4;

			if ((force_light_state & 1) || (TheCar && *((BYTE*)TheCar + 2365)))  // FORCE_HEADLIGHTS_ON
			{
				IntsLeftHeadlight += 1.0f;
				IntsRightHeadlight = IntsLeftHeadlight;
				IntsCentreHeadlight = IntsLeftHeadlight;
			}
			else if (force_light_state & 8) // FORCE_HEADLIGHTS_OFF
			{
				IntsLeftHeadlight = 0.0f;
				IntsRightHeadlight = 0.0f;
				IntsCentreHeadlight = 0.0f;
			}

			if (force_light_state & 2) // FORCE_BRAKELIGHTS_ON
			{
				IntsLeftBrakelight += 1.0f;
				IntsRightBrakelight = IntsLeftBrakelight;
				IntsCentreBrakelight = 1.0f;
			}
			else if (TheCar && *((BYTE*)TheCar + 2372))
			{
				IntsLeftBrakelight += 16.0f;
				IntsRightBrakelight = IntsLeftBrakelight;
				IntsCentreBrakelight = IntsLeftBrakelight * 0.5f;
			}
			else if (force_light_state & 16) // FORCE_BRAKELIGHTS_OFF
			{
				IntsLeftBrakelight = 0.85f;
				IntsRightBrakelight = 0.85f;
				IntsCentreBrakelight = 0.0f;
			}

			if (force_light_state & 4) // FORCE_REVERSELIGHTS_ON
			{
				IntsLeftReverse = 1.0f;
				IntsRightReverse = 1.0f;
			}
			else if (force_light_state & 32) // FORCE_REVERSELIGHTS_OFF
			{
				IntsLeftReverse = 0.0f;
				IntsRightReverse = 0.0f;
			}

			CarRenderInfo_SetLightState(CarRenderInfo, -1, true); // Read all (HL + BL) from Car struct and write into CRI first

			// evaluate on and broken lights
			DWORD OnLights = CarRenderInfo[CRI_Loc_OnLights];
			//if (ForceSignalsOn) OnLights += 0xF00;
			DWORD BrokenLights = CarRenderInfo[CRI_Loc_BrokenLights];

			
			// LIGHT_LHEAD
			if (OnLights & 1) IntsLeftHeadlight = 1.0f;
			if (BrokenLights & 1) IntsLeftHeadlight = 0.0f;

			// LIGHT_RHEAD
			if (OnLights & 2) IntsRightHeadlight = 1.0f;
			if (BrokenLights & 2) IntsRightHeadlight = 0.0f;

			// LIGHT_CHEAD
			if (OnLights & 4) IntsCentreHeadlight = 1.0f;
			if (BrokenLights & 4) IntsCentreHeadlight = 0.0f;

			// LIGHT_LBRAKE
			if (OnLights & 8) IntsLeftBrakelight += 16.0f;
			if (BrokenLights & 8) IntsLeftBrakelight = 0.0f;

			// LIGHT_RBRAKE
			if (OnLights & 16) IntsRightBrakelight += 16.0f;
			if (BrokenLights & 16) IntsRightBrakelight = 0.0f;

			// LIGHT_CBRAKE
			if (OnLights & 32) IntsCentreBrakelight = 16.5f;
			if (BrokenLights & 32) IntsCentreBrakelight = 0.0f;

			// LIGHT_LREVERSE
			if (OnLights & 64) IntsLeftReverse += 17.0f;
			if (BrokenLights & 64) IntsLeftReverse = 0.0f;

			// LIGHT_RREVERSE
			if (OnLights & 128) IntsRightReverse += 17.0f;
			if (BrokenLights & 128) IntsRightReverse = 0.0f;

			// LIGHT_LRSIGNAL
			if (OnLights & 256) IntsRearLeftSignal = 1.0f;
			if (BrokenLights & 256) IntsRearLeftSignal = 0.0f;

			// LIGHT_RRSIGNAL
			if (OnLights & 512) IntsRearRightSignal = 1.0f;
			if (BrokenLights & 512) IntsRearRightSignal = 0.0f;

			// LIGHT_LFSIGNAL
			if (OnLights & 1024) IntsFrontLeftSignal = 1.0f;
			if (BrokenLights & 1024) IntsFrontLeftSignal = 0.0f;

			// LIGHT_RFSIGNAL
			if (OnLights & 2048) IntsFrontRightSignal = 1.0f;
			if (BrokenLights & 2048) IntsFrontRightSignal = 0.0f;

			// LIGHT_COPRED
			if (OnLights & 4096) IntsCopRed = cpr;
			if (BrokenLights & 4096) IntsCopRed = 0.0f;

			// LIGHT_COPBLUE
			if (OnLights & 8192) IntsCopBlue = cpb;
			if (BrokenLights & 8192) IntsCopBlue = 0.0f;

			// LIGHT_COPWHITE
			if (OnLights & 16384)
			{
				IntsCopWhite = cpw;
				FlickerHeadlights = 1;
			}
			if (BrokenLights & 16384) IntsCopWhite = 0.0f;

			int flarecount = 0;
			float copflicker = coplightflicker(time, 0);
			float signalintensity = signalflicker(time, 0.7f);

			float IntensityScale = 0.0f;
			float ReflectionOverride = 0.0f;
			int ReflexionAction = 0;
			int hl = 0;
			int bl = 0;
			int v35 = 0;

			// Spoiler
			DWORD* SpoilerPart = RideInfo_GetPart(RideInfo, CARSLOTID_SPOILER);
			if (SpoilerPart && *(BYTE*)(SpoilerPart + 5) >> 5 && RemoveCentreBrakeWithCustomSpoiler(RideInfo[0]))
				IntsCentreBrakelight = 0.0;

			// Other stuff
			v35 = RideInfo[569] ? *(char*)(RideInfo[569] + 4) : 69;

			// Render
			for (eLightFlare* Flare = (eLightFlare*)CarRenderInfo[252]; //this->LightFlareList
				(DWORD*)Flare != CarRenderInfo + 252;
				Flare = Flare->Next)
			{
				IntensityScale = 0.0f;
				ReflectionOverride = 0.0f;
				
				if (IsTraffic(RideInfo[0]) && Flare->Type == 1) Flare->Type == 2; // ELF_CAR_TRAFFIC_BRAKELIGHT

				//if ((!(renderFlareFlags & 2) || Flare->Type == 1)
				//	&& (!(renderFlareFlags & 1) || Flare->Type - 5 <= 7))
				{
					switch (Flare->NameHash)
					{
					case 0x9DB90133: // LEFT_HEADLIGHT
						//if (TheCar && !*((BYTE*)TheCar + 2365)) continue;
						IntensityScale = FlickerHeadlights ? copflicker * IntsLeftHeadlight : IntsLeftHeadlight;
						//Flare->ColourTint = CarRenderInfo[224];
						break;

					case 0xD09091C6: // RIGHT_HEADLIGHT
						//if (TheCar && !*((BYTE*)TheCar + 2365)) continue;
						IntensityScale = FlickerHeadlights ? (1.0f - copflicker) * IntsRightHeadlight : IntsRightHeadlight;
						//Flare->ColourTint = CarRenderInfo[224];
						break;

					case 0x7A5BCF69: // CENTRE_HEADLIGHT
						//if (TheCar && !*((BYTE*)TheCar + 2365)) continue;
						IntensityScale = IntsCentreHeadlight;
						//Flare->ColourTint = CarRenderInfo[224];
						break;

					case 0x31A66786: // LEFT_BRAKELIGHT
						IntensityScale = IntsLeftBrakelight;
						break;

					case 0xBF700A79: // RIGHT_BRAKELIGHT
						IntensityScale = IntsRightBrakelight;
						break;

					case 0xA2A2FC7C: // CENTRE_BRAKELIGHT
						IntensityScale = IntsCentreBrakelight;
						break;

					case 0x7A5B2F25: // LEFT_REVERSE
						IntensityScale = IntsLeftReverse;
						break;

					case 0x7ADF7EF8: // RIGHT_REVERSE
						IntensityScale = IntsRightReverse;
						break;

					case 0x2E68A46F: // FRONT_LEFT_SIGNAL
						IntensityScale = USParkingLights ? IntsLeftHeadlight : signalintensity * IntsFrontLeftSignal;
						break;

					case 0x6045CE90: // REAR_LEFT_SIGNAL
						IntensityScale = signalintensity * IntsRearLeftSignal;
						break;

					case 0x513456E2: // FRONT_RIGHT_SIGNAL
						IntensityScale = USParkingLights ? IntsRightHeadlight : signalintensity * IntsFrontRightSignal;
						break;

					case 0xBEB6C523: // REAR_RIGHT_SIGNAL
						IntensityScale = signalintensity * IntsRearRightSignal;
						break;

					case 0x1E4150B4: // COPLIGHTRED
					case 0x41489594: // COPLIGHTBRIGHTRED
						IntensityScale = coplightflicker2(time, 0, flarecount) * IntsCopRed;
						break;

					case 0xE662C161: // COPLIGHTBLUE
					case 0x6A52A241: // COPLIGHTBRIGHTBLUE
						IntensityScale = coplightflicker2(time, 1, flarecount) * IntsCopBlue;
						break;

					case 0xB4348DBA: // COPLIGHTWHITE
						IntensityScale = bSin(coplightflicker2(time, 2, flarecount) * IntsCopWhite * copWhiteMul);
						break;

					case 0x28CD78F5: // COPLIGHTORANGE
						IntensityScale = 1.0f;
						break;
					}

					if ((v35 == 15 && !bl) || (v35 != 14 || !hl))
					{
						if (IntensityScale > 0.0f)
						{
							if (IntensityScale > 1.0f) IntensityScale = 1.0f;
							if (scale != 0.0f) IntensityScale = IntensityScale * scale;

							if (reflection)
							{
								if (TheCar && (int)TheCar[265] < 3)
								{
									ReflectionOverride = 0.0f;
									ReflexionAction = 1;
								}
								else
								{
									ReflectionOverride = Flare->PositionX * LocalWorld->v0.z
										+ Flare->PositionY * LocalWorld->v1.z
										+ -Flare->PositionZ * LocalWorld->v2.z
										+ LocalWorld->v3.z;
									ReflexionAction = 2;
								}
								hb_eRenderLightFlare.fun(view, Flare, LocalWorld, IntensityScale, ReflexionAction, 2, ReflectionOverride, Flare->ColourTint);
							}
							else
							{
								hb_eRenderLightFlare.fun(view, Flare, LocalWorld, IntensityScale, 0, 0/*renderFlareFlags & 1*/, 0.0f, Flare->ColourTint);
							}

							flarecount++;
						}
					}
				}
			}

			/*
			// NIS Light stuff
			if (view[1] == 1 && !reflection)
			{
				float NISLightIntensity = 1.0f;
				bVector3 NISLightPos;
				NISLightPos.x = *(float*)_gTWEAKER_NISLightPosX + position->x;
				NISLightPos.y = *(float*)_gTWEAKER_NISLightPosY + position->y;
				NISLightPos.z = *(float*)_gTWEAKER_NISLightPosZ + position->z;

				if (*(bool*)_gTWEAKER_NISLightEnabled)
				{
					position = &NISLightPos;
					NISLightIntensity = *(float*)_gTWEAKER_NISLightIntensity;
				}
				if (IntsCopRed > 0.0f || IntsCopBlue > 0.0f)
				{
					if (IntsCopRed <= 0.0f)
					{
						if (IntsCopBlue > 0.0f)
						{
							IntensityScale = NISLightIntensity * IntsCopBlue;
							AddQuickDynamicLight((DWORD*)_ShaperLightsCharacters, *(int*)_Lightslot, 0.2f, 0.2f, 0.8f, IntensityScale, position);
						}
					}
					else
					{
						IntensityScale = NISLightIntensity * IntsCopRed;
						AddQuickDynamicLight((DWORD*)_ShaperLightsCharacters, *(int*)_Lightslot, 0.8f, 0.2f, 0.0f, IntensityScale, position);
					}
				}
				else
				{
					RestoreShaperRig((DWORD*)_ShaperLightsCharacters, *(int*)_Lightslot, (DWORD*)_ShaperLightsCharactersBackup);
				}
			}
			*/
		}
	}
}