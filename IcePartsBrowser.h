#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall IcePartsBrowser_RefreshHeader(DWORD* IcePartsBrowser, void* EDX_Unused)
{
	IcePartsBrowser_RefreshHeader_Game(IcePartsBrowser);

	RefreshDetailsPane((char const*)IcePartsBrowser[1], IcePartsBrowser + 21, IcePartsBrowser + 20, (DWORD*)IcePartsBrowser[23]);
}

void __fastcall IcePartsBrowser_DoTheAnimation(DWORD* IcePartsBrowser, void* EDX_Unused, int location, bool on)
{
    // Do custom animations
    DWORD* CategoryNode = (DWORD*)IcePartsBrowser[68];
    if (CategoryNode)
    {
        GetAndDoFEPartAnim(CategoryNode[18], on, 1.0f);
    }
}

void(__thiscall* IcePartsBrowser_BuildPartsList_Game)(DWORD* IcePartsBrowser) = (void(__thiscall*)(DWORD*))0x5606E0;
void __fastcall IcePartsBrowser_BuildPartsList(DWORD* IcePartsBrowser, void* EDX_Unused)
{
    IcePartsBrowser_BuildPartsList_Game(IcePartsBrowser);

    // Colorize Part icons
    DWORD* CategoryNode = (DWORD*)IcePartsBrowser[68];
    if (CategoryNode)
    {
        int CarSlotID = CategoryNode[18];
        if (CarSlotID == CARSLOTID_WHEEL_MANUFACTURER
            || CarSlotID == CARSLOTID_MISC)
        {
            DWORD* PartsList = IcePartsBrowser + 21;

            DWORD* parts_sel = PartsList;
            DWORD* parts_tail = (DWORD*)PartsList[1];

            int PartsCount = bList_TraversebList(parts_sel, parts_tail);

            if (PartsCount)
            {
                for (parts_sel = (DWORD*)parts_sel[0];
                    parts_sel != PartsList;
                    parts_sel = (DWORD*)parts_sel[0])
                {
                    DWORD* Part = (DWORD*)parts_sel[2];
                    Color* clr = (Color*)(parts_sel + 6); // different format, channels swapped on purpose
                    clr->b = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0) % 256;
                    clr->g = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0) % 256;
                    clr->r = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0) % 256;
                }
            }
        }
    }
}