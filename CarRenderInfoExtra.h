#pragma once

#define CRI_Loc_Extra 0x3C/4 // ModelOffset.pad, normally unused

bool UseUnlimiterEmitter = 1;

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

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_MISC);
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

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_WHEEL_MANUFACTURER);
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

	void Init()
	{
		ApplyExhaustFlameColor();
		ApplyTireSmokeColor();
	}
};