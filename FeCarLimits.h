#pragma once
#include "includes\injector\injector.hpp"

#include <vector>
#include <algorithm>

#include "InGameFunctions.h"

#define SAVE_REGS_EDX __asm\
{\
	__asm push ebx\
	__asm push ecx\
	__asm push eax\
	__asm push edi\
	__asm push esi\
}\

#define RESTORE_REGS_EDX __asm\
{\
	__asm pop esi\
	__asm pop edi\
	__asm pop eax\
	__asm pop ecx\
	__asm pop ebx\
}\

struct FEStockCar
{
	int vTable;
	int unk1;
	unsigned int Hash;
	unsigned int PerformanceMeasure;
	int unk2;
	unsigned int Filter;
	int Type;
};

struct FETunedCar : FEStockCar
{
	unsigned int Parts[170];
	unsigned int Dyno[92];

	void InitializeFromStockCar(unsigned int hash, FEStockCar* stockCar)
	{
		static auto _InitializeFromStockCar = (void(__thiscall*)(FETunedCar*, unsigned int, FEStockCar*))0x00516B90;
		_InitializeFromStockCar(this, hash, stockCar);
	}
};

struct FEOnlineCar : FETunedCar
{
	int unk3;
};

struct FECareerCar : FETunedCar
{
	int unk3[240];
	BYTE unk4[4];

	void ReserParams()
	{
		static auto _ReserParams = (void(__thiscall*)(void*))0x00503E70;
		_ReserParams(this->unk3);
	}
};

struct FEPlayerCarDB
{
	FEStockCar StockCars[48];
	int NumStockCars;
	FETunedCar TunedCars[20];
	FECareerCar CareerCars[5];
	int NumCareerCars;
	FEOnlineCar OnlineCars[6];
	int NumOnlineCars;
	FEStockCar SponsorCars[12];
	int NumSponsorCars;
	FEStockCar* pStockCars[48];
	FETunedCar* pTunedCars[20];
	FECareerCar* pCareerCars[5];
	FEStockCar* pSponsorCars[12];
	FEOnlineCar* pOnlineCars[6];

	void DefaultStockCars()
	{
		static auto _DefaultStockCars = (void(__thiscall*)(FEPlayerCarDB*, int))0x00516500;
		_DefaultStockCars(this, 0);
	}

	void ResetPointers()
	{
		static auto _ResetPointers = (void(__thiscall*)(FEPlayerCarDB*))0x005036F0;
		_ResetPointers(this);
	}
};

auto FEPlayerCarDB_Player1 = (FEPlayerCarDB*)0x0083AD90;
auto FEPlayerCarDB_Player2 = (FEPlayerCarDB*)0x008458DC;

struct CarTypeInfo
{
	int Data[0x224];
};

CarTypeInfo*& CarTypeInfos = *(CarTypeInfo**)0x008A1CCC;

struct CombinedPointers
{
	FEStockCar* pStockCars1[48];
	FEStockCar* pStockCars2[48];
	FETunedCar* pTunedCars1[20];
	FETunedCar* pTunedCars2[20];
	FECareerCar* pCareerCars1[5];
	FECareerCar* pCareerCars2[5];
	FEStockCar* pSponsorCars1[12];
	FEStockCar* pSponsorCars2[12];
	FEOnlineCar* pOnlineCars1[6];
	FEOnlineCar* pOnlineCars2[6];
};

int StockCarStage(FEStockCar* car)
{
	if (!car || !car->unk2) return 0x7FFFFFFF; // unassigned slot, park it at the end

	char const* Name = GetCarTypeName(car->Type);
	if (!Name || !Name[0]) return -1;

	DWORD* Event = find_event_that_has_car_as_a_reward(bStringHash(Name));

	// No event awards it, so it is available from the start. That covers the starter cars and
	// any add-on nothing in the career unlocks, and both belong at the top of the list.
	if (!Event) return -1;

	return (int)Event[55];
}

std::vector<FEStockCar> ExtraStockCars;
std::vector<FEStockCar*> AllCarPointers;
FEStockCar** AllCarPointersData = nullptr;
int AllCarPointersCount = 0;

void InitCombinePointers()
{
	AllCarPointers.clear();
	AllCarPointers.reserve(182 + ExtraStockCars.size());

	std::vector<FEStockCar*> Stock;
	Stock.reserve(96 + ExtraStockCars.size());

	for (int i = 0; i < 48; i++) Stock.push_back(FEPlayerCarDB_Player1->pStockCars[i]);
	for (int i = 0; i < 48; i++) Stock.push_back(FEPlayerCarDB_Player2->pStockCars[i]);

	for (size_t i = 0; i < ExtraStockCars.size(); i++) Stock.push_back(&ExtraStockCars[i]);

	if (SortStockCarsByStage)
	{
		std::stable_sort(Stock.begin(), Stock.end(), [](FEStockCar* a, FEStockCar* b)
			{
				return StockCarStage(a) < StockCarStage(b);
			});
	}
	
	for (size_t i = 0; i < Stock.size(); i++) AllCarPointers.push_back(Stock[i]);

	for (int i = 0; i < 20; i++) AllCarPointers.push_back(FEPlayerCarDB_Player1->pTunedCars[i]);
	for (int i = 0; i < 20; i++) AllCarPointers.push_back(FEPlayerCarDB_Player2->pTunedCars[i]);
	for (int i = 0; i < 5; i++)  AllCarPointers.push_back(FEPlayerCarDB_Player1->pCareerCars[i]);
	for (int i = 0; i < 5; i++)  AllCarPointers.push_back(FEPlayerCarDB_Player2->pCareerCars[i]);
	for (int i = 0; i < 12; i++) AllCarPointers.push_back(FEPlayerCarDB_Player1->pSponsorCars[i]);
	for (int i = 0; i < 12; i++) AllCarPointers.push_back(FEPlayerCarDB_Player2->pSponsorCars[i]);
	for (int i = 0; i < 6; i++)  AllCarPointers.push_back(FEPlayerCarDB_Player1->pOnlineCars[i]);
	for (int i = 0; i < 6; i++)  AllCarPointers.push_back(FEPlayerCarDB_Player2->pOnlineCars[i]);

	AllCarPointersData = AllCarPointers.data();
	AllCarPointersCount = (int)AllCarPointers.size();
}

void __fastcall DefaultStockCars(FEPlayerCarDB* player1, int, int)
{
	player1->DefaultStockCars();

	if (player1->NumStockCars == 0x30)
	{
		int stockCars = 0;
		ExtraStockCars.clear();

		int StockCarVTable = FEPlayerCarDB_Player1->StockCars[0].vTable;

		struct Pending { unsigned int Hash; int Type; };
		std::vector<Pending> Found;

		for (int i = 0; i < CarCount; i++)
		{
			auto carTypeInfo = CarTypeInfos + i;
			
			if (!carTypeInfo->Data[0x10]) continue;          // has geometry file name?
			if ((carTypeInfo->Data[0x36] & 0x20000) == 0) continue; // WhatGame?
			if (carTypeInfo->Data[0x211] != 0) continue;     // carUsageType, 0 = player car

			stockCars++;
			if (stockCars <= 0x30) continue;                 // the game's own array holds these

			Pending e;
			e.Hash = FEngHashString("STOCK_%s", carTypeInfo);
			e.Type = i;


			Found.push_back(e);
		}

		ExtraStockCars.reserve(Found.size());

		for (size_t n = 0; n < Found.size(); n++)
		{
			FEStockCar sc;

			sc.vTable = StockCarVTable;
			sc.unk1 = 0;
			sc.Hash = Found[n].Hash;
			sc.PerformanceMeasure = 0;
			sc.unk2 = 1;
			sc.Filter = 1;
			sc.Type = Found[n].Type;

			ExtraStockCars.push_back(sc);
		}
	}

	FEPlayerCarDB_Player1->ResetPointers();
	FEPlayerCarDB_Player2->ResetPointers();
	InitCombinePointers();
}

FEStockCar* __fastcall GetCarFiltered(FEPlayerCarDB*, int, unsigned int filter, FEStockCar* past)
{
	int prev = -1;
	if (past)
	{
		for (size_t i = 0; i < AllCarPointers.size(); i++)
		{
			if (AllCarPointers[i] == past)
			{
				prev = (int)i;
			}
		}
	}

	FEStockCar* result = nullptr;
	for (size_t i = (size_t)(prev + 1); i < AllCarPointers.size(); i++)
	{
		FEStockCar* record = AllCarPointers[i];
		if (record && record->unk2 && ((record->Filter & filter) != 0))
		{
			result = record;
			break;
		}
	}

	return result;
}

FEStockCar* __fastcall GetCarRecordByHandle(FEPlayerCarDB*, int, unsigned int hash)
{
	FEStockCar* result = nullptr;
	for (size_t i = 0; i < AllCarPointers.size(); i++)
	{
		FEStockCar* record = AllCarPointers[i];
		if (record && record->unk2 && record->Hash == hash)
		{
			return record;
		}
	}

	return nullptr;
}

bool __fastcall IsCarStock(FEPlayerCarDB*, int, unsigned int hash)
{
	for (int i = 0; i < FEPlayerCarDB_Player1->NumStockCars; i++)
	{
		if (FEPlayerCarDB_Player1->StockCars[i].Hash == hash)
		{
			return true;
		}
	}

	for (int i = 0; i < FEPlayerCarDB_Player2->NumStockCars; i++)
	{
		if (FEPlayerCarDB_Player2->StockCars[i].Hash == hash)
		{
			return true;
		}
	}

	for (size_t i = 0; i < ExtraStockCars.size(); i++)
	{
		if (ExtraStockCars[i].Hash == hash) return true;
	}

	return false;
}

__declspec(naked) void IsCarStockHook()
{
	static constexpr auto hExit1 = 0x00552D8A;
	static constexpr auto hExit2 = 0x00552D6C;

	__asm
	{
		pushad;
		push ebx;
		call IsCarStock;
		test al, al;
		popad;

		je notStock;
		jmp hExit1;
	notStock:
		jmp hExit2;
	}
}

FEStockCar* __fastcall GetStockCarByCarType(FEPlayerCarDB*, int, unsigned int type)
{
	for (int i = 0; i < FEPlayerCarDB_Player1->NumStockCars; i++)
	{
		if (FEPlayerCarDB_Player1->StockCars[i].Type == type)
		{
			return FEPlayerCarDB_Player1->pStockCars[i];
		}
	}

	for (int i = 0; i < FEPlayerCarDB_Player2->NumStockCars; i++)
	{
		if (FEPlayerCarDB_Player2->StockCars[i].Type == type)
		{
			return FEPlayerCarDB_Player2->pStockCars[i];
		}
	}

	for (size_t i = 0; i < ExtraStockCars.size(); i++)
	{
		if (ExtraStockCars[i].Type == type) return &ExtraStockCars[i];
	}

	return nullptr;
}

FEStockCar* __fastcall GetStockCarByHash(FEPlayerCarDB*, int, unsigned int hash)
{
	for (int i = 0; i < FEPlayerCarDB_Player1->NumStockCars; i++)
	{
		if (FEPlayerCarDB_Player1->StockCars[i].Hash == hash)
		{
			return FEPlayerCarDB_Player1->pStockCars[i];
		}
	}

	for (int i = 0; i < FEPlayerCarDB_Player2->NumStockCars; i++)
	{
		if (FEPlayerCarDB_Player2->StockCars[i].Hash == hash)
		{
			return FEPlayerCarDB_Player2->pStockCars[i];
		}
	}

	for (size_t i = 0; i < ExtraStockCars.size(); i++)
	{
		if (ExtraStockCars[i].Hash == hash) return &ExtraStockCars[i];
	}

	return nullptr;
}

FETunedCar* __fastcall GetFreeTunedCar()
{
	for (int i = 0; i < 20; i++)
	{
		if (!FEPlayerCarDB_Player1->TunedCars[i].Hash)
		{
			return FEPlayerCarDB_Player1->pTunedCars[i];
		}
	}

	for (int i = 0; i < 20; i++)
	{
		if (!FEPlayerCarDB_Player2->TunedCars[i].Hash)
		{
			return FEPlayerCarDB_Player2->pTunedCars[i];
		}
	}

	return nullptr;
}

bool __fastcall HasFreeTunedCar()
{
	for (int i = 0; i < 20; i++)
	{
		if (!FEPlayerCarDB_Player1->TunedCars[i].Hash)
		{
			return true;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		if (!FEPlayerCarDB_Player2->TunedCars[i].Hash)
		{
			return true;
		}
	}

	return false;
}

FECareerCar* __fastcall GetCurrentCareerCar(FEPlayerCarDB*, int, unsigned int hash)
{
	for (int i = 0; i < FEPlayerCarDB_Player1->NumCareerCars; i++)
	{
		if (FEPlayerCarDB_Player1->CareerCars[i].Hash == hash)
		{
			return FEPlayerCarDB_Player1->pCareerCars[i];
		}
	}

	return nullptr;
}

void* __fastcall GetTunedCarByHandle(FEPlayerCarDB* player, int, unsigned int hash)
{
	for (int i = 0; i < 20; i++)
	{
		if (FEPlayerCarDB_Player1->TunedCars[i].Hash == hash)
		{
			return FEPlayerCarDB_Player1->pTunedCars[i];
		}

		if (FEPlayerCarDB_Player2->TunedCars[i].Hash == hash)
		{
			return FEPlayerCarDB_Player2->pTunedCars[i];
		}
	}

	return GetCurrentCareerCar(player, 0, hash);
}

unsigned int __cdecl TunedCarHash(char* str, int)
{
	int num = -1;
	for (int i = 0; i < 20; i++)
	{
		if (!FEPlayerCarDB_Player1->TunedCars[i].Hash)
		{
			num = i;
			break;
		}
	}

	if (num == -1)
	{
		for (int i = 0; i < 20; i++)
		{
			if (!FEPlayerCarDB_Player2->TunedCars[i].Hash)
			{
				num = i + 20;
				break;
			}
		}
	}

	return FEngHashString(str, num);
}

void __fastcall CreateCareerCar(FEPlayerCarDB*, int, int index, unsigned int stockHash)
{
	auto careerCar = GetCurrentCareerCar(0, 0, FEngHashString("CAREER_SLOT_%d", index));
	auto stockCar = GetStockCarByHash(0, 0, stockHash);

	careerCar->InitializeFromStockCar(careerCar->Hash, stockCar);
	careerCar->Filter = 4;
	careerCar->unk4[1] = 1;
}

void __fastcall BuyCar(void* _this, int, unsigned int hash, unsigned int stockHash)
{
	auto careerCar = GetCurrentCareerCar(0, 0, hash);
	careerCar->ReserParams();
	auto stockCar = GetStockCarByHash(0, 0, stockHash);

	careerCar->InitializeFromStockCar(careerCar->Hash, stockCar);
	careerCar->Filter = 4;
	careerCar->unk4[1] = 1;
}

__declspec(naked) void StartQuickRaceHook1()
{
	static constexpr auto hExit = 0x00525FC1;

	__asm
	{
		mov edx, [AllCarPointersData];

		jmp hExit;
	}
}

__declspec(naked) void StartQuickRaceHook2()
{
	static constexpr auto hExit = 0x00525FD3;

	__asm
	{
		add edx, 04;
		cmp eax, [AllCarPointersCount];

		jmp hExit;
	}
}

__declspec(naked) void StartQuickRaceHook3()
{
	static constexpr auto hExit = 0x00525FE2;

	__asm
	{
		mov edi, [AllCarPointersData];
		mov edi, [edi + eax * 4];

		jmp hExit;
	}
}

void InitFeCarLimits()
{
	injector::MakeCALL(0x0052A8A8, DefaultStockCars); // FEPlayerCarDB::Default
	injector::MakeNOP(0x0053471A, 12); // cFrontendDatabase::Default
	injector::MakeJMP(0x005162D0, GetCarFiltered); // FEPlayerCarDB::GetCarFiltered (23 references)
	injector::MakeJMP(0x00503510, GetCarRecordByHandle); // FEPlayerCarDB::GetCarRecordByHandle (23 references)
	injector::MakeJMP(0x00516360, IsCarStock); // FEPlayerCarDB::IsCarStock (4 references)
	injector::MakeJMP(0x00552D60, IsCarStockHook); // BeginCarCustomize
	injector::MakeJMP(0x005034C0, GetStockCarByCarType); // FEPlayerCarDB::GetStockCarByCarType
	injector::MakeJMP(0x00503550, GetStockCarByHash); // FEPlayerCarDB::GetStockCarByHandle
	injector::MakeJMP(0x005036C0, GetFreeTunedCar); // FEPlayerCarDB::FindEmptyTunedCarSlot
	injector::MakeJMP(0x005165E0, HasFreeTunedCar); // FEPlayerCarDB::AbleToAddNewTunedCar
	injector::MakeJMP(0x005035C0, GetTunedCarByHandle); // FEPlayerCarDB::GetTunedCarByHandle
	injector::MakeCALL(0x0052A75D, TunedCarHash); // FEPlayerCarDB::AddNewTunedCar
	injector::MakeCALL(0x0052A7D2, TunedCarHash); // FEPlayerCarDB::AddNewTunedCar
	injector::MakeJMP(0x00503680, GetCurrentCareerCar); // PlayerCareerState::GetCurrentCareerCar
	injector::MakeJMP(0x005348E0, CreateCareerCar); // FEPlayerCarDB::CareerCarChosen
	injector::MakeJMP(0x00496050, BuyCar); // UICareerCarLot::CommitTrade??
	injector::MakeJMP(0x00525FBB, StartQuickRaceHook1); // RaceStarter::SetupPlayerCarsAndStuff
	injector::MakeJMP(0x00525FCD, StartQuickRaceHook2);
	injector::MakeJMP(0x00525FDB, StartQuickRaceHook3);
}