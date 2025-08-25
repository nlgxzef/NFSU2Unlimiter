#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall ChooseDecalScreen_RefreshHeader(DWORD* ChooseDecalScreen, void* EDX_Unused)
{
	ChooseDecalScreen_RefreshHeader_Game(ChooseDecalScreen);

	RefreshDetailsPane((char const*)ChooseDecalScreen[1], ChooseDecalScreen + 21, ChooseDecalScreen + 20, (DWORD*)ChooseDecalScreen[23]);
}