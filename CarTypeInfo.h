#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

struct CarTypeInfo
{
	char CarTypeName[32];
	char BaseModelName[32];
	char GeometryFilename[32];
	char GeometryLZCFilename[32];
	char field_0x080[64]; // "\0"
	char ManufacturerName[16];
	unsigned int CarTypeNameHash;
	float HeadlightFOV;
	char padHighPerformance;
	char NumAvailableSkinNumbers;
	char WhatGame;
	char ConvertableFlag;
	char WheelOuterRadius;
	char WheelInnerRadiusMin;
	char WheelInnerRadiusMax;
	char pad0;
	bVector3 HeadlightPosition;
	bVector3 DriverRenderingOffset;
	bVector3 InCarSteeringWheelRenderingOffset;
	CompleatCarPhysicsInfo PhysicsInfo;
	CarCameraInfo Camera;
	DWORD field_0x7F0;
	DWORD field_0x7F4;
	DWORD field_0x7F8;
	DWORD field_0x7FC;
	DWORD field_0x800;
	DWORD field_0x804;
	DWORD field_0x808;
	DWORD field_0x80C;
	DWORD field_0x810;
	DWORD field_0x814;
	DWORD field_0x818;
	DWORD field_0x81C;
	DWORD field_0x820;
	DWORD field_0x824;
	DWORD field_0x828;
	DWORD field_0x82C;
	DWORD field_0x830;
	DWORD field_0x834;
	DWORD field_0x838;
	DWORD field_0x83C;
	int Index;
	CarUsageType UsageType;
	int field_0x848;
	unsigned int DefaultBasePaint;
	unsigned int DefaultBasePaint2;
	BYTE MaxInstances[5];
	BYTE KeepLoaded[5];
	short PaddingInt16_0;
	float MinTimeBetweenUses[5];
	int DefaultSkinNumber; // maybe? based on MW and C
	int field_0x878;
	int cost;
	BYTE AvailableSkinNumbers[10]; // 1, 0, 0, ...
	short IsSUV; // 0
	int Skinnable; // 1

	unsigned int GetCarTypeNameHash()
	{
		return CarTypeNameHash;
	}

	char const *GetManufacturerName()
	{
		return ManufacturerName;
	}

	int GetNumAvailableSkins()
	{
		if (UsageType == CAR_USAGE_TYPE_COP) return 1;
		return NumAvailableSkinNumbers;
	}

	bool IsRacer()
	{
		return UsageType == CAR_USAGE_TYPE_RACING;
	}

	bool IsCop()
	{
		return UsageType == CAR_USAGE_TYPE_COP;
	}

	bool IsTraffic()
	{
		return UsageType == CAR_USAGE_TYPE_TRAFFIC;
	}

	bool GetIsSUV()
	{
		return IsSUV != 0;
	}

	bool IsSkinnable()
	{
		return Skinnable != 0;
	}

	bool SetSkinnable(bool value)
	{
		Skinnable = value ? 1 : 0;
		return IsSkinnable();
	}
};

CarTypeInfo* CarTypeInfoArray;

/*
CarTypeInfo* GetCarTypeInfo(int CarTypeID)
{
	return &CarTypeInfoArray[CarTypeID];
}
*/