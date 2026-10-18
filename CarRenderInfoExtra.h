#pragma once

#define CRI_Loc_Extra 0x3C/4 // ModelOffset.pad, normally unused

static injector::hook_back<int(*)(DWORD, int, int)> hb_GetTextureInfo; // for TexWizard compatibililty, also used in CarRenderInfo.h
void TexturePainter_PaintTireTexture(DWORD*, DWORD); // TexturePainter.h

bool UseUnlimiterEmitter = 1;

#define NUM_LICENSE_PLATE_TEXTURES 12

DWORD DefaultLicensePlateReplacementTextures[]
{
	CT_bStringHash("LICENSE_PLATE"),
	CT_bStringHash("LICENSE_PLATE_FRAME"),
	CT_bStringHash("LICENSE_PLATE_STICKERS"),
	CT_bStringHash("LICENSE_PLATE_DIGIT1"),
	CT_bStringHash("LICENSE_PLATE_DIGIT2"),
	CT_bStringHash("LICENSE_PLATE_DIGIT3"),
	CT_bStringHash("LICENSE_PLATE_DIGIT4"),
	CT_bStringHash("LICENSE_PLATE_DIGIT5"),
	CT_bStringHash("LICENSE_PLATE_DIGIT6"),
	CT_bStringHash("LICENSE_PLATE_DIGIT7"),
	CT_bStringHash("LICENSE_PLATE_DIGIT8"),
	CT_bStringHash("LICENSE_PLATE_DIGIT9"),
};

struct CarRenderInfoExtra
{
	DWORD /*CarRenderInfo*/* pCarRenderInfo;
	DWORD /*RideInfo*/* pRideInfo;
	DWORD /*Car*/* pCar;

	bMatrix4 ExhaustFlameColorMatrix;
	bMatrix4 TireSmokeColorMatrix;
	bTList<AcidEmitter> TireSmokeEmitters[NUM_TERRAIN_TYPES];
	bTList<AcidEmitter> TireSpewEmitters[NUM_TERRAIN_TYPES];

	int OnLights = 0;
	int BrokenLights = 0;
	float TimeBaseStart = 0.0f;

	DWORD /*TextureInfo*/* pTireTextureInfo = nullptr;

	ReplacementTextureEntry LicensePlateReplacementTextureTable[NUM_LICENSE_PLATE_TEXTURES];

	CarRenderInfoExtra(DWORD* render_info, DWORD* ride, DWORD *car)
	{
		pCarRenderInfo = render_info;
		pRideInfo = ride;
		pCar = car;

		for (int i = 0; i < NUM_TERRAIN_TYPES; i++)
		{
			TireSmokeEmitters[i].InitList();
			TireSpewEmitters[i].InitList();
		}
	}

	~CarRenderInfoExtra()
	{
		for (int i = 0; i < NUM_TERRAIN_TYPES; i++)
		{
			sub_4367C0(&TireSmokeEmitters[i]); // empty list
			sub_4367C0(&TireSpewEmitters[i]); // empty list
		}
	}

	void ApplyExhaustFlameColor()
	{
		if (DisableExhaustFlameAndTireSmoke) return;

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_UL_EXHAUST_FLAME);
		bTList<AcidEmitter>* EmitterList_CARFX_NITRO = (bTList<AcidEmitter>*)(pCarRenderInfo + (0x4A8 / 4));
		float hue, lum, sat;

		if (Part && Part[0] != CT_bStringHash("VINYL_L1_COLOR01")) // First color is unused as it's 5, 5, 5 and barely visible.
		{
			int r = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0);
			int g = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0);
			int b = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0);
			ConvertRGBtoHSL((float)r, (float)g, (float)b, &hue, &sat, &lum);

			// CARFX_NITRO
			aFxGetEmitters(EmitterList_CARFX_NITRO, UseUnlimiterEmitter ? CT_bStringHash("CARFX_NITRO_UL") : CT_bStringHash("CARFX_NITRO"));
			afxShiftEmitterColours(EmitterList_CARFX_NITRO, &ExhaustFlameColorMatrix, hue, sat, lum);

			ExhaustFlameColorMatrix.v3.x = (float)r;
			ExhaustFlameColorMatrix.v3.y = (float)g;
			ExhaustFlameColorMatrix.v3.z = (float)b;
		}
	}

	void ApplyTireSmokeColor()
	{
		if (DisableExhaustFlameAndTireSmoke) return;

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_UL_TIRE_SMOKE);
		bTList<AcidEmitter>* EmitterList_CARFX_SKID_SMOKE = (bTList<AcidEmitter>*)(pCarRenderInfo + (0x470 / 4));
		float hue, lum, sat;

		TerrainTypeInfo* tti = TerrainTypeInfoTable;
		// Find a way to send them with CarRenderInfo_TriggerEffect instead of TerrainEffectEmitters

		if (Part && Part[0] != CT_bStringHash("VINYL_L1_COLOR01")) // First color is unused as it's 5, 5, 5 and barely visible.
		{
			int r = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0);
			int g = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0);
			int b = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0);
			ConvertRGBtoHSL((float)r, (float)g, (float)b, &hue, &sat, &lum);

			char const* em;
			// Collect emitter data for all terrain types
			for (int i = 0; i < NUM_TERRAIN_TYPES; i++)
			{
				// SKID_SMOKE
				em = tti->SkidSmokeEmitterGroup;

				if (em && *em)
				{
					aFxGetEmitters(&TireSmokeEmitters[i], bStringHash(em));
					afxShiftEmitterColours(&TireSmokeEmitters[i], &TireSmokeColorMatrix, hue, sat, lum);
				}

				// TIRE_SPEW
				em = tti->TireSpewEmitterGroup;

				if (em && *em)
				{
					aFxGetEmitters(&TireSpewEmitters[i], bStringHash(em));
					afxShiftEmitterColours(&TireSpewEmitters[i], &TireSmokeColorMatrix, hue, sat, lum);
				}

				tti++; // get next terrain
			}

			TireSmokeColorMatrix.v3.x = (float)r;
			TireSmokeColorMatrix.v3.y = (float)g;
			TireSmokeColorMatrix.v3.z = (float)b;
		}
	}

	void GetTireTextureInfo()
	{
		if (DisableTextureReplacement || DisableMaterialBasedReplacement) return;

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_UL_TIRE_TEXTURE);
		if (Part)
		{
			DWORD TireTextureHash = CarPart_GetTextureName(Part);
			if (TireTextureHash) pTireTextureInfo = (DWORD*)hb_GetTextureInfo.fun(TireTextureHash, 0, 0);

			TexturePainter_PaintTireTexture(pRideInfo, TireTextureHash);
		}
	}

	void InitLicensePlateReplacementTextureTable()
	{
		DWORD PlateTextureHash = 0;

		// Get plate part first
		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_LICENSE_PLATE);
		if (Part)
		{
			PlateTextureHash = CarPart_GetTextureName(Part);
		}
		
		// Initialize with empty table
		for (int i = 0; i < NUM_LICENSE_PLATE_TEXTURES; i++)
		{
			LicensePlateReplacementTextureTable[i].hOldNameHash = DefaultLicensePlateReplacementTextures[i];
			LicensePlateReplacementTextureTable[i].hNewNameHash = CT_bStringHash("DEFAULTALPHA");
			LicensePlateReplacementTextureTable[i].pTextureInfo = (DWORD*)0xFFFFFFFF;
			LicensePlateReplacementTextureTable[i].ppReplacedTextureInfo = 0;

			if (i == 0)
			{
				LicensePlateReplacementTextureTable[i].hNewNameHash = 
					PlateTextureHash ? PlateTextureHash : DefaultLicensePlateReplacementTextures[i];
			}
		}

		// Attach this table to the license plate model
		for (int i = 0; i < 8; i++)
		{
			eModel* LPModel = (eModel*)(pCarRenderInfo[0x17A8 / 4 + CARSLOTID_LICENSE_PLATE * 8 + i]);
			if (LPModel)
			{
				LPModel->pReplacementTextureTable = (DWORD*)LicensePlateReplacementTextureTable;
				LPModel->NumReplacementTextures = NUM_LICENSE_PLATE_TEXTURES;
			}
		}
	}

	char *RandomizeLicensePlate(int num)
	{
		char buf[NUM_LICENSE_PLATE_TEXTURES - 2]; // 9 chars + \0

		for (int i = 0; i < num; i++)
		{
			char c = bRandom(26) + 'A';
			char n = bRandom(10) + '0';

			if (num == 7)
			{
				switch (i)
				{
				case 1:
				case 2:
				case 3:
					buf[i] = c;
					break;
				default:
					buf[i] = n;
					break;
				}
			}
			else
			{
				switch (i)
				{
				case 2:
				case 3:
				case 4:
					buf[i] = c;
					break;
				default:
					buf[i] = n;
					break;
				}
			}
		}

		return buf;
	}

	void AssignLicensePlate()
	{
		int NumMorphTargets = 0;
		char buf[64];
		char texbuf[64];
		
		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_LICENSE_PLATE);
		if (Part)
		{
			NumMorphTargets = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("MORPHTARGET_NUM"), 0);
		}

		if (!NumMorphTargets) return; // No need to assign, the plate is not customizable

		int r = NumMorphTargets - 2;

		DWORD* DriverInfo = nullptr;
		char* DriverName = nullptr;
		if (pCar)
		{
			DriverInfo = (DWORD*)pCar[5];
			if (DriverInfo)
			{
				DriverName = (char*)DriverInfo + 0x90C;
			}
		}
		else if (*(int*)TheGameFlowManager == 3) // // TheGameFlowManager->mCurrentState, no Car in FE
		{
			DriverName = GetProfileName();
		}

		if (!DriverName)
		{
			DriverName = RandomizeLicensePlate(r);
		}

		strcpy(buf, DriverName); // Copy driver name so we don't accidentally modify the original string
		strupr(buf); // Convert to uppercase

		int len = strlen(buf);
		int i = 0;
		if (len < r) i = (r - len) / 2;

		for (int q = 0; q < NUM_LICENSE_PLATE_TEXTURES - 3; i++, q++)
		{
			if (!buf[q]) break;

			if ((buf[q] >= '0' && buf[q] <= '9')
				|| (buf[q] >= 'A' && buf[q] <= 'Z'))
			{
				sprintf(texbuf, "LP_CHAR_%c", buf[q]);
			}
			else // If not a letter or number
			{
				buf[q] = '-'; // Replace with dash
				sprintf(texbuf, "LP_CHAR_DASH");
			}

			LicensePlateReplacementTextureTable[i + 3].hNewNameHash = bStringHash(texbuf); // 3-11 = chars
		}
	}

	void Init()
	{
		ApplyExhaustFlameColor();
		ApplyTireSmokeColor();

		GetTireTextureInfo();

		InitLicensePlateReplacementTextureTable();
		AssignLicensePlate();
	}
 };