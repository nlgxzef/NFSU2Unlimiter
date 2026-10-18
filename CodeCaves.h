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