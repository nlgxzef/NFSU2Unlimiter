#pragma once

DWORD*(__thiscall* NitrousGauge_ctor_Game)(DWORD* NitrousGauge, char const* PackageName, int player_num) = (DWORD*(__thiscall*)(DWORD*, char const*, int))0x4F6A90;
DWORD* __fastcall NitrousGauge_ctor_Hook(DWORD* NitrousGauge, void* EDX_Unused, char const* PackageName, int player_num)
{
	DWORD* result = NitrousGauge_ctor_Game(NitrousGauge, PackageName, player_num);

	// Change color of Nitrous Gauge based on Exhaust Flame color
	DWORD* DriverInfo = RaceParameters_GetDriverInfoByPlayerNumber((DWORD*)_TheRaceParameters, player_num);
	if (DriverInfo)
	{
		DWORD* ride = DriverInfo + 4; // Get RideInfo from DriverInfo
		if (ride)
		{
			DWORD* Part = RideInfo_GetPart(ride, CARSLOTID_UL_EXHAUST_FLAME); // get Exhaust Flame
			if (Part && Part[0] != CT_bStringHash("VINYL_L1_COLOR01")) // First color is unused
			{

				BYTE r = Clamp(CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0), 0, 255);
				BYTE g = Clamp(CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0xB4), 0, 255);
				BYTE b = Clamp(CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0xFA), 0, 255);
				BYTE a = 0x96; // Default alpha

				((BYTE*)NitrousGauge)[0x2C] = b/2;
				((BYTE*)NitrousGauge)[0x2D] = g/2;
				((BYTE*)NitrousGauge)[0x2E] = r/2;
				((BYTE*)NitrousGauge)[0x2F] = a;
				((BYTE*)NitrousGauge)[0x30] = b;
				((BYTE*)NitrousGauge)[0x31] = g;
				((BYTE*)NitrousGauge)[0x32] = r;
				((BYTE*)NitrousGauge)[0x33] = a;

				// Fix color of NOS_LINE object
				FEngSetColor((DWORD*)NitrousGauge[8], NitrousGauge[11]);
			}
		}
	}

	return result;
}