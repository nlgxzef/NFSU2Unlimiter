#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

struct LoadedSkinLayer /* : public bTNode<LoadedSkinLayer>*/ {
    // total size: 0x10
	LoadedSkinLayer* next; // offset 0x0, size 0x4
	LoadedSkinLayer* prev; // offset 0x4, size 0x4
    unsigned int NameHash; // offset 0x8, size 0x4
    short NumInstances; // offset 0xC, size 0x2
    char LoadState; // offset 0xE, size 0x1
    char pad0; // offset 0xF, size 0x1
};