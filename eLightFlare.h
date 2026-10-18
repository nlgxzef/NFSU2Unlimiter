#pragma once

#include "stdafx.h"
#include "stdio.h"

#include "Color.h"

enum ForceLightState : __int32
{
	FORCE_HEADLIGHTS_ON = 0x1,
	FORCE_BRAKELIGHTS_ON = 0x2,
	FORCE_REVERSELIGHTS_ON = 0x4,
	FORCE_HEADLIGHTS_OFF = 0x8,
	FORCE_BREAKLIGHTS_OFF = 0x10,
	FORCE_REVERSELIGHTS_OFF = 0x20,
};

enum VehicleFX_ID : __int32
{
	LIGHT_NONE = 0x0,
	LIGHT_LHEAD = 0x1,
	LIGHT_RHEAD = 0x2,
	LIGHT_CHEAD = 0x4,
	LIGHT_LBRAKE = 0x8,
	LIGHT_RBRAKE = 0x10,
	LIGHT_CBRAKE = 0x20,
	LIGHT_LREVERSE = 0x40,
	LIGHT_RREVERSE = 0x80,
	LIGHT_LRSIGNAL = 0x100,
	LIGHT_RRSIGNAL = 0x200,
	LIGHT_LFSIGNAL = 0x400,
	LIGHT_RFSIGNAL = 0x800,
	LIGHT_COPRED = 0x1000,
	LIGHT_COPBLUE = 0x2000,
	LIGHT_COPWHITE = 0x4000,
	LIGHT_COPS = 0x7000,
	LIGHT_LSIGNAL = 0x500,
	LIGHT_RSIGNAL = 0xA00,
	LIGHT_HEADLIGHTS = 0x7,
	LIGHT_REVERSE = 0xC0,
	LIGHT_BRAKELIGHTS = 0x38,
};

enum eLightFlareType : char // NFSU
{
	ELF_CAR_HEADLIGHT = 0x0,
	ELF_CAR_BRAKELIGHT = 0x1,
	ELF_CAR_REVERSELIGHT = 0x2,
	ELF_CAR_FOGLIGHT = 0x3,
	ELF_CAR_COPLIGHTRED = 0x4,
	ELF_CAR_COPLIGHTBLUE = 0x5,
	ELF_CAR_COPLIGHTWHITE = 0x6,
	ELF_CAR_COPHEADLIGHTRIGHT = 0x7,
	ELF_CAR_COPHEADLIGHTLEFT = 0x8,
	ELF_LAMPPOST = 0x9,
};

enum eLightFlareFlags : char // NFSU
{
	ELF_BIDIRECTIONAL = 0x1
};

struct eLightFlare : bTNode<eLightFlare>
{
	DWORD NameHash;
	Color ColourTint;
	float PositionX;
	float PositionY;
	float PositionZ;
	float ReflectPosZ;
	float DirectionX;
	float DirectionY;
	float DirectionZ;
	char Type;
	char Flags;
	short ScenerySectionNumber;

	inline operator void* () noexcept { return reinterpret_cast<void*>(this); }
};