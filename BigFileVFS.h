#pragma once

#include "stdafx.h"
#include "stdio.h"
#include <filesystem>
#include "GlobalVariables.h"

// The big file VFS (big_vfs.cpp) behind every "bf:" path into SDATA\sdat.viv keeps two singly
// linked lists in its own object: free nodes at +0x3C and open ones at +0x48. Open pops from one
// and appends to the other (0x6F6AEF, 0x6F6B14), Close does the reverse (0x6F69C8, 0x6F69DB).
//
// Neither is synchronised. The pool of wrapper nodes around them is, in sub_6F7112 and sub_6F7143,
// which take RealSystem::Mutex, but both of those run after the vtable call, so the lists are
// always edited unlocked. A trace of a normal session (three crash reports' worth of guesswork
// ended there) showed why that matters:
//
//   - 759 opens against 759 closes, and never more than 3 handles live at once
//   - three threads involved: two only open, one only closes, one does both
//   - 32001 thread switches across 87940 logged operations
//
// So the lists are edited concurrently all the time, and both crashes seen were the result:
// 0x6F6AF9 storing through a NULL that sub_6F6B56 returned because the free list head had been
// torn off, and 0x6F80B6 reading [edx] after edx went to 0 between the guard and the load in
// sub_6F807C. A pool of 64 with a peak usage of 3 came back with an empty free list, which rules
// out exhaustion and leaves corruption as the only explanation.
//
// Serialising the two vtable entries covers every list edit, since construction and destruction
// are the only other places that touch them and both are single threaded.

bool SerializeBigFileVFS = true;

CRITICAL_SECTION BigFileVFSLock;

int(__thiscall* BigFileVFS_Open_Game)(DWORD* This, char* Name, int Flags, int* Out) = (int(__thiscall*)(DWORD*, char*, int, int*))0x6F69E6;
void(__thiscall* BigFileVFS_Close_Game)(DWORD* This, int Handle) = (void(__thiscall*)(DWORD*, int))0x6F69B2;

// Bumped by the code cave below. Should stay at zero: with the lists Serialized the free list only
// empties if the game really does open more files at once than the pool holds, and the measured
// peak is 3. Anything else here means serialising missed a path, so it gets reported rather than
// quietly swallowed.
int BigFileVFSPoolEmptyCount = 0;
int BigFileVFSPoolEmptyReported = 0;

void BigFileVFSReportEmptyPool()
{
	auto Path = CurrentWorkingDirectory / "UnlimiterData" / "_BigFileVFS.txt";
	FILE* f = fopen(Path.string().c_str(), "a");

	if (!f) return;

	fprintf(f, "free list empty on open, %d time(s), pool is %d slots. Open returned -1 instead of faulting at 0x6F6AF9.\n",
		BigFileVFSPoolEmptyCount, (int)injector::ReadMemory<BYTE>(0x486541, true));

	fclose(f);
}

// Replaces 0x6F6AEC in sub_6F69E6, which is
//
//   006F6AEC  lea  ecx, [edi+3Ch]      ; the free list
//   006F6AEF  call sub_6F6B56          ; pop a node, returns NULL when the list is empty
//   006F6AF4  mov  ebx, [ebp+var_4]
//   006F6AF7  mov  esi, eax
//   006F6AF9  mov  [esi+8], ebx        ; stores through it without ever checking
//
// This is only a net under the serialisation above, not the fix. A failed open costs a sound that
// does not play; the unchecked store costs the process. 0x6F6B2B is the function's own "or eax,
// 0FFFFFFFFh" return, which every caller already handles as file not found.
void __declspec(naked) BigFileVFSOpenGuardCodeCave()
{
	_asm
	{
		lea ecx, [edi + 0x3C]
		mov eax, 0x6F6B56
		call eax
		test eax, eax
		jz PoolEmpty

		mov ebx, [ebp - 4]
		mov esi, eax
		push 0x6F6AF9
		retn

		PoolEmpty :
		inc dword ptr ds : [BigFileVFSPoolEmptyCount]
			push 0x6F6B2B
			retn
	}
}

int __fastcall BigFileVFS_Open(DWORD* This, void* EDX_Unused, char* Name, int Flags, int* Out)
{
	EnterCriticalSection(&BigFileVFSLock);
	int Handle = BigFileVFS_Open_Game(This, Name, Flags, Out);
	LeaveCriticalSection(&BigFileVFSLock);

	// Reported from here rather than from the cave so that nothing but plain asm runs on the
	// game's stack in there.
	if (BigFileVFSPoolEmptyCount != BigFileVFSPoolEmptyReported)
	{
		BigFileVFSPoolEmptyReported = BigFileVFSPoolEmptyCount;
		BigFileVFSReportEmptyPool();
	}

	return Handle;
}

void __fastcall BigFileVFS_Close(DWORD* This, void* EDX_Unused, int Handle)
{
	EnterCriticalSection(&BigFileVFSLock);
	BigFileVFS_Close_Game(This, Handle);
	LeaveCriticalSection(&BigFileVFSLock);
}

void InitBigFileVFS()
{
	injector::MakeJMP(0x6F6AEC, BigFileVFSOpenGuardCodeCave, true); // sub_6F69E6

	if (!SerializeBigFileVFS) return;

	InitializeCriticalSection(&BigFileVFSLock);

	injector::WriteMemory(0x7C2E38, &BigFileVFS_Open, true);  // vtable +0Ch, sub_6F69E6
	injector::WriteMemory(0x7C2E3C, &BigFileVFS_Close, true); // vtable +10h, sub_6F69B2
}