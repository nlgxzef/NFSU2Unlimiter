#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define dword_83898C *(DWORD*)0x83898C
#define dword_838990 *(DWORD*)0x838990
#define dword_838998 *(DWORD*)0x838998
#define dword_8389A0 *(DWORD*)0x8389A0

char const* __fastcall CustomizationScreenManager_CalcPrevScreen(DWORD* CustomizationScreenManager)
{
    char const* result; // eax

    if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_CUSTOMIZE_CATEGORY))
    {
        switch (dword_83898C)
        {
        case 1:
            result = "UI_CareerCrib.fng";
            break;
        case 2:
        case 3:
            result = "UI_QRCarSelect.fng";
            break;
        case 4:
            result = "UI_OLCarSelect.fng";
            break;
        default:
            return "";
        }
        return result;
    }
    if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_PERF_CATEGORY))
        return *(char const**)_gCC_CHOOSE_CUSTOMIZE_CATEGORY;
    if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_PERF_PACKAGE))
        return *(char const**)_gCC_CHOOSE_PERF_CATEGORY;
    if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_BUY_PERF_PARTS))
        return *(char const**)_gCC_CHOOSE_PERF_PACKAGE;
    if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_GENERIC_PARTS_BROWSER))
    {
        if (dword_838990 != 2)
            return dword_83898C != 0 ? *(char const**)_gCC_CHOOSE_CUSTOMIZE_CATEGORY : 0;
        if (dword_8389A0 == 1)
            return *(char const**)_gCC_CHOOSE_VINYL_LAYER;
        return *(char const**)_gCC_CHOOSE_PAINT_CATEGORY;
    }
    if (bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_PAINT_CATEGORY))
    {
        if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_ICE_CATEGORY))
        {
        LABEL_34:
            switch (dword_838998)
            {
            case 1:
            case 2:
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
                dword_838998 = 0;
                result = *(char const**)_gCC_CHOOSE_ICE_CATEGORY;
                break;
            case 4:
                dword_838998 = 3;
                result = *(char const**)_gCC_CHOOSE_ICE_CATEGORY;
                break;
            case 9:
                dword_838998 = 7;
                result = *(char const**)_gCC_NEON_MAIN;
				break;
            default:
                result = *(char const**)_gCC_CHOOSE_CUSTOMIZE_CATEGORY;
                break;
            }
            return result;
        }
        if (bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_PAINT_BROWSER))
        {
            if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_VINYL_LAYER)
                || !bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_DECALS_MAIN))
            {
                return *(char const**)_gCC_CHOOSE_PAINT_CATEGORY;
            }
            if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_ONLINE_CAR_LOT))
                return *(char const**)_gCC_CHOOSE_CUSTOMIZE_CATEGORY;
            if (bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_NEON_MAIN))
            {
                if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_RIM_BRAND))
                    return *(char const**)_gCC_GENERIC_PARTS_BROWSER;
                if (!bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_RIMS_BROWSER))
                    return *(char const**)_gCC_CHOOSE_RIM_BRAND;
                if (bStrICmp(*(const char**)(CustomizationScreenManager[3] + 8), *(char const**)_gCC_CHOOSE_SPINNERS))
                    return "";
                dword_838998 = 0;
                return *(char const**)_gCC_CHOOSE_ICE_CATEGORY;
            }
            goto LABEL_34;
        }
        if (dword_8389A0)
        {
            if (dword_8389A0 == 1)
                return *(char const**)_gCC_CHOOSE_VINYL_LAYER;
            if (dword_8389A0 == 2)
                return "";
        }
        return *(char const**)_gCC_CHOOSE_PAINT_CATEGORY;
    }
    else
    {
        result = *(char const**)_gCC_CHOOSE_CUSTOMIZE_CATEGORY;
        if (!dword_83898C)
            return "";
    }
    return result;
}