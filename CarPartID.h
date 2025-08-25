#pragma once

#include "stdafx.h"
#include "stdio.h"

enum CAR_PART_ID : int
{
	p___INVALID = -1,
	p_BASE,
	p_FRONT_BUMPER,
	p_REAR_BUMPER,
	p_LEFT_SIDE_MIRROR,
	p_RIGHT_SIDE_MIRROR,
	p_BODY,
	p_WIDE_BODY,
	p_ROOF,
	p_TOP,
	p_HOOD,
	p_TRUNK,
	p_SKIRT,
	p_SPOILER,
	p_ENGINE,
	p_HEADLIGHT,
	p_BRAKELIGHT,
	p_DOOR_LEFT,
	p_DOOR_RIGHT,
	p_DOOR_PANEL_LEFT,
	p_DOOR_PANEL_RIGHT,
	p_DOOR_SILL_LEFT,
	p_DOOR_SILL_RIGHT,
	p_FENDER,
	p_QUARTER,
	p_HOOD_UNDER,
	p_TRUNK_UNDER,
	p_EXHAUST,
	p_BRAKE,
	p_WHEEL,
	p_SPINNER,
	p_WING_MIRROR,
	p_LICENSE_PLATE,
	p_DAMAGE_FRONT,
	p_DAMAGE_REAR,
	p_DAMAGE_LEFT,
	p_DAMAGE_RIGHT,
	p_DAMAGE_TOP,
	p_DECAL_HOOD,
	p_DECAL_FRONT_WINDOW,
	p_DECAL_REAR_WINDOW,
	p_DECAL_LEFT_DOOR,
	p_DECAL_RIGHT_DOOR,
	p_DECAL_LEFT_QUARTER,
	p_DECAL_RIGHT_QUARTER,
	p_WIDEBODY_DECAL_LEFT_DOOR,
	p_WIDEBODY_DECAL_RIGHT_DOOR,
	p_WIDEBODY_DECAL_LEFT_QUARTER,
	p_WIDEBODY_DECAL_RIGHT_QUARTER,
	p_PAINT,
	p_VINYL_PAINT,
	p_VINYL,
	p_CARBON_FIBRE,
	p_DECAL_TEXTURE,
	p_WINDOW_TINT,
	p_NEON,
	p_CABIN_NEON_FRAME,
	p_HEADLIGHT_BULB,
	p_DOOR_STYLE,
	p_HYDRAULICS,
	p_NOS_PURGE,
	p_TRUNK_AUDIO,
	p_TRUNK_AUDIO_COMP_SMALL,
	p_TRUNK_AUDIO_COMP_MEDIUM,
	p_TRUNK_AUDIO_COMP_LARGE,
	p_CUSTOM_HUD,
	p_CUSTOM_HUD_PAINT,
	p_CV,
	p_WHEEL_MANUFACTURER,
	p_MISC,
	p___NUM = 69,
};

struct CarPartIDName
{
	CAR_PART_ID CarPartID;
	char const* Name;
};

CarPartIDName* CarPartIDNames = (CarPartIDName*)0x803338;

char const* GetCarPartIDName(int CarPartID)
{
	if (CarPartID < 0 || CarPartID >= __NUM)
		return "NONE";
	return CarPartIDNames[CarPartID].Name;
}

int GetCarPartIDFromName(char const* Name)
{
	for (int i = 0; i < __NUM; ++i)
	{
		if (strcmp(CarPartIDNames[i].Name, Name) == 0)
			return CarPartIDNames[i].CarPartID;
	}
	return __INVALID;
}