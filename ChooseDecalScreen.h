#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define DECAL_INITIAL_ALL '?'
#define DECAL_INITIAL_START '@'
#define DECAL_INITIAL_END 'Z'

BYTE DecalInitial = DECAL_INITIAL_START;

void ScrollDecalInitial(int direction)
{
	DecalInitial += direction;
	if (DecalInitial > DECAL_INITIAL_END)
	{
		DecalInitial = FilterDecalsByInitials == 2 ? DECAL_INITIAL_ALL : DECAL_INITIAL_START;
	}
	if (DecalInitial < (FilterDecalsByInitials == 2 ? DECAL_INITIAL_ALL : DECAL_INITIAL_START))
	{
		DecalInitial = DECAL_INITIAL_END;
	}
}

BYTE GetDecalInitial(DWORD* part)
{
	char const* name = CarPart_GetName(part);
	return *(BYTE*)name;
}

bool IsDecalInitial(DWORD* part)
{
	if (DecalInitial == DECAL_INITIAL_ALL) return true;
	
	BYTE intl = GetDecalInitial(part);
	if (DecalInitial == DECAL_INITIAL_START)
	{
		return intl <= DECAL_INITIAL_START || intl > DECAL_INITIAL_END;
	}
	return intl == DecalInitial;
}

void __fastcall ChooseDecalScreen_RefreshHeader(DWORD* ChooseDecalScreen, void* EDX_Unused)
{
	ChooseDecalScreen_RefreshHeader_Game(ChooseDecalScreen);

	DWORD* CurrSelPart = (DWORD*)ChooseDecalScreen[23];
	RefreshDetailsPane((char const*)ChooseDecalScreen[1], ChooseDecalScreen + 21, ChooseDecalScreen + 20, CurrSelPart);

	if (FilterDecalsByInitials)
	{
		if (DecalInitial != DECAL_INITIAL_ALL) FEPrintf((char const*)ChooseDecalScreen[1], CT_bStringHash("INITIAL"), "%c", DecalInitial);
		else FEPrintf((char const*)ChooseDecalScreen[1], CT_bStringHash("INITIAL"), "*");
	}
	else
	{
		FEngSetInvisible_Pkg((char const*)ChooseDecalScreen[1], CT_bStringHash("INITIAL_GROUP"));
	}
}

bool IsDecalWhite(DWORD* part)
{
	DWORD name = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("NAME"), 0);
	DWORD brand_name = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BRAND_NAME"), 0);

	return name != brand_name;
}

bool IsDecalAvailable(DWORD* Part, DWORD IsWhite)
{
	bool result = 0;
	
	result = IsWhite != IsDecalWhite(Part);
	if (result && FilterDecalsByInitials) result = IsDecalInitial(Part);

	return result;
}

int GetDecalsList(int CarSlotID, bTList<SelectablePart> *DecalsList, DWORD IsWhite)
{
	int result = 0;

	if (CarSlotID >= CARSLOTID_DECAL_HOOD_TEX0 && CarSlotID <= CARSLOTID_DECAL_RIGHT_QUARTER_TEX7)
	{
		DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
		int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
		int CarPartID = GetCarPartFromSlot(CarSlotID);

		for (DWORD *Part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CarSlotID, 0, 0, -1);
			Part;
			Part = CarPartDatabase_NewGetCarPart((DWORD*)_CarPartDB, CarTypeID, CarSlotID, 0, Part, -1))
		{

			if (IsDecalAvailable(Part, IsWhite))
			{
				SelectablePart* newPart = (SelectablePart*)j__malloc(sizeof(SelectablePart));
				if (newPart)
				{
					newPart->part = (CarPart*)Part;
					newPart->slot = CARSLOTID_MODEL_FIRST; // Unused??

					DecalsList->AddTail(newPart);

					result++;
				}
			}
			
		}
	}
	
	return result;
}

void(__thiscall* ChooseDecalScreen_Setup)(DWORD* ChooseDecalScreen) = (void(__thiscall*)(DWORD*))0x560250;

void __fastcall ChooseDecalScreen_ScrollInitials(DWORD* ChooseDecalScreen, void* EDX_Unused, int direction)
{
	ScrollDecalInitial(direction);
	
	ChooseDecalScreen_Setup(ChooseDecalScreen);
	ChooseDecalScreen_RefreshHeader(ChooseDecalScreen, EDX_Unused);
}

void __fastcall ChooseDecalScreen_NotificationMessage(DWORD* ChooseDecalScreen, void* EDX_Unused, DWORD message, DWORD* fe_obj, DWORD param1, DWORD param2)
{
	switch (message)
	{
		// TODO: Add message responses to FNGs
	case CT_bStringHash("PAD_LTRIGGER"):
		ChooseDecalScreen_ScrollInitials(ChooseDecalScreen, EDX_Unused, -1);
		break;
	case CT_bStringHash("PAD_RTRIGGER"):
		ChooseDecalScreen_ScrollInitials(ChooseDecalScreen, EDX_Unused, 1);
		break;
	case CT_bStringHash("PAD_BUTTON1"):
	case CT_bStringHash("PAD_BUTTON1_RELEASED"):
		ChooseDecalScreen_ToggleColors_Game(ChooseDecalScreen);
		break;
	default:
		ChooseDecalScreen_NotificationMessage_Game(ChooseDecalScreen, message, fe_obj, param1, param2);
		break;
	}
}

// 0x56BE1D
void __declspec(naked) DoubleMessageFixCodeCave_ChooseDecalScreen_ToggleColors()
{
	_asm
	{
		cmp eax, edi
		jnz loc_56BE22 // 2nd message has different eax and edi values

		call ChooseDecalScreen_ToggleColors_Game

		loc_56BE22 :
		push 0x56BE22
			retn
	}
}