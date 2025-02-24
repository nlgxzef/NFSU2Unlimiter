#pragma once
#include "includes\injector\injector.hpp"

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

void __fastcall DefaultStockCars(FEPlayerCarDB* player1, int, int)
{
	player1->DefaultStockCars();

	if (player1->NumStockCars == 0x30)
	{
		int stockCars = 0;
		FEPlayerCarDB_Player2->NumStockCars = 0;
		for (int i = 0; i < 0x30; i++)
		{
			auto stockCar = FEPlayerCarDB_Player2->StockCars + i;
			stockCar->Hash = 0;
			stockCar->PerformanceMeasure = 0;
			stockCar->unk1 = 0;
			stockCar->unk2 = 0;
			stockCar->Filter = 1;
		}

		for (int i = 0; i < CarCount; i++)
		{
			auto carTypeInfo = CarTypeInfos + i;
			if (carTypeInfo->Data[0x10]) // Has geometry file name?
			{
				if ((carTypeInfo->Data[0x36] & 0x20000) != 0) // WhatGame?
				{
					if (carTypeInfo->Data[0x211] == 0) // carUsageType
					{
						stockCars++;
						if (stockCars > 0x30)
						{
							auto stockCar = FEPlayerCarDB_Player2->StockCars + FEPlayerCarDB_Player2->NumStockCars++;
							stockCar->Hash = FEngHashString("STOCK_%s", carTypeInfo);
							stockCar->Filter = 1;
							stockCar->unk2 = 1;
							stockCar->unk1 = 0;
							stockCar->PerformanceMeasure = 0;
							stockCar->Type = i;
						}
					}
				}
			}
		}

		if (FEPlayerCarDB_Player2->NumStockCars > 0)
		{
			FEPlayerCarDB_Player2->ResetPointers();
		}
	}
}

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

void InitCombinePointers(FEStockCar** pointers)
{
	CombinedPointers* combined = (CombinedPointers*)pointers;

	memcpy(combined->pStockCars1, FEPlayerCarDB_Player1->pStockCars, 48 * 4);
	memcpy(combined->pStockCars2, FEPlayerCarDB_Player2->pStockCars, 48 * 4);

	memcpy(combined->pTunedCars1, FEPlayerCarDB_Player1->pTunedCars, 20 * 4);
	memcpy(combined->pTunedCars2, FEPlayerCarDB_Player2->pTunedCars, 20 * 4);

	memcpy(combined->pCareerCars1, FEPlayerCarDB_Player1->pCareerCars, 5 * 4);
	memcpy(combined->pCareerCars2, FEPlayerCarDB_Player2->pCareerCars, 5 * 4);

	memcpy(combined->pSponsorCars1, FEPlayerCarDB_Player1->pSponsorCars, 12 * 4);
	memcpy(combined->pSponsorCars2, FEPlayerCarDB_Player2->pSponsorCars, 12 * 4);

	memcpy(combined->pOnlineCars1, FEPlayerCarDB_Player1->pOnlineCars, 6 * 4);
	memcpy(combined->pOnlineCars2, FEPlayerCarDB_Player2->pOnlineCars, 6 * 4);
}

FEStockCar* __fastcall GetCarFiltered(FEPlayerCarDB*, int, unsigned int filter, FEStockCar* past)
{
	FEStockCar* pointers[182];
	InitCombinePointers(pointers);

	int prev = -1;
	if (past)
	{
		for (int i = 0; i < 182; i++)
		{
			if (pointers[i] == past)
			{
				prev = i;
			}
		}
	}

	FEStockCar* result = nullptr;
	for (int i = prev + 1; i < 182; i++)
	{
		FEStockCar* record = pointers[i];
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
	FEStockCar* pointers[182];
	InitCombinePointers(pointers);

	FEStockCar* result = nullptr;
	for (int i = 0; i < 182; i++)
	{
		FEStockCar* record = pointers[i];
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

void InitFeCarLimits()
{
	injector::MakeCALL(0x0052A8A8, DefaultStockCars);
	injector::MakeNOP(0x0053471A, 12);
	injector::MakeJMP(0x005162D0, GetCarFiltered);
	injector::MakeJMP(0x00503510, GetCarRecordByHandle);
	injector::MakeJMP(0x00516360, IsCarStock);
	injector::MakeJMP(0x00552D60, IsCarStockHook);
	injector::MakeJMP(0x005034C0, GetStockCarByCarType);
	injector::MakeJMP(0x00503550, GetStockCarByHash);
	injector::MakeJMP(0x005036C0, GetFreeTunedCar);
	injector::MakeJMP(0x005165E0, HasFreeTunedCar);
	injector::MakeJMP(0x005035C0, GetTunedCarByHandle);
	injector::MakeCALL(0x0052A75D, TunedCarHash);
	injector::MakeCALL(0x0052A7D2, TunedCarHash);
	injector::MakeJMP(0x00503680, GetCurrentCareerCar);
	injector::MakeJMP(0x005348E0, CreateCareerCar);
	injector::MakeJMP(0x00496050, BuyCar);
}