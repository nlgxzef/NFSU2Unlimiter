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