#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall NeonPartsBrowser_RefreshHeader(DWORD* NeonPartsBrowser, void* EDX_Unused)
{
	NeonPartsBrowser_RefreshHeader_Game(NeonPartsBrowser);

	RefreshDetailsPane((char const*)NeonPartsBrowser[1], NeonPartsBrowser + 21, NeonPartsBrowser + 20, (DWORD*)NeonPartsBrowser[23]);
}