#pragma once

#include "stdafx.h"
#include "stdio.h"
#include "InGameFunctions.h"
#include "GlobalVariables.h"
#include "FeCarLimits.h"

// ExtendFeCarLimits reimplements FEPlayerCarDB::GetCarFiltered (0x5162D0),
// GetCarRecordByHandle (0x503510) and the RaceStarter patch at 0x525FBB because the vanilla
// versions cannot walk its enlarged arrays. Those are the exact three addresses the preset hooks
// below take over, and whichever Init ran last silently destroyed the other's patch. So the
// fallthrough paths chain into the FeCarLimits replacements when that feature is on, instead of
// jumping back into vanilla code that would then walk a resized array.

#include <vector>
#include <algorithm>

// ---------------------------------------------------------------------------------------------
// Preset cars
//
// Ported from yugecin's nfsu2-re research:
//   BLOGx02 Customizing sponsor cars
//   BLOGx03 Customizing preset cars
//   BLOGx04 Use preset cars in quickrace
//   https://github.com/yugecin/nfsu2-re
//
// The game data contains a linked list of PresetCar entries (DDAY_PLAYER_CAR, CALEB_GTO,
// SNOOP_DOGG, the DEMO_* presets, ...). Only the sponsor subset is normally reachable, and only
// in Quick Race after typing a cheat. This exposes all of them as an extra car-select category.
//
// The original hooks were raw naked-asm patches over the C stubs. Here the dispatch logic lives
// in plain C++ and the naked stubs only do the register shuffling that cannot be expressed in C,
// matching how the rest of the Unlimiter is written.
//
// Presets are enumerated from the game's runtime PresetCar list at 0x8A31E4, which is populated
// from bin block 0x00030220 in GLOBAL\\GLOBALB.LZC. Presets added through Binary therefore show
// up here automatically, with no fixed cap on how many.
//
// Note: SpeedReflect (MaxHwoy) solves the same problem differently, by writing presets into the
// 12 vanilla sponsor car slots via a hook at 0x516402. That approach caps at 12 and only covers
// presets whose FE marker is recognised. It does not overlap with the hooks below, but running
// both at once will list the same cars twice.
// ---------------------------------------------------------------------------------------------

// Made-up bits that do not collide with INVENTORY_CAR_FLAGS (1 stock, 2 tuned, 4 career,
// 8 sponsor, 0x10 online). One per preset group, so the presets can be split into as many
// categories as there are bits: sponsor cars, boss cars, debug cars, whatever _PresetCars.ini
// says. Group 0 keeps the old value so nothing that referenced it changes meaning.
#define PARTLINK_MAX_PRESET_GROUPS 4
#define CUSTOM_IS_PRESET_CAR 0x20
#define PRESET_GROUP_BIT(g) (CUSTOM_IS_PRESET_CAR << (g))
#define PRESET_GROUP_MASK   (CUSTOM_IS_PRESET_CAR * ((1 << PARTLINK_MAX_PRESET_GROUPS) - 1))

char PresetGroupNames[PARTLINK_MAX_PRESET_GROUPS][64] =
{
	"Sponsor cars", "Boss cars", "Debug cars", "Other cars"
};

#define MENU_STATE_MAIN_MENU                       0x02
#define MENU_STATE_2P_SPLITSCREEN                  0x04
#define MENU_STATE_ONLINE_MAIN_MENU                0x08
#define MENU_STATE_CAR_CUSTOMIZE                   0x20
#define MENU_STATE_CUSTOMIZE_FROM_ONLINE_MAIN_MENU 0x100
#define MENU_STATE_LAN_MAIN_MENU                   0x200

// Customizing, from the garage or from an online lobby
#define MENU_STATES_CUSTOMIZE (MENU_STATE_CAR_CUSTOMIZE | MENU_STATE_CUSTOMIZE_FROM_ONLINE_MAIN_MENU)

// Car select outside the career: quick race, splitscreen, online and LAN
#define MENU_STATES_CARSELECT (MENU_STATE_MAIN_MENU | MENU_STATE_2P_SPLITSCREEN \
	| MENU_STATE_ONLINE_MAIN_MENU | MENU_STATE_LAN_MAIN_MENU)

#define MSG_CARSELECT_PREV 0x5073EF13
#define MSG_CARSELECT_NEXT 0xD9FEEC59

#define _carSelectCategory 0x7F444C
#define _PresetCars        0x8A31E4
#define _sponsorCarVtable  0x79AD18
#define _profileData       0x83A9D0
#define _menuCarInstanceB  0x8389D0 // scratch MenuCarInstance, overwritten later in Customize anyway

#define carSelectCategory   *(DWORD*)_carSelectCategory
#define profileMenuState    *(DWORD*)(_profileData + 0x156A8)
#define profilePlayerIndex  *(DWORD*)(_profileData + 0x20358)

// ProfileData.players[i].d4.currentCarSelectionCategory
#define SizeOfPlayer            0xAB4C
#define OffsetOfPlayers         0x10
#define OffsetOfD4              0xD4
#define OffsetOfSelectCategory  0x0C

struct ObjectLink
{
	ObjectLink* next;
	ObjectLink* prev;
};

struct PresetCar // size 0x338
{
	ObjectLink link;      // 0x00, sentinel node is PresetCars
	char modelName[32];   // 0x08, e.g. ESCALADE
	char Name[32];        // 0x28, e.g. SNOOP_DOGG
	char _pad[0x2F0];
};

struct InventoryCar // size 0x18
{
	DWORD* vtable;    // 0x00
	int field_4;      // 0x04
	DWORD slotHash;   // 0x08
	float field_C;    // 0x0C
	int field_10;     // 0x10
	DWORD flags;      // 0x14, INVENTORY_CAR_FLAGS
};

struct SponsorCarInfo // size 0x1C
{
	InventoryCar Parent;  // 0x00
	DWORD PresetCarHash;  // 0x18
};

PresetCar* (__cdecl* FindFEPresetCar)(DWORD NameHash) = (PresetCar * (__cdecl*)(DWORD))0x61C460;

int(__thiscall* UIQRCarSelect_GetNumberOfCarsForFilter)(DWORD* UIQRCarSelect, DWORD Flags)
= (int(__thiscall*)(DWORD*, DWORD))0x497EE0;
void(__thiscall* UIQRCarSelect_RebuildCarListFromFilter)(DWORD* UIQRCarSelect)
= (void(__thiscall*)(DWORD*))0x4EEC90;
void(__thiscall* UIQRCarSelect_RefreshHeader)(DWORD* UIQRCarSelect)
= (void(__thiscall*)(DWORD*))0x4B2310;
DWORD* (__thiscall* FEPlayerCarDB_AddNewTunedCar)(DWORD* FEPlayerCarDB, DWORD SlotNameHash)
= (DWORD * (__thiscall*)(DWORD*, DWORD))0x52A710;
void(__thiscall* SponsorCarInfo_BuildRide)(SponsorCarInfo* Car, int PlayerIndex, DWORD* MenuCarInstance, int Unk)
= (void(__thiscall*)(SponsorCarInfo*, int, DWORD*, int))0x5039D0;
void(__thiscall* PresetCarSlot_FillWithRide)(DWORD* PresetCarSlot, DWORD* MenuCarInstance)
= (void(__thiscall*)(DWORD*, DWORD*))0x503950;

// No fixed cap. The entries live in a vector that is sized once from the game's own PresetCar
// list, and the naked stubs index it through PresetCarsBase rather than a static array symbol.
// Only cap: slotHash is the 1-based index, and the browser skips a slotHash of 0.
// Per-preset overrides from UnlimiterData\_PresetCars.ini, keyed by the preset's own name.
struct PresetCarOverride
{
	DWORD NameHash;
	bool Enabled;
	int Group;
	int UnlockCondition;   // PRESET_UNLOCK_*
	char UnlockValue[32];  // the code as typed, for PRESET_UNLOCK_CODE
	char DisplayName[32];
};

#define PRESET_UNLOCK_NONE 0
#define PRESET_UNLOCK_CODE 1

// The game's cheat table: a pointer and a count it is happy for us to read.
#define _gEasterEggs 0x865930
#define gEasterEggs_Cheats    *(BYTE**)(_gEasterEggs + 0x0C)
#define gEasterEggs_NumCheats *(int*)(_gEasterEggs + 0x10)
#define SIZEOF_CHEATDATA 0x34

// A CheatData is name[32] at +0, hasBeenTriggered at +0x30. Comparing the typed name means a
// code works whether it is one of the game's own or one we add to the table later.
bool PresetIsCodeEntered(const char* Code)
{
	if (!Code || !Code[0]) return false;

	BYTE* Cheats = gEasterEggs_Cheats;
	int Count = gEasterEggs_NumCheats;

	if (!Cheats || Count <= 0 || Count > 256) return false;

	for (int i = 0; i < Count; i++)
	{
		BYTE* c = Cheats + i * SIZEOF_CHEATDATA;

		if (!c[0x30]) continue;                    // not triggered
		if (_stricmp((char*)c, Code) == 0) return true;
	}

	return false;
}

std::vector<PresetCarOverride> PresetCarOverrides;
bool PresetCarsOnlyListed = false;

PresetCarOverride* FindPresetCarOverride(DWORD NameHash)
{
	for (size_t i = 0; i < PresetCarOverrides.size(); i++)
		if (PresetCarOverrides[i].NameHash == NameHash) return &PresetCarOverrides[i];

	return nullptr;
}

std::vector<SponsorCarInfo> PresetCarsAsInventoryCars;
SponsorCarInfo* PresetCarsBase = nullptr;
int NumPresetCars = 0;

// Builds fake SponsorCar entries over the PresetCar list. Runs once, on first use, because the
// preset list is not populated yet when the Unlimiter initialises.
void BuildPresetCarList()
{
	if (NumPresetCars) return;

	ObjectLink* Sentinel = (ObjectLink*)_PresetCars;
	if (!Sentinel->next) return; // list not loaded yet

	struct Pending { DWORD Hash; int Group; };
	std::vector<Pending> Entries;

	// Order is the order of UnlimiterData\_PresetCars.ini. Listed presets first, in the order
	// they appear in the file, then anything the file does not mention. Moving a line moves the
	// car, which beats guessing at sort numbers.
	for (size_t o = 0; o < PresetCarOverrides.size(); o++)
	{
		PresetCarOverride& ov = PresetCarOverrides[o];

		if (!ov.Enabled) continue;
		if (ov.UnlockCondition == PRESET_UNLOCK_CODE && !PresetIsCodeEntered(ov.UnlockValue)) continue;

		for (ObjectLink* i = Sentinel->next; i && i != Sentinel; i = i->next)
		{
			if (FEHashUpper(((PresetCar*)i)->Name) != ov.NameHash) continue;

			Pending e;
			e.Hash = ov.NameHash;
			e.Group = (ov.Group >= 0 && ov.Group < PARTLINK_MAX_PRESET_GROUPS) ? ov.Group : 0;
			Entries.push_back(e);
			break;
		}
	}

	if (!PresetCarsOnlyListed)
	{
		for (ObjectLink* i = Sentinel->next; i && i != Sentinel; i = i->next)
		{
			DWORD Hash = FEHashUpper(((PresetCar*)i)->Name);
			if (FindPresetCarOverride(Hash)) continue; // already placed above, or deliberately out

			Pending e;
			e.Hash = Hash;
			e.Group = 0;
			Entries.push_back(e);
		}
	}

	if (Entries.empty()) return;


	PresetCarsAsInventoryCars.clear();
	PresetCarsAsInventoryCars.reserve(Entries.size());

	for (size_t n = 0; n < Entries.size(); n++)
	{
		SponsorCarInfo s;

		s.Parent.vtable = (DWORD*)_sponsorCarVtable;
		s.Parent.field_4 = 0;
		// +1 because entries with a slotHash of 0 get skipped by the car browser
		s.Parent.slotHash = (DWORD)n + 1;
		s.Parent.field_C = .0f;
		s.Parent.field_10 = 0;
		s.Parent.flags = PRESET_GROUP_BIT(Entries[n].Group);
		s.PresetCarHash = Entries[n].Hash;

		PresetCarsAsInventoryCars.push_back(s);
	}

	// The vector never grows again after this point, so the base pointer stays valid
	PresetCarsBase = PresetCarsAsInventoryCars.data();
	NumPresetCars = (int)PresetCarsAsInventoryCars.size();
}

int CountPresetCars()
{
	BuildPresetCarList();
	return NumPresetCars;
}

void SetCarSelectCategory(DWORD NewCategory)
{
	carSelectCategory = NewCategory;

	*(DWORD*)(_profileData + OffsetOfPlayers + profilePlayerIndex * SizeOfPlayer
		+ OffsetOfD4 + OffsetOfSelectCategory) = NewCategory;
}

// Returns 1 if the category rotation was handled here, 0 to fall through to the game's own code.
int PresetGroupCount(DWORD Flags); // defined further down

// The rotation used to be two mirrored switch statements with a fall-through chain, which cannot
// grow past one preset category. It is now an ordered list built per call from whatever actually
// has cars in it, so adding a group needs no code.
int PresetBuildCategoryOrder(DWORD* UIQRCarSelect, bool InQuickRace, DWORD* Out)
{
	int n = 0;

	Out[n++] = 1 | 2; // stock and tuned together, the screen always opens on this
	Out[n++] = 1;     // IS_STOCK_CAR

	if (UIQRCarSelect_GetNumberOfCarsForFilter(UIQRCarSelect, 2)) Out[n++] = 2; // tuned
	if (InQuickRace && UIQRCarSelect_GetNumberOfCarsForFilter(UIQRCarSelect, 4)) Out[n++] = 4; // career

	// The game's own SPONSOR CARS category is left out on purpose: the sponsor cars are part of
	// the preset list, so keeping it would show every one of them twice.
	for (int g = 0; g < PARTLINK_MAX_PRESET_GROUPS; g++)
		if (PresetGroupCount(PRESET_GROUP_BIT(g))) Out[n++] = PRESET_GROUP_BIT(g);

	return n;
}

// Returns 1 if the rotation was handled here, 0 to fall through to the game's own code.
int __fastcall UIQRCarSelect_ScrollLists(DWORD* UIQRCarSelect, void* EDX_Unused, DWORD Message)
{
	DWORD MenuState = profileMenuState;

	bool InCustomize = (MenuState & MENU_STATES_CUSTOMIZE) && PresetCarsInCustomize;
	bool InQuickRace = (MenuState & MENU_STATES_CARSELECT) && PresetCarsInQuickRace;

	if (!InCustomize && !InQuickRace) return 0;

	DWORD Order[8 + PARTLINK_MAX_PRESET_GROUPS];
	int Count = PresetBuildCategoryOrder(UIQRCarSelect, InQuickRace, Order);

	if (Count < 2) return 0;

	DWORD Current = carSelectCategory;
	int At = 0;

	for (int i = 0; i < Count; i++)
		if (Order[i] == Current) { At = i; break; }

	if (Message == MSG_CARSELECT_NEXT) At = (At + 1) % Count;
	else if (Message == MSG_CARSELECT_PREV) At = (At + Count - 1) % Count;
	else return 0;

	SetCarSelectCategory(Order[At]);
	UIQRCarSelect_RebuildCarListFromFilter(UIQRCarSelect);
	UIQRCarSelect_RefreshHeader(UIQRCarSelect);

	return 1;
}

// 0x4EED10 UIQRCarSelect::ScrollLists
void __declspec(naked) UIQRCarSelect_ScrollListsCodeCave()
{
	_asm
	{
		push ecx                                   // preserve this
		push dword ptr[esp + 8]                    // message
		mov ecx, [esp + 4]                         // this
		call UIQRCarSelect_ScrollLists     // __fastcall: ecx = this, stack = message
		pop ecx                                    // restore this
		test eax, eax
		jnz Handled

		// Not our category rotation: replay the original prologue and jump back in.
		// The overwritten instruction was a 5 byte 'mov eax, ds:[0x7F444C]'.
		mov eax, _carSelectCategory                // immediate
		mov eax, [eax]
		mov edx, 0x4EED15                          // edx is clobbered later in that proc anyway
		jmp edx

		Handled :
		retn 0x4
	}
}

// 0x534850 FEPlayerCarDBWithPointers::CountAvailableCars
int PresetGroupCount(DWORD Flags)
{
	BuildPresetCarList();

	int n = 0;

	for (int i = 0; i < NumPresetCars; i++)
		if (PresetCarsBase[i].Parent.flags & Flags) n++;

	return n;
}

// 0x534850 FEPlayerCarDBWithPointers::CountAvailableCars
int __stdcall CountAvailablePresetCars(DWORD Flags)
{
	// Outside the customize menu the category must not stick around unless quick race support is
	// on, otherwise the player can carry it into a menu that cannot handle it.
	DWORD MenuState = profileMenuState;

	if (MenuState & MENU_STATES_CUSTOMIZE)
	{
		if (!PresetCarsInCustomize) return 0;
	}
	else if (MenuState & MENU_STATES_CARSELECT)
	{
		if (!PresetCarsInQuickRace) return 0;
	}
	else return 0;

	return PresetGroupCount(Flags);
}

void __declspec(naked) FEPlayerCarDB_CountAvailableCarsCodeCave()
{
	_asm
	{
		test dword ptr[esp + 4], PRESET_GROUP_MASK
		jnz ItsPreset

		// Original prologue, then jump back
		sub esp, 8
		push ebx
		mov ebx, [esp + 0xC + 8]
		mov eax, 0x534858
		jmp eax

		ItsPreset :
		push dword ptr[esp + 4]        // the group flags
			call CountAvailablePresetCars  // __stdcall, cleans its own argument
			retn 8
	}
}

// 0x5162D0 FEPlayerCarDBWithPointers::FindCarWithFlagAfterGivenCar
InventoryCar* __stdcall FindPresetCarAfterGivenCar(DWORD FlagsToCheck, InventoryCar* GivenCar)
{
	BuildPresetCarList();

	if (!NumPresetCars) return nullptr;

	// Walk only the group being browsed. The entries of every group live in one array, so this is
	// the filter that keeps the categories apart.
	int Start = 0;

	if (GivenCar)
	{
		Start = -1;

		for (int i = 0; i < NumPresetCars; i++)
			if ((InventoryCar*)&PresetCarsBase[i] == GivenCar) { Start = i + 1; break; }

		if (Start < 0) return nullptr;
	}

	for (int i = Start; i < NumPresetCars; i++)
		if (PresetCarsBase[i].Parent.flags & FlagsToCheck) return &PresetCarsBase[i].Parent;

	return nullptr;
}

void __declspec(naked) FEPlayerCarDB_FindCarWithFlagAfterGivenCarCodeCave()
{
	_asm
	{
		test dword ptr[esp + 4], PRESET_GROUP_MASK
		jnz ItsPreset

		cmp byte ptr[ExtendFeCarLimits], 0
		jne ChainFe

		// Original prologue, then jump back
		push ebx
		push esi
		push edi
		mov edi, [esp + 0x14]
		mov eax, 0x5162D7
		jmp eax

		ChainFe :
		jmp GetCarFiltered

			ItsPreset :
		jmp FindPresetCarAfterGivenCar
	}
}

// 0x503510 FEPlayerCarDBWithPointers::GetCarForSlot
void __declspec(naked) FEPlayerCarDB_GetCarForSlotCodeCave()
{
	_asm
	{
		// Slot hash 0 means "no car in this slot" and turns up constantly while the collection
		// is walked. The old check was a bare jbe, so 0 fell into the preset path, dec'd to -1
		// and returned PresetCarsBase - 0x1C instead of null. Needs a lower bound and a null
		// base check; flags-only comparisons so nothing is clobbered on the fallthrough.
		cmp dword ptr[esp + 4], 0
		je NotPreset
		mov eax, [NumPresetCars]
		cmp dword ptr[esp + 4], eax
		ja NotPreset
		cmp dword ptr[PresetCarsBase], 0
		je NotPreset

		mov eax, [esp + 4]
		dec eax
		imul eax, 0x1C                 // sizeof(SponsorCar)
		add eax, [PresetCarsBase]
		retn 4

		NotPreset :
		cmp byte ptr[ExtendFeCarLimits], 0
			jne ChainFe

			// Original prologue, then jump back
			push esi
			push edi
			mov edi, [esp + 0xC]
			mov eax, 0x503516
			jmp eax

			ChainFe :
		// __fastcall(this in ecx, unused edx, hash on the stack) matches this entry state
		jmp GetCarRecordByHandle
	}
}

// The car lookup family, 0x503510 / 0x503550 / 0x5035C0 / 0x503680
//
// CustomizeCar asks several of these in turn for a car matching the slot hash before it reaches
// the patch below. Our entries carry a 1 based index as their slot hash, which matches nothing,
// and the vanilla lookups do not return null for it: they walk into the second player's
// collection from a null base. That is the 0xA118 fault in GetTunedCarByHandle and the one in
// GetCurrentCareerCar right after it, each showing the preset number in ebx.
//
// Null is the answer CustomizeCar already knows how to handle: the patch below sees esi == 0 and
// builds the tuned car from the preset. So every one of them answers null for our range and hands
// everything else to the original, which stays in place because the call sites are redirected
// rather than the function overwritten. That also composes with ExtendFeCarLimits, whose own
// replacements are JMPs at the start of the same functions.

bool PresetOwnsSlotHash(DWORD SlotHash)
{
	BuildPresetCarList();

	// Unsigned. A real slot hash like 2509970966 does not fit in a signed int, so casting it made
	// it negative and every genuine car looked like preset number "less than zero", which is why
	// ordinary cars started faulting: this answered null for all of them.
	return SlotHash != 0 && SlotHash <= (DWORD)NumPresetCars;
}

typedef void* (__thiscall* CarLookupFn)(DWORD*, DWORD);

CarLookupFn GameGetStockCarByHash = (CarLookupFn)0x503550;
CarLookupFn GameGetTunedCarByHandle = (CarLookupFn)0x5035C0;
CarLookupFn GameGetCurrentCareerCar = (CarLookupFn)0x503680;

// 0x503640, the online car lookup. Not in FeCarLimits' list, which is why it took a disassembly
// to find: GetTunedCarByHandle ends at 0x50362F and this starts after the alignment, so the
// 0x503650 in every crash log was never inside the function I thought it was.
//
//   lea esi, [ecx+8138h]      base
//   mov edx, [esi]            0x503650, the faulting read
//   add esi, 438h ; cmp eax,6 six entries of 0x438, the onlineCars array
//
// Called from BeginCarCustomize at 0x552DAA, which is the 0x552DAF return address in every log.
// It answers null for a miss anyway, so returning null for our range is its own normal path.
CarLookupFn GameGetOnlineCarByHandle = (CarLookupFn)0x503640;

void* __fastcall PresetGetStockCarByHash(DWORD* Db, void* EDX_Unused, DWORD SlotHash)
{
	bool Ours = PresetOwnsSlotHash(SlotHash);


	return Ours ? nullptr : GameGetStockCarByHash(Db, SlotHash);
}

void* __fastcall PresetGetTunedCarByHandle(DWORD* Db, void* EDX_Unused, DWORD SlotHash)
{
	bool Ours = PresetOwnsSlotHash(SlotHash);


	return Ours ? nullptr : GameGetTunedCarByHandle(Db, SlotHash);
}

void* __fastcall PresetGetOnlineCarByHandle(DWORD* Db, void* EDX_Unused, DWORD SlotHash)
{
	bool Ours = PresetOwnsSlotHash(SlotHash);


	return Ours ? nullptr : GameGetOnlineCarByHandle(Db, SlotHash);
}

void* __fastcall PresetGetCurrentCareerCar(DWORD* Db, void* EDX_Unused, DWORD SlotHash)
{
	bool Ours = PresetOwnsSlotHash(SlotHash);


	return Ours ? nullptr : GameGetCurrentCareerCar(Db, SlotHash);
}

// Redirects every 5 byte relative call landing exactly on Target, leaving the target intact.
template<class T>
int PresetRedirectCallsTo(DWORD Target, T Replacement)
{
	const DWORD ScanStart = 0x401000;
	const DWORD ScanEnd = 0x7C0000;

	int Patched = 0;

	for (DWORD a = ScanStart; a < ScanEnd - 5; a++)
	{
		if (*(BYTE*)a != 0xE8) continue;
		if ((DWORD)(a + 5 + *(int*)(a + 1)) != Target) continue;

		injector::MakeCALL(a, Replacement, true);
		Patched++;
	}

	return Patched;
}

// 0x552DBB inside CustomizeCar: no TunedCar instance was found for the slot hash, so build one
// from the preset and copy its tuning in.
// The player 1 car collection. Every lookup in the game that works passes this; the ecx captured
// at the 0x552DBB patch site is something else, and a crash log caught it as 0x001AFF64, a stack
// address, which then made CreateNewTunedCarFromDataAtSlot walk the collection from there.
#define _FEPlayerCarDBPlayer1 0x0083AD90

DWORD* __stdcall CreateTunedCarFromPresetCar(DWORD* FEPlayerCarDBFromEcx, DWORD SlotNameHash)
{
	DWORD* FEPlayerCarDB = (DWORD*)_FEPlayerCarDBPlayer1;

	char Buf[64];

	BuildPresetCarList();

	if (SlotNameHash == 0 || SlotNameHash > (DWORD)NumPresetCars || !PresetCarsBase) return nullptr;

	SponsorCarInfo* sp = &PresetCarsBase[SlotNameHash - 1];

	PresetCar* Preset = FindFEPresetCar(sp->PresetCarHash);
	if (!Preset) return nullptr;

	// A preset whose model name is empty or not text means the PresetCar we resolved is not one,
	// and hashing it would hand CreateNewTunedCarFromDataAtSlot a hash matching no stock car.
	if (!Preset->modelName[0]) return nullptr;

	for (int c = 0; c < 31; c++)
	{
		char ch = Preset->modelName[c];
		if (ch == 0) break;
		if (ch < 0x20 || ch > 0x7E) return nullptr;
	}

	sprintf(Buf, "STOCK_%s", Preset->modelName);

	DWORD* TunedCar = FEPlayerCarDB_AddNewTunedCar(FEPlayerCarDB, bStringHash(Buf));
	if (!TunedCar) return nullptr;

	DWORD* MenuCarInstance = (DWORD*)_menuCarInstanceB;

	SponsorCarInfo_BuildRide(sp, profilePlayerIndex, MenuCarInstance, 0);
	PresetCarSlot_FillWithRide(TunedCar + (0x18 / 4), MenuCarInstance);

	return TunedCar;
}

void __declspec(naked) BeginCarCustomize_SetCarInstanceIfMissingCodeCave()
{
	_asm
	{
		test esi, esi
		jnz AllGood

		// No car instance: assume a preset car was picked
		push ebx                       // slotNameHash
		push ecx                       // this (FEPlayerCarDB)
		call CreateTunedCarFromPresetCar
		test eax, eax
		jz AllGood                     // not one of ours, let the game fail the way it always did
		mov esi, eax
		mov byte ptr[ebp + 8 + 3], 1   // same flag the game sets when creating a tuned car from stock

		AllGood :
		mov cl, byte ptr[ebp + 8 + 3]  // overwrote this
			push 0                     // overwrote this
			mov eax, 0x552DC0
			jmp eax
	}
}

// 0x525FBB: going in-game, the player car collection is searched again by slot hash
void __declspec(naked) FindPresetCarWhenTuningForIngameCarCodeCave()
{
	_asm
	{
		// Same lower bound problem as GetCarForSlot above
		test edi, edi
		jz NotPreset
		cmp edi, [NumPresetCars]
		ja NotPreset
		cmp dword ptr[PresetCarsBase], 0
		je NotPreset

		dec edi
		imul edi, 0x1C                 // sizeof(SponsorCar)
		add edi, [PresetCarsBase]
		mov eax, 0x525FE6
		jmp eax

		NotPreset :
		cmp byte ptr[ExtendFeCarLimits], 0
			jne ChainFe

			// Original instruction, then jump back
			add edx, 0x9BD8                // sizeof(FEPlayerCarDB)
			mov ecx, 0x525FC1
			jmp ecx

			ChainFe :
		jmp StartQuickRaceHook1
	}
}

// 0x4B2855 tail of UIQRCarSelect::UpdateUI. The game only knows its own categories and
// falls back to the "Stock cars" label for anything else, so relabel and show the preset name.
#define hashof_carselect_category_label 0x3E81DE59
#define hashof_OL_CarMode_Group         0x2043ABA0
#define hashof_racemode                 0x6578FB3F
#define hashof_racemodevalue            0xA9205BD8

#define OffsetOfCurrentSelectedCar 0x58 // UIQRCarSelect.currentSelectedCar
#define OffsetOfEntrySlotHash      0x920
#define OffsetOfUIElementPos       0x2C
#define OffsetOfUIPosLeftOffset    0x1C

void __stdcall UIQRCarSelect_PostRefreshHeader(DWORD* UIQRCarSelect)
{
	if ((carSelectCategory & PRESET_GROUP_MASK) == 0)
	{
		FEngSetInvisible_Pkg("UI_QRCarSelect.fng", hashof_OL_CarMode_Group);
		return;
	}

	int CategoryGroup = 0;

	for (int g = 0; g < PARTLINK_MAX_PRESET_GROUPS; g++)
		if (carSelectCategory == PRESET_GROUP_BIT(g)) { CategoryGroup = g; break; }

	FEPrintf("UI_QRCarSelect.fng", hashof_carselect_category_label, "%s", PresetGroupNames[CategoryGroup]);

	// Reuse the online ranked-car "race mode" label group to show the preset name
	DWORD* Group = (DWORD*)FEngFindObject("UI_QRCarSelect.fng", hashof_OL_CarMode_Group);
	if (Group)
	{
		FEngSetVisible(Group);
		// Clearing the hidden flag is not enough, the group also has a HIDE script running
		FEngSetScript_Obj(Group, "SHOW", 0);
	}

	FEngSetInvisible_Pkg("UI_QRCarSelect.fng", hashof_racemodevalue);

	DWORD* SelectedEntry = (DWORD*)*(DWORD*)((BYTE*)UIQRCarSelect + OffsetOfCurrentSelectedCar);
	if (!SelectedEntry) return;

	DWORD SlotHash = *(DWORD*)((BYTE*)SelectedEntry + OffsetOfEntrySlotHash);
	if (SlotHash == 0 || SlotHash > (DWORD)NumPresetCars || !PresetCarsBase) return;

	DWORD PresetHash = PresetCarsBase[SlotHash - 1].PresetCarHash;

	PresetCar* Preset = FindFEPresetCar(PresetHash);
	if (!Preset) return;

	FEPrintf("UI_QRCarSelect.fng", hashof_racemode, "%s", Preset->Name);


	DWORD* Label = (DWORD*)FEngFindObject("UI_QRCarSelect.fng", hashof_racemode);
	if (Label)
	{
		BYTE* Pos = *(BYTE**)((BYTE*)Label + OffsetOfUIElementPos);
		if (Pos) *(float*)(Pos + OffsetOfUIPosLeftOffset) = 85.0f; // the label text is right aligned
	}
}

void __declspec(naked) UIQRCarSelect_RefreshHeaderCodeCave()
{
	_asm
	{
		pushad
		push ecx
		call UIQRCarSelect_PostRefreshHeader
		popad
		// We overwrote a jmp, so just do it
		mov eax, 0x497CD0
		jmp eax
	}
}

// 0x579D70 EasterEggs::IsPresetUnlocked
int __stdcall IsPresetUnlocked_AlwaysTrue(DWORD* SponsorCarHash)
{
	return 1;
}

void InitPresetCars()
{
	if (UnlockSponsorCarsWithoutCheats)
	{
		injector::MakeJMP(0x579D70, IsPresetUnlocked_AlwaysTrue, true); // EasterEggs::IsPresetUnlocked
	}

	if (!PresetCarsInCustomize && !PresetCarsInQuickRace) return;

	injector::MakeJMP(0x4EED10, UIQRCarSelect_ScrollListsCodeCave, true);              // UIQRCarSelect::ChangeCategory
	injector::MakeJMP(0x4B2855, UIQRCarSelect_RefreshHeaderCodeCave, true);                    // UIQRCarSelect::UpdateUI (tail)
	injector::MakeJMP(0x534850, FEPlayerCarDB_CountAvailableCarsCodeCave, true);               // FEPlayerCarDBWithPointers::CountAvailableCars
	injector::MakeJMP(0x5162D0, FEPlayerCarDB_FindCarWithFlagAfterGivenCarCodeCave, true);     // FEPlayerCarDBWithPointers::FindCarWithFlagAfterGivenCar
	injector::MakeJMP(0x503510, FEPlayerCarDB_GetCarForSlotCodeCave, true);                    // FEPlayerCarDBWithPointers::GetCarForSlot

	if (PresetCarsInCustomize)
	{
		// Must come before the 0x552DBB patch below can ever run
		PresetRedirectCallsTo(0x503550, PresetGetStockCarByHash);
		PresetRedirectCallsTo(0x5035C0, PresetGetTunedCarByHandle);
		PresetRedirectCallsTo(0x503680, PresetGetCurrentCareerCar);
		PresetRedirectCallsTo(0x503640, PresetGetOnlineCarByHandle);

		// Not a byte overlap with FeCarLimits, but 0x552D60 (IsCarStockHook) redirects the start
		// of the same function, so this patch may simply never be reached with that feature on.
		injector::MakeJMP(0x552DBB, BeginCarCustomize_SetCarInstanceIfMissingCodeCave, true);       // CustomizeCar
	}

	if (PresetCarsInQuickRace)
	{
		injector::MakeJMP(0x525FBB, FindPresetCarWhenTuningForIngameCarCodeCave, true);        // RaceStarter
	}
}