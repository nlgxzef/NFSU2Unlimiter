#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void ChooseLoadableTextures(int car_type, DWORD* engine_info, int units, DWORD* loadable_tex, int hud_index)
{
    bool IsDrag; // bl
    char* fill; // eax
    char StringToHash[29]; // [esp+10h] [ebp-20h] BYREF

    memset(loadable_tex, 0, 0x20u);
    if (!*(BYTE*)0x89E7D8 && !*(BYTE*)0x890108 /* g_tweakIsDragRace */ || (IsDrag = 1, *(DWORD*)0x89E7C0 != 1))
        IsDrag = 0;
    memset(StringToHash, 0, sizeof(StringToHash));
    bSPrintf(StringToHash, "TACH_FILL_CUSTOM_%02d", hud_index);

    fill = IsDrag ? (char*)"" : StringToHash;
    loadable_tex[2] = bStringHash(fill);

    DWORD lines_hash = 0;
    DWORD redline_hash = 0;

    DWORD prefix_hash = IsDrag ? CT_bStringHash("DRAG_RPM_") : CT_bStringHash("3RDPERSON_");

    // Check if the gauge part has a custom prefix
    if (!IsDrag)
    {
        DWORD* DriverInfoByPlayerNumber = RaceParameters_GetDriverInfoByPlayerNumber((DWORD*)_TheRaceParameters, 0);
        if (DriverInfoByPlayerNumber && *(char*)DriverInfoByPlayerNumber <= 0)
        {
            DWORD* ride = DriverInfoByPlayerNumber + 4; // Get RideInfo from DriverInfo
            if (ride)
            {
                DWORD* Part = RideInfo_GetPart(ride, CARSLOTID_CUSTOM_HUD); // get HUD
                if (Part)
                {
                    prefix_hash = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("TEXTURE_NAME"), prefix_hash); // Get custom prefix

                    // Add underscore if not 3RDPERSON_ and fix tach_fill as it doesn't have prefix by default
                    if (prefix_hash != CT_bStringHash("3RDPERSON_"))
                    {
                        prefix_hash = bStringHash2("_", prefix_hash);

                        bSPrintf(StringToHash, "TACH_FILL_CUSTOM_%02d", hud_index);
                        loadable_tex[2] = bStringHash2(StringToHash, prefix_hash);
                    }
                }
            }
        }
    }

    int TachStyle = GetTachStyle(engine_info);

    if (IsDrag)
    {
        switch(TachStyle)
        {
        case 0:
            lines_hash = bStringHash2("10000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_9000", prefix_hash);
            break;
        case 1:
            lines_hash = bStringHash2("10000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_8500", prefix_hash);
            break;
        case 2:
            lines_hash = bStringHash2("9000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_8000", prefix_hash);
            break;
        case 3:
            lines_hash = bStringHash2("9000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_7500", prefix_hash);
            break;
        case 4:
            lines_hash = bStringHash2("8000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_7000", prefix_hash);
            break;
        case 5:
            lines_hash = bStringHash2("8000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_6500", prefix_hash);
            break;
        case 6:
        default:
            lines_hash = bStringHash2("7000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_6000", prefix_hash);
            break;
        case 7:
            lines_hash = bStringHash2("7000_LINES", prefix_hash);
            redline_hash = bStringHash2("REDLINES_5500", prefix_hash);
            break;
        }
    }
    else
    {
        bSPrintf(StringToHash, "TURBODIAL_CUSTOM_%02d", hud_index);
        loadable_tex[7] = bStringHash2(StringToHash, prefix_hash);
        bSPrintf(StringToHash, "TURBOFILL_CUSTOM_%02d", hud_index);
        loadable_tex[6] = bStringHash2(StringToHash, prefix_hash);
        bSPrintf(StringToHash, "TURBOLINES_CUSTOM_%02d", hud_index);
        loadable_tex[5] = bStringHash2(StringToHash, prefix_hash);
        bSPrintf(StringToHash, "RPMDIAL_CUSTOM_%02d", hud_index);
        loadable_tex[4] = bStringHash2(StringToHash, prefix_hash);

        switch (TachStyle)
        {
        case 0:
            bSPrintf(StringToHash, "10500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_9000", prefix_hash);
            break;
        case 1:
            bSPrintf(StringToHash, "10500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_8500", prefix_hash);
            break;
        case 2:
            bSPrintf(StringToHash, "9500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_8000", prefix_hash);
            break;
        case 3:
            bSPrintf(StringToHash, "9500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_7500", prefix_hash);
            break;
        case 4:
            bSPrintf(StringToHash, "8500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_7000", prefix_hash);
            break;
        case 5:
            bSPrintf(StringToHash, "8500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_6500", prefix_hash);
            break;
        case 6:
        default:
            bSPrintf(StringToHash, "7500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_6000", prefix_hash);
            break;
        case 7:
            bSPrintf(StringToHash, "7500LINES_CUSTOM_%02d", hud_index);
            lines_hash = bStringHash2(StringToHash, prefix_hash);
            redline_hash = bStringHash2("REDLINE_5500", prefix_hash);
            break;
        }
    }

    loadable_tex[1] = redline_hash;
    loadable_tex[0] = lines_hash;
}