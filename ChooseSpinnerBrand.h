#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall ChooseSpinnerBrand_NotificationMessage(DWORD* ChooseSpinnerBrand, void* EDX_Unused, DWORD MessageHash, DWORD* fe_obj, DWORD param1, DWORD param2)
{
	ChooseSpinnerBrand_NotificationMessage_Game(ChooseSpinnerBrand, MessageHash, fe_obj, param1, param2);
	if (MessageHash == CT_bStringHash("PAD_LEFT") || MessageHash == CT_bStringHash("PAD_RIGHT"))
	SetDetailsPaneVisibility((char const*)ChooseSpinnerBrand[1], 0);
}

void __fastcall ChooseSpinnerBrand_RefreshHeader(DWORD* ChooseSpinnerBrand, void* EDX_Unused)
{
	ChooseSpinnerBrand_RefreshHeader_Game(ChooseSpinnerBrand);

	RefreshDetailsPane((char const*)ChooseSpinnerBrand[1], ChooseSpinnerBrand + 85, ChooseSpinnerBrand + 84, (DWORD*)ChooseSpinnerBrand[87]);
}

void __fastcall ChooseSpinnerBrand_StartBrowsingRims(DWORD* ChooseSpinnerBrand, void* EDX_Unused)
{
	ChooseSpinnerBrand_StartBrowsingRims_Game(ChooseSpinnerBrand);

	SetDetailsPaneVisibility((char const*)ChooseSpinnerBrand[1], 1);
}

void __fastcall ChooseSpinnerBrand_StopBrowsingRims(DWORD* ChooseSpinnerBrand, void* EDX_Unused, bool ass)
{
	ChooseSpinnerBrand_StopBrowsingRims_Game(ChooseSpinnerBrand, ass);

	SetDetailsPaneVisibility((char const*)ChooseSpinnerBrand[1], 0);
}