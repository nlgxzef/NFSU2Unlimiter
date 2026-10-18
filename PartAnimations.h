#pragma once

#include "stdafx.h"
#include "stdio.h"
#include "InGameFunctions.h"

// Animation sets: a part that opens its own way.
//
// A car's CARS\<CAR>\PARTS_ANIMATIONS.BIN is 21 animations (0xE34010), one for each way a hood,
// trunk or door opens, and a header (0x37100) holding the car's name hash and a pivot for each of
// them. LoaderCarPartsAnimHeader (0x4381B0) makes a CAnimCarPartsScene for the header and binds
// the animations named <CAR>_ZAM_<NAME>_q/_t to it, and every question about a part's animation
// finds that scene by the car's hash. The animation and the pivot are then found by location and
// variant: GetCarPartEntity (0x433AD0) by name, GetCarPartPivotMatrix (0x433F40) by position in the
// header, 64 bytes each.
//
// The stock animations are rotation only, their translation track is zero, so the pivot is where
// the part hinges: GetCarPartAnimMatrix (0x434190) puts the pivot's position into the matrix when
// the animation has none, and GetCarPartInitAnimMatrix (0x434460) is the pivot itself.
//
// An animation set is another such block under a name of its own: its header carries the set's
// name hash and its animations are named <SET>_ZAM_<NAME>. The game loads it like the car's own
// (one scene per header, each with its own pivots, each removed by the header's unloader 0x439D70),
// so a set has its own pivots and is edited with the same tools. It can leave out the animations
// it does not change: their entities find no tracks, and those are taken from the car.
//
// A set goes in a file of its own next to the car's (see below), or appended to the car's file.
// Either way its blocks have to stay a multiple of 16 bytes long, since the chunk loaders find
// their data by aligning the chunk's address. Tools\PartAnimSet.py makes one from the car's file:
// a longer name does not fit in place in every animation, and renaming in PartAnimatorulator eats
// into the data after the name and leaves the _t track pointing at the old one.
//
// A part picks a set with a Key attribute naming it, one per location:
//
//   ANIM_HOOD, ANIM_TRUNK, ANIM_LEFT_DOOR, ANIM_RIGHT_DOOR
//   ANIM_HOOD2, ANIM_TRUNK2, ANIM_LEFT_DOOR2, ANIM_RIGHT_DOOR2
//
// The first is the set everything at that location follows. The second is for the second piece of
// the location's own part (HOOD, TRUNK, DOOR_LEFT, DOOR_RIGHT): a trunk split in two, like the split
// hoods, has ANIM_TRUNK for its first half and ANIM_TRUNK2 for the other. They are looked for on the
// location's own part, then for doors the door style part, then the body kit, and a set that is not
// loaded counts as none.
//
// Only where the animation and its pivot come from changes. The variant is still the game's choice
// (AnimateCarPart, 0x61F820: hood style, door ANIMSTYLE, widebody), so a set is used the way the car's
// own animations are, and the open and closed state stays in the car's own scene, where the menus
// that open a part trigger it and ask about it.

// Moved from Helpers.h
void DoFEPartAnim(int location, bool on, float amount)
{
	if (on && FEGetAnimationState(location))
	{
		FEDoCarPartAnimNow(location, on, amount);
	}
	if (!on && !FEGetAnimationState(location))
	{
		FEDoCarPartAnimNow(location, on, amount);
	}
}

void GetAndDoFEPartAnim(int CarSlotID, bool on, float amount)
{
	DWORD FECarConfig = *(DWORD*)_FECarConfigRef;
	int CarTypeID = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 4))(FECarConfig);
	int CarTypeNameHash = (*(int(__thiscall**)(int))(*(DWORD*)FECarConfig + 8))(FECarConfig);

	int location = CarConfigs[CarTypeID].Animations.AnimationType[CarSlotID];
	bool loaded = IsCarPartsAnimLoadedForCar(CarTypeNameHash);

	if (loaded)
	{
		if (location == 6) // Both hood and trunk
		{
			DoFEPartAnim(0, on, amount);
			DoFEPartAnim(1, on, amount);
		}
		else if (location == 5) // Both doors
		{
			DoFEPartAnim(2, on, amount);
			DoFEPartAnim(3, on, amount);
		}
		else if (location == 4) // Explode (all)
		{
			DoFEPartAnim(0, on, amount);
			DoFEPartAnim(1, on, amount);
			DoFEPartAnim(2, on, amount);
			DoFEPartAnim(3, on, amount);
		}
		else if (location >= 0 && location <= 3) // Not none
		{
			DoFEPartAnim(location, on, amount);
		}
	}
}

#define PARTANIM_LOCATIONS 4

DWORD* (*CAnimCarPartsScene_Find)(DWORD CarHash) = (DWORD * (*)(DWORD))0x433A30;
DWORD* (__thiscall* CAnimCarPartsScene_GetCarPartEntity)(DWORD* Scene, int Location, int Variant) = (DWORD * (__thiscall*)(DWORD*, int, int))0x433AD0;
void (*GetCarPartPivotMatrix)(DWORD CarHash, int Location, bMatrix4* Matrix, int Variant) = (void(*)(DWORD, int, bMatrix4*, int))0x433F40;
void (*GetCarPartAnimMatrix)(DWORD CarHash, int Location, bMatrix4* Matrix, int Piece) = (void(*)(DWORD, int, bMatrix4*, int))0x434190;
void (*GetCarPartInitAnimMatrix)(DWORD CarHash, int Location, bMatrix4* Matrix, int Piece) = (void(*)(DWORD, int, bMatrix4*, int))0x434460;

#define OffsetOfCarPartEntityAnim 0x10 // left null by the entity's Init when its tracks were not found

const DWORD PartAnim_SetAttributes[PARTANIM_LOCATIONS][2] =
{
	{ CT_bStringHash("ANIM_HOOD"),       CT_bStringHash("ANIM_HOOD2") },
	{ CT_bStringHash("ANIM_TRUNK"),      CT_bStringHash("ANIM_TRUNK2") },
	{ CT_bStringHash("ANIM_LEFT_DOOR"),  CT_bStringHash("ANIM_LEFT_DOOR2") },
	{ CT_bStringHash("ANIM_RIGHT_DOOR"), CT_bStringHash("ANIM_RIGHT_DOOR2") },
};

const int PartAnim_OwnSlots[PARTANIM_LOCATIONS] = { CARSLOTID_HOOD, CARSLOTID_TRUNK, CARSLOTID_DOOR_LEFT, CARSLOTID_DOOR_RIGHT };

// The slot and piece CarRenderInfo::Render is drawing, from its anim location code cave (0x623458).
DWORD* PartAnim_RenderCarRenderInfo = 0;
int PartAnim_RenderSlotPiece = 0; // CarSlotID * 2 + piece

// The set to take the animation and the pivot from, while the render asks for a part's matrices.
DWORD PartAnim_SetHash = 0;

DWORD PartAnim_FindSet(DWORD* RideInfo, int Location, int CarSlotID, int Piece)
{
	if (!RideInfo || Location < 0 || Location >= PARTANIM_LOCATIONS) return 0;

	int Slots[3] = { PartAnim_OwnSlots[Location], Location >= CarSlotAnimLocation::LeftDoor ? CARSLOTID_DOOR_STYLE : -1, CARSLOTID_BODY };
	bool SecondPiece = Piece == 1 && CarSlotID == PartAnim_OwnSlots[Location];

	for (int Attribute = SecondPiece ? 1 : 0; Attribute >= 0; Attribute--)
	{
		for (int s : Slots)
		{
			if (s < 0) continue;

			DWORD* Part = RideInfo_GetPart(RideInfo, s);
			if (!Part) continue;

			DWORD Set = CarPart_GetAppliedAttributeUParam(Part, PartAnim_SetAttributes[Location][Attribute], 0);
			if (Set && CAnimCarPartsScene_Find(Set)) return Set;
		}
	}

	return 0;
}

DWORD PartAnim_FindRenderSet(int Location, int Piece)
{
	DWORD* CarRenderInfo = PartAnim_RenderCarRenderInfo;
	if (!CarRenderInfo) return 0;

	return PartAnim_FindSet((DWORD*)CarRenderInfo[1], Location, PartAnim_RenderSlotPiece / 2, Piece); // RideInfo
}

// 0x6235C4 CarRenderInfo::Render
void PartAnim_GetCarPartInitAnimMatrix(DWORD CarHash, int Location, bMatrix4* Matrix, int Piece)
{
	PartAnim_SetHash = PartAnim_FindRenderSet(Location, Piece);
	GetCarPartInitAnimMatrix(CarHash, Location, Matrix, Piece);
	PartAnim_SetHash = 0;
}

// 0x6235F5 CarRenderInfo::Render
void PartAnim_GetCarPartAnimMatrix(DWORD CarHash, int Location, bMatrix4* Matrix, int Piece)
{
	PartAnim_SetHash = PartAnim_FindRenderSet(Location, Piece);
	GetCarPartAnimMatrix(CarHash, Location, Matrix, Piece);
	PartAnim_SetHash = 0;
}

// 0x434273 GetCarPartAnimMatrix, 0x434518 GetCarPartInitAnimMatrix
// Both ask for the animation before the pivot, so a set without a usable animation for this
// location and variant hands the pivot back to the car too.
DWORD* __fastcall PartAnim_GetCarPartEntity(DWORD* Scene, void* EDX_Unused, int Location, int Variant)
{
	if (PartAnim_SetHash)
	{
		DWORD* SetScene = CAnimCarPartsScene_Find(PartAnim_SetHash);
		DWORD* Entity = SetScene ? CAnimCarPartsScene_GetCarPartEntity(SetScene, Location, Variant) : 0;

		if (Entity && *(DWORD*)((BYTE*)Entity + OffsetOfCarPartEntityAnim)) return Entity;

		PartAnim_SetHash = 0;
	}

	return CAnimCarPartsScene_GetCarPartEntity(Scene, Location, Variant);
}

// 0x43441D GetCarPartAnimMatrix, 0x434531 GetCarPartInitAnimMatrix
void PartAnim_GetCarPartPivotMatrix(DWORD CarHash, int Location, bMatrix4* Matrix, int Variant)
{
	GetCarPartPivotMatrix(PartAnim_SetHash ? PartAnim_SetHash : CarHash, Location, Matrix, Variant);
}

// Sets in files of their own
//
// A set can also be a file of its own next to the car's, CARS\<CAR>\PARTS_ANIMATIONS_<ANYTHING>.BIN,
// which PartAnimatorulator opens as it opens the car's. Those are loaded right after the car's own
// file starts loading (CarLoader::LoadPartsAnim, 0x61C600) and unloaded right before it, at each of
// the four places the game unloads it: every one goes on to remove all the parts animation scenes
// left (0x438140 with 0), which would leave a set's header without its scene if the set's file
// were unloaded later. UnloadResourceFile (0x580E90) waits for a file still loading.

#define PARTANIM_MAX_SET_FILES 8

DWORD* (*LoadResourceFile)(const char* Filename, int, int, int, void(*Callback)(void*), void* Param) = (DWORD * (*)(const char*, int, int, int, void(*)(void*), void*))0x57CFB0;
void (*UnloadResourceFile)(DWORD* File) = (void(*)(DWORD*))0x580E90;

DWORD* PartAnim_SetFiles[PARTANIM_MAX_SET_FILES];
char PartAnim_SetFileNames[PARTANIM_MAX_SET_FILES][MAX_PATH]; // kept while the files load, as the car's own name is

void PartAnim_SetFileLoaded(void*)
{
}

void PartAnim_UnloadSetFiles()
{
	for (int i = 0; i < PARTANIM_MAX_SET_FILES; i++)
	{
		if (PartAnim_SetFiles[i]) UnloadResourceFile(PartAnim_SetFiles[i]);
		PartAnim_SetFiles[i] = 0;
	}
}

void PartAnim_LoadSetFiles(const char* CarFile)
{
	PartAnim_UnloadSetFiles();

	// CARS\<CAR>\ from the car's own file, looked for next to the game's exe
	const char* Slash = strrchr(CarFile, '\\');
	if (!Slash) return;
	std::string Folder(CarFile, Slash + 1 - CarFile);

	char GamePath[MAX_PATH];
	DWORD Length = GetModuleFileNameA(NULL, GamePath, MAX_PATH);
	char* GameSlash = (Length && Length < MAX_PATH) ? strrchr(GamePath, '\\') : 0;
	if (!GameSlash) return;
	GameSlash[1] = 0;

	std::string Pattern = std::string(GamePath) + Folder + "PARTS_ANIMATIONS_*.BIN";

	WIN32_FIND_DATAA Found;
	HANDLE Find = FindFirstFileA(Pattern.c_str(), &Found);
	if (Find == INVALID_HANDLE_VALUE) return;

	int Count = 0;

	do
	{
		if (Found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
		if (Folder.size() + strlen(Found.cFileName) >= MAX_PATH) continue;

		sprintf(PartAnim_SetFileNames[Count], "%s%s", Folder.c_str(), Found.cFileName);
		PartAnim_SetFiles[Count] = LoadResourceFile(PartAnim_SetFileNames[Count], 0, 0, 1, PartAnim_SetFileLoaded, 0);
		Count++;
	} while (Count < PARTANIM_MAX_SET_FILES && FindNextFileA(Find, &Found));

	FindClose(Find);
}

// 0x61C6D6 CarLoader::LoadPartsAnim
DWORD* PartAnim_LoadPartsAnimFile(const char* Filename, int a2, int a3, int a4, void(*Callback)(void*), void* Param)
{
	DWORD* File = LoadResourceFile(Filename, a2, a3, a4, Callback, Param);
	PartAnim_LoadSetFiles(Filename);
	return File;
}

// 0x61C5C2, 0x61C61F, 0x6379E5, 0x63B0F8, where the car's parts animations are unloaded
void PartAnim_UnloadPartsAnimFile(DWORD* File)
{
	PartAnim_UnloadSetFiles();
	UnloadResourceFile(File);
}

void InitPartAnimations()
{
	injector::MakeCALL(0x61C6D6, PartAnim_LoadPartsAnimFile, true); // CarLoader::LoadPartsAnim
	injector::MakeCALL(0x61C5C2, PartAnim_UnloadPartsAnimFile, true); // CarLoader, 0x61C5B0
	injector::MakeCALL(0x61C61F, PartAnim_UnloadPartsAnimFile, true); // CarLoader::LoadPartsAnim
	injector::MakeCALL(0x6379E5, PartAnim_UnloadPartsAnimFile, true);
	injector::MakeCALL(0x63B0F8, PartAnim_UnloadPartsAnimFile, true);

	injector::MakeCALL(0x6235C4, PartAnim_GetCarPartInitAnimMatrix, true); // CarRenderInfo::Render
	injector::MakeCALL(0x6235F5, PartAnim_GetCarPartAnimMatrix, true); // CarRenderInfo::Render

	injector::MakeCALL(0x434273, PartAnim_GetCarPartEntity, true); // GetCarPartAnimMatrix
	injector::MakeCALL(0x434518, PartAnim_GetCarPartEntity, true); // GetCarPartInitAnimMatrix
	injector::MakeCALL(0x43441D, PartAnim_GetCarPartPivotMatrix, true); // GetCarPartAnimMatrix
	injector::MakeCALL(0x434531, PartAnim_GetCarPartPivotMatrix, true); // GetCarPartInitAnimMatrix
}