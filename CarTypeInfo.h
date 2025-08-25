#pragma once

#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"

DWORD GetCarTypeLogoHash(int type, bool manu)
{
	DWORD result = 0;
	int BuildRegion = GetBuildRegion();
	char const *CarTypeName = (char const*)CarConfigs[type].CarTypeInfo;
	DWORD CarTypeNameHash = bStringHash(CarTypeName);

	if (manu)
	{
		char const* CarManuName = CarTypeInfo_GetManufacturerName(CarConfigs[type].CarTypeInfo);
		result = FEngHashString("CARSELECT_MANUFACTURER_%s", CarManuName);
		if (IsTraffic(type)) return FEngHashString("CARSELECT_MANUFACTURER_TRAFFIC_%s", CarManuName);

		if (IsBuildRegionEurope()) // EU
		{
			switch (CarTypeNameHash)
			{
			case CT_bStringHash("CORSA"):
				return GetCurrentLanguage() ? CT_bStringHash("CARSELECT_MANUFACTURER_OPEL") : result;
				break;
			}
		}
		else
		{
			if (BuildRegion == 3)
			{
				switch (CarTypeNameHash)
				{
				case CT_bStringHash("G35"):
					return CT_bStringHash("CARSELECT_MANUFACTURER_NISSAN");
					break;
				case CT_bStringHash("IS300"):
					return CT_bStringHash("CARSELECT_MANUFACTURER_TOYOTA");
					break;
				}
			}

			//if (CarTypeNameHash == CT_bStringHash("CORSA")) return CT_bStringHash("CARSELECT_MANUFACTURER_OPEL");
		}
	}
	else
	{
		result = FEngHashString("SECONDARY_LOGO_%s", CarTypeName);
		if (IsTraffic(type)) return FEngHashString("SECONDARY_LOGO_TRAFFIC_%s", CarTypeName);

		if (!BuildRegion)
		{
			switch (CarTypeNameHash)
			{
			case CT_bStringHash("MIATA"):
				return result;
				break;
			case CT_bStringHash("TIBURON"):
				return result;
				break;
			}
		}

		if (IsBuildRegionEurope()) // EU
		{
			switch (CarTypeNameHash)
			{
			case CT_bStringHash("MIATA"):
				return CT_bStringHash("SECONDARY_LOGO_MX5");
				break;
			case CT_bStringHash("TIBURON"):
				return CT_bStringHash("SECONDARY_LOGO_COUPE");
				break;
			case CT_bStringHash("CORSA"):
				return GetCurrentLanguage() ? CT_bStringHash("SECONDARY_LOGO_OPEL_CORSA") : result;
				break;
			}
		}

		if (BuildRegion == 2)
		{
			switch (CarTypeNameHash)
			{
			case CT_bStringHash("MIATA"):
				return CT_bStringHash("SECONDARY_LOGO_MX5");
				break;
			case CT_bStringHash("TIBURON"):
				return CT_bStringHash("SECONDARY_LOGO_TUSCANI");
				break;
			}
		}
		else
		{
			if (BuildRegion == 3)
			{
				switch (CarTypeNameHash)
				{
				case CT_bStringHash("MIATA"):
					return CT_bStringHash("SECONDARY_LOGO_MIATA_ROADSTER");
					break;
				case CT_bStringHash("COROLLA"):
					return CT_bStringHash("SECONDARY_LOGO_TRUENO");
					break;
				case CT_bStringHash("IS300"):
					return CT_bStringHash("SECONDARY_LOGO_ALTEZZA");
					break;
				case CT_bStringHash("G35"):
					return CT_bStringHash("SECONDARY_LOGO_SKYLINE_JAPAN");
					break;
				case CT_bStringHash("350Z"):
					return CT_bStringHash("SECONDARY_LOGO_FAIRLADYZ");
					break;
				case CT_bStringHash("CORSA"):
					return CT_bStringHash("SECONDARY_LOGO_OPEL_CORSA");
					break;
				case CT_bStringHash("TIBURON"):
					return CT_bStringHash("SECONDARY_LOGO_COUPE");
					break;
				}
			}
			else
			{
				switch (CarTypeNameHash)
				{
				case CT_bStringHash("MIATA"):
					return CT_bStringHash("SECONDARY_LOGO_MX5");
					break;
				case CT_bStringHash("TIBURON"):
					return CT_bStringHash("SECONDARY_LOGO_COUPE");
					break;
				}
			}
		}

		//if (CarTypeNameHash == CT_bStringHash("CORSA")) return CT_bStringHash("SECONDARY_LOGO_OPEL_CORSA");

	}

	return result;
}