#pragma once

struct TableBase
{
	int NumEntries;
	float MinArg;
	float MaxArg;
	float IndexMultiplier;
};

struct Table
{
	TableBase __base;
	float* pTable;
};
