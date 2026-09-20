#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

// 0x4D855B
void __declspec(naked) StaticCameraInfoCodeCave_GarageMainScreen_HandleTick()
{
	_asm
	{
		mov edx, dword ptr ds : [edi + 0x840] // Car Type ID
		//mov ecx, dword ptr ds : [esi + 0x118] // SelectCarCameraMover
		call SelectCarCameraMover_GetAndMoveToNewDefaultOrientation

		push 0x4D8560
		retn
	}
}

// 0x4EAF13
void __declspec(naked) StaticCameraInfoCodeCave_GarageMainScreen_ctor()
{
	_asm
	{
		mov edx, dword ptr ds : [ebp + 0x10] // Car Type ID
		mov edx, dword ptr ds : [edx]
		//mov ecx, dword ptr ds : [esi + 0x118] // SelectCarCameraMover
		call SelectCarCameraMover_GetAndSetDefaultOrientation
		test eax, eax
		jz loc_4EAF18
		mov edi, eax

		loc_4EAF18:
		push 0x4EAF18
		retn
	}
}

void __fastcall GarageMainScreen_TriggerCarEffect(DWORD* GarageMainScreen, void* EDX_Unused, int car_effect, float power, int terrain_type, int unk)
{
	DWORD* CarRenderInfo;
	int position; // eax
	bVector3 location;

	CarRenderInfo = (DWORD*)GarageMainScreen[2];
	if (CarRenderInfo)
	{
		position = 0;
		if (car_effect == CARFX_SKID_SMOKE || car_effect == CARFX_TIRE_SPEW || car_effect == CARFX_BLOWN_TIRE || car_effect == CARFX_DRIVE_ON_FLAT_TIRE)
			position = terrain_type >> 16;
		memset(location, 0, 12);

		CarRenderInfo_TriggerEffect(CarRenderInfo, EDX_Unused, car_effect, power, terrain_type, position, (bVector3*)(GarageMainScreen + 580), (bMatrix4*)(GarageMainScreen + 584), &location);
	}
}