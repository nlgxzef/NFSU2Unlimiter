#pragma once

void __declspec(naked) CarLotFixCodeCaveWrite()
{
	_asm
	{
		cmp ecx, 256
		jae Skip

		mov byte ptr ds : [CarLotUnlockData + ecx] , 0
		
			Skip:
		push 0x513CDA
		retn
	}
}

void __declspec(naked) CarLotFixCodeCaveRead()
{
	_asm
	{
		cmp esi, 256
		jae OutOfRange
		
		mov al, [CarLotUnlockData + esi]
		jmp Done

			OutOfRange :
		xor al, al          // a bogus index reads as locked rather than as garbage

			Done :
		test al, al
		push 0x513CF6
		retn
	}
}

void __declspec(naked) PerformanceConfigFixCodeCave()
{
	_asm mov eax, 1;
	_asm pushad;

	FillUpPerformanceConfig();

	_asm popad;
	_asm push 0x5994C0;
	_asm retn;
}

// 0x61B67C
int IsUG1_Hash(DWORD CarTypeNameHash)
{
	return IsUG1(GetCarTypeIDFromHash(CarTypeNameHash));
}

// 0x61B686
int IsUG2_Hash(DWORD CarTypeNameHash)
{
	return IsUG2(GetCarTypeIDFromHash(CarTypeNameHash));
}

// 0x61B68E
int IsSUV_Hash(DWORD CarTypeNameHash)
{
	return IsSUV(GetCarTypeIDFromHash(CarTypeNameHash));
}

void __declspec(naked) IsSUV_UnInlineCodeCave()
{
	_asm
	{
		mov dword ptr ds : [esp + 0x28] , eax // IsUG2 result

		push ebp
		call IsSUV_Hash
		mov dword ptr ds : [esp + 0x20] , eax
		add esp, 4
		push 0x61B6BA
		retn
	}
}

#define DRIVER_TYPE_TRAFFIC 3

// TrackLoader::LoadHandler: load every driver's car except real traffic drivers
void __declspec(naked) PlayerTrafficCarFixCodeCave_TrackLoader_LoadHandler()
{
	_asm
	{
		cmp dword ptr[ecx + eax + 0x844], 2 // CarTypeInfo::UsageType == traffic
		jne LoadCar
		cmp dword ptr[esi + 4], DRIVER_TYPE_TRAFFIC // DriverInfo::Type
		jne LoadCar

		push 0x57F6E8 // skip, the traffic manager loads it
		retn

		LoadCar :
		lea edx, [esi + 0x10] // DriverInfo::Ride
			push 0x57F6AF
			retn
	}
}

// StreamingTrafficCarManager::SetInitialDesiredCarTypes: give traffic car types to traffic drivers only
void __declspec(naked) PlayerTrafficCarFixCodeCave_StreamingTrafficCarManager_SetInitialDesiredCarTypes()
{
	_asm
	{
		mov dword ptr[esp + 0x18], ebx // original instruction (DriverInfo)
		cmp dword ptr[eax + edx + 0x844], 2
		jne NotTraffic
		cmp dword ptr[ebx + 4], DRIVER_TYPE_TRAFFIC
		jne NotTraffic

		push 0x412C8E
		retn

		NotTraffic :
		push 0x412D75
			retn
	}
}

// StreamingTrafficCarManager::UnloadEverything: reset traffic drivers' rides only
void __declspec(naked) PlayerTrafficCarFixCodeCave_StreamingTrafficCarManager_UnloadEverything()
{
	_asm
	{
		cmp dword ptr[eax + ecx + 0x844], 2
		jne NotTraffic
		cmp dword ptr[edi + 4], DRIVER_TYPE_TRAFFIC
		jne NotTraffic

		push 0x4201AC // RideInfo = DummyRideInfo
		retn

		NotTraffic :
		push 0x4201BF
			retn
	}
}