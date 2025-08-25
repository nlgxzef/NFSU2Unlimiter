#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall IcePartsBrowser_RefreshHeader(DWORD* IcePartsBrowser, void* EDX_Unused)
{
	IcePartsBrowser_RefreshHeader_Game(IcePartsBrowser);

	RefreshDetailsPane((char const*)IcePartsBrowser[1], IcePartsBrowser + 21, IcePartsBrowser + 20, (DWORD*)IcePartsBrowser[23]);
}