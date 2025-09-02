#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

// 0x56BE1D
void __declspec(naked) DoubleMessageFixCodeCave_ChooseDecalScreen_ToggleColors()
{
	_asm
	{
		cmp eax, edi 
		jnz loc_56BE22 // 2nd message has different eax and edi values

		call ChooseDecalScreen_ToggleColors

		loc_56BE22:
		push 0x56BE22
		retn
	}
}

void __fastcall ChooseDecalScreen_RefreshHeader(DWORD* ChooseDecalScreen, void* EDX_Unused)
{
	ChooseDecalScreen_RefreshHeader_Game(ChooseDecalScreen);

	RefreshDetailsPane((char const*)ChooseDecalScreen[1], ChooseDecalScreen + 21, ChooseDecalScreen + 20, (DWORD*)ChooseDecalScreen[23]);
}