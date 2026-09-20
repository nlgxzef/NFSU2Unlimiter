#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

bool ShouldShowColorValuesInsteadOfName(int CarSlotID)
{
	bool result = 0;

	switch (CarSlotID)
	{
	case CARSLOTID_WHEEL_MANUFACTURER:
	case CARSLOTID_MISC:
		result = 1;
		break;
	default:
		result = 0;
		break;
	}

	return result;
}

bool ShouldShowPaintedIcons(int CarSlotID)
{
	bool result = 0;

	switch (CarSlotID)
	{
	case CARSLOTID_WHEEL_MANUFACTURER:
	case CARSLOTID_MISC:
	case CARSLOTID_HEADLIGHT_BULB:
		result = 1;
		break;
	default:
		result = 0;
		break;
	}

	return result;
}

void __fastcall IcePartsBrowser_RefreshHeader(DWORD* IcePartsBrowser, void* EDX_Unused)
{
	//IcePartsBrowser_RefreshHeader_Game(IcePartsBrowser);

	char const* FEPkg = (char const*)IcePartsBrowser[1];
	DWORD* CategoryNode = (DWORD*)IcePartsBrowser[68];

	if ((DWORD*)IcePartsBrowser[21] != IcePartsBrowser + 21)
	{
		PartNode* CurSelPart = (PartNode*)IcePartsBrowser[23];

		if (CurSelPart)
		{
			CarPart* part = CurSelPart->part_2;

			if (part)
			{
				if (ShouldShowColorValuesInsteadOfName(CategoryNode[18]))
				{
					Color clr;
					clr.r = CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("RED"), 0) % 256;
					clr.g = CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("GREEN"), 0) % 256;
					clr.b = CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("BLUE"), 0) % 256;

					if (part->PartNameHash == CT_bStringHash("VINYL_L1_COLOR01"))
						FEngSetLanguageHash(FEPkg, 0xD28B9316, CT_bStringHash("STOCK"));
					else FEPrintf(FEPkg, 0xD28B9316, "%d, %d, %d (0x%0.6x)", clr.r, clr.g, clr.b, clr.rgba);
				}
				else
				{
					DWORD LanguageHash = CarPart_GetAppliedAttributeUParam((DWORD*)part, CT_bStringHash("LANGUAGEHASH"), 0);

					if (LanguageHash) FEngSetLanguageHash(FEPkg, 0xD28B9316, LanguageHash);
					else FEPrintf(FEPkg, 0xD28B9316, CarPart_GetName((DWORD*)part));
				}
				
			}
			else FEPrintf(FEPkg, 0xD28B9316, "None");
		}

		int CurrentNodeIndex = ThumbnailScroller_GetCurrenNodeIndex(IcePartsBrowser + 20);
		FEScrollBar_Update(IcePartsBrowser + 45, 1, IcePartsBrowser[67], CurrentNodeIndex);

		if (*(int*)_FEDatabase != 1 || *(int*)0x83898C == 1)
		{
			FEngSetInvisible_Pkg(FEPkg, 0x380B632);
		}
		else if (CurSelPart)
		{
			DWORD* CategoryNode = (DWORD*)IcePartsBrowser[68];
			int Cost = PlayerCareerState_GetCarPartCost_Game((DWORD*)ThePlayerCareer, CategoryNode[18], (DWORD*)CurSelPart->part_2);
			FEPrintf(FEPkg, 0x380B632, "%d", Cost);
		}
	}

	RefreshDetailsPane(FEPkg, IcePartsBrowser + 21, IcePartsBrowser + 20, (DWORD*)IcePartsBrowser[23]);
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
void __fastcall IcePartsBrowser_BuildPartsList_2(DWORD* IcePartsBrowser, void* EDX_Unused)
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

void __fastcall IcePartsBrowser_BuildPartsList(DWORD* IcePartsBrowser, void* EDX_Unused)
{
	DWORD* CategoryNode = (DWORD*)IcePartsBrowser[68];
	int CarSlotID = -1;
	if (CategoryNode) CarSlotID = CategoryNode[18];

	PartSelectionScreen_ResetCategories(IcePartsBrowser + 20);

	IcePartsBrowser[67] = 0; // number of parts

	// Add "None" part to the list
	PartNode* node;
	DWORD* nodeFEObj;
	
	/* // Unused empty part, left for reference
	
	if (CarSlotID == CARSLOTID_WHEEL_MANUFACTURER
		|| CarSlotID == CARSLOTID_MISC)
	{
		node = (PartNode*)j__malloc(sizeof(PartNode));
		if (node)
		{
			node->part = 0;
			node->FEngObject = 0;
			node->XPos = 0.0f;
			node->YPos = 0.0f;
			node->IsGreyOut = 0;
			node->field_0x21 = 1;
			node->field_0x22 = 0;
			node->field_0x23 = 0;
			node->part_2 = 0;
			((DWORD*)node)[0] = _PartNode_vtbl;

			nodeFEObj = ThumbnailScroller_AddNode(IcePartsBrowser + 20, (DWORD*)node, 1, -1);

			// Icon
			if (nodeFEObj)
			{
				nodeFEObj[9] = CT_bStringHash("GENERIC_ICON_PARTS"); // handle
				nodeFEObj[7] |= 0x400000u; // flags
			}

			++IcePartsBrowser[67];
		}
	}
	*/
	// Get parts list
	bTList<SelectablePart> PartList;
	PartList.InitList();
	GetIcePartsList(CarSlotID, (DWORD*)&PartList, 0);

	if (PartList.IsEmpty())
	{
		sub_4DEF30((DWORD*)&PartList);
	}
	else
	{
		for (SelectablePart* selPart = PartList.GetHead();
			selPart != PartList.EndOfList();
			selPart = (SelectablePart*)selPart->Next)
		{
			node = (PartNode*)j__malloc(sizeof(PartNode));
			if (node)
			{
				node->part = selPart->part;
				node->FEngObject = 0;
				node->XPos = 0.0f;
				node->YPos = 0.0f;
				node->IsGreyOut = 0;
				node->field_0x21 = 1;
				node->field_0x22 = 0;
				node->field_0x23 = 0;
				node->part_2 = selPart->part;
				((DWORD*)node)[0] = _PartNode_vtbl;

				Color clr(0xFFFFFFFF);

				if (node->part_2 && ShouldShowPaintedIcons(CarSlotID))
				{
					DWORD* Part = (DWORD*)node->part_2;
					if (node->part_2->PartNameHash != CT_bStringHash("VINYL_L1_COLOR01"))
					{
						clr.b = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("RED"), 0) % 256;
						clr.g = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("GREEN"), 0) % 256;
						clr.r = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BLUE"), 0) % 256;
					}
				}

				nodeFEObj = ThumbnailScroller_AddNode(IcePartsBrowser + 20, (DWORD*)node, 1, clr);

				// Icon and color
				if (nodeFEObj)
				{
					if (selPart->part)
					{
						nodeFEObj[9] = CarPart_GetAppliedAttributeUParam((DWORD*)selPart->part, CT_bStringHash("TEXTUREHASH"), CT_bStringHash("GENERIC_ICON_PARTS")); // Icon via attribute
					}
					else
					{
						nodeFEObj[9] = CT_bStringHash("GENERIC_ICON_PARTS"); // GENERIC_ICON_PARTS
					}

					nodeFEObj[7] |= 0x400000u; // flags
				}

				++IcePartsBrowser[67];
			}
		}

		if ((DWORD*)IcePartsBrowser[21] == IcePartsBrowser + 21)
		{
			sub_4DEF30((DWORD*)&PartList);
		}
		else // Find and snap to installed part
		{
			DWORD* Part = RideInfo_GetPart((DWORD*)gTheRideInfo, CarSlotID);
			if (!Part || CarSlotID == CARSLOTID_FRONT_WHEEL)
			{
				DWORD* lst = (DWORD*)IcePartsBrowser[21];
				DWORD* nd = lst ? (DWORD*)(lst - 1) : 0;

				ThumbnailScroller_SnapToNode(IcePartsBrowser + 20, nd);
			}
			else
			{
				DWORD* nd = (DWORD*)ThumbnailScroller_FindNode(IcePartsBrowser + 20, (DWORD)Part);
				if (!nd)
				{
					DWORD* lst = (DWORD*)IcePartsBrowser[21];
					nd = lst ? (DWORD*)(lst - 1) : 0;
				}
				ThumbnailScroller_SnapToNode(IcePartsBrowser + 20, nd);
				bList_TraversebList(IcePartsBrowser + 21, 0);
			}

			*((BYTE*)IcePartsBrowser + 127) = 1;
			ThumbnailScroller_SetVisibility(IcePartsBrowser + 20, 1);

			PartNode* cur = (PartNode*)IcePartsBrowser[23];

			if (cur && cur->part_2)
			{
				CarCustomizeManager_PreviewPart((DWORD*)gCarCustomizeManager, CarSlotID, (DWORD*)cur->part_2);
			}

			CarViewer_SetLookAtPart(CarSlotID, 0, 0, 0); // doesn't reference this??

			sub_4DEF30((DWORD*)&PartList);
		}
	}
}

void __fastcall IcePartsBrowser_NotificationMessage(DWORD* IcePartsBrowser, void* EDX_Unused, DWORD message, DWORD* fe_obj, DWORD param1, DWORD param2)
{
	DWORD* CategoryNode = (DWORD*)IcePartsBrowser[68];
	int CarSlotID = -1;
	if (CategoryNode) CarSlotID = CategoryNode[18];

	if (message == CT_bStringHash("PAD_BUTTON1"))
	{
		if (CarSlotID == CARSLOTID_MISC)
		{
			FETriggerCarEffect(CARFX_NITRO, 1.0f, TERRAIN_TYPE_NONE);
		}

		if (CarSlotID == CARSLOTID_WHEEL_MANUFACTURER)
		{
			FETriggerCarEffect(CARFX_SKID_SMOKE, 1.0f, TERRAIN_TYPE_ROAD);
		}
		
		goto Game;
	}
	else
	{
		Game:
		IcePartsBrowser_NotificationMessage_Game(IcePartsBrowser, message, fe_obj, param1, param2);
	}
}