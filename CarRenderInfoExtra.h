#pragma once

#define CRI_Loc_Extra 0x3C/4 // ModelOffset.pad, normally unused

struct CarRenderInfoExtra
{
	DWORD /*CarRenderInfo*/* pCarRenderInfo;
	DWORD /*RideInfo*/* pRideInfo;
	DWORD /*Car*/* pCar;

	bMatrix4 ExhaustFlameColorMatrix;
	bMatrix4 TireSmokeColorMatrix;

	int OnLights = 0;
	int BrokenLights = 0;
	float TimeBaseStart = 0.0f;

	CarRenderInfoExtra(DWORD* render_info, DWORD* ride, DWORD *car)
	{
		pCarRenderInfo = render_info;
		pRideInfo = ride;
		pCar = car;
	}

	~CarRenderInfoExtra()
	{

	}

	void ApplyExhaustFlameColor()
	{
		if (DisableExhaustFlameAndTireSmoke) return;

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_MISC);
		float hue, lum, sat;

		if (Part)
		{
			int r = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0);
			int g = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0);
			int b = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0);
			ConvertRGBtoHSL((float)r, (float)g, (float)b, &hue, &sat, &lum);

			// CARFX_NITRO
			aFxGetEmitters(pCarRenderInfo + (0x4A8 / 4), CT_bStringHash("CARFX_NITRO"));
			afxShiftEmitterColours(pCarRenderInfo + (0x4A8 / 4), &ExhaustFlameColorMatrix, hue, sat, lum);

			ExhaustFlameColorMatrix.v3.x = (float)r;
			ExhaustFlameColorMatrix.v3.y = (float)g;
			ExhaustFlameColorMatrix.v3.z = (float)b;
		}
	}

	void ApplyTireSmokeColor()
	{
		if (DisableExhaustFlameAndTireSmoke) return;

		DWORD* Part = RideInfo_GetPart(pRideInfo, CARSLOTID_WHEEL_MANUFACTURER);
		float hue, lum, sat;

		if (Part)
		{
			int r = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0);
			int g = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0);
			int b = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0);
			ConvertRGBtoHSL((float)r, (float)g, (float)b, &hue, &sat, &lum);

			// CARFX_SKID_SMOKE
			//aFxGetEmitters(pCarRenderInfo + (0x470 / 4), CT_bStringHash("XEBO_ROAD"));
			//aFxGetEmitters(pCarRenderInfo + (0x470 / 4), CT_bStringHash("XEBO_DIRT"));
			//afxShiftEmitterColours(pCarRenderInfo + (0x470 / 4), &TireSmokeColorMatrix, hue, sat, lum);

			// CARFX_TIRE_SPEW
			aFxGetEmitters(pCarRenderInfo + (0x478 / 4), CT_bStringHash("XETT_ROAD_SK"));
			aFxGetEmitters(pCarRenderInfo + (0x478 / 4), CT_bStringHash("XETT_ROAD_SP"));
			afxShiftEmitterColours(pCarRenderInfo + (0x478 / 4), &TireSmokeColorMatrix, hue, sat, lum);

			TireSmokeColorMatrix.v3.x = (float)r;
			TireSmokeColorMatrix.v3.y = (float)g;
			TireSmokeColorMatrix.v3.z = (float)b;

		}
	}

	void Init()
	{
		ApplyExhaustFlameColor();
		//ApplyTireSmokeColor();
	}
};