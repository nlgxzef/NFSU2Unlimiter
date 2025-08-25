#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

struct LoadedSkin /* : public bTNode<LoadedSkin>*/ {
    // total size: 0x2E0
    LoadedSkin* next; // offset 0x0, size 0x4
    LoadedSkin* prev; // offset 0x4, size 0x4
    UsedCarTextureInfo SkinLayers; // offset 0x8, size 0x2FC
    DWORD*/*RideInfo**/ pRideInfo; // offset 0x304, size 0x4
    BYTE LoadStatePerm; // offset 0x308, size 0x1
    BYTE LoadStateTemp; // offset 0x30C, size 0x1
    BYTE DoneComposite; // offset 0x310, size 0x1
    BYTE IsPlayerSkin; // offset 0x314, size 0x4
    int InFrontEnd; // offset 0x318, size 0x4
    DWORD* /*LoadedTexturePack**/ pLoadedTexturesPack; // offset 0x31C, size 0x4
    DWORD* /*LoadedTexturePack**/ pLoadedVinylsPack; // offset 0x320, size 0x4
    int NumLoadedSkinLayersPerm; // offset 0x318, size 0x4
    LoadedSkinLayer* LoadedSkinLayersPerm[MaxPermTex]; // offset 0x31C, size 0x114
    int NumLoadedSkinLayersTemp; // offset 0x430, size 0x4
    LoadedSkinLayer* LoadedSkinLayersTemp[MaxTempTex]; // offset 0x434, size 0x114
};

LoadedSkin *__fastcall LoadedSkin_Create(LoadedSkin *_this, void* EDX_Unused, DWORD* ride_info, int in_front_end, int is_player_skin)  
{  
    _this->InFrontEnd = is_player_skin;
    _this->pRideInfo = ride_info;
    _this->LoadStatePerm = 0;
    _this->LoadStateTemp = 0;
    _this->DoneComposite = 0;
    GetUsedCarTextureInfo(&_this->SkinLayers, ride_info, in_front_end, is_player_skin);
    _this->pLoadedTexturesPack = 0;
    _this->pLoadedVinylsPack = 0;
    _this->NumLoadedSkinLayersPerm = 0;
    memset(_this->LoadedSkinLayersPerm, 0, sizeof(_this->LoadedSkinLayersPerm));
    _this->NumLoadedSkinLayersTemp = 0;
    memset(_this->LoadedSkinLayersTemp, 0, sizeof(_this->LoadedSkinLayersTemp));
    return _this;
}