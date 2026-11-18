#include "stdio.h"
#include "InGameFunctions.h"

#include "CarRenderInfoExtra.h"

struct Car;
struct RideInfo;
struct CarTypeInfo;

#pragma pack(push, 1)
struct NeonColour
{
	BYTE Colour[4];
	signed char ColourDiff[4];
	DWORD CurrentColour;
};

struct NeonMarkerList
{
	int NumMarkers; // 2 * number of START/END
	ePositionMarker* Markers[18]; // START_n, END_n pairs (e.g. "ENGINE_NEON_START_%d")
};

struct CarEmitterPositionList // bSList<CarEmitterPosition>
{
	void* Head;
	void* Tail;
};

struct CarPartInfo
{
	bVector3 Position;
	int NumCulledNotVisible;
	int NumCulledVisible;
	bool IsVisible;
	char _pad19[7];
};

struct CarPartCuller
{
	CarPartInfo CarPartInfoTable[11];
};

struct CarRenderInfo
{
	Car* pCar;                                                      // 0x0000
	RideInfo* pRideInfo;                                            // 0x0004
	CarTypeInfo* pCarTypeInfo;                                      // 0x0008 &CarTypeInfoArray[ride->Type]
	short LastCarPartChanged;                                       // 0x000C init -1
	char _pad00E[2];
	bVector3 AABBMin;                                               // 0x0010
	bVector3 AABBMax;                                               // 0x0020
	bVector3 PivotPosition;                                         // 0x0030 CarRenderInfoExtra* in .pad
	UsedCarTextureInfo mUsedTextureInfos;                           // 0x0040
	DWORD* ShadowTexture;                                           // 0x033C TextureInfo*, "%s_SHADOWFE" / "%s_SHADOWIG"
	DWORD* ShadowRampTexture;                                       // 0x0340 TextureInfo*
	DWORD* NeonTexture;                                             // 0x0344 TextureInfo*, "%s_NEON"
	DWORD* NeonBlurTexture;                                         // 0x0348 TextureInfo*, "NEONBLUR"
	BYTE UnderglowNeonColour[4];                                    // 0x034C NEON
	signed char UnderglowNeonColourDiff[4];                         // 0x0350
	DWORD _unk354;                                                  // 0x0354 zeroed in the ctor
	DWORD UnderglowNeonCurrentColour;                               // 0x0358
	NeonColour EngineNeonColour;                                    // 0x035C NEON_ENGINE
	NeonColour CabinNeonColour;                                     // 0x0368 NEON_CABIN
	NeonColour TrunkNeonColour;                                     // 0x0374 NEON_TRUNK
	DWORD HeadlightBulbColour;                                      // 0x0380
	DWORD* LightMaterial_CarSkin;                                   // 0x0384 eLightMaterial*
	DWORD* LightMaterial_Carbon;                                    // 0x0388
	DWORD* LightMaterial_WindowTint;                                // 0x038C
	DWORD* LightMaterial_WheelRim;                                  // 0x0390
	DWORD* LightMaterial_Exhaust;                                   // 0x0394
	DWORD* LightMaterial_Caliper;                                   // 0x0398
	DWORD* LightMaterial_Audio;                                     // 0x039C
	DWORD* LightMaterial_Spoiler;                                   // 0x03A0
	DWORD* LightMaterial_Roof;                                      // 0x03A4
	DWORD* LightMaterial_Mirror;                                    // 0x03A8
	DWORD* LightMaterial_Spinner;                                   // 0x03AC
	int CarbonFrontBumper;                                          // 0x03B0
	int CarbonRearBumper;                                           // 0x03B4
	int CarbonBodyKit;                                              // 0x03B8
	int CarbonHood;                                                 // 0x03BC
	int CarbonDoors;                                                // 0x03C0
	int CarbonTrunk;                                                // 0x03C4
	int CarbonWideBody;                                             // 0x03C8
	char _unk3CC[0x24];                                             // 0x03CC
	bTList<eLightFlare> LightFlareList;                             // 0x03F0
	int CarFrame;                                                   // 0x03F8 bRandom(8)
	int mReplacementTexturesUpdated;                                // 0x03FC
	float WheelYRenderOffset[4][2];                                 // 0x0400 per wheel {normal, widebody}
	float WheelWidths[4];                                           // 0x0420
	float WheelBrakeMarkerY[2];                                     // 0x0430 front/rear
	float WheelSpinnerOffset[4][2];                                 // 0x0438
	CarEmitterPositionList EffectEmitterList[30];                   // 0x0458
	CarEmitterPositionList EmitterPositionList[30];                 // 0x0548
	ReplacementTextureEntry MasterReplacementTextureTable[73];      // 0x0638
	ReplacementTextureEntry CarbonReplacementTextureTable[73];      // 0x0AC8
	ReplacementTextureEntry KitCarbonReplacementTextureTable[73];   // 0x0F58
	ReplacementTextureEntry DecalReplacementTextureTable[7][8];     // 0x13E8
	ReplacementTextureEntry BrakeLeftReplacementTextureTable[2];    // 0x1768
	ReplacementTextureEntry BrakeRightReplacementTextureTable[2];   // 0x1788
	eModel* mCarPartModels[63][2][4];                               // 0x17A8 [CAR_SLOT_ID][piece][LOD]
	char CarLights[0x104];                                          // 0x1F88 eDynamicLightPack "CarLights"
	char _unk208C[0x128];                                           // 0x208C
	eModel TrunkAudioModels[2][4];                                  // 0x21B4
	eModel TrunkAudioCompModels[12][2][4];                          // 0x2274
	eModel TrunkAudioEmptyModels[3][2];                             // 0x2B74
	ePositionMarker* SpoilerPositionMarker;                         // 0x2C04
	ePositionMarker* RoofScoopPositionMarker;                       // 0x2C08
	ePositionMarker* MirrorBodyLeftPositionMarker;                  // 0x2C0C
	ePositionMarker* MirrorBodyRightPositionMarker;                 // 0x2C10
	NeonMarkerList UnderglowNeonMarkers;                            // 0x2C14
	NeonMarkerList EngineNeonMarkers;                               // 0x2C60
	NeonMarkerList CabinNeonMarkers;                                // 0x2CAC
	NeonMarkerList TrunkNeonMarkers;                                // 0x2CF8
	CarRenderInfo* pAnimRenderInfo;                                 // 0x2D44 = this, CarAnimManager handle
	bool mHoodVertSplit;                                            // 0x2D48
	signed char NosBlowoffColourIndex[2];                           // 0x2D49
	char _pad2D4B[0x15];
	CarPartCuller TheCarPartCuller;                                 // 0x2D60
};
#pragma pack(pop)

static_assert(sizeof(eModel) == 0x18, "eModel size");
static_assert(sizeof(ReplacementTextureEntry) == 0x10, "ReplacementTextureEntry size");
static_assert(sizeof(UsedCarTextureInfo) == 0x2FC, "UsedCarTextureInfo size");
static_assert(sizeof(bVector3) == 0x10, "bVector3 size");
static_assert(offsetof(CarRenderInfo, mUsedTextureInfos) == 0x40, "CarRenderInfo::mUsedTextureInfos");
static_assert(offsetof(CarRenderInfo, NeonTexture) == 0x344, "CarRenderInfo::NeonTexture");
static_assert(offsetof(CarRenderInfo, CarbonDoors) == 0x3C0, "CarRenderInfo::CarbonDoors");
static_assert(offsetof(CarRenderInfo, LightFlareList) == 0x3F0, "CarRenderInfo::LightFlareList");
static_assert(offsetof(CarRenderInfo, WheelYRenderOffset) == 0x400, "CarRenderInfo::WheelYRenderOffset");
static_assert(offsetof(CarRenderInfo, MasterReplacementTextureTable) == 0x638, "CarRenderInfo::MasterReplacementTextureTable");
static_assert(offsetof(CarRenderInfo, CarbonReplacementTextureTable) == 0xAC8, "CarRenderInfo::CarbonReplacementTextureTable");
static_assert(offsetof(CarRenderInfo, KitCarbonReplacementTextureTable) == 0xF58, "CarRenderInfo::KitCarbonReplacementTextureTable");
static_assert(offsetof(CarRenderInfo, mCarPartModels) == 0x17A8, "CarRenderInfo::mCarPartModels");
static_assert(offsetof(CarRenderInfo, TrunkAudioModels) == 0x21B4, "CarRenderInfo::TrunkAudioModels");
static_assert(offsetof(CarRenderInfo, UnderglowNeonMarkers) == 0x2C14, "CarRenderInfo::UnderglowNeonMarkers");
static_assert(offsetof(CarRenderInfo, pAnimRenderInfo) == 0x2D44, "CarRenderInfo::pAnimRenderInfo");
static_assert(offsetof(CarRenderInfo, TheCarPartCuller) == 0x2D60, "CarRenderInfo::TheCarPartCuller");
static_assert(sizeof(CarRenderInfo) == 0x2EC0, "CarRenderInfo size");

inline CarRenderInfoExtra*& CarRenderInfo_ExtraSlot(CarRenderInfo* car_render_info)
{
	return *(CarRenderInfoExtra**)car_render_info->PivotPosition.pad; // CRI_Loc_Extra
}

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

int LinkRearWindowDecalToTrunk(int CarTypeID)
{
	return CarConfigs[CarTypeID].RenderInfo.LinkRearWindowDecalToTrunk;
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

	CarRenderInfo* car_render_info;
	DWORD* RideInfo, * partPtr;
	DWORD ShowEngine;

	_asm mov car_render_info, esi;
	_asm mov ShowEngine, eax;

	if (!ShowEngine)
	{
		if (car_render_info)
		{
			RideInfo = (DWORD*)car_render_info->pRideInfo; // RideInfo
			if (RideInfo)
			{
				partPtr = (DWORD*)RideInfo[356 + CARSLOTID_HOOD]; // HOOD
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

CarRenderInfoExtra* CarRenderInfo_GetExtra(CarRenderInfo* car_render_info)
{
	return CarRenderInfo_ExtraSlot(car_render_info);
}

int AnimLocationForExtraAttachment(int CarType, int CarSlotID, int orig)
{
	// LinkSpoiler 2: the spoiler stays where its marker puts it instead of following the trunk.
	if (CarSlotID == CARSLOTID_SPOILER && CarConfigs[CarType].RenderInfo.LinkSpoiler == 2)
		return -1;

	BodyShopSection& B = CarConfigs[CarType].BodyShop;

	bool ExtraEnabled[6] = {
		B.Attachment5, B.Attachment6, B.Attachment7,
		B.Attachment8, B.Attachment9, B.Attachment10,
	};

	for (int i = 0; i < 6; i++)
	{
		if (ExtraAttachmentSlots[i] == CarSlotID)
		{
			if (B.Attachments > 5 + i && ExtraEnabled[i])
				return -1;
			else
				return orig;
		}
	}

	return orig;
}

// 0x623458
void __declspec(naked) CarRenderInfo_Render_AnimLocationForExtraAttachmentCodeCave()
{
	CarRenderInfo* car_render_info;
	DWORD* RideInfo;
	int AnimLoc, CarSlotID_x2, esi_backup;

	_asm
	{
		movsx ecx, byte ptr ds : [eax + edx]
		mov AnimLoc, ecx
		mov CarSlotID_x2, eax
		mov esi_backup, esi
		mov esi, dword ptr ds: [esp + 0x58]
		mov car_render_info, esi
		mov PartAnim_RenderCarRenderInfo, esi // for the part's animation set (PartAnimations.h)
		mov PartAnim_RenderSlotPiece, eax
		pushad
	}

	if (car_render_info)
	{
		RideInfo = (DWORD*)car_render_info->pRideInfo; // RideInfo
		if (RideInfo)
		{
			AnimLoc = AnimLocationForExtraAttachment(RideInfo[0], CarSlotID_x2 / 2, AnimLoc);
		}
	}

	_asm
	{
		popad
		mov esi, esi_backup
		mov ecx, AnimLoc
		mov eax, dword ptr ds : [esp + 0xC8]
		push 0x623463
		retn
	}
}

// 0x6274CE
void __declspec(naked) CarRenderInfo_Render_RearSpinnerCodeCave()
{
	_asm
	{
		cmp dword ptr[edx + 0x1B58], ecx // front spinner model (ecx = 0), original instruction
		jne Done                         // ZF = 0: has spinners
		cmp dword ptr[edx + 0x1B78], ecx // rear spinner model; ZF = 1 only when neither exists

		Done :
		push 0x6274D4 // the flags are read by the jz at 0x6274F8
			retn
	}
}

#define CRI_Loc_Models 1514 // CarRenderInfo+17A8h: an eModel* for each slot * 2 + piece, 4 LODs each

ePositionMarker* CarRenderInfo_FindSlotMarker(CarRenderInfo* car_render_info, int CarSlotID, DWORD NameHash)
{
	eModel* Model = car_render_info->mCarPartModels[CarSlotID][0][0]; // first piece, LOD A
	if (!Model || !Model->Solid) return 0;
	return eSolid_GetPositionMarker((DWORD*)Model->Solid, NameHash);
}

ePositionMarker* __stdcall CarRenderInfo_GetBaseMarker(DWORD NameHash, DWORD* BaseSolid, CarRenderInfo* car_render_info)
{
	DWORD* RideInfo = (DWORD*)car_render_info->pRideInfo;
	if (RideInfo)
	{
		CarRenderInfoSection& R = CarConfigs[RideInfo[0]].RenderInfo;
		int From = -1;

		if (NameHash == CT_bStringHash("SPOILER") && R.LinkSpoiler == 1) From = CARSLOTID_TRUNK;
		else if (NameHash == CT_bStringHash("ROOF_SCOOP")) From = CARSLOTID_ROOF;

		if (From >= 0)
		{
			ePositionMarker* Marker = CarRenderInfo_FindSlotMarker(car_render_info, From, NameHash);
			if (Marker) return Marker;
		}
	}

	return eSolid_GetPositionMarker(BaseSolid, NameHash);
}

// 0x635021 (SPOILER) and 0x635033 (ROOF_SCOOP)
void __declspec(naked) CarRenderInfo_GetBaseMarkerCodeCave()
{
	_asm
	{
		push ebx                   // CarRenderInfo
		push ecx                   // the base's solid
		push dword ptr[esp + 0x0C] // the marker's hash
		call CarRenderInfo_GetBaseMarker
		retn 4                     // as eSolid::GetPositionMarker
	}
}

DWORD* __stdcall CarRenderInfo_GetCabinNeonModel(CarRenderInfo* car_render_info)
{
	DWORD* RideInfo = (DWORD*)car_render_info->pRideInfo;

	if (RideInfo
		&& CarRenderInfo_FindSlotMarker(car_render_info, CARSLOTID_ROOF, CT_bStringHash("CABIN_NEON_START_0")))
		return (DWORD*)car_render_info->mCarPartModels[CARSLOTID_ROOF][0][0];

	return (DWORD*)car_render_info->mCarPartModels[0][0][0]; // the base, as the game does
}

// 0x6359AC CarRenderInfo::UpdateCarParts: mov eax, [ebx+17A8h], the model the cabin neon markers
// are looked for in. ecx holds a count the code after this uses.
void __declspec(naked) CarRenderInfo_GetCabinNeonModelCodeCave()
{
	_asm
	{
		push ecx
		push edx
		push ebx // CarRenderInfo
		call CarRenderInfo_GetCabinNeonModel
		pop edx
		pop ecx
		push 0x6359B2
		retn
	}
}

bTList<AcidEmitter>* CRI_TSE_tmp;
int CRI_TSE_LoadingFromExtra = 0;

// 0x615D0E
void __declspec(naked) CarRenderInfo_TriggerEffect_TireSmokeCodeCave()
{
	_asm
	{
		cmp eax, 5
		jg LoadAcidEmitterListFromCRI

		//cmp eax, 3 // Tire Smoke
		//jne LoadAcidEmitterListFromTerrain

		// Load from extra instead
		LoadFromExtra:
		mov ecx, CRI_TSE_LoadingFromExtra
		test ecx, ecx
		jz LoadAcidEmitterListFromTerrain
		mov edi, CRI_TSE_tmp
		jmp LoadAcidEmitterListFromCRI

		LoadAcidEmitterListFromTerrain:
			push 0x615D13
			retn

		LoadAcidEmitterListFromCRI:
			push 0x615D32
			retn
	}
}

void __fastcall CarRenderInfo_TriggerEffect(CarRenderInfo* car_render_info, void* EDX_Unused, int CarEffect, float power, unsigned __int16 terrain_type, int position, bVector3* bV3, bMatrix4* bM4, bVector3* bV3_2)
{
	CRI_TSE_LoadingFromExtra = 0;
	
	// Get tire smoke emitter
	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(car_render_info);
	if (extra)
	{
		if (extra->pRideInfo)
		{
			DWORD* Part = RideInfo_GetPart(extra->pRideInfo, CARSLOTID_UL_TIRE_SMOKE);

			if ((CarEffect == CARFX_SKID_SMOKE || CarEffect == CARFX_TIRE_SPEW)
				&& Part && Part[0] != CT_bStringHash("VINYL_L1_COLOR01"))
			{
				switch (CarEffect)
				{
				case CARFX_SKID_SMOKE:
					CRI_TSE_tmp = &extra->TireSmokeEmitters[terrain_type];
					CRI_TSE_LoadingFromExtra = 1;
					break;
				case CARFX_TIRE_SPEW:
					CRI_TSE_tmp = &extra->TireSpewEmitters[terrain_type];
					CRI_TSE_LoadingFromExtra = 1;
					break;
				}
			}
		}
	}

	CarRenderInfo_TriggerEffect_Game((DWORD*)car_render_info, CarEffect, power, terrain_type, position, bV3, bM4, bV3_2);
}

void __fastcall CarRenderInfo_UpdateWheelYRenderOffset(CarRenderInfo* car_render_info, void* EDX_Unused)
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
	float FrontTireWidth;
	float RearTireWidth;
	float FrontTireRadius;
	float RearTireRadius;
	int UseCustomWidth; // eax
	float CurrWheelWidth; // st7
	int FrontWheelSomething; // [esp+Ch] [ebp-10h]
	int RearWheelSomething; // [esp+10h] [ebp-Ch]
	float WheelWidth; // [esp+18h] [ebp-4h]
	DWORD AttrVal;

	CarTypeInfo = (DWORD*)car_render_info->pCarTypeInfo;
	if (CarTypeInfo)
	{
		RideInfo = (DWORD*)car_render_info->pRideInfo;
		if (RideInfo)
		{
			FrontWheelPart = (DWORD*)RideInfo[356 + CARSLOTID_FRONT_WHEEL]; // FRONT_WHEEL
			RearWheelPart = (DWORD*)RideInfo[356 + CARSLOTID_REAR_WHEEL]; // REAR_WHEEL
			FenderPart = (DWORD*)RideInfo[356 + CARSLOTID_FENDER]; // FENDER
			QuarterPart = (DWORD*)RideInfo[356 + CARSLOTID_QUARTER]; // QUARTER
			WideBodyPart = (DWORD*)RideInfo[356 + CARSLOTID_WIDE_BODY]; // WIDE_BODY
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
		FrontTireWidth = -1.0f;
		RearTireWidth = -1.0f;
		FrontTireRadius = -1.0f;
		RearTireRadius = -1.0f;

		WheelWidthOffsets = car_render_info->WheelWidths; // [4]
		WheelOffsets = &car_render_info->WheelYRenderOffset[0][0]; // [4][2] + [4] + [2] + [4]??

		bool HasWideBody = WideBodyPart && (*((BYTE*)RideInfo + 2104 + CARSLOTID_WIDE_BODY) == 1);

		if (HasWideBody) // check has WIDE_BODY and its visibility
		{
			FrontTireOffset = CarPart_GetAppliedAttributeFParam((CarPart*)WideBodyPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_OFFSET"), 0.0f);
			RearTireOffset = CarPart_GetAppliedAttributeFParam((CarPart*)WideBodyPart, EDX_Unused, CT_bStringHash("REAR_TIRE_OFFSET"), 0.0f);

			FrontTireWidth = CarPart_GetAppliedAttributeFParam((CarPart*)WideBodyPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_WIDTH"), -1.0f);
			RearTireWidth = CarPart_GetAppliedAttributeFParam((CarPart*)WideBodyPart, EDX_Unused, CT_bStringHash("REAR_TIRE_WIDTH"), -1.0f);

			FrontTireRadius = CarPart_GetAppliedAttributeFParam((CarPart*)WideBodyPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_RADIUS"), -1.0f);
			RearTireRadius = CarPart_GetAppliedAttributeFParam((CarPart*)WideBodyPart, EDX_Unused, CT_bStringHash("REAR_TIRE_RADIUS"), -1.0f);
		}

		bool TakeBodyOffsets = AccumulateTireOffsets || !HasWideBody;
		
		if (TakeBodyOffsets && FenderPart && (*((BYTE*)RideInfo + 2104 + CARSLOTID_FENDER) == 1)) // check has FENDER and its visibility
		{
			FrontTireOffset += CarPart_GetAppliedAttributeFParam((CarPart*)FenderPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_OFFSET"), 0.0f);
		}
		if (TakeBodyOffsets && QuarterPart && (*((BYTE*)RideInfo + 2104 + CARSLOTID_QUARTER) == 1)) // check has QUARTER and its visibility
		{
			RearTireOffset += CarPart_GetAppliedAttributeFParam((CarPart*)QuarterPart, EDX_Unused, CT_bStringHash("REAR_TIRE_OFFSET"), 0.0f);
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

			// Spinner offset from the rim on this axle: rear wheels (WheelID 2, 3) use the rear rim when there is one
			DWORD* SpinnerWheelPart = (WheelID >= 2 && RearWheelPart) ? RearWheelPart : FrontWheelPart;
			if (SpinnerWheelPart)
			{
				AttrVal = CarPart_GetAppliedAttributeUParam(SpinnerWheelPart, CT_bStringHash("SPINNEROFFSET"), 0);
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

			if (WheelID <= 1)
				WheelWidthOffsets[0] = FrontTireWidth != -1.0f ? FrontTireWidth : CurrWheelWidth;
			else
				WheelWidthOffsets[0] = RearTireWidth != -1.0f ? RearTireWidth : CurrWheelWidth;
			++WheelWidthOffsets;

			WheelOffsets += 2;
		}
	}
	else
	{
		memset(car_render_info->WheelYRenderOffset, 0, sizeof(car_render_info->WheelYRenderOffset));
	}

	// Brake position markers
	int MinLodLevel = 0; //CarRenderInfo[1418]; (MW)
	eModel* front_wheel_model = car_render_info->mCarPartModels[CARSLOTID_FRONT_WHEEL][0][MinLodLevel];
	eModel* rear_wheel_model = car_render_info->mCarPartModels[CARSLOTID_REAR_WHEEL][0][MinLodLevel];
	ePositionMarker* front_position_marker = front_wheel_model ? eModel_GetPostionMarker((DWORD*)front_wheel_model, CT_bStringHash("MARKER_BRAKE")) : 0;
	ePositionMarker * rear_position_marker = rear_wheel_model ? eModel_GetPostionMarker((DWORD*)rear_wheel_model, CT_bStringHash("MARKER_BRAKE")) : 0;

	float* WheelBrakeMarkerY = car_render_info->WheelBrakeMarkerY;

	WheelBrakeMarkerY[0] = front_position_marker != nullptr ? front_position_marker->Matrix.v3.y : 0.0f; // 268
	WheelBrakeMarkerY[1] = rear_position_marker != nullptr ? rear_position_marker->Matrix.v3.y : 0.0f; // 269
}

DWORD GetNeonTextureInfo(DWORD name_hash, int return_default_texture_if_not_found, int include_unloaded_textures)
{
	// Required code is implemented in CarRenderInfo_UpdateCarReplacementTextures
	return hb_GetTextureInfo.fun(name_hash, return_default_texture_if_not_found, include_unloaded_textures);
}

void __fastcall CarRenderInfo_RenderNeon(CarRenderInfo* car_render_info, void* EDX_Unused, int eView, float* bV3, float* bM4)
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

	RideInfo = (DWORD*)car_render_info->pRideInfo;
	if (RideInfo)
	{
		AnimationThing = (DWORD*)car_render_info->pAnimRenderInfo;
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
				(DWORD*)car_render_info,
				eView,
				bV3,
				bM4,
				car_render_info->UnderglowNeonMarkers.NumMarkers,
				(DWORD*)car_render_info->UnderglowNeonMarkers.Markers,
				car_render_info->UnderglowNeonCurrentColour,
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
			HoodPart = (DWORD*)RideInfo[356 + CARSLOTID_HOOD];
			if (HoodPart)
			{
				ShowEngineThruHood = CarPart_GetAppliedAttributeUParam(HoodPart, CT_bStringHash("SHOWENGINE"), 0);
				if (ShowEngineThruHood) AnimationTime = 1.0;
			}
			
			CarRenderInfo_RenderNeon2(
				(DWORD*)car_render_info,
				eView,
				bV3,
				bM4,
				car_render_info->EngineNeonMarkers.NumMarkers,
				(DWORD*)car_render_info->EngineNeonMarkers.Markers,
				car_render_info->EngineNeonColour.CurrentColour,
				flt_7A6738,
				0,
				AnimationTime);
			a8 = flt_7A65F8 * flt_7A6738;

			CarRenderInfo_RenderNeon2(
				(DWORD*)car_render_info,
				eView,
				bV3,
				bM4,
				car_render_info->EngineNeonMarkers.NumMarkers,
				(DWORD*)car_render_info->EngineNeonMarkers.Markers,
				car_render_info->EngineNeonColour.CurrentColour & 0xFFFFFF | ((unsigned int)(unsigned __int64)((double)((BYTE*)&car_render_info->EngineNeonColour.CurrentColour)[3]
					* flt_7A65FC) << 24),
				a8,
				0,
				AnimationTime);
		}

		// Cabin Neon
		if (RideInfo[512]) // NEON_CABIN
		{
			CarRenderInfo_RenderNeon2(
				(DWORD*)car_render_info,
				eView,
				bV3,
				bM4,
				car_render_info->CabinNeonMarkers.NumMarkers,
				(DWORD*)car_render_info->CabinNeonMarkers.Markers,
				car_render_info->CabinNeonColour.CurrentColour,
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
			TrunkPart = (DWORD*)RideInfo[356 + CARSLOTID_TRUNK];
			if (TrunkPart)
			{
				ShowAudioThruTrunk = CarPart_GetAppliedAttributeUParam(TrunkPart, CT_bStringHash("SHOWTRUNK"), 0);
				if (ShowAudioThruTrunk) AnimationTime = 1.0;
			}
			
			CarRenderInfo_RenderNeon2(
				(DWORD*)car_render_info,
				eView,
				bV3,
				bM4,
				car_render_info->TrunkNeonMarkers.NumMarkers,
				(DWORD*)car_render_info->TrunkNeonMarkers.Markers,
				car_render_info->TrunkNeonColour.CurrentColour,
				flt_7A6740,
				0,
				AnimationTime);
			a8a = flt_7A65E8 * flt_7A6740;
			CarRenderInfo_RenderNeon2(
				(DWORD*)car_render_info,
				eView,
				bV3,
				bM4,
				car_render_info->TrunkNeonMarkers.NumMarkers,
				(DWORD*)car_render_info->TrunkNeonMarkers.Markers,
				car_render_info->TrunkNeonColour.CurrentColour & 0xFFFFFF | ((unsigned int)(unsigned __int64)((double)((BYTE*)&car_render_info->TrunkNeonColour.CurrentColour)[3]
					* flt_7A65EC) << 24),
				a8a,
				0,
				AnimationTime);
		}
	}
}

void CarRenderInfo_SetLightState(CarRenderInfo* car_render_info, int LightID, bool on)
{
	DWORD* TheCar = (DWORD*)car_render_info->pCar;
	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(car_render_info);

	if (extra)
	{
		if (LightID == -1)
		{
			if (TheCar) // Read from Car struct
			{
				// Headlights
				if (((BYTE*)TheCar)[2365]) extra->OnLights |= LIGHT_HEADLIGHTS;
				else extra->OnLights &= ~LIGHT_HEADLIGHTS;

				// Brakelights
				if (((BYTE*)TheCar)[2372]) extra->OnLights |= LIGHT_BRAKELIGHTS;
				else extra->OnLights &= ~LIGHT_BRAKELIGHTS;

				// Reverse lights
				if (TheCar[308] == 0) extra->OnLights |= LIGHT_REVERSE; // Gear == 0 (reverse)
				else extra->OnLights &= ~LIGHT_REVERSE;

				// Flashers
				if (Car_IsBlinking(TheCar)) extra->OnLights |= (LIGHT_LSIGNAL + LIGHT_RSIGNAL); // blinking (reset)
				else extra->OnLights &= ~(LIGHT_LSIGNAL + LIGHT_RSIGNAL);
			}
			else // FE??
			{
				extra->OnLights |= LIGHT_HEADLIGHTS + LIGHT_BRAKELIGHTS;
			}
		}
		else
		{
			if (on)
				extra->OnLights |= LightID;
			else
				extra->OnLights &= ~LightID;
		}
	}
}

int CarRenderInfo_GetLightState(CarRenderInfo* car_render_info, int LightID)
{
	int result = 0; // OFF
	bool LightsOn = false;
	bool DamageLights = false;

	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(car_render_info);

	if (extra)
	{
		CarRenderInfo_SetLightState(car_render_info, -1, true); // Read all (HL + BL) from Car struct and write into CRI first

		DWORD* RideInfo = (DWORD*)car_render_info->pRideInfo;
		if (RideInfo)
		{
			LightsOn = CarConfigs[RideInfo[0]].Textures.HeadlightOn;
			DamageLights = CarConfigs[RideInfo[0]].Textures.DamageLights;
		}

		int OnLights = extra->OnLights;
		int BrokenLights = extra->BrokenLights;

		if (OnLights & LightID) result = 1; // ON
		if (BrokenLights & LightID) result = DamageLights ? 2 : 0; // DAMAGE0 or OFF
	}

	return result;
}

void __fastcall CarRenderInfo_UpdateLightStateTextures(CarRenderInfo* car_render_info, void* EDX_Unused)
{
	// TODO: Update with dynamic light states (also find or save them in CarRenderInfo like MW)
	
	int LeftBrakelightState; // eax
	int RightBrakelightState; // edx

	ReplacementTextureEntry* BodyReplacementTextureTable = car_render_info->MasterReplacementTextureTable;
	ReplacementTextureEntry* BodyReplacementTextureTableCarbon = car_render_info->CarbonReplacementTextureTable;
	ReplacementTextureEntry* BodyReplacementTextureTableCF2 = car_render_info->KitCarbonReplacementTextureTable;
	UsedCarTextureInfo* UsedTextureInfos = &car_render_info->mUsedTextureInfos;

	// Headlights
	int LeftHLState = (*(int*)ForceHeadlightsOn) ? 1 : CarRenderInfo_GetLightState(car_render_info, 1);
	int RightHLState = (*(int*)ForceHeadlightsOn) ? 1 : CarRenderInfo_GetLightState(car_render_info, 2);

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
	if (car_render_info->pCar && *((BYTE*)car_render_info->pCar + 2372))
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

char const* DummyWheelRear = "DUMMY_WHEEL_REAR%d";
char const* DummySpinnerRear = "DUMMY_SPINNER_REAR%d";

#define REAR_RIM_REPLACEMENT_ENTRY 46
#define REAR_SPINNER_REPLACEMENT_ENTRY 47
#define MAX_COMPOSITE_ID 20

DWORD GetWheelTextureHash(DWORD* _RideInfo, int index); // defined further down
DWORD GetWheelTextureMaskHash(DWORD* _RideInfo, int index);
DWORD GetSpinnerTextureHash(DWORD* _RideInfo, int index);
DWORD GetSpinnerTextureMaskHash(DWORD* _RideInfo, int index);

bool RideInfo_HasSeparateRearRim(DWORD* _RideInfo)
{
	DWORD FrontWheel = _RideInfo[356 + CARSLOTID_FRONT_WHEEL];
	DWORD RearWheel = _RideInfo[356 + CARSLOTID_REAR_WHEEL];

	return FrontWheel && RearWheel && FrontWheel != RearWheel;
}

bool RideInfo_HasSeparateRearSpinner(DWORD* _RideInfo)
{
	if (!RideInfo_HasSeparateRearRim(_RideInfo)) return false;

	DWORD RearSpinnerTextureHash = GetSpinnerTextureHash(_RideInfo, 1);
	return RearSpinnerTextureHash && RearSpinnerTextureHash != GetSpinnerTextureHash(_RideInfo, 0);
}

int RideInfo_GetCompositeID(DWORD* _RideInfo)
{
	DWORD CompositeWheelHash = _RideInfo[354];
	if (!CompositeWheelHash) return -1;

	char buf[64];
	for (int id = 0; id < MAX_COMPOSITE_ID; id++)
	{
		bSPrintf(buf, "DUMMY_WHEEL%d", id);
		if (bStringHash(buf) == CompositeWheelHash) return id;
	}

	return -1;
}

DWORD RideInfo_GetCompositeRearHash(DWORD* _RideInfo, char const* Format)
{
	int id = RideInfo_GetCompositeID(_RideInfo);
	if (id < 0) return 0;

	char buf[64];
	bSPrintf(buf, Format, id);
	return bStringHash(buf);
}

DWORD RideInfo_GetCompositeRearWheelHash(DWORD* _RideInfo)
{
	return RideInfo_GetCompositeRearHash(_RideInfo, DummyWheelRear);
}

DWORD RideInfo_GetCompositeRearSpinnerHash(DWORD* _RideInfo)
{
	return RideInfo_GetCompositeRearHash(_RideInfo, DummySpinnerRear);
}

void CarRenderInfo_SetRearReplacementEntry(ReplacementTextureEntry* Table, int Index, DWORD SourceHash, DWORD CompositeHash)
{
	// No dummy for this id: leave the model on its own (unpainted) texture rather than on a blank one
	if (SourceHash && CompositeHash && !GetTextureInfo(CompositeHash, 0, 1))
	{
		bReleasePrintf("CarRenderInfo_SetRearReplacementEntry: rear composite texture %08X not found\n", CompositeHash);
		CompositeHash = 0;
	}
	if (!SourceHash || !CompositeHash) SourceHash = CompositeHash = 0;

	ReplacementTextureEntry* Entry = &Table[Index];
	if (Entry->hOldNameHash != SourceHash || Entry->hNewNameHash != CompositeHash)
	{
		Entry->hOldNameHash = SourceHash;
		Entry->hNewNameHash = CompositeHash;
		Entry->pTextureInfo = CompositeHash ? (DWORD*)-1 : 0;
		Entry->ppReplacedTextureInfo = 0;
	}
}

void __fastcall CarRenderInfo_UpdateCarReplacementTextures(CarRenderInfo* car_render_info, void* EDX_Unused)
{
	ReplacementTextureEntry* BodyReplacementTextureTable = car_render_info->MasterReplacementTextureTable;
	ReplacementTextureEntry* BodyReplacementTextureTableCarbon = car_render_info->CarbonReplacementTextureTable;
	ReplacementTextureEntry* BodyReplacementTextureTableCF2 = car_render_info->KitCarbonReplacementTextureTable;
	UsedCarTextureInfo* UsedTextureInfos = &car_render_info->mUsedTextureInfos;

	DWORD* CarDamage = 0;
	if (car_render_info->pCar) CarDamage = *(DWORD**)((BYTE*)car_render_info->pCar + 2392); // CarDamage

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
		DWORD* RideInfo = (DWORD*)car_render_info->pRideInfo;

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
				car_render_info->NeonTexture = (DWORD*)GetNeonTextureInfo(TextureHash ? TextureHash : srcTextureHash, 0, 0); // Neon Texture Info
			}
		}
	}

	// Separate rear rim and rear spinner
	if (SeparateRims && car_render_info->pRideInfo)
	{
		DWORD* _RideInfo = (DWORD*)car_render_info->pRideInfo;
		bool Composited = _RideInfo[353] != 0; // skinnable cars only, like the front ones

		// Rear rim, unless it uses the front rim's texture (entry 6 already covers that)
		DWORD RearRimTextureHash = (Composited && RideInfo_HasSeparateRearRim(_RideInfo)) ? GetWheelTextureHash(_RideInfo, 1) : 0;
		if (RearRimTextureHash == UsedTextureInfos->MappedWheelHash) RearRimTextureHash = 0;
		CarRenderInfo_SetRearReplacementEntry(BodyReplacementTextureTable, REAR_RIM_REPLACEMENT_ENTRY,
			RearRimTextureHash, RearRimTextureHash ? RideInfo_GetCompositeRearWheelHash(_RideInfo) : 0);

		// Rear spinner, unless it uses the front spinner's texture (entry 7 already covers that)
		DWORD RearSpinnerTextureHash = (Composited && RideInfo_HasSeparateRearSpinner(_RideInfo)) ? GetSpinnerTextureHash(_RideInfo, 1) : 0;
		CarRenderInfo_SetRearReplacementEntry(BodyReplacementTextureTable, REAR_SPINNER_REPLACEMENT_ENTRY,
			RearSpinnerTextureHash, RearSpinnerTextureHash ? RideInfo_GetCompositeRearSpinnerHash(_RideInfo) : 0);
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
		CarPart* part = (CarPart*)_RideInfo[356 + BodyPartsToLookForDoorline[i]];

		if (part) 
		{
			if (part->GroupNumber_UpgradeLevel >> 5) // non-stock
				return i == 0
				? CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("DOORLINE"), resultWide)
				: CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("DOORLINE"), resultKit);

			else if (i < 4) continue;
			else return CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("DOORLINE"), result); // stock
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
		CarPart* part = (CarPart*)_RideInfo[356 + BodyPartsToLookForDoorline[i]];

		if (part)
		{
			if (part->GroupNumber_UpgradeLevel >> 5) // non-stock
				return i == 0
				? CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("DOORLINE_MASK"), resultWide)
				: CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("DOORLINE_MASK"), resultKit);
			else if (i < 4) continue;
			else return CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("DOORLINE_MASK"), result); // stock
		}
	}

	return result;
}

#define TextureInfo_SizeWidth(t) (*(short*)((BYTE*)(t) + 0x44))
#define TextureInfo_SizeHeight(t) (*(short*)((BYTE*)(t) + 0x46))
#define TextureInfo_SizeIs32Bit(t) (*((BYTE*)(t) + 0x4A) == 32)

unsigned int CompositeWheel_GetPaint(DWORD* ride_info, int paint_slot)
{
	DWORD* PaintPart = (DWORD*)ride_info[356 + paint_slot];
	if (!PaintPart) return 0xFFFFFFFF;

	unsigned int Red = CarPart_GetAppliedAttributeUParam(PaintPart, CT_bStringHash("RED"), 0) & 0xFF;
	unsigned int Green = CarPart_GetAppliedAttributeUParam(PaintPart, CT_bStringHash("GREEN"), 0) & 0xFF;
	unsigned int Blue = CarPart_GetAppliedAttributeUParam(PaintPart, CT_bStringHash("BLUE"), 0) & 0xFF;
	unsigned int Gloss = CarPart_GetAppliedAttributeUParam(PaintPart, CT_bStringHash("GLOSS"), 0) & 0xFF;
	return Red | (Green << 8) | (Blue << 16) | (Gloss << 24);
}

// Bilinear sample of a packed ARGB image at (u, v) in 0..1
unsigned int CompositeWheel_Sample(unsigned int* Pixels, int Width, int Height, float u, float v)
{
	float x = u * Width - 0.5f, y = v * Height - 0.5f;
	if (x < 0.0f) x = 0.0f;
	if (y < 0.0f) y = 0.0f;
	int x0 = (int)x, y0 = (int)y;
	if (x0 > Width - 1) x0 = Width - 1;
	if (y0 > Height - 1) y0 = Height - 1;
	int x1 = x0 + 1 < Width ? x0 + 1 : x0, y1 = y0 + 1 < Height ? y0 + 1 : y0;
	float fx = x - x0, fy = y - y0;
	if (fx > 1.0f) fx = 1.0f;
	if (fy > 1.0f) fy = 1.0f;

	unsigned int c00 = Pixels[y0 * Width + x0], c10 = Pixels[y0 * Width + x1];
	unsigned int c01 = Pixels[y1 * Width + x0], c11 = Pixels[y1 * Width + x1];
	unsigned int Result = 0;
	for (int Shift = 0; Shift < 32; Shift += 8)
	{
		float Top = ((c00 >> Shift) & 0xFF) * (1.0f - fx) + ((c10 >> Shift) & 0xFF) * fx;
		float Bottom = ((c01 >> Shift) & 0xFF) * (1.0f - fx) + ((c11 >> Shift) & 0xFF) * fx;
		int Channel = (int)(Top * (1.0f - fy) + Bottom * fy + 0.5f);
		Result |= (unsigned int)(Channel > 255 ? 255 : Channel) << Shift;
	}
	return Result;
}

int CompositeWheel32_AnySize(void* Dest, void* Source, void* Mask, unsigned int Paint)
{
	Paint = CompositeWheel_ConvertPaint(&Paint);

	int Width = TextureInfo_SizeWidth(Dest), Height = TextureInfo_SizeHeight(Dest);
	int SourceWidth = TextureInfo_SizeWidth(Source), SourceHeight = TextureInfo_SizeHeight(Source);
	int MaskWidth = TextureInfo_SizeWidth(Mask), MaskHeight = TextureInfo_SizeHeight(Mask);
	if (Width <= 0 || Height <= 0 || SourceWidth <= 0 || SourceHeight <= 0 || MaskWidth <= 0 || MaskHeight <= 0) return 0;

	unsigned int* DestPixels = (unsigned int*)TextureInfo_LockImage(Dest, 1);
	unsigned int* SourcePixels = (unsigned int*)TextureInfo_LockImage(Source, 0);
	unsigned int* MaskPixels = (unsigned int*)TextureInfo_LockImage(Mask, 0);

	if (DestPixels && SourcePixels && MaskPixels)
	{
		for (int y = 0; y < Height; y++)
		{
			float v = (y + 0.5f) / Height;
			for (int x = 0; x < Width; x++)
			{
				float u = (x + 0.5f) / Width;
				unsigned int SourcePixel = CompositeWheel_Sample(SourcePixels, SourceWidth, SourceHeight, u, v);
				unsigned int MaskWeight = CompositeWheel_Sample(MaskPixels, MaskWidth, MaskHeight, u, v) & 0xFF;

				unsigned int Result = SourcePixel;
				if (MaskWeight)
				{
					float Weight = MaskWeight * (1.0f / 255.0f);
					unsigned int Colours[2] = { SourcePixel, Colour_Modulate(SourcePixel, Paint) };
					float Weights[2] = { 1.0f - Weight, Weight };
					Result = SourcePixel ^ ((SourcePixel ^ Colour_Blend(Colours, Weights, 2, 0)) & 0xFFFFFF); // RGB painted, alpha from the source
				}
				DestPixels[y * Width + x] = Result;
			}
		}
	}

	if (MaskPixels) TextureInfo_UnlockImage(Mask, MaskPixels);
	if (SourcePixels) TextureInfo_UnlockImage(Source, SourcePixels);
	if (DestPixels) TextureInfo_UnlockImage(Dest, DestPixels);
	return DestPixels && SourcePixels && MaskPixels;
}

int CompositeWheel8_AnySize(void* Dest, void* Source, void* Mask, unsigned int Paint)
{
	int Width = TextureInfo_SizeWidth(Dest), Height = TextureInfo_SizeHeight(Dest);
	int SourceWidth = TextureInfo_SizeWidth(Source), SourceHeight = TextureInfo_SizeHeight(Source);
	int MaskWidth = TextureInfo_SizeWidth(Mask), MaskHeight = TextureInfo_SizeHeight(Mask);
	if (Width <= 0 || Height <= 0 || SourceWidth <= 0 || SourceHeight <= 0 || MaskWidth <= 0 || MaskHeight <= 0) return 0;

	BYTE* DestPixels = (BYTE*)TextureInfo_LockImage(Dest, 1);
	unsigned int* DestPalette = TextureInfo_LockPalette(Dest, 1);
	BYTE* SourcePixels = (BYTE*)TextureInfo_LockImage(Source, 0);
	unsigned int* SourcePalette = TextureInfo_LockPalette(Source, 0);
	BYTE* MaskPixels = (BYTE*)TextureInfo_LockImage(Mask, 0);
	unsigned int* MaskPalette = TextureInfo_LockPalette(Mask, 0);

	bool Locked = DestPixels && DestPalette && SourcePixels && SourcePalette && MaskPixels && MaskPalette;
	if (Locked)
	{
		// The dummy's palette: 16 mask levels x 16 source colours
		for (int Level = 0; Level < 16; Level++)
		{
			float Weight = Level * (1.0f / 15.0f);
			for (int Index = 0; Index < 16; Index++)
			{
				unsigned int SourceColour = SourcePalette[Index];
				unsigned int Colours[2] = { SourceColour, Colour_Modulate(SourceColour, Paint) };
				float Weights[2] = { 1.0f - Weight, Weight };
				DestPalette[Level * 16 + Index] = SourceColour ^ ((SourceColour ^ Colour_Blend(Colours, Weights, 2, 0)) & 0xFFFFFF);
			}
		}

		for (int y = 0; y < Height; y++)
		{
			float v = (y + 0.5f) / Height;
			for (int x = 0; x < Width; x++)
			{
				float u = (x + 0.5f) / Width;

				// Source colour index, nearest pixel
				int sx = (int)(u * SourceWidth), sy = (int)(v * SourceHeight);
				if (sx > SourceWidth - 1) sx = SourceWidth - 1;
				if (sy > SourceHeight - 1) sy = SourceHeight - 1;
				int SourceIndex = SourcePixels[sy * SourceWidth + sx] & 0x0F;

				// Mask level, bilinear between the four nearest pixels
				float mx = u * MaskWidth - 0.5f, my = v * MaskHeight - 0.5f;
				if (mx < 0.0f) mx = 0.0f;
				if (my < 0.0f) my = 0.0f;
				int mx0 = (int)mx, my0 = (int)my;
				if (mx0 > MaskWidth - 1) mx0 = MaskWidth - 1;
				if (my0 > MaskHeight - 1) my0 = MaskHeight - 1;
				int mx1 = mx0 + 1 < MaskWidth ? mx0 + 1 : mx0, my1 = my0 + 1 < MaskHeight ? my0 + 1 : my0;
				float fx = mx - mx0, fy = my - my0;
				if (fx > 1.0f) fx = 1.0f;
				if (fy > 1.0f) fy = 1.0f;
#define MASK_LEVEL(px, py) ((MaskPalette[MaskPixels[(py) * MaskWidth + (px)]] >> 4) & 0x0F)
				float Top = MASK_LEVEL(mx0, my0) * (1.0f - fx) + MASK_LEVEL(mx1, my0) * fx;
				float Bottom = MASK_LEVEL(mx0, my1) * (1.0f - fx) + MASK_LEVEL(mx1, my1) * fx;
#undef MASK_LEVEL
				int MaskLevel = (int)(Top * (1.0f - fy) + Bottom * fy + 0.5f);
				if (MaskLevel > 15) MaskLevel = 15;

				DestPixels[y * Width + x] = (BYTE)(SourceIndex + 16 * MaskLevel);
			}
		}
	}

	if (MaskPixels) TextureInfo_UnlockImage(Mask, MaskPixels);
	if (SourcePixels) TextureInfo_UnlockImage(Source, SourcePixels);
	if (DestPixels) TextureInfo_UnlockImage(Dest, DestPixels);
	return Locked;
}

int CompositeWheel_AnySize(DWORD* ride_info, DWORD dest_namehash, DWORD src_namehash, DWORD mask_namehash, int paint_slot)
{
	if (!AnySizeWheelComposite)
		return CompositeWheel(ride_info, dest_namehash, src_namehash, mask_namehash, paint_slot);

	void* Dest = dest_namehash ? GetTextureInfo(dest_namehash, 0, 0) : nullptr;
	void* Source = src_namehash ? GetTextureInfo(src_namehash, 0, 0) : nullptr;
	void* Mask = mask_namehash ? GetTextureInfo(mask_namehash, 0, 0) : nullptr;

	bool AllThere = Dest && Source && Mask;
	bool All32Bit = AllThere && TextureInfo_SizeIs32Bit(Dest) && TextureInfo_SizeIs32Bit(Source) && TextureInfo_SizeIs32Bit(Mask);
	bool All8Bit = AllThere && !TextureInfo_SizeIs32Bit(Dest) && !TextureInfo_SizeIs32Bit(Source) && !TextureInfo_SizeIs32Bit(Mask);
	bool SameSize = AllThere
		&& TextureInfo_SizeWidth(Source) == TextureInfo_SizeWidth(Dest) && TextureInfo_SizeHeight(Source) == TextureInfo_SizeHeight(Dest)
		&& TextureInfo_SizeWidth(Mask) == TextureInfo_SizeWidth(Dest) && TextureInfo_SizeHeight(Mask) == TextureInfo_SizeHeight(Dest);

	if (All32Bit && !SameSize)
		return CompositeWheel32_AnySize(Dest, Source, Mask, CompositeWheel_GetPaint(ride_info, paint_slot));
	if (All8Bit && !SameSize)
		return CompositeWheel8_AnySize(Dest, Source, Mask, CompositeWheel_GetPaint(ride_info, paint_slot));

	// Same size, or missing textures, or a mix of 32-bit and 8-bit (which the game refuses too): the game as before
	return CompositeWheel(ride_info, dest_namehash, src_namehash, mask_namehash, paint_slot);
}

void CompositeRim(DWORD* _RideInfo)
{
	// Front rim
	CompositeWheel_AnySize(_RideInfo, _RideInfo[354], GetWheelTextureHash(_RideInfo, 0), GetWheelTextureMaskHash(_RideInfo, 0), CARSLOTID_PAINT_RIM);

	if (!RideInfo_HasSeparateRearRim(_RideInfo)) return;

	// Rear rim
	DWORD CompositeRearWheelHash = RideInfo_GetCompositeRearWheelHash(_RideInfo);
	if (CompositeRearWheelHash)
		CompositeWheel_AnySize(_RideInfo, CompositeRearWheelHash, GetWheelTextureHash(_RideInfo, 1), GetWheelTextureMaskHash(_RideInfo, 1), CARSLOTID_PAINT_RIM);

	// Front spinner is already handled in CompositeSpinner

	// Rear spinner
	if (RideInfo_HasSeparateRearSpinner(_RideInfo))
	{
		DWORD CompositeRearSpinnerHash = RideInfo_GetCompositeRearSpinnerHash(_RideInfo);
		if (CompositeRearSpinnerHash)
			CompositeWheel_AnySize(_RideInfo, CompositeRearSpinnerHash, GetSpinnerTextureHash(_RideInfo, 1), GetSpinnerTextureMaskHash(_RideInfo, 1), CARSLOTID_PAINT_SPINNER);
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

	// REAR SPINNER
	if (RideInfo_HasSeparateRearSpinner(ride))
	{
		DWORD RearSpinnerTextureHash = GetSpinnerTextureHash(ride, 1);
		DWORD RearSpinnerTextureMaskHash = GetSpinnerTextureMaskHash(ride, 1);
		if (RearSpinnerTextureHash && RearSpinnerTextureMaskHash)
		{
			num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, RearSpinnerTextureHash);
			num_textures += UsedCarTextureAddToTable(textures_to_load, num_textures, max_textures, RearSpinnerTextureMaskHash);
		}
	}
	
	return num_textures;
}

void GetUsedCarTextureInfo(UsedCarTextureInfo* info, DWORD* ride_info, int front_end_only, int engine)
{
	// Return if invalid ride_info or car type index is out of bounds
	if ((int)ride_info[0] < 0 || (int)ride_info[0] >= CarCount)
	{
		memset(info, 0, sizeof(UsedCarTextureInfo));
		return;
	}

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
	
	bool Composited = ride_info[353] != 0;
	DWORD CompositeRearWheelHash = (Composited && RideInfo_HasSeparateRearRim(ride_info)) ? RideInfo_GetCompositeRearWheelHash(ride_info) : 0;
	DWORD CompositeRearSpinnerHash = (Composited && RideInfo_HasSeparateRearSpinner(ride_info)) ? RideInfo_GetCompositeRearSpinnerHash(ride_info) : 0;

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
		//Part = (DWORD*)ride_info[356 + CARSLOTID_BASE_PAINT]; // paint

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
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, CompositeRearWheelHash); // Add replace rear wheel hash
	NumUsedCarTextures += UsedCarTextureAddToTable(info->TexturesToLoadPerm, NumUsedCarTextures, MaxPermTex, CompositeRearSpinnerHash); // Add replace rear spinner hash

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
		HLTex = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME"), HLTex); // TEXTURE_NAME
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
		TextureHash = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME"), TextureHash); // TEXTURE_NAME
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
void __declspec(naked) RearWheelLightMaterialCodeCave()
{
	_asm
	{
		mov ecx, dword ptr ds : [esp + eax + 0x178]
		test ecx, ecx // wheel model != 0
		jz loc_62787A
		mov ecx, dword ptr ds : [esp + eax + 0x1F0]
		test ecx, ecx // light material != 0
		jz loc_62787A
		mov eax, dword ptr ds : [esp + 0xE0]
		mov ecx, dword ptr ds : [esp + eax + 0x178]
		mov eax, dword ptr ds : [esp + eax + 0x1F0]
		push eax
		push 0x22719FA9 // MAGSILVER
		call eModel_ReplaceLightMaterial_Game
		mov eax, dword ptr ds : [esp + 0xE0]
		mov ecx, dword ptr ds : [esp + eax + 0x1F0]
		push ecx
		mov ecx, dword ptr ds : [esp + eax + 0x17C]
		push 0xFD102A92 // MAGCHROME
		call eModel_ReplaceLightMaterial_Game
		mov eax, dword ptr ds : [esp + 0xE0]
		mov ecx, dword ptr ds : [esp + eax + 0x1F0]
		push ecx
		mov ecx, dword ptr ds : [esp + eax + 0x17C]
		push 0x12049251 // MAGGUNMETAL
		call eModel_ReplaceLightMaterial_Game
		mov eax, dword ptr ds : [esp + 0xE0]

		loc_62787A :
			push 0x62787A
			retn
	}
}

// Light Flare Color Stuff

Color __fastcall CarRenderInfo_GetColor(CarRenderInfo* car_render_info, void* EDX_Unused, int CarSlotID, int ColorID, Color original, int AlphaType)
{
	Color color = 0;

	DWORD* TheRideInfo = (DWORD*)car_render_info->pRideInfo;

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
	a1a = 6.2831855f / flt_9B3504 * time + 1.5707964f;
	v4 = bSin(a1a);
	v9 = v4 * v4;
	if (v3 == 2)
	{
		v7 = ((float)flarecount * copt + time) * copm + 1.5707964f;
		counter_32323 = (counter_32323 + 1) % copModulo;
		return bSin(v7) * v9;
	}
	else
	{
		v8 = 6.2831855f / flt_9B0BD0 * time + *(float*)&whichcolor + 1.5707964f;
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

void __fastcall CarRenderInfo_CreateCarLightFlares(CarRenderInfo* car_render_info, void* EDX_Unused)
{
	DWORD* Model;
	ePositionMarker* Marker;
	eLightFlare* Flare, *Last;
	int FlareType = 0;
	Color FlareTint = 0;

	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(car_render_info);

	extra->TimeBaseStart = bRandomF(1.0f); // Random start time offset

	if (car_render_info->pCarTypeInfo) // CarTypeInfo
	{
		for (int i = CARSLOTID_MODEL_NUM * 2 - 1; i >= 0; i--) // 63 slots, 2 models for 4 LODs
		{
			Marker = 0;
			Model = (DWORD*)(&car_render_info->mCarPartModels[0][0][0])[i * 4]; // LOD A of all models

			if (Model)
			{
				Marker = eModel_GetPositionMarker(Model, Marker);
				do
				{
					if (!Marker) break; // No (more) markers

					switch (Marker->NameHash)
					{
					case CT_bStringHash("CENTRE_HEADLIGHT"):
						FlareType = 0; // ELF_CAR_HEADLIGHT
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_HEADLIGHT_BULB, 0, CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_HEADLIGHT, 2, 0, 0), 0);
						if (!FlareTint) goto _LeftHeadlight;
						break;
					case CT_bStringHash("RIGHT_HEADLIGHT"):
						FlareType = 0; // ELF_CAR_HEADLIGHT
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_HEADLIGHT_BULB, 0, CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_HEADLIGHT, 1, 0, 0), 0);
						if (!FlareTint) goto _LeftHeadlight;
						break;
					case CT_bStringHash("LEFT_HEADLIGHT"):
						FlareType = 0; // ELF_CAR_HEADLIGHT
					_LeftHeadlight:
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_HEADLIGHT_BULB, 0, CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_HEADLIGHT, 0, 0, 0), 0);
						break;


					case CT_bStringHash("CENTRE_BRAKELIGHT"):
						FlareType = 1; // ELF_CAR_BRAKELIGHT
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_BRAKELIGHT, 2, 0, 0);
						if (!FlareTint) goto _LeftBrakelight;
						break;
					case CT_bStringHash("RIGHT_BRAKELIGHT"):
						FlareType = 1; // ELF_CAR_BRAKELIGHT
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_BRAKELIGHT, 1, 0, 0);
						if (!FlareTint) goto _LeftBrakelight;
						break;
					case CT_bStringHash("LEFT_BRAKELIGHT"):
						FlareType = 1; // ELF_CAR_BRAKELIGHT
					_LeftBrakelight:
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_BRAKELIGHT, 0, 0, 0);
						break;


					case CT_bStringHash("RIGHT_REVERSE"):
						FlareType = 3; // ELF_CAR_REVERSELIGHT
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_BRAKELIGHT, 4, 0, 0);
						if (!FlareTint) goto _LeftReverse;
						break;
					case CT_bStringHash("LEFT_REVERSE"):
						FlareType = 3; // ELF_CAR_REVERSELIGHT
					_LeftReverse:
						FlareTint = CarRenderInfo_GetColor(car_render_info, 0, CARSLOTID_BRAKELIGHT, 3, 0, 0);
						break;


					case CT_bStringHash("COPLIGHTRED"):
						FlareTint = 0x9300007F;
						FlareType = 5; // ELF_CAR_COPLIGHTRED
						break;

					case CT_bStringHash("COPLIGHTBLUE"):
						FlareTint = 0x937F0000;
						FlareType = 6; // ELF_CAR_COPLIGHTBLUE
						break;

					case CT_bStringHash("COPLIGHTWHITE"):
						FlareTint = 0x7F7F7F7F;
						FlareType = 7; // ELF_CAR_COPLIGHTWHITE
						break;

					case CT_bStringHash("COPLIGHTBRIGHTRED"):
						FlareTint = 0x660000AA;
						FlareType = 10; // ELF_CAR_COPLIGHTBRIGHTRED
						break;

					case CT_bStringHash("COPLIGHTBRIGHTBLUE"):
						FlareTint = 0x66870300;
						FlareType = 11; // ELF_CAR_COPLIGHTBRIGHTBLUE
						break;

					case CT_bStringHash("COPLIGHTORANGE"):
						FlareTint = 0x662684FF;
						FlareType = 12; // ELF_CAR_COPLIGHTORANGE
						break;


					case CT_bStringHash("FRONT_LEFT_SIGNAL"):
					case CT_bStringHash("FRONT_RIGHT_SIGNAL"):
					case CT_bStringHash("REAR_LEFT_SIGNAL"):
					case CT_bStringHash("REAR_RIGHT_SIGNAL"):
						FlareTint = 0x9300496D;
						FlareType = 11; //17; // ELF_BLINKING_AMBER
						break;

					default:
						goto _NextMarker;
					}

					Flare = (eLightFlare*)j__malloc(0x30); // new flare
					if (!Flare) goto _NextMarker;

					Flare->NameHash = Marker->NameHash;
					Flare->Type = FlareType;
					Flare->ColourTint = FlareTint;

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

					Flare->PositionX = Marker->Matrix.v3.x - car_render_info->PivotPosition.x;
					Flare->PositionY = Marker->Matrix.v3.y - car_render_info->PivotPosition.y;
					Flare->PositionZ = Marker->Matrix.v3.z - car_render_info->PivotPosition.z;
					Flare->ReflectPosZ = 0;
					Flare->DirectionX = Marker->Matrix.v2.x;
					Flare->DirectionY = Marker->Matrix.v2.y;
					Flare->DirectionZ = Marker->Matrix.v2.z;

					//Add to list
					Last = (eLightFlare*)car_render_info->LightFlareList.HeadNode.Prev;
					Last->Next = Flare;
					car_render_info->LightFlareList.HeadNode.Prev = Flare;
					Flare->Prev = Last;
					Flare->Next = (eLightFlare*)&car_render_info->LightFlareList.HeadNode;

				_NextMarker:
					Marker = eModel_GetPositionMarker(Model, Marker); // get next marker
				}
				while (Marker);
			}
		}
	}
}

bool gTWEAKER_NISLightEnabled = false;
float gTWEAKER_NISLightIntensity = 1.0f;
float gTWEAKER_NISLightPosX = 0.0f;
float gTWEAKER_NISLightPosY = 0.0f;
float gTWEAKER_NISLightPosZ = 0.0f;

void __fastcall CarRenderInfo_RenderFlaresOnCar(CarRenderInfo* car_render_info, void* EDX_Unused, DWORD* view, bVector3* position, bMatrix4* body_matrix, int force_light_state, int reflection, float scale)
{
	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(car_render_info);

	float time = extra->TimeBaseStart + *(float*)0x7FB718; //WorldTimeSeconds + this->CarTimebaseStart
	bMatrix4* LocalWorld = eFrameMallocMatrix(1);

	DWORD* TheCar = (DWORD*)car_render_info->pCar; // this

	if (LocalWorld)
	{
		bCopy(LocalWorld, body_matrix);
		LocalWorld->v3.x = position->x;
		LocalWorld->v3.y = position->y;
		LocalWorld->v3.z = position->z;
		LocalWorld->v3.w = 1.0f;

		//if (reflection) CarRenderInfo_RenderTextureHeadlights_Hook(CarRenderInfo, 0, view, (float*)LocalWorld, 0);
		// Debug cop lights
		CarRenderInfo_SetLightState(car_render_info, ForceLightFlaresOn, true);

		DWORD* RideInfo = (DWORD*)car_render_info->pRideInfo; // this->pRideInfo
		if (IsCop(RideInfo[0]) && (extra->OnLights & LIGHT_COPRED) != 0)// UsageType = Cop && mOnLights = Cop Lights
			++view[21];

		// Check for US parking lights
		bool USParkingLights = 0;
		DWORD* HeadlightPart = RideInfo_GetPart(RideInfo, CARSLOTID_HEADLIGHT);
		if (HeadlightPart) USParkingLights = CarPart_GetAppliedAttributeUParam(HeadlightPart, CT_bStringHash("US_PARKING_LIGHTS"), 0) != 0;

		int PixelSize = eView_GetPixelSize(view, position, 3.0f);

		if (eGetCurrentViewMode() == 3) PixelSize = (int)(PixelSize * *(float*)0x7A6700); // PLAYER1_RVM??

		if (PixelSize >= 4/*view[9]*/ && eView_GetVisibleState(view, &car_render_info->AABBMin, &car_render_info->AABBMax, LocalWorld)) // this->AABBMin, this->AABBMax
		{
			// define default intensity values
			float IntsLeftHeadlight = 0.0f;
			float IntsRightHeadlight = 0.0f;
			float IntsCentreHeadlight = 0.0f;
			float IntsLeftBrakelight = 0.85f;
			float IntsRightBrakelight = 0.85f;
			float IntsCentreBrakelight = 0.0f;

			/* // force hl on for non-traffic cars??
			if (!(TheCar && *(DWORD*)(TheCar[5] + 4) == 3))
			{
				IntsLeftHeadlight = 1.0f;
				IntsRightHeadlight = 1.0f;
				IntsCentreHeadlight = 1.0f;
			}
			*/

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

			if ((force_light_state & FORCE_HEADLIGHTS_ON) || (TheCar && *((BYTE*)TheCar + 2365)))
			{
				IntsLeftHeadlight += 1.0f;
				IntsRightHeadlight = IntsLeftHeadlight;
				IntsCentreHeadlight = IntsLeftHeadlight;
			}
			else if (force_light_state & FORCE_HEADLIGHTS_OFF)
			{
				IntsLeftHeadlight = 0.0f;
				IntsRightHeadlight = 0.0f;
				IntsCentreHeadlight = 0.0f;
			}

			if (force_light_state & FORCE_BRAKELIGHTS_ON)
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
			else if (force_light_state & FORCE_BREAKLIGHTS_OFF)
			{
				IntsLeftBrakelight = 0.85f;
				IntsRightBrakelight = 0.85f;
				IntsCentreBrakelight = 0.0f;
			}

			if (force_light_state & FORCE_REVERSELIGHTS_ON)
			{
				IntsLeftReverse = 1.0f;
				IntsRightReverse = 1.0f;
			}
			else if (force_light_state & FORCE_REVERSELIGHTS_OFF)
			{
				IntsLeftReverse = 0.0f;
				IntsRightReverse = 0.0f;
			}

			CarRenderInfo_SetLightState(car_render_info, -1, true); // Read all (HL + BL) from Car struct and write into CRI first

			// evaluate on and broken lights
			DWORD OnLights = extra->OnLights;
			//if (ForceSignalsOn) OnLights += 0xF00;
			DWORD BrokenLights = extra->BrokenLights;

			
			// LIGHT_LHEAD
			if (OnLights & LIGHT_LHEAD) IntsLeftHeadlight = 1.0f;
			if (BrokenLights & LIGHT_LHEAD) IntsLeftHeadlight = 0.0f;

			// LIGHT_RHEAD
			if (OnLights & LIGHT_RHEAD) IntsRightHeadlight = 1.0f;
			if (BrokenLights & LIGHT_RHEAD) IntsRightHeadlight = 0.0f;

			// LIGHT_CHEAD
			if (OnLights & LIGHT_CHEAD) IntsCentreHeadlight = 1.0f;
			if (BrokenLights & LIGHT_CHEAD) IntsCentreHeadlight = 0.0f;

			// LIGHT_LBRAKE
			if (OnLights & LIGHT_LBRAKE) IntsLeftBrakelight += 16.0f;
			if (BrokenLights & LIGHT_LBRAKE) IntsLeftBrakelight = 0.0f;

			// LIGHT_RBRAKE
			if (OnLights & LIGHT_RBRAKE) IntsRightBrakelight += 16.0f;
			if (BrokenLights & LIGHT_RBRAKE) IntsRightBrakelight = 0.0f;

			// LIGHT_CBRAKE
			if (OnLights & LIGHT_CBRAKE) IntsCentreBrakelight = 16.5f;
			if (BrokenLights & LIGHT_CBRAKE) IntsCentreBrakelight = 0.0f;

			// LIGHT_LREVERSE
			if (OnLights & LIGHT_LREVERSE) IntsLeftReverse += 17.0f;
			if (BrokenLights & LIGHT_LREVERSE) IntsLeftReverse = 0.0f;

			// LIGHT_RREVERSE
			if (OnLights & LIGHT_RREVERSE) IntsRightReverse += 17.0f;
			if (BrokenLights & LIGHT_RREVERSE) IntsRightReverse = 0.0f;

			// LIGHT_LRSIGNAL
			if (OnLights & LIGHT_LRSIGNAL) IntsRearLeftSignal = 1.0f;
			if (BrokenLights & LIGHT_LRSIGNAL) IntsRearLeftSignal = 0.0f;

			// LIGHT_RRSIGNAL
			if (OnLights & LIGHT_RRSIGNAL) IntsRearRightSignal = 1.0f;
			if (BrokenLights & LIGHT_RRSIGNAL) IntsRearRightSignal = 0.0f;

			// LIGHT_LFSIGNAL
			if (OnLights & LIGHT_LFSIGNAL) IntsFrontLeftSignal = 1.0f;
			if (BrokenLights & LIGHT_LFSIGNAL) IntsFrontLeftSignal = 0.0f;

			// LIGHT_RFSIGNAL
			if (OnLights & LIGHT_RFSIGNAL) IntsFrontRightSignal = 1.0f;
			if (BrokenLights & LIGHT_RFSIGNAL) IntsFrontRightSignal = 0.0f;

			// LIGHT_COPRED
			if (OnLights & LIGHT_COPRED) IntsCopRed = cpr;
			if (BrokenLights & LIGHT_COPRED) IntsCopRed = 0.0f;

			// LIGHT_COPBLUE
			if (OnLights & LIGHT_COPBLUE) IntsCopBlue = cpb;
			if (BrokenLights & LIGHT_COPBLUE) IntsCopBlue = 0.0f;

			// LIGHT_COPWHITE
			if (OnLights & LIGHT_COPWHITE)
			{
				IntsCopWhite = cpw;
				FlickerHeadlights = 1;
			}
			if (BrokenLights & LIGHT_COPWHITE) IntsCopWhite = 0.0f;

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
			for (eLightFlare* Flare = (eLightFlare*)car_render_info->LightFlareList.HeadNode.Next; //this->LightFlareList
				(bNode*)Flare != &car_render_info->LightFlareList.HeadNode;
				Flare = Flare->GetNext())
			{
				IntensityScale = 0.0f;
				ReflectionOverride = 0.0f;
				
				//if (IsTraffic(RideInfo[0]) && Flare->Type == 1) Flare->Type = 2; // ELF_CAR_TRAFFIC_BRAKELIGHT

				//if ((!(renderFlareFlags & 2) || Flare->Type == 1)
				//	&& (!(renderFlareFlags & 1) || Flare->Type - 5 <= 7))
				{
					switch (Flare->NameHash)
					{
					case 0x9DB90133: // LEFT_HEADLIGHT
						IntensityScale = FlickerHeadlights ? copflicker * IntsLeftHeadlight : IntsLeftHeadlight;
						break;

					case 0xD09091C6: // RIGHT_HEADLIGHT
						IntensityScale = FlickerHeadlights ? (1.0f - copflicker) * IntsRightHeadlight : IntsRightHeadlight;
						break;

					case 0x7A5BCF69: // CENTRE_HEADLIGHT
						IntensityScale = IntsCentreHeadlight;
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
						IntensityScale = coplightflicker2(time, 0, flarecount) * IntsCopRed;
						break;

					case 0x41489594: // COPLIGHTBRIGHTRED
						IntensityScale = coplightflicker2(time, 1, flarecount) * IntsCopRed;
						break;

					case 0xE662C161: // COPLIGHTBLUE
						IntensityScale = coplightflicker2(time, 1, flarecount) * IntsCopBlue;
						break;

					case 0x6A52A241: // COPLIGHTBRIGHTBLUE
						IntensityScale = coplightflicker2(time, 0, flarecount) * IntsCopBlue;
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
				NISLightPos.x = gTWEAKER_NISLightPosX + position->x;
				NISLightPos.y = gTWEAKER_NISLightPosY + position->y;
				NISLightPos.z = gTWEAKER_NISLightPosZ + position->z;

				if (gTWEAKER_NISLightEnabled)
				{
					position = &NISLightPos;
					NISLightIntensity = gTWEAKER_NISLightIntensity;
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

void __declspec(naked) CarRenderInfo_UpdateCarParts_CFDoorsFixCodeCave()
{
	_asm
	{
		cmp ecx, CARSLOTID_DOOR_LEFT
		jz CFDoors
		cmp ecx, CARSLOTID_DOOR_RIGHT
		jz CFDoors
		
		// Orig code
		push 0x634BED
		retn

		CFDoors:
		mov eax, dword ptr ds: [esp + 0x34] // RideInfo
		mov edx, dword ptr ds: [eax + 0x70C] // CARSLOTID_DOOR_CARBON

		test edx, edx
		jz AttachMasterTable
		mov cl, byte ptr ds : [edx + 5] // CarPart->GroupID_UpgradeLevel
		shr cl, 5
		test cl, cl
		jle AttachMasterTable

		// AttachCarbonTable
		mov edi, dword ptr ds : [esp + 0x14] // CarRenderInfo
		push 0
		push 73
		lea eax, dword ptr ds : [edi + 0xAC8] // CarbonReplacementTextureTable
		push eax
		mov ecx, esi
		call eModel_AttachReplacementTextureTable
		mov dword ptr ds : [edi + 0x3C0], 1 // CarbonDoors
		jmp loc_634E63

		AttachMasterTable:
		mov edi, dword ptr ds : [esp + 0x14] // CarRenderInfo
		push 0
		push 73
		lea eax, dword ptr ds : [edi + 0x638] // MasterReplacementTextureTable
		push eax
		mov ecx, esi
		call eModel_AttachReplacementTextureTable
		mov dword ptr ds : [edi + 0x3C0], 0 // CarbonDoors

		loc_634E63:
		push 0x634E63
		retn

	}
}

static injector::hook_back<CarRenderInfo*(__fastcall*)(CarRenderInfo*, void*, DWORD*, DWORD*)> hb_CarRenderInfo_ctor;
CarRenderInfo* __fastcall CarRenderInfo_ctor_Hook(CarRenderInfo* car_render_info, void* EDX_Unused, DWORD* RideInfo, DWORD* Car)
{
	// Allocate extra data for CarRenderInfo
	CarRenderInfoExtra* extra = new CarRenderInfoExtra((DWORD*)car_render_info, RideInfo, Car);
	CarRenderInfo_ExtraSlot(car_render_info) = extra;

	// Fix neon crash at 0x6388D1
	car_render_info->NeonTexture = 0;
	
	// Call the original constructor
	CarRenderInfo* result = hb_CarRenderInfo_ctor.fun(car_render_info, EDX_Unused ,RideInfo, Car);

	// Initialize CarRenderInfoExtra
	extra->Init();

	return result;
}

static injector::hook_back<void (__fastcall*)(CarRenderInfo*, void*)> hb_CarRenderInfo_dtor;
void __fastcall CarRenderInfo_dtor_Hook(CarRenderInfo* car_render_info, void* EDX_Unused)
{
	// Free CarRenderInfoExtra
	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(car_render_info);
	if (extra) delete extra;
	CarRenderInfo_ExtraSlot(car_render_info) = nullptr;

	// Call the original destructor
	hb_CarRenderInfo_dtor.fun(car_render_info, EDX_Unused);
}

// CarRenderInfo::Render - final boss

#include "RideInfo.h"
#include "eViewPlatInterface.h"
#include "RidePhysicsInfo.h"

#define STRINGHASH_CARSKIN CT_bStringHash("CARSKIN")
#define STRINGHASH_WINDSHIELD CT_bStringHash("WINDSHIELD")
#define STRINGHASH_CARSKIN_CARBONFIBRE CT_bStringHash("CARSKIN_CARBONFIBRE")
#define STRINGHASH_MAGSILVER CT_bStringHash("MAGSILVER")
#define STRINGHASH_MAGCHROME CT_bStringHash("MAGCHROME")
#define STRINGHASH_MAGGUNMETAL CT_bStringHash("MAGGUNMETAL")
#define STRINGHASH_CALIPER CT_bStringHash("CALIPER")
#define STRINGHASH_CALLIPER CT_bStringHash("CALLIPER")
#define STRINGHASH_EXHAUST_TIP CT_bStringHash("EXHAUST_TIP")
#define STRINGHASH_MIRROR CT_bStringHash("MIRROR")
#define STRINGHASH_AUDIO CT_bStringHash("AUDIOCARSKIN")

enum CarPartLod { CARPART_LOD_OFF = -1, CARPART_LOD_A = 0, CARPART_LOD_B = 1, CARPART_LOD_C = 2, CARPART_LOD_D = 3 };
enum CullableCarPart { CULLABLE_CAR_PART_TIRE_FL = 0, CULLABLE_CAR_PART_BRAKE_FL = 4, CULLABLE_CAR_PART_UNDERNEATH = 10 };
enum CarEmitterList { CAR_EMITTER_LIST_EXHAUST = 10, CAR_EMITTER_LIST_LICENSE_PLATE = 27 };
enum { GAMEFLOW_STATE_FRONTEND = 3, GAMEFLOW_STATE_RACING = 6 };
enum { EVIEW_PLAYER1 = 1, EVIEW_PLAYER1_RVM = 3, EVIEWMODE_TWOH = 3 };

#define CAR_TYPE_POPUP_HEADLIGHTS 8 // the game hard-codes this car type: its headlights move with the hood
#define LIGHT_CONTEXT_SIZE 0x1C0

#define Car_pDriverInfo_Type(car) (*(int*)(*(BYTE**)((BYTE*)(car) + 0x14) + 4)) // 1 = player
#define Car_pControls(car) (*(BYTE**)((BYTE*)(car) + 0x30))                    // steering (unsigned short) at +0x1E0
#define Car_pCollisionVolume(car) (*(void**)((BYTE*)(car) + 0x3C))
#define Car_WheelAttached(car, w) (*((BYTE*)(car) + 0x40 + 0x3B4 + (w)) != 0)  // mState.WheelAttached[4]
#define Car_mState_field_3E4(car) (*(int*)((BYTE*)(car) + 0x40 + 0x3E4))
#define Car_mTopology(car) ((void*)((BYTE*)(car) + 0x5C4))
#define Car_pDamage(car) (*(BYTE**)((BYTE*)(car) + 0x958))
#define CarDamage_Amount(damage, offset) (*(float*)((BYTE*)(damage) + (offset))) // +0x120 L mirror, +0x124 R mirror, +0x130 L door, +0x134 R door
#define eView_ID(view) ((int)(view)[1])
#define eView_CameraPosition(view) ((bVector3*)(*(BYTE**)((view) + 0x10) + 0x40))
#define LightContext_Lights(light_context) ((bVector4*)((BYTE*)(light_context) + 0x120)) // 10 light colours
#define CarRenderInfo_AmbientColour(car_render_info) (*(bVector3*)&(car_render_info)->_unk3CC[0x14])               // 0x3E0
#define CarRenderInfo_TrunkAudioCompMarkers(car_render_info) ((ePositionMarker**)&(car_render_info)->_unk208C[0xF8]) // 0x2184
#define CarRenderInfo_UnderglowRoadColour(car_render_info) ((car_render_info)->_unk354)                             // 0x354

// GC: GetCarSlotAnim (inlined on PC)
int GetCarSlotAnim(int CarSlotID, int piece)
{
	CarSlotAnim& anim = CarSlotAnimHookupTable->slots[CarSlotID];
	return (char)(piece ? anim.secondary : anim.primary); // None (0xFF) becomes -1, as in the game
}

// GC: GetCarDamageWobblingPartFromCAR_SLOT_ID (inlined on PC)
int GetCarDamageWobblingPartFromCAR_SLOT_ID(int CarSlotID)
{
	for (int i = 0; i < 6; i++)
		if (BreakableCarDamageInfoTable[i * 4] == CarSlotID)
			return i;
	return -1;
}

int CarPart_GetUpgradeLevel(CarPart* car_part)
{
	return car_part ? car_part->GroupNumber_UpgradeLevel >> 5 : 0;
}

void bIdentity(bMatrix4* m)
{
	m->v0 = bVector4(1.0f, 0.0f, 0.0f, 0.0f);
	m->v1 = bVector4(0.0f, 1.0f, 0.0f, 0.0f);
	m->v2 = bVector4(0.0f, 0.0f, 1.0f, 0.0f);
	m->v3 = bVector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void CarRenderInfo_CamberMatrix(bMatrix4* out, bMatrix4* in, unsigned short angle)
{
	bMatrix4 copy = *in;
	copy.v3 = bVector4(0.0f, 0.0f, 0.0f, 1.0f);
	eRotateX(&copy, &copy, angle);
	copy.v3 = bVector4(in->v3.x, in->v3.y, in->v3.z, copy.v3.w);
	*out = copy;
}

unsigned int CarRenderInfo_PulseColour(BYTE* colour, signed char* diff, float pulse, int extra_alpha)
{
	int c[4];
	for (int i = 0; i < 4; i++)
	{
		c[i] = colour[i] + (int)((float)(unsigned char)diff[i] * pulse);
		if (i == 3) c[i] += extra_alpha;
		if (c[i] < 0) c[i] = 0;
		if (c[i] > 254) c[i] = 254;
	}
	return c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);
}

DWORD* CarRenderInfo_MakePartLightContext(DWORD* base, float light_scale, unsigned int neon, float neon_scale, bool add_neon)
{
	DWORD* light_context = (DWORD*)eFrameMalloc(LIGHT_CONTEXT_SIZE);
	if (!light_context) return nullptr;
	memcpy(light_context, base, LIGHT_CONTEXT_SIZE);
	bVector4* lights = LightContext_Lights(light_context);
	for (int i = 0; i < 10; i++)
	{
		lights[i].x *= light_scale; lights[i].y *= light_scale; lights[i].z *= light_scale; lights[i].w *= light_scale;
	}
	if (add_neon)
	{
		float alpha = (neon >> 24) * ByteToFloat;
		lights[0].x += alpha * ((neon & 0xFF) * ByteToFloat) * neon_scale;
		lights[0].y += alpha * (float)(((neon >> 8) & 0xFF) * NeonGreenScale) * neon_scale;
		lights[0].z += alpha * (((neon >> 16) & 0xFF) * ByteToFloat) * neon_scale;
	}
	return light_context;
}

void CarRenderInfo_RenderPart(DWORD* view, eModel* model, bMatrix4* local_world, DWORD* light_context, unsigned int flags, bool is_tire = false)
{
	if (!model) return;
	if (is_tire && !DisableTextureReplacement && !DisableMaterialBasedReplacement)
		eViewPlatInterface_RenderWheel(view, nullptr, model, local_world, light_context, flags, nullptr);
	else
		eViewPlatInterface_Render(view, model, local_world, light_context, flags, nullptr);
}

// neon pulse (inlined into Render on PC)
void CarRenderInfo_UpdateNeonPulse(CarRenderInfo* car_render_info, float underglow_up)
{
	RideInfo* ride_info = car_render_info->pRideInfo;
	CarPart* neon = ride_info->mPartsTable[CARSLOTID_NEON];
	CarPart* neon_engine = ride_info->mPartsTable[CARSLOTID_NEON_ENGINE];
	CarPart* neon_trunk = ride_info->mPartsTable[CARSLOTID_NEON_TRUNK];
	CarPart* neon_cabin = ride_info->mPartsTable[CARSLOTID_NEON_CABIN];
	bool any_neon = CarPart_GetUpgradeLevel(neon) || CarPart_GetUpgradeLevel(neon_engine) || CarPart_GetUpgradeLevel(neon_trunk)
		|| (CabinNeonFix && CarPart_GetUpgradeLevel(neon_cabin)); // CabinNeonFixCodeCave1 @ 0x6227C8
	if (!any_neon) return;

	float time = (*(int*)_TheGameFlowManager == GAMEFLOW_STATE_FRONTEND ? (float)RealTimer : (float)WorldTimer) * NeonTimeScale;
	if (MagazineViewer_GetCurrentMode()) time = 1.0f;
	float period = 2.0f * NeonPeriod;
	float a = NeonFreqA * time, b = time * NeonFreqB, c = time * NeonFreqC;
	a -= (int)(a / period) * period;
	b -= (int)(b / period) * period;
	c -= (int)(c / period) * period;
	int flicker = (int)(fabsf(bSin(b)) * fabsf(bSin(a)) * NeonFlickerAlpha);
	float pulse = fabsf(bSin(c));

	car_render_info->UnderglowNeonCurrentColour = CarRenderInfo_PulseColour(car_render_info->UnderglowNeonColour, car_render_info->UnderglowNeonColourDiff, pulse, flicker);
	int road_alpha = (int)((car_render_info->UnderglowNeonCurrentColour >> 24) * underglow_up) * 4;
	if (road_alpha > 255) road_alpha = 255;
	CarRenderInfo_UnderglowRoadColour(car_render_info) = (car_render_info->UnderglowNeonCurrentColour & 0xFFFFFF) | (road_alpha << 24);

	if (neon_engine)
		car_render_info->EngineNeonColour.CurrentColour = CarRenderInfo_PulseColour(car_render_info->EngineNeonColour.Colour, car_render_info->EngineNeonColour.ColourDiff, pulse, flicker);
	if (CabinNeonFix && (neon_engine || neon_cabin)) // CabinNeonFixCodeCave2 @ 0x622BD9 (needed an engine neon there)
		car_render_info->CabinNeonColour.CurrentColour = CarRenderInfo_PulseColour(car_render_info->CabinNeonColour.Colour, car_render_info->CabinNeonColour.ColourDiff, pulse, flicker);
	if (neon_trunk)
		car_render_info->TrunkNeonColour.CurrentColour = CarRenderInfo_PulseColour(car_render_info->TrunkNeonColour.Colour, car_render_info->TrunkNeonColour.ColourDiff, pulse, flicker);
}

// MW: DrawKeithProjShadow (U2: the collision hull projected on the road)
void CarRenderInfo_DrawKeithProjShadow(CarRenderInfo* car_render_info, DWORD* view, bVector3* position, bMatrix4* local_world, bMatrix4* biased_identity,
	int body_lod, int pixel_size, float shadow_fade)
{
	DWORD* car = (DWORD*)car_render_info->pCar;
	void* volume = car ? Car_pCollisionVolume(car) : nullptr;
	if (body_lod > CARPART_LOD_C || *(int*)_TheGameFlowManager == GAMEFLOW_STATE_FRONTEND || !volume || !DrawFancyCarShadow
		|| g_CarShadowEnable <= 1 || !car_render_info->ShadowRampTexture || (view && eView_ID(view) == EVIEW_PLAYER1_RVM))
		return;

	void* polytope = (*(void* (__thiscall**)(void*, int))(*(BYTE**)volume + 0x7C))(volume, 0);
	int num_points = *(int*)((BYTE*)polytope + 0x14);
	sh_Setup(position);

	float ground_z = position->z;
	for (int i = 0; i < num_points; i++)
	{
		BYTE vertex[0x20] = {};
		GJKPolytope_GetPoint(polytope, i, vertex);
		bVector3 point(*(float*)(vertex + 0x10), *(float*)(vertex + 0x14), *(float*)(vertex + 0x18) + ShadowVolumeLift);
		bVector3 world;
		eMulVector(&world, local_world, &point); // the game transforms with an uninitialised w; a point here
		float drop = -((world.z - ground_z) * ShadowProjectionScale);
		ShadowHullPoints[i].x = drop * ShadowLightDirection[0] + world.x;
		ShadowHullPoints[i].y = drop * ShadowLightDirection[1] + world.y;
	}

	convex_hull(ShadowHullPoints, &num_points, Car_mTopology(car), ShadowGroundPosition.z + ShadowHullLift, ShadowHullLift, body_lod >= CARPART_LOD_C);

	bVector3* hull;
	if (body_lod != CARPART_LOD_A && ShadowSkipSmoothing)
		hull = ShadowHullPoints2;
	else
	{
		num_points = smooth_shadow_corners(num_points);
		hull = ShadowSmoothedPoints;
	}
	if (num_points < 3) return;

	bVector3 centre((hull[0].x + hull[num_points / 2].x) * 0.5f, (hull[0].y + hull[num_points / 2].y) * 0.5f, (hull[0].z + hull[num_points / 2].z) * 0.5f);
	float alpha = shadow_fade * ShadowAlphaScale + ShadowAlphaBase;
	FancyCarShadowEdgeMult = (ShadowEdgeMultMax - ShadowEdgeMultMin) * shadow_fade + ShadowEdgeMultMin;
	if (body_lod == CARPART_LOD_C)
		alpha *= ((float)pixel_size - CarBodyLodSwapSize[2]) / (CarBodyLodSwapSize[1] - CarBodyLodSwapSize[2]);
	int a = (int)alpha;
	if (a < 0) a = 0;
	if (a > 254) a = 254;
	unsigned int colour = 0x808080 | (a << 24);
	DWORD* texture = car_render_info->ShadowRampTexture;

	if (DrawShadowCentre)
	{
		for (int i = 0; i < (num_points & ~1) - 1; i += 2)
		{
			if (!eBeginStrip(texture, 4, biased_identity)) continue;
			eAddVertex(&hull[i]); eAddVertex(&centre); eAddVertex(&hull[i + 1]); eAddVertex(&hull[(i + 2) % num_points]);
			for (int k = 0; k < 4; k++) eAddColour(colour);
			eAddUV(ShadowRampU, 0.0f); eAddUV(0.0f, 0.0f); eAddUV(ShadowRampU, 0.0f); eAddUV(ShadowRampU, 0.0f);
			eEndStrip();
		}
		if ((num_points & 1) && eBeginStrip(texture, 3, biased_identity))
		{
			eAddVertex(&hull[num_points - 1]); eAddVertex(&centre); eAddVertex(&hull[0]);
			for (int k = 0; k < 3; k++) eAddColour(colour);
			eAddUV(ShadowRampU, 0.0f); eAddUV(0.0f, 0.0f); eAddUV(ShadowRampU, 0.0f);
			eEndStrip();
		}
	}

	int third = num_points / 3;
	for (int strip = 0, first = 0; strip < 3; strip++, first += third)
	{
		int count = strip == 2 ? num_points - first : third;
		if (!eBeginStrip(texture, 2 * count + 2, biased_identity)) continue;
		for (int n = 0; n <= count; n++)
		{
			int i = first + n;
			if (n == count && i >= num_points) i = 0;
			bVector3 outer;
			FancyShadow_EdgeLerp(&outer, &hull[i], &centre);
			eAddVertex(&hull[i]); eAddVertex(&outer);
			exAddColour(colour); exAddColour(colour);
			eAddUV(ShadowRampU, 0.0f); eAddUV(1.0f, 0.0f);
		}
		eEndStrip();
	}
}

// MW: DrawAmbientShadow (U2: the shadow grid, plus the underglow on the road in Render)
void CarRenderInfo_DrawAmbientShadow(CarRenderInfo* car_render_info, bVector3* position, bMatrix4* local_world, bMatrix4* biased_identity,
	float shadow_fade, bVector3* car_centre, bool with_underglow)
{
	if (!g_CarShadowEnable) return;

	bool in_front_end = *(int*)_TheGameFlowManager == GAMEFLOW_STATE_FRONTEND;
	float shadow_w = in_front_end ? ShadowWidthFE : ShadowWidthGame;
	float shadow_l = in_front_end ? ShadowLengthFE : ShadowLengthGame;
	float inner_lift = in_front_end ? ShadowInnerLiftFE : ShadowInnerLiftGame;
	float grow = ShadowGrowWithHeight * shadow_fade + 1.0f;
	float min_y = car_render_info->AABBMin.y * grow * shadow_w, max_y = car_render_info->AABBMax.y * grow * shadow_w;
	float min_x = car_render_info->AABBMin.x * grow * shadow_l, max_x = car_render_info->AABBMax.x * grow * shadow_l;

	bVector3 grid[16];
	float grid_uv[16][2];
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			bVector3 local(min_x + col * (max_x - min_x) / 3.0f, min_y + row * (max_y - min_y) / 3.0f, 0.0f);
			eMulVector(&grid[row * 4 + col], local_world, &local);
			grid[row * 4 + col].z = position->z;
			grid_uv[row * 4 + col][0] = col / 3.0f;
			grid_uv[row * 4 + col][1] = row / 3.0f;
		}
	}

	int dropped_points[4] = {}; // per grid row
	DWORD* car = (DWORD*)car_render_info->pCar;
	if (!in_front_end && car)
	{
		// a road plane under each wheel, used where the topology has no surface
		bVector3 wheel_ground[4], wheel_normal[4];
		bool wheel_valid[4];
		CarPhysicsInfo* physics = (CarPhysicsInfo*)RidePhysicsInfo_GetCarPhysicsInfo((DWORD*)&car_render_info->pRideInfo->mPhysics, true);
		for (int w = 0; w < 4; w++)
		{
			eMulVector(&wheel_ground[w], local_world, (bVector3*)&physics->Wheels[w].Position);
			wheel_ground[w].z = TopologyCoordinate_GetElevation(Car_mTopology(car), &wheel_ground[w], nullptr, &wheel_normal[w], &wheel_valid[w]);
			float k = -1.0f / wheel_normal[w].z;
			wheel_normal[w].x *= k; wheel_normal[w].y *= k; wheel_normal[w].z *= k;
		}
		static const int grid_wheel[16] = { 2, 2, 1, 1, 2, 2, 1, 1, 3, 3, 0, 0, 3, 3, 0, 0 };
		float last_good_z = car_centre->z;
		for (int i = 0; i < 16; i++)
		{
			bool valid = false;
			grid[i].z = TopologyCoordinate_GetElevation(Car_mTopology(car), &grid[i], nullptr, nullptr, &valid);
			int w = grid_wheel[i];
			if (!valid && wheel_valid[w])
			{
				grid[i].z = (grid[i].y - wheel_ground[w].y) * wheel_normal[w].y + (grid[i].x - wheel_ground[w].x) * wheel_normal[w].x + wheel_ground[w].z;
				valid = true;
			}
			if (((i ^ (i >> 1)) & 5) == 5) // the four inner points
				grid[i].z += inner_lift;
			if (valid && ShadowPointNearGround(grid[i].z, ShadowGroundPosition.z) && ShadowPointOnRoad(&grid[i]))
			{
				last_good_z = grid[i].z;
				grid[i].z += ShadowGridLift;
			}
			else
			{
				grid[i].z = last_good_z + ShadowGridLift;
				dropped_points[i / 4]++;
			}
		}
	}
	else
	{
		for (int i = 0; i < 16; i++)
			grid[i].z = ShadowGridHeightFE + ((((i ^ (i >> 1)) & 5) == 5) ? inner_lift : 0.0f);
	}

	float lit = 1.0f - shadow_fade;
	float up = local_world->v2.z < 0.0f ? 0.0f : local_world->v2.z > 1.0f ? lit : local_world->v2.z * lit;
	int base = in_front_end ? 0x80 : 0;
	int top = in_front_end ? 0x80 : 0x72;
	int alpha = (int)(fabsf(local_world->v2.z) * lit * (top - base) * 2.0f + base * 2.0f);
	if (alpha < 0) alpha = 0;
	if (alpha > 254) alpha = 254;
	unsigned int shadow_colour = 0x808080 | (alpha << 24);

	if (with_underglow)
		CarRenderInfo_UpdateNeonPulse(car_render_info, up);

	for (int pass = 0; pass < (with_underglow ? 2 : 1); pass++) // the shadow, then the underglow light on the road
	{
		DWORD* texture = pass == 0 ? car_render_info->ShadowTexture : car_render_info->NeonTexture;
		unsigned int colour = pass == 0 ? shadow_colour : CarRenderInfo_UnderglowRoadColour(car_render_info);
		if (!texture || !(colour & 0xFF000000)) continue;
		for (int row = 0; row < 3; row++)
		{
			if (dropped_points[row] + dropped_points[row + 1] >= 4) continue;
			if (!eBeginStrip(texture, 8, biased_identity)) continue;
			for (int col = 0; col < 4; col++)
			{
				int t = row * 4 + col, b = (row + 1) * 4 + col;
				eAddVertex(&grid[t]); exAddColour(colour); eAddUV(grid_uv[t][0], grid_uv[t][1]);
				eAddVertex(&grid[b]); exAddColour(colour); eAddUV(grid_uv[b][0], grid_uv[b][1]);
			}
			eEndStrip();
		}
	}
}

// shadow fade with height above the road
float CarRenderInfo_GetShadowFade(DWORD* car, bMatrix4* local_world, bVector3* car_centre)
{
	bVector3 origin(0.0f, 0.0f, 0.0f);
	eMulVector(car_centre, local_world, &origin);
	float height = 0.0f;
	if (*(int*)_TheGameFlowManager != GAMEFLOW_STATE_FRONTEND && car)
		height = car_centre->z - TopologyCoordinate_GetElevation(Car_mTopology(car), car_centre, nullptr, nullptr, nullptr);
	float fade = height > ShadowFadeEnd ? 1.0f : 0.0f;
	if (height > ShadowFadeStart && height < ShadowFadeEnd)
		fade = (height - ShadowFadeStart) / (ShadowFadeEnd - ShadowFadeStart);
	if (fade < 0.0f) fade = 0.0f;
	if (fade > 1.0f) fade = 1.0f;
	if (ShadowFadeOverride != 0.0f) fade = ShadowFadeOverride;
	return fade;
}

// hood / trunk / door animation matrix of a part
bMatrix4* CarRenderInfo_GetAnimatedPartMatrix(int car_hash, int location, int piece, eModel* base_model, bMatrix4* local_world, bMatrix4* biased_local_world)
{
	bMatrix4 inverse_local_world;
	bInvertMatrix(&inverse_local_world, local_world);
	bMatrix4* anim = eFrameMallocMatrix(1);
	if (!anim) return nullptr;
	*anim = *biased_local_world;

	bMatrix4 rest, current;
	PartAnim_GetCarPartInitAnimMatrix(car_hash, location, &rest, piece); // 0x6235C4
	PartAnim_GetCarPartAnimMatrix(car_hash, location, &current, piece);  // 0x6235F5

	bVector3 moved(current.v3.x - rest.v3.x, current.v3.y - rest.v3.y, current.v3.z - rest.v3.z);
	current.v3.x = current.v3.y = current.v3.z = 0.0f;
	bVector4 pivot;
	eModel_GetPivotPosition(base_model, &pivot);
	bVector3 to_pivot(pivot.x - rest.v3.x, pivot.y - rest.v3.y, pivot.z - rest.v3.z);
	bVector3 from_pivot(-to_pivot.x, -to_pivot.y, -to_pivot.z);

	eMulMatrix(anim, anim, &inverse_local_world);
	eTranslate(anim, anim, &to_pivot);

	bMatrix4 inverse_rest;
	bInvertMatrix(&inverse_rest, &rest);
	inverse_rest.v3.x = inverse_rest.v3.y = inverse_rest.v3.z = 0.0f;
	bool undo_rest;
	if (location == CarSlotAnimLocation::LeftDoor || location == CarSlotAnimLocation::RightDoor)
	{
		int style = CarAnimManager_GetAnimStyle(car_hash, location, piece);
		undo_rest = style == 1 || style == 3 || style == 4 || style == 5 || style == 6;
	}
	else
	{
		undo_rest = location == CarSlotAnimLocation::Hood;
	}
	if (undo_rest)
		eMulMatrix(anim, anim, &inverse_rest);

	eMulMatrix(anim, anim, &current);
	eTranslate(anim, anim, &moved);
	eTranslate(anim, anim, &from_pivot);
	eMulMatrix(anim, anim, local_world);
	return anim;
}

struct CarRenderInfo_WheelCamber
{
	unsigned short angle[4] = {}; // FL, FR, RR, RL
	float push_down = 0.0f;
};

CarRenderInfo_WheelCamber CarRenderInfo_GetWheelCamber(CarRenderInfo* car_render_info, int body_lod, bool in_render_fast)
{
	CarRenderInfo_WheelCamber wheel_camber;
	if (body_lod > CARPART_LOD_B && !RenderRewriteFullWheelCamber) return wheel_camber;

	float camber = CamberTweak != 0.0f ? CamberTweak : RidePhysicsInfo_GetCamberPercent(&car_render_info->pRideInfo->mPhysics, nullptr); // 0x6270C7 / 0x617554
	unsigned short angle = (unsigned short)((int)(CamberToAngle * camber * DegToAng) / 360);
	if (in_render_fast && !RenderRewriteFullWheelCamber) angle = 0;
	wheel_camber.push_down = CamberPushDown * (RenderRewriteCamberPushDownFromCamber ? camber : CamberTweak); // the game uses the tweak (0)
	wheel_camber.angle[0] = angle;
	wheel_camber.angle[1] = (unsigned short)-angle;
	if (RenderRewriteFullWheelCamber)
	{
		wheel_camber.angle[2] = (unsigned short)-angle;
		wheel_camber.angle[3] = angle;
	}
	return wheel_camber;
}

eModel* CarRenderInfo_GetRearTireModel(CarRenderInfo* car_render_info, eModel* tire, int tire_lod, bMatrix4* tire_world, DWORD* view, bool in_game_only)
{
	if (RenderRewriteNoEdgeOnTireLOD || tire_lod != CARPART_LOD_A) return tire;
	if (in_game_only && *(int*)_TheGameFlowManager != GAMEFLOW_STATE_RACING) return tire;
	if (TireFace(tire_world, view) < FaceCos)
		return car_render_info->mCarPartModels[CARSLOTID_REAR_WHEEL][0][CARPART_LOD_B];
	return tire;
}

void CarRenderInfo_PaintRims(eModel* front, eModel* rear, int material)
{
	if (!material) return;
	for (eModel* model : { front, rear })
	{
		if (!model) continue;
		model->ReplaceLightMaterial(STRINGHASH_MAGSILVER, material);
		model->ReplaceLightMaterial(STRINGHASH_MAGCHROME, material);
		model->ReplaceLightMaterial(STRINGHASH_MAGGUNMETAL, material);
	}
}

int __fastcall CarRenderInfo_Render(CarRenderInfo* car_render_info, void* EDX_Unused, DWORD* view, void* shaper_lights, bVector3* world_position,
	bMatrix4* body_matrix, bMatrix4* tire_matrices, bMatrix4* brake_matrices, bMatrix4* spinner_matrices, unsigned int extra_render_flags,
	int force_light_state, int reflexion, int tireLOD, int carLOD)
{
	if (!DisableTextureReplacement && !DisableMaterialBasedReplacement) // CarRenderInfo_Render_GetCurrCRICodeCave @ 0x620AF8
		CurrCarRenderInfo = (DWORD*)car_render_info;

	CarRenderInfo* anim_render_info = (CarRenderInfo*)car_render_info->pAnimRenderInfo;
	int car_hash = anim_render_info ? anim_render_info->pCarTypeInfo->CarTypeNameHash : 0;
	int trunk_open = anim_render_info && CarAnimManager_GetLocationState(car_hash, CarSlotAnimLocation::Trunk) != 1;
	int hood_open = anim_render_info && CarAnimManager_GetLocationState(car_hash, CarSlotAnimLocation::Hood) != 1;
	int left_door_open = anim_render_info && CarAnimManager_GetLocationState(car_hash, CarSlotAnimLocation::LeftDoor) != 1;
	int right_door_open = anim_render_info && CarAnimManager_GetLocationState(car_hash, CarSlotAnimLocation::RightDoor) != 1;
	float hood_light = hood_open ? CarAnimManager_GetAnimationTime(car_hash, CarSlotAnimLocation::Hood, 0) : 1.0f;    // 0x620BC9: was 0
	float trunk_light = trunk_open ? CarAnimManager_GetAnimationTime(car_hash, CarSlotAnimLocation::Trunk, 0) : 0.7f; // 0x620C02: was 0
	int show_inner_parts = 1;                                                                                          // 0x620C32: was (in front end)

	DWORD* car = (DWORD*)car_render_info->pCar;
	if (PartDamage_Test && car)
	{
		DWORD* part_damage = Car_GetCarPartDamage(car);
		if (part_damage)
		{
			CarPartDamage_SetDamageAmount(part_damage, 0);
			PartDamage_Test = 0;
		}
	}

	int pixel_size = eView_GetPixelSize(view, world_position, 3.0f);
	if (eGetCurrentViewMode() == EVIEWMODE_TWOH && *(int*)_TheGameFlowManager != GAMEFLOW_STATE_FRONTEND)
	{
		pixel_size = (int)((float)pixel_size * CarTwoPlayerLODFactor);
		carLOD = tireLOD = CARPART_LOD_B;
	}
	if (pixel_size < 4) return 0;

	BYTE* controls = car ? Car_pControls(car) : nullptr;
	bVector3* camera_position = eView_CameraPosition(view);
	bool in_front_end = *(int*)_TheGameFlowManager == GAMEFLOW_STATE_FRONTEND;
	bool racing = *(int*)_TheGameFlowManager == GAMEFLOW_STATE_RACING;

	DWORD* light_context = (DWORD*)eFrameMalloc(LIGHT_CONTEXT_SIZE);
	bMatrix4* local_world = eFrameMallocMatrix(1);
	bMatrix4* cpy_local_world = eFrameMallocMatrix(1);
	bMatrix4* biased_local_world = eFrameMallocMatrix(1);
	bMatrix4* biased_identity = eFrameMallocMatrix(1);
	if (!light_context || !local_world || !cpy_local_world || !biased_local_world || !biased_identity) return 0;

	bCopy(local_world, body_matrix);
	local_world->v3 = bVector4(world_position->x, world_position->y, world_position->z, 1.0f);
	if (reflexion && !in_front_end && car)
	{
		float elevation = TopologyCoordinate_GetElevation(Car_mTopology(car), world_position, nullptr, nullptr, nullptr);
		local_world->v3.z = elevation - (world_position->z - elevation); // mirrored under the road
	}
	if (!eView_GetVisibleState(view, &car_render_info->AABBMin, &car_render_info->AABBMax, local_world)) return 0;

	bVector4 camera_world_position(camera_position->x, camera_position->y, camera_position->z, 1.0f);

	if (PrintQueryLightMat) PrintLightQuery = 1;
	if (car) *cpy_local_world = *Car_GetRenderBodyMatrix(car, (char)eView_ID(view));
	else bCopy(cpy_local_world, body_matrix);
	cpy_local_world->v3 = bVector4(world_position->x, world_position->y, world_position->z, 1.0f);
	if (racing) eRemoveDynamicLightPack((DWORD*)car_render_info->CarLights);
	DWORD base_light_context[LIGHT_CONTEXT_SIZE / 4];
	elResetLightContext(base_light_context);
	elSetupLights(base_light_context, shaper_lights, world_position, nullptr, hack_man_matrix, view);
	elCloneLightContext(light_context, cpy_local_world, hack_man_matrix, &camera_world_position, view, base_light_context);
	CarRenderInfo_AmbientColour(car_render_info) = *(bVector3*)LightContext_Lights(light_context);
	if (racing) eAddDynamicLightPack((DWORD*)car_render_info->CarLights);
	if (PrintQueryLightMat) PrintLightQuery = 0;

	if (!DisableTextureReplacement) CarRenderInfo_UpdateLightStateTextures(car_render_info, nullptr); // 0x621079
	else CarRenderInfo_UpdateLightStateTextures_Game((DWORD*)car_render_info);

	BYTE* damage = car ? Car_pDamage(car) : nullptr;
	unsigned int env_flag = (!EnableEnvMap || (float)pixel_size < DisableEnvMapSize) ? 1 : 0;

	int body_lod = carLOD, tire_lod_wanted = tireLOD;
	while ((float)pixel_size < CarBodyLodSwapSize[body_lod])
	{
		body_lod++;
		tire_lod_wanted++;
	}
	if (IsInShowcase())
		body_lod = tire_lod_wanted = CARPART_LOD_A;
	else if (body_lod > CARPART_LOD_C)
	{
		body_lod = CARPART_LOD_D;
		if (car && Car_mState_field_3E4(car) == 0 && car_render_info->TheCarPartCuller.CarPartInfoTable[CULLABLE_CAR_PART_UNDERNEATH].IsVisible)
			body_lod = CARPART_LOD_C;
	}
	if (ForceCarLOD != CARPART_LOD_OFF) body_lod = ForceCarLOD;
	if (ForceTireLOD != CARPART_LOD_OFF) tire_lod_wanted = ForceTireLOD;
	if (body_lod == CARPART_LOD_OFF)
	{
		if (tire_lod_wanted == CARPART_LOD_OFF) return 0;
	}
	else if (body_lod >= CARPART_LOD_B) extra_render_flags |= 0x800;

	bMatrix4 world_local;
	bVector3 camera_eye;
	bInvertMatrix(&world_local, local_world);
	bMulMatrix(&camera_eye, &world_local, camera_position);
	float distance = bDistBetween((float*)camera_position, (float*)&local_world->v3);
	if (body_lod == CARPART_LOD_A && distance < NearCameraDistance && eView_ID(view) == EVIEW_PLAYER1)
	{
		camera_eye.z += 0.5f;
		if (bBoundingBoxIsInside(&car_render_info->AABBMin, &car_render_info->AABBMax, &camera_eye, 0.0f)) return 0;
		camera_eye.z -= 0.5f;
	}
	CarPartCuller_CullParts((DWORD*)&car_render_info->TheCarPartCuller, &camera_eye, controls ? *(unsigned short*)(controls + 0x1E0) : 0);

	bCopy(biased_local_world, local_world);
	biased_local_world->v3.z += fCarFudgeElevation;
	float bias = (distance - fCarFudgeBias) / distance;
	BYTE* player = (BYTE*)Player_pPlayersByIndex[0];
	if (player && *(BYTE**)(player + 0x238) && *(int*)(*(BYTE**)(player + 0x238) + 0xC) != 1)
		bias = (distance - fCarFudgeBiasNoTarget) / distance;

	// shadows
	if (!reflexion)
	{
		bCopy_Position(biased_identity, eMathIdentityMatrix, world_position);
		eView_BiasMatrixForZSorting(view, biased_identity, bias);
		bMatrix4 pre_translate;
		bIdentity(&pre_translate);
		pre_translate.v3 = bVector4(-world_position->x, -world_position->y, -world_position->z, 1.0f);
		eMulMatrix(biased_identity, &pre_translate, biased_identity);

		ShadowGroundPosition = *world_position;
		if (car) ShadowGroundPosition.z = TopologyCoordinate_GetElevation(Car_mTopology(car), &ShadowGroundPosition, nullptr, ShadowGroundNormal, nullptr);
		bVector3 car_centre;
		float fade = CarRenderInfo_GetShadowFade(car, local_world, &car_centre);
		CarRenderInfo_DrawKeithProjShadow(car_render_info, view, world_position, local_world, biased_identity, body_lod, pixel_size, fade);
		CarRenderInfo_DrawAmbientShadow(car_render_info, world_position, local_world, biased_identity, fade, &car_centre, true);
	}

	// body parts
	unsigned int body_render_flags = racing ? 0x8000 : 0;
	unsigned int body_flags = extra_render_flags | env_flag | body_render_flags;
	int material_carskin = (int)car_render_info->LightMaterial_CarSkin;
	int material_carbon = (int)car_render_info->LightMaterial_Carbon;
	int material_spoiler = car_render_info->LightMaterial_Spoiler ? (int)car_render_info->LightMaterial_Spoiler : material_carskin;
	int material_roof = car_render_info->LightMaterial_Roof ? (int)car_render_info->LightMaterial_Roof : material_carskin;
	int material_mirror = car_render_info->LightMaterial_Mirror ? (int)car_render_info->LightMaterial_Mirror : material_carskin;
	RideInfo* ride_info = car_render_info->pRideInfo;
	int has_wide_body = CarPart_GetUpgradeLevel(ride_info->mPartsTable[CARSLOTID_WIDE_BODY]) != 0;
	int car_type = ride_info->Type;

	left_door_open |= damage && TheCarDamageMap_LeftDoor && CarDamage_Amount(damage, 0x130) > CarPartDamageCutoff;
	right_door_open |= damage && TheCarDamageMap_RightDoor && CarDamage_Amount(damage, 0x134) > CarPartDamageCutoff;

	bMatrix4* trunk_matrix = nullptr, * left_door_matrix = nullptr, * right_door_matrix = nullptr;
	for (int slot = CARSLOTID_MODEL_FIRST; slot < CARSLOTID_MODEL_NUM; slot++)
	{
		switch (slot) // drawn further down
		{
		case CARSLOTID_LEFT_SIDE_MIRROR: case CARSLOTID_RIGHT_SIDE_MIRROR: case CARSLOTID_ROOF:
		case CARSLOTID_SPOILER: case CARSLOTID_ENGINE: case CARSLOTID_EXHAUST:
			continue;
		}
		if (slot >= CARSLOTID_FRONT_BRAKE && slot <= CARSLOTID_TRUNK_AUDIO_COMP_11) // brakes, wheels, spinner, mirrors, plate, trunk audio
			continue;
		for (int piece = 0; piece < 2; piece++)
		{
			eModel** lods = car_render_info->mCarPartModels[slot][piece];
			eModel* model = lods[0] ? lods[body_lod] : nullptr;
			if (!model) continue;

			int draw = ride_info->mPartsEnabled[slot];
			int damage_index = GetCarDamageWobblingPartFromCAR_SLOT_ID(slot);
			int material = material_carskin;
			bool lod_ok = true;
			switch (slot)
			{
			case CARSLOTID_FRONT_BUMPER:
				if (car_render_info->CarbonFrontBumper) material = material_carbon;
				lod_ok = body_lod < CARPART_LOD_D;
				break;
			case CARSLOTID_REAR_BUMPER:
				if (car_render_info->CarbonRearBumper) material = material_carbon;
				lod_ok = body_lod < CARPART_LOD_D;
				break;
			case CARSLOTID_WIDE_BODY:
				if (car_render_info->CarbonWideBody) material = material_carbon;
				break;
			case CARSLOTID_TOP:
			{
				// sunroof cars: piece 1 with a roof part installed, piece 0 without
				bool roof_installed = HasSunroof(car_type) && CarPart_GetUpgradeLevel(ride_info->mPartsTable[CARSLOTID_ROOF]);
				if (piece == (roof_installed ? 0 : 1)) draw = 0;
				draw &= 1;
				break;
			}
			case CARSLOTID_HOOD:
				if (car_render_info->CarbonHood) material = material_carbon;
				draw = piece == 1 ? body_lod <= CARPART_LOD_A : 1;
				lod_ok = body_lod < CARPART_LOD_D;
				break;
			case CARSLOTID_TRUNK:
				if (*(int*)_DrawTrunkAudio) draw = 0;
				if (car_render_info->CarbonTrunk) material = material_carbon;
				lod_ok = body_lod < CARPART_LOD_D;
				break;
			case CARSLOTID_SKIRT:
				lod_ok = body_lod < CARPART_LOD_D;
				break;
			case CARSLOTID_DOOR_LEFT:
			case CARSLOTID_DOOR_RIGHT:
				if (CFDoorsFix ? car_render_info->CarbonDoors : car_render_info->CarbonWideBody) material = material_carbon; // 0x6231FD
				draw &= 1;
				lod_ok = !(has_wide_body && body_lod > CARPART_LOD_C);
				break;
			case CARSLOTID_DOOR_PANEL_LEFT:
				draw = left_door_open || show_inner_parts;
				damage_index = 4;
				lod_ok = body_lod <= CARPART_LOD_A;
				break;
			case CARSLOTID_DOOR_PANEL_RIGHT:
				draw = right_door_open || show_inner_parts;
				damage_index = 5;
				lod_ok = body_lod <= CARPART_LOD_A;
				break;
			case CARSLOTID_DOOR_SILL_LEFT:
				draw = left_door_open || show_inner_parts;
				lod_ok = body_lod <= CARPART_LOD_A;
				break;
			case CARSLOTID_DOOR_SILL_RIGHT:
				draw = right_door_open || show_inner_parts;
				lod_ok = body_lod <= CARPART_LOD_A;
				break;
			case CARSLOTID_HOOD_UNDER:
				draw = hood_open;
				break;
			case CARSLOTID_TRUNK_UNDER:
				if (*(int*)_DrawTrunkAudio) continue;
				draw = show_inner_parts ? (trunk_open || ShowTrunkUnderInFE(car_type)) : trunk_open; // 0x62331B
				lod_ok = body_lod <= CARPART_LOD_A;
				break;
			default:
				draw &= 1;
				break;
			}
			if (!lod_ok || !draw) continue;

			model->ReplaceLightMaterial(STRINGHASH_CARSKIN, material);
			model->ReplaceLightMaterial(STRINGHASH_WINDSHIELD, (int)car_render_info->LightMaterial_WindowTint);
			model->ReplaceLightMaterial(STRINGHASH_CARSKIN_CARBONFIBRE, car_render_info->CarbonBodyKit ? material_carbon : material_carskin);

			// CarRenderInfo_Render_AnimLocationForExtraAttachmentCodeCave @ 0x623458
			PartAnim_RenderCarRenderInfo = (DWORD*)car_render_info;
			PartAnim_RenderSlotPiece = slot * 2 + piece;
			int location = AnimLocationForExtraAttachment(car_type, slot, GetCarSlotAnim(slot, piece));
			if (slot == CARSLOTID_DECAL_REAR_WINDOW)
			{
				if (LinkRearWindowDecalToTrunk(car_type)) location = CarSlotAnimLocation::Trunk; // 0x623479
			}
			else if (slot == CARSLOTID_HEADLIGHT && car_type == CAR_TYPE_POPUP_HEADLIGHTS)
				location = CarSlotAnimLocation::Hood;

			int state = anim_render_info ? CarAnimManager_GetLocationState(car_hash, location) : 1;
			if (state != 1 && state != -1)
			{
				int anim_piece = (slot == CARSLOTID_HEADLIGHT && car_type == CAR_TYPE_POPUP_HEADLIGHTS && car_render_info->mHoodVertSplit) ? 1 : piece;
				bMatrix4* anim = CarRenderInfo_GetAnimatedPartMatrix(car_hash, location, anim_piece, lods[0], local_world, biased_local_world);
				if (!anim) return 0;
				if (slot == CARSLOTID_TRUNK) trunk_matrix = anim;
				else if (slot == CARSLOTID_DOOR_LEFT) left_door_matrix = anim;
				else if (slot == CARSLOTID_DOOR_RIGHT) right_door_matrix = anim;
				CarRenderInfo_RenderPart(view, model, anim, light_context, body_flags);
			}
			else if (!damage || !CarPartDamage_IsPartDamaged((DWORD*)damage, damage_index))
			{
				CarRenderInfo_RenderPart(view, model, biased_local_world, light_context, body_flags);
			}
			else if (!RenderRewriteNoDamagedPartAlloc && !eFrameMallocMatrix(1))
			{
				return 0; // the game allocates a matrix for a damaged part and draws nothing
			}
		}
	}

	// exhaust tips and license plates on their markers (LOD A models)
	bMatrix4 marker_rotate;
	bIdentity(&marker_rotate);
	eRotateY(&marker_rotate, &marker_rotate, 0x4000);
	for (int list = 0; list < 2; list++)
	{
		bool plates = list == 1;
		if (!(reflexion || body_lod <= (plates ? CARPART_LOD_C : CARPART_LOD_B))) continue;
		CarEmitterPositionList* emitters = &car_render_info->EmitterPositionList[plates ? CAR_EMITTER_LIST_LICENSE_PLATE : CAR_EMITTER_LIST_EXHAUST];
		int part_slot = plates ? CARSLOTID_LICENSE_PLATE : CARSLOTID_EXHAUST;
		for (BYTE* emitter = (BYTE*)emitters->Head; emitter && emitter != (BYTE*)emitters; emitter = *(BYTE**)emitter)
		{
			bMatrix4* part_world = eFrameMallocMatrix(1);
			if (!part_world) continue;
			bMatrix4 marker_local;
			ePositionMarker* marker = *(ePositionMarker**)(emitter + 0x10);
			eMulMatrix(&marker_local, &marker_rotate, &marker->Matrix);
			eMulMatrix(part_world, &marker_local, biased_local_world);
			for (int piece = 0; piece < 2; piece++)
			{
				eModel* part = car_render_info->mCarPartModels[part_slot][piece][CARPART_LOD_A];
				if (!part) continue;
				if (!plates && car_render_info->LightMaterial_Exhaust)
					part->ReplaceLightMaterial(STRINGHASH_EXHAUST_TIP, (int)car_render_info->LightMaterial_Exhaust);
				if (plates && trunk_matrix && LinkLicensePlateToTrunk(car_type)) // LinkLicensePlateToTrunkCodeCave @ 0x6253ED
					eMulMatrix(part_world, &marker_local, trunk_matrix);
				CarRenderInfo_RenderPart(view, part, part_world, light_context, body_flags);
			}
		}
	}

	// spoiler, roof scoop, mirrors
	if (body_lod < CARPART_LOD_D)
	{
		bMatrix4* spoiler_world = eFrameMallocMatrix(2);
		for (int piece = 0; spoiler_world && piece < 2; piece++)
		{
			eModel* spoiler = car_render_info->mCarPartModels[CARSLOTID_SPOILER][piece][body_lod];
			if (!spoiler) continue;
			bMatrix4* where = biased_local_world;
			if (car_render_info->SpoilerPositionMarker)
			{
				eMulMatrix(&spoiler_world[piece], &car_render_info->SpoilerPositionMarker->Matrix, (trunk_matrix && piece == 0) ? trunk_matrix : biased_local_world);
				where = &spoiler_world[piece];
			}
			spoiler->ReplaceLightMaterial(STRINGHASH_CARSKIN, material_spoiler);
			CarRenderInfo_RenderPart(view, spoiler, where, light_context, body_flags);
		}

		bMatrix4* roof_world = eFrameMallocMatrix(1);
		if (roof_world)
		{
			bMatrix4* where = biased_local_world;
			if (car_render_info->RoofScoopPositionMarker)
			{
				eMulMatrix(roof_world, &car_render_info->RoofScoopPositionMarker->Matrix, biased_local_world);
				where = roof_world;
			}
			for (int piece = 0; piece < 2; piece++)
			{
				eModel* scoop = car_render_info->mCarPartModels[CARSLOTID_ROOF][piece][body_lod];
				if (!scoop) continue;
				scoop->ReplaceLightMaterial(STRINGHASH_CARSKIN, material_roof);
				CarRenderInfo_RenderPart(view, scoop, where, light_context, body_flags);
			}
		}

		if (CarPart_GetUpgradeLevel(ride_info->mPartsTable[CARSLOTID_WING_MIRROR]))
		{
			// aftermarket mirrors on the body mirror markers, following the doors
			for (int side = 0; side < 2; side++)
			{
				bMatrix4* mirror_world = eFrameMallocMatrix(1);
				ePositionMarker* marker = side == 0 ? car_render_info->MirrorBodyLeftPositionMarker : car_render_info->MirrorBodyRightPositionMarker;
				eModel* mirror = car_render_info->mCarPartModels[CARSLOTID_WING_MIRROR][side][body_lod];
				if (!mirror_world || !marker || !mirror) continue;
				*mirror_world = marker->Matrix;
				if (side == 1) mirror_world->v1.y = -mirror_world->v1.y;
				bool broken = damage && (side == 0 ? TheCarDamageMap_LeftMirror : TheCarDamageMap_RightMirror)
					&& CarDamage_Amount(damage, side == 0 ? 0x120 : 0x124) > CarPartDamageCutoff;
				if (broken) continue;
				bMatrix4* door = side == 0 ? left_door_matrix : right_door_matrix;
				eMulMatrix(mirror_world, mirror_world, door ? door : biased_local_world);
				mirror->ReplaceLightMaterial(STRINGHASH_MIRROR, material_mirror);
				CarRenderInfo_RenderPart(view, mirror, mirror_world, light_context, body_flags);
			}
		}
		else
		{
			// stock mirrors: piece 0 left, piece 1 right
			for (int piece = 0; piece < 2; piece++)
			{
				eModel* stock = car_render_info->mCarPartModels[CARSLOTID_WING_MIRROR][piece][body_lod];
				if (!stock) continue;
				bMatrix4* door = piece == 0 ? left_door_matrix : right_door_matrix;
				bMatrix4* where;
				if (RenderRewriteStockMirrorsPerDoor) where = door;
				else where = left_door_matrix ? door : nullptr; // the game: follow the doors only while the left one animates
				if (!where) where = biased_local_world;
				stock->ReplaceLightMaterial(STRINGHASH_CARSKIN, material_carskin);
				CarRenderInfo_RenderPart(view, stock, where, light_context, body_flags);
			}
		}
	}

	// wheel matrices
	float standard_width = *(float*)_WheelStandardWidth;
	float front_brake_marker = TweakBrakeMarkerY[0] != 0.0f ? TweakBrakeMarkerY[0] : car_render_info->WheelBrakeMarkerY[0];
	float rear_brake_marker = TweakBrakeMarkerY[1] != 0.0f ? TweakBrakeMarkerY[1] : car_render_info->WheelBrakeMarkerY[1];
	CarRenderInfo_WheelCamber camber = CarRenderInfo_GetWheelCamber(car_render_info, body_lod, false);

	bMatrix4 trans_pivot[4], trans_axle[4], brake_axle[4], spinner_axle[4];
	for (int w = 0; w < 4; w++)
	{
		float width = car_render_info->WheelWidths[w];
		float y_offset = car_render_info->WheelYRenderOffset[w][has_wide_body];
		float spinner_offset = car_render_info->WheelSpinnerOffset[w][has_wide_body];
		float brake_marker = w >= 2 ? rear_brake_marker : front_brake_marker;
		float pivot_y = width * WheelPivotTranslationAmount; // the game: 0 when the width is 0
		if (width == 0.0f) width = standard_width;
		if (RenderRewriteWheelPivotUsesFallbackWidth) pivot_y = width * WheelPivotTranslationAmount;
		float scale = width / standard_width;

		bIdentity(&trans_pivot[w]);
		bIdentity(&trans_axle[w]);
		bIdentity(&brake_axle[w]);
		bIdentity(&spinner_axle[w]);
		trans_pivot[w].v1.y = scale;
		trans_pivot[w].v3.y = -pivot_y;
		float axle_y = -(y_offset >= 0.0f ? y_offset - pivot_y : pivot_y + y_offset);
		trans_axle[w].v3.y = axle_y;
		spinner_axle[w].v3.y = y_offset >= 0.0f ? axle_y - spinner_offset : axle_y + spinner_offset;
		float brake_pivot = brake_marker != 0.0f ? scale * brake_marker : pivot_y;
		brake_axle[w].v3.y = y_offset >= 0.0f ? -(y_offset - brake_pivot) : -(y_offset + brake_pivot);
		if (w < 2 || camber.angle[w])
		{
			trans_axle[w].v3.z -= camber.push_down;
			spinner_axle[w].v3.z -= camber.push_down;
			brake_axle[w].v3.z -= camber.push_down;
		}
		tire_matrices[w].v3.y = 0.0f;
		brake_matrices[w].v3.y = 0.0f;
		spinner_matrices[w].v3.y = 0.0f;
	}

	// tires and spinners
	bool has_spinners[2] = {
		car_render_info->mCarPartModels[CARSLOTID_FRONT_WHEEL][1][CARPART_LOD_A] != nullptr,
		car_render_info->mCarPartModels[CARSLOTID_REAR_WHEEL][1][CARPART_LOD_A] != nullptr };
	DWORD* tire_light_context = nullptr;
	int tire_visible[4] = {};
	if (ride_info->mPartsEnabled[CARSLOTID_FRONT_WHEEL] && body_lod <= CARPART_LOD_C)
	{
		bMatrix4* tire_world = eFrameMallocMatrix(4);
		bMatrix4* spinner_world = eFrameMallocMatrix(4);
		tire_light_context = (DWORD*)eFrameMalloc(4 * LIGHT_CONTEXT_SIZE);
		DWORD* spinner_light_context = (DWORD*)eFrameMalloc(4 * LIGHT_CONTEXT_SIZE);
		if (tire_world && tire_light_context && spinner_world && spinner_light_context)
		{
			for (int w = 0; w < 4; w++)
				tire_visible[w] = car_render_info->TheCarPartCuller.CarPartInfoTable[CULLABLE_CAR_PART_TIRE_FL + w].IsVisible && (!car || Car_WheelAttached(car, w));
			int tire_lod = tire_lod_wanted > CARPART_LOD_C ? CARPART_LOD_C : tire_lod_wanted;
			bMatrix4 flip;
			bIdentity(&flip);
			eRotateZ(&flip, &flip, 0x8000);

			// piece 0 tire and rim, piece 1 spinner
			eModel* front_models[2], * rear_models[2];
			for (int piece = 0; piece < 2; piece++)
			{
				front_models[piece] = car_render_info->mCarPartModels[CARSLOTID_FRONT_WHEEL][piece][tire_lod];
				rear_models[piece] = car_render_info->mCarPartModels[CARSLOTID_REAR_WHEEL][piece][tire_lod];
			}
			CarRenderInfo_PaintRims(front_models[0], rear_models[0], (int)car_render_info->LightMaterial_WheelRim);
			CarRenderInfo_PaintRims(front_models[1], rear_models[1], (int)car_render_info->LightMaterial_Spinner);

			for (int w = 0; w < 4; w++)
			{
				if (!tire_visible[w]) continue;
				bool front = w < 2, left = w == 0 || w == 3;
				bMatrix4* axles[2] = { &trans_axle[w], &spinner_axle[w] };
				bMatrix4* inputs[2] = { &tire_matrices[w], &spinner_matrices[w] };
				bMatrix4* outputs[2] = { &tire_world[w], &spinner_world[w] };
				DWORD* contexts[2] = { tire_light_context + w * (LIGHT_CONTEXT_SIZE / 4), spinner_light_context + w * (LIGHT_CONTEXT_SIZE / 4) };
				for (int piece = 0; piece < 2; piece++)
				{
					if (piece == 1 && !has_spinners[front ? 0 : 1]) break;
					bMatrix4 cambered;
					bMatrix4* wheel_matrix = inputs[piece];
					if (camber.angle[w])
					{
						CarRenderInfo_CamberMatrix(&cambered, wheel_matrix, camber.angle[w]);
						wheel_matrix = &cambered;
					}
					bMatrix4 m;
					if (left) eMulMatrix(&m, &trans_pivot[w], &flip);
					else m = trans_pivot[w];
					eMulMatrix(&m, &m, wheel_matrix);
					eMulMatrix(&m, &m, axles[piece]);
					eMulMatrix(outputs[piece], &m, biased_local_world);
					elCloneLightContext(contexts[piece], outputs[piece], hack_man_matrix, &camera_world_position, view, base_light_context);

					eModel* model = front ? front_models[piece] : rear_models[piece];
					if (piece == 0)
					{
						if (!front) model = CarRenderInfo_GetRearTireModel(car_render_info, model, tire_lod, outputs[0], view, true);
						CarRenderInfo_RenderPart(view, model, outputs[0], contexts[0], extra_render_flags | env_flag | body_render_flags, true);
					}
					else
					{
						CarRenderInfo_RenderPart(view, model, outputs[1], contexts[1], extra_render_flags | env_flag);
					}
				}
			}
		}
	}

	// brakes
	CarPart* front_brake_part = ride_info->mPartsTable[CARSLOTID_FRONT_BRAKE];
	if (body_lod <= CARPART_LOD_B && front_brake_part && car_render_info->mCarPartModels[CARSLOTID_FRONT_BRAKE][0][CARPART_LOD_A])
	{
		int brake_visible[4];
		for (int w = 0; w < 4; w++)
			brake_visible[w] = car_render_info->TheCarPartCuller.CarPartInfoTable[CULLABLE_CAR_PART_BRAKE_FL + w].IsVisible;
		bMatrix4* brake_world = nullptr;
		DWORD* brake_light_context = nullptr;
		if (brake_visible[0] | brake_visible[1] | brake_visible[2] | brake_visible[3])
		{
			brake_world = eFrameMallocMatrix(4);
			brake_light_context = (DWORD*)eFrameMalloc(4 * LIGHT_CONTEXT_SIZE);
		}
		if (brake_world && brake_light_context)
		{
			int rear_piece = CarPart_GetUpgradeLevel(front_brake_part) == 0; // stock brakes use piece 1 at the back
			eModel* front_brake = car_render_info->mCarPartModels[CARSLOTID_FRONT_BRAKE][0][CARPART_LOD_A]; // 0x62E01D: LOD A in game too
			eModel* rear_brake = car_render_info->mCarPartModels[CARSLOTID_FRONT_BRAKE][rear_piece][CARPART_LOD_A];
			for (eModel* brake : { front_brake, rear_brake })
			{
				if (!brake || !car_render_info->LightMaterial_Caliper) continue;
				brake->ReplaceLightMaterial(STRINGHASH_CALIPER, (int)car_render_info->LightMaterial_Caliper);
				brake->ReplaceLightMaterial(STRINGHASH_CALLIPER, (int)car_render_info->LightMaterial_Caliper);
			}
			bMatrix4 brake_mirror;
			bIdentity(&brake_mirror);
			brake_mirror.v0.x = BrakeMirrorX;
			brake_mirror.v1.y = BrakeMirrorY;
			brake_mirror.v2.z = BrakeMirrorZ;

			for (int w = 0; w < 4; w++)
			{
				if (!brake_visible[w]) continue;
				bool front = w < 2, left = w == 0 || w == 3;
				bMatrix4* brake_matrix = &brake_matrices[w];
				bMatrix4 cambered;
				if (camber.angle[w])
				{
					if (RenderRewriteBrakeCamberOnCopy || !front)
					{
						CarRenderInfo_CamberMatrix(&cambered, brake_matrix, camber.angle[w]);
						brake_matrix = &cambered;
					}
					else
					{
						CarRenderInfo_CamberMatrix(brake_matrix, brake_matrix, camber.angle[w]); // the game: in place
					}
				}
				bMatrix4 m;
				if (left) eMulMatrix(&m, &brake_mirror, brake_matrix);
				else m = *brake_matrix;
				eMulMatrix(&m, &m, &brake_axle[w]);
				eMulMatrix(&brake_world[w], &m, biased_local_world);

				eModel* brake = front ? front_brake : rear_brake;
				if (!brake) continue;
				brake->AttachReplacementTextureTable(left ? (DWORD*)car_render_info->BrakeLeftReplacementTextureTable : (DWORD*)car_render_info->BrakeRightReplacementTextureTable, 2, 0);
				DWORD* context = tire_light_context ? tire_light_context + w * (LIGHT_CONTEXT_SIZE / 4) : nullptr;
				if (!(tire_visible[w] && racing) || !context)
				{
					context = brake_light_context + w * (LIGHT_CONTEXT_SIZE / 4);
					elCloneLightContext(context, &brake_world[w], hack_man_matrix, &camera_world_position, view, base_light_context);
				}
				CarRenderInfo_RenderPart(view, brake, &brake_world[w], context, extra_render_flags | body_render_flags);
			}
		}
	}

	// engine (ShowEngineAttrCodeCave @ 0x6304C4: a SHOWENGINE hood shows it too)
	CarPart* hood_part = ride_info->mPartsTable[CARSLOTID_HOOD];
	if (hood_open || (hood_part && CarPart_GetAppliedAttributeUParam((DWORD*)hood_part, CT_bStringHash("SHOWENGINE"), 0)))
	{
		CarPart* engine_part = ride_info->mPartsTable[CARSLOTID_ENGINE];
		DWORD* engine_light_context = CarRenderInfo_MakePartLightContext(light_context, EngineLightScale * hood_light,
			car_render_info->EngineNeonColour.CurrentColour, EngineNeonScale * hood_light, CarPart_GetUpgradeLevel(engine_part) == 3);
		for (int piece = 0; engine_light_context && piece < 2; piece++)
		{
			eModel* engine = car_render_info->mCarPartModels[CARSLOTID_ENGINE][piece][CARPART_LOD_A];
			if (!engine) continue;
			engine->ReplaceLightMaterial(STRINGHASH_CARSKIN, material_carskin);
			CarRenderInfo_RenderPart(view, engine, biased_local_world, piece == 0 ? engine_light_context : light_context, extra_render_flags | body_render_flags);
		}
	}

	// trunk audio (player cars only)
	CarPart* audio_part = ride_info->mPartsTable[CARSLOTID_TRUNK_AUDIO];
	if ((!car || Car_pDriverInfo_Type(car) == 1) && audio_part)
	{
		int audio_level = CarPart_GetUpgradeLevel(audio_part);
		DWORD* trunk_light_context = CarRenderInfo_MakePartLightContext(light_context, TrunkLightScale * trunk_light,
			car_render_info->TrunkNeonColour.CurrentColour, TrunkNeonScale * trunk_light, audio_level == 3);
		if (!trunk_light_context) return 0;
		int comp_piece = audio_level > 1;
		unsigned int in_game_flag = racing && !IsInShowcase();
		for (int piece = 0; piece < 2; piece++) // the trunk box
		{
			eModel* box = &car_render_info->TrunkAudioModels[piece][0];
			if (!box->Solid) continue;
			box->ReplaceLightMaterial(STRINGHASH_CARSKIN, material_carskin);
			box->ReplaceLightMaterial(STRINGHASH_AUDIO, audio_level == 3 ? (int)car_render_info->LightMaterial_Audio : material_carskin);
			CarRenderInfo_RenderPart(view, box, biased_local_world, piece == 0 ? trunk_light_context : light_context, extra_render_flags | body_render_flags | in_game_flag);
		}
		const int num_comps = CARSLOTID_TRUNK_AUDIO_COMP_11 - CARSLOTID_TRUNK_AUDIO_COMP_0 + 1;
		bMatrix4* comp_world = eFrameMallocMatrix(num_comps);
		ePositionMarker** markers = CarRenderInfo_TrunkAudioCompMarkers(car_render_info);
		for (int c = 0; comp_world && c < num_comps; c++)
		{
			if (!markers[c]) continue;
			eMulMatrix(&comp_world[c], &markers[c]->Matrix, biased_local_world);
			eModel* comp = &car_render_info->TrunkAudioCompModels[c][comp_piece][0]; // 0x630C27: the real components in game too
			if (!comp->Solid) continue;
			comp->ReplaceLightMaterial(STRINGHASH_CARSKIN, (int)car_render_info->LightMaterial_Audio);
			CarRenderInfo_RenderPart(view, comp, &comp_world[c], trunk_light_context, body_render_flags | extra_render_flags);
		}
	}
	return 1;
}

void __fastcall CarRenderInfo_RenderFast(CarRenderInfo* car_render_info, void* EDX_Unused, DWORD* view, void* shaper_lights, bVector3* world_position,
	bMatrix4* body_matrix, bMatrix4* tire_matrices, bMatrix4* brake_matrices, bMatrix4* spinner_matrices, unsigned int extra_render_flags,
	int force_light_state, int reflexion, int tireLOD, int carLOD)
{
	if (!DisableTextureReplacement && !DisableMaterialBasedReplacement) // CarRenderInfo_RenderFast_GetCurrCRICodeCave @ 0x6162FD
		CurrCarRenderInfo = (DWORD*)car_render_info;

	int pixel_size = eView_GetPixelSize(view, world_position, 3.0f);
	if (eGetCurrentViewMode() == EVIEWMODE_TWOH && *(int*)_TheGameFlowManager != GAMEFLOW_STATE_FRONTEND)
	{
		pixel_size = (int)((float)pixel_size * CarTwoPlayerLODFactor);
		carLOD = tireLOD = CARPART_LOD_B;
	}
	if (pixel_size < 4) return;

	DWORD* car = (DWORD*)car_render_info->pCar;
	BYTE* controls = car ? Car_pControls(car) : nullptr;
	bVector3* camera_position = eView_CameraPosition(view);
	bool racing = *(int*)_TheGameFlowManager == GAMEFLOW_STATE_RACING;

	DWORD* light_context = (DWORD*)eFrameMalloc(LIGHT_CONTEXT_SIZE);
	bMatrix4* local_world = eFrameMallocMatrix(1);
	bMatrix4* cpy_local_world = eFrameMallocMatrix(1);
	bMatrix4* biased_local_world = eFrameMallocMatrix(1);
	bMatrix4* biased_identity = eFrameMallocMatrix(1);
	if (!light_context || !local_world || !cpy_local_world || !biased_local_world || !biased_identity) return;

	bCopy(local_world, body_matrix);
	local_world->v3 = bVector4(world_position->x, world_position->y, world_position->z, 1.0f);
	if (!eView_GetVisibleState(view, &car_render_info->AABBMin, &car_render_info->AABBMax, local_world)) return;
	bVector4 camera_world_position(camera_position->x, camera_position->y, camera_position->z, 1.0f);

	if (PrintQueryLightMat) PrintLightQuery = 1;
	if (car) *cpy_local_world = *Car_GetRenderBodyMatrix(car, (char)eView_ID(view));
	else bCopy(cpy_local_world, body_matrix);
	cpy_local_world->v3 = bVector4(world_position->x, world_position->y, world_position->z, 1.0f);
	if (racing) eRemoveDynamicLightPack((DWORD*)car_render_info->CarLights);
	DWORD base_light_context[LIGHT_CONTEXT_SIZE / 4];
	elResetLightContext(base_light_context);
	elSetupLights(base_light_context, shaper_lights, world_position, nullptr, hack_man_matrix, view);
	elCloneLightContext(light_context, cpy_local_world, hack_man_matrix, &camera_world_position, view, base_light_context);
	CarRenderInfo_AmbientColour(car_render_info) = *(bVector3*)LightContext_Lights(light_context);
	if (racing) eAddDynamicLightPack((DWORD*)car_render_info->CarLights);
	if (PrintQueryLightMat) PrintLightQuery = 0;

	unsigned int env_flag = (!EnableEnvMap || (float)pixel_size < DisableEnvMapSize) ? 1 : 0;

	int body_lod = carLOD, tire_lod_wanted = tireLOD;
	while ((float)pixel_size < CarBodyLodSwapSizeFast[body_lod])
	{
		body_lod++;
		tire_lod_wanted++;
	}
	if (body_lod > CARPART_LOD_C)
	{
		body_lod = CARPART_LOD_D;
		if (car && Car_mState_field_3E4(car) == 0 && car_render_info->TheCarPartCuller.CarPartInfoTable[CULLABLE_CAR_PART_UNDERNEATH].IsVisible)
			body_lod = CARPART_LOD_C;
	}
	if (ForceCarLOD != CARPART_LOD_OFF) body_lod = ForceCarLOD;
	if (ForceTireLOD != CARPART_LOD_OFF) tire_lod_wanted = ForceTireLOD;
	if (body_lod == CARPART_LOD_OFF && tire_lod_wanted == CARPART_LOD_OFF) return;
	unsigned int render_flags = extra_render_flags | 0x800;

	bMatrix4 world_local;
	bVector3 camera_eye;
	bInvertMatrix(&world_local, local_world);
	bMulMatrix(&camera_eye, &world_local, camera_position);
	CarPartCuller_CullParts((DWORD*)&car_render_info->TheCarPartCuller, &camera_eye, controls ? *(unsigned short*)(controls + 0x1E0) : 0);
	bCopy(biased_local_world, local_world);
	biased_local_world->v3.z += fCarFudgeElevation;

	// shadow grid (no projected shadow, no underglow)
	if (!reflexion)
	{
		bCopy_Position(biased_identity, eMathIdentityMatrix, world_position);
		bMatrix4 pre_translate;
		bIdentity(&pre_translate);
		pre_translate.v3 = bVector4(-world_position->x, -world_position->y, -world_position->z, 1.0f);
		eMulMatrix(biased_identity, &pre_translate, biased_identity);

		ShadowGroundPosition = *world_position;
		if (car) ShadowGroundPosition.z = TopologyCoordinate_GetElevation(Car_mTopology(car), &ShadowGroundPosition, nullptr, ShadowGroundNormal, nullptr);
		bVector3 car_centre;
		float fade = CarRenderInfo_GetShadowFade(car, local_world, &car_centre);
		CarRenderInfo_DrawAmbientShadow(car_render_info, world_position, local_world, biased_identity, fade, &car_centre, false);
	}

	// body: every enabled model slot, with the same paint choices as Render (the game used car skin everywhere)
	RideInfo* ride_info = car_render_info->pRideInfo;
	int has_wide_body = CarPart_GetUpgradeLevel(ride_info->mPartsTable[CARSLOTID_WIDE_BODY]) != 0;
	int material_carskin = (int)car_render_info->LightMaterial_CarSkin;
	int material_carbon = (int)car_render_info->LightMaterial_Carbon;
	unsigned int body_flags = render_flags | env_flag | 0x8000;
	for (int slot = CARSLOTID_MODEL_FIRST; slot < CARSLOTID_MODEL_NUM; slot++)
	{
		if (slot == CARSLOTID_FRONT_WHEEL || slot == CARSLOTID_REAR_WHEEL || slot == CARSLOTID_LICENSE_PLATE) continue;
		if (!(ride_info->mPartsEnabled[slot] & 1)) continue;
		for (int piece = 0; piece < 2; piece++)
		{
			eModel** lods = car_render_info->mCarPartModels[slot][piece];
			eModel* model = lods[0] ? lods[body_lod] : nullptr;
			if (!model) continue;
			bool carbon = (slot == CARSLOTID_FRONT_BUMPER && car_render_info->CarbonFrontBumper)
				|| (slot == CARSLOTID_REAR_BUMPER && car_render_info->CarbonRearBumper)
				|| (slot == CARSLOTID_WIDE_BODY && car_render_info->CarbonWideBody)
				|| (slot == CARSLOTID_HOOD && car_render_info->CarbonHood)
				|| (slot == CARSLOTID_TRUNK && car_render_info->CarbonTrunk)
				|| ((slot == CARSLOTID_DOOR_LEFT || slot == CARSLOTID_DOOR_RIGHT) && (CFDoorsFix ? car_render_info->CarbonDoors : car_render_info->CarbonWideBody));
			model->ReplaceLightMaterial(STRINGHASH_CARSKIN, carbon ? material_carbon : material_carskin);
			model->ReplaceLightMaterial(STRINGHASH_WINDSHIELD, (int)car_render_info->LightMaterial_WindowTint);
			model->ReplaceLightMaterial(STRINGHASH_CARSKIN_CARBONFIBRE, car_render_info->CarbonBodyKit ? material_carbon : material_carskin);
			CarRenderInfo_RenderPart(view, model, biased_local_world, light_context, body_flags);
			if (!RenderRewriteFastAllPieces) break; // the game draws only the first piece of a slot
		}
	}

	// tires (no spinners or brakes in RenderFast)
	float standard_width = *(float*)_WheelStandardWidth;
	CarRenderInfo_WheelCamber camber = CarRenderInfo_GetWheelCamber(car_render_info, body_lod, true);
	bMatrix4 trans_pivot[4], trans_axle[4];
	for (int w = 0; w < 4; w++)
	{
		float width = car_render_info->WheelWidths[w];
		float y_offset = car_render_info->WheelYRenderOffset[w][has_wide_body]; // the game always used [w][0]
		float pivot_y = width * WheelPivotTranslationAmount;
		if (width == 0.0f) width = standard_width;
		if (RenderRewriteWheelPivotUsesFallbackWidth) pivot_y = width * WheelPivotTranslationAmount;
		bIdentity(&trans_pivot[w]);
		bIdentity(&trans_axle[w]);
		trans_pivot[w].v1.y = width / standard_width;
		trans_pivot[w].v3.y = -pivot_y;
		trans_axle[w].v3.y = -(y_offset >= 0.0f ? y_offset - pivot_y : pivot_y + y_offset);
		if (w < 2 || camber.angle[w]) trans_axle[w].v3.z -= camber.push_down;
		tire_matrices[w].v3.y = 0.0f;
	}

	if (!ride_info->mPartsEnabled[CARSLOTID_FRONT_WHEEL] || body_lod > CARPART_LOD_C) return;
	bMatrix4* tire_world = eFrameMallocMatrix(4);
	DWORD* tire_light_context = (DWORD*)eFrameMalloc(4 * LIGHT_CONTEXT_SIZE);
	if (!tire_world || !tire_light_context) return;
	int tire_lod = tire_lod_wanted > CARPART_LOD_C ? CARPART_LOD_C : tire_lod_wanted;
	bMatrix4 flip;
	bIdentity(&flip);
	eRotateZ(&flip, &flip, 0x8000);
	eModel* front_tire = car_render_info->mCarPartModels[CARSLOTID_FRONT_WHEEL][0][tire_lod];
	eModel* rear_tire = car_render_info->mCarPartModels[CARSLOTID_REAR_WHEEL][0][tire_lod];
	CarRenderInfo_PaintRims(front_tire, rear_tire, (int)car_render_info->LightMaterial_WheelRim);
	for (int w = 0; w < 4; w++)
	{
		if (!(car_render_info->TheCarPartCuller.CarPartInfoTable[CULLABLE_CAR_PART_TIRE_FL + w].IsVisible && (!car || Car_WheelAttached(car, w)))) continue;
		bool front = w < 2, left = w == 0 || w == 3;
		bMatrix4 cambered;
		bMatrix4* tire_matrix = &tire_matrices[w];
		if (camber.angle[w])
		{
			CarRenderInfo_CamberMatrix(&cambered, tire_matrix, camber.angle[w]);
			tire_matrix = &cambered;
		}
		bMatrix4 m;
		if (left) eMulMatrix(&m, &trans_pivot[w], &flip);
		else m = trans_pivot[w];
		eMulMatrix(&m, &m, tire_matrix);
		eMulMatrix(&m, &m, &trans_axle[w]);
		eMulMatrix(&tire_world[w], &m, biased_local_world);
		DWORD* context = tire_light_context + w * (LIGHT_CONTEXT_SIZE / 4);
		elCloneLightContext(context, &tire_world[w], hack_man_matrix, &camera_world_position, view, base_light_context);
		eModel* tire = front ? front_tire : CarRenderInfo_GetRearTireModel(car_render_info, rear_tire, tire_lod, &tire_world[w], view, false);
		CarRenderInfo_RenderPart(view, tire, &tire_world[w], context, render_flags | env_flag | 0x8000, true);
	}
}
