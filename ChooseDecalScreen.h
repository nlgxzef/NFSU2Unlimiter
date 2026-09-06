#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#define DECAL_INITIAL_START '@'
#define DECAL_INITIAL_END 'Z'

BYTE DecalInitial = DECAL_INITIAL_START;

void ScrollDecalInitial()
{
	DecalInitial++;
	if (DecalInitial > DECAL_INITIAL_END)
	{
		DecalInitial = DECAL_INITIAL_START;
	}
}

bool IsDecalInitial(DWORD* part)
{
	char const* name = CarPart_GetName(part);
	if (DecalInitial == DECAL_INITIAL_START)
	{
		return *(BYTE*)name <= DECAL_INITIAL_START || *(BYTE*)name > DECAL_INITIAL_END;
	}
	return *(BYTE*)name == DecalInitial;
}

void __fastcall ChooseDecalScreen_RefreshHeader(DWORD* ChooseDecalScreen, void* EDX_Unused)
{
	ChooseDecalScreen_RefreshHeader_Game(ChooseDecalScreen);

	RefreshDetailsPane((char const*)ChooseDecalScreen[1], ChooseDecalScreen + 21, ChooseDecalScreen + 20, (DWORD*)ChooseDecalScreen[23]);

	if (FilterDecalsByInitials) FEPrintf((char const*)ChooseDecalScreen[1], 0xD28B9316, "%c", DecalInitial);
}

bool IsDecalWhite(DWORD* part)
{
	DWORD name = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("NAME"), 0);
	DWORD brand_name = CarPart_GetAppliedAttributeUParam(part, CT_bStringHash("BRAND_NAME"), 0);

	return name != brand_name;
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
			DWORD name = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("NAME"), 0);
			DWORD brand_name = CarPart_GetAppliedAttributeUParam(Part, CT_bStringHash("BRAND_NAME"), 0);

			//if (IsWhite != IsDecalWhite(Part))
			if (IsDecalInitial(Part))
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
	else
	{
		return result;
	}
}

void(__thiscall* ChooseDecalScreen_Setup)(DWORD* ChooseDecalScreen) = (void(__thiscall*)(DWORD*))0x560250;

void __fastcall ChooseDecalScreen_ToggleColors(DWORD* ChooseDecalScreen, void* EDX_Unused)
{
	//DWORD v1 = ChooseDecalScreen[23];
	//((BYTE*)ChooseDecalScreen)[274] ^= 1u;
	//ChooseDecalScreen[69] = ((DWORD*)v1)[44];

	ScrollDecalInitial();
	
	ChooseDecalScreen_Setup(ChooseDecalScreen);
	ChooseDecalScreen_RefreshHeader(ChooseDecalScreen, EDX_Unused);
}

// 0x56BE1D
void __declspec(naked) DoubleMessageFixCodeCave_ChooseDecalScreen_ToggleColors()
{
	_asm
	{
		cmp eax, edi
		jnz loc_56BE22 // 2nd message has different eax and edi values

		call ChooseDecalScreen_ToggleColors

		loc_56BE22 :
		push 0x56BE22
			retn
	}
}