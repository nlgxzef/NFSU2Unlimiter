#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

int(__thiscall* CarLoader_LoadSkinLayers)(DWORD* CarLoader, DWORD* name_hash_table, int max_name_hashes, LoadedSkinLayer** loaded_skin_layer_table, int num_loaded_skin_layers) = (int(__thiscall*)(DWORD*, DWORD*, int, LoadedSkinLayer**, int))0x6108A0;
int(__thiscall* CarLoader_LoadedSkinLayers)(DWORD* CarLoader, LoadedSkinLayer** loaded_skin_layer_table, int num_loaded_skin_layers) = (int(__thiscall*)(DWORD*, LoadedSkinLayer**, int))0x6108E0;
int(__thiscall* CarLoader_UnloadSkinLayers)(DWORD* CarLoader, DWORD* name_hash_table, int max_name_hashes, LoadedSkinLayer** loaded_skin_layer_table, int num_loaded_skin_layers) = (int(__thiscall*)(DWORD*, DWORD*, int, LoadedSkinLayer**, int))0x637C10;
int(__thiscall* CarLoader_RemoveSomethingFromCarMemoryPool)(DWORD* CarLoader, bool force_unload) = (int(__thiscall*)(DWORD*, bool))0x63A1B0;
int(__thiscall* CarLoader_UnloadTexturePack)(DWORD* CarLoader, DWORD* loaded_texture_pack) = (int(__thiscall*)(DWORD*, DWORD*))0x637B50;
int(__thiscall* CarLoader_ServiceLoading)(DWORD* CarLoader) = (int(__thiscall*)(DWORD*))0x63B160;
void(__cdecl* CarLoader_LoadedSkinCallbackBridge)(unsigned int param) = (void(__cdecl*)(unsigned int))0x61CB40;

int __fastcall CarLoader_LoadSkin(DWORD* CarLoader, void* EDX_Unused, LoadedSkin* loaded_skin, int load_perm_layers)
{
    int SkinLayers; // esi
    DWORD v5; // eax
    int memory_pool_num; // bp
    DWORD name_hash_table[MaxPermTex]; // [esp+Ch] [ebp-114h] BYREF

    if (load_perm_layers)
    {
        SkinLayers = CarLoader_LoadSkinLayers(CarLoader, name_hash_table, MaxPermTex, loaded_skin->LoadedSkinLayersPerm, loaded_skin->NumLoadedSkinLayersPerm);
        loaded_skin->LoadStatePerm = (SkinLayers == 0) + 1;
    }
    else
    {
        SkinLayers = CarLoader_LoadSkinLayers(CarLoader, name_hash_table, MaxTempTex, loaded_skin->LoadedSkinLayersTemp, loaded_skin->NumLoadedSkinLayersTemp);
        loaded_skin->LoadStateTemp = (SkinLayers == 0) + 1;
    }
    if (SkinLayers <= 0)
        return 0;
    if (load_perm_layers || (v5 = CarLoader[2], v5 == 2)) // LoadingMode
    {
        memory_pool_num = 4;
        if (eStreamPackLoader_TestLoadStreamingEntry((DWORD*)StreamingTexturePackLoader, name_hash_table, SkinLayers, 4))
        {
            while (CarLoader_RemoveSomethingFromCarMemoryPool(CarLoader, 1)
                && eStreamPackLoader_TestLoadStreamingEntry((DWORD*)StreamingTexturePackLoader, name_hash_table, SkinLayers, 4))
                ;
        }
    }
    else
    {
        memory_pool_num = v5 == 1;
    }
    CarLoader[5] = 1;
    eLoadStreamingTexture(name_hash_table, SkinLayers, CarLoader_LoadedSkinCallbackBridge, (int)loaded_skin, memory_pool_num);
    return 1;
}

int __fastcall CarLoader_UnloadSkinTemporaries(DWORD* CarLoader, void *EDX_Unused, LoadedSkin* loaded_skin, int force_unload)
{
    int unloaded_something; // edi
    LoadedSkinLayer** LoadedSkinLayersTemp;
    int NumLoadedSkinLayersTemp;
    int num_name_hashes; // eax
    DWORD *pLoadedVinylsPack; // eax
    DWORD name_hash_table[MaxTempTex]; // [esp+8h] [ebp-114h] BYREF

    if ((loaded_skin->LoadStateTemp != 2 || !loaded_skin->DoneComposite) && !force_unload)
        return 0;
    unloaded_something = 0;
    if (loaded_skin->NumLoadedSkinLayersTemp > 0)
    {
        LoadedSkinLayersTemp = loaded_skin->LoadedSkinLayersTemp;
        NumLoadedSkinLayersTemp = loaded_skin->NumLoadedSkinLayersTemp;
        do
        {
            if (*LoadedSkinLayersTemp)
                --(*LoadedSkinLayersTemp)->NumInstances;
            ++LoadedSkinLayersTemp;
            --NumLoadedSkinLayersTemp;
        } while (NumLoadedSkinLayersTemp);
    }
    num_name_hashes = CarLoader_UnloadSkinLayers(CarLoader, name_hash_table, MaxTempTex, loaded_skin->LoadedSkinLayersTemp, loaded_skin->NumLoadedSkinLayersTemp);
    loaded_skin->NumLoadedSkinLayersTemp = 0;
    if (num_name_hashes)
    {
        eUnloadStreamingTexture(name_hash_table, num_name_hashes);
        unloaded_something = 1;
    }
    pLoadedVinylsPack = loaded_skin->pLoadedVinylsPack;
    if (pLoadedVinylsPack)
    {
        --*((WORD*)pLoadedVinylsPack + 40);
        CarLoader_UnloadTexturePack(CarLoader, loaded_skin->pLoadedVinylsPack);
        loaded_skin->pLoadedVinylsPack = 0;
        ++unloaded_something;
    }
    return unloaded_something;
}

int __fastcall CarLoader_UnloadSkinPerms(DWORD *CarLoader, void *EDX_Unused, LoadedSkin* loaded_skin)
{
    LoadedSkinLayer** LoadedSkinLayersPerm;
    int NumLoadedSkinLayersPerm; // edi
    int num_name_hashes; // eax
    DWORD name_hash_table[MaxPermTex]; // [esp+4h] [ebp-114h] BYREF

    if (loaded_skin->NumLoadedSkinLayersPerm > 0)
    {
        LoadedSkinLayersPerm = loaded_skin->LoadedSkinLayersPerm;
        NumLoadedSkinLayersPerm = loaded_skin->NumLoadedSkinLayersPerm;
        do
        {
            if (*LoadedSkinLayersPerm)
                --(*LoadedSkinLayersPerm)->NumInstances;
            ++LoadedSkinLayersPerm;
            --NumLoadedSkinLayersPerm;
        } while (NumLoadedSkinLayersPerm);
    }
    num_name_hashes = CarLoader_UnloadSkinLayers(CarLoader, name_hash_table, MaxPermTex, loaded_skin->LoadedSkinLayersPerm, loaded_skin->NumLoadedSkinLayersPerm);
    if (!num_name_hashes)
        return 0;
    eUnloadStreamingTexture(name_hash_table, num_name_hashes);
    loaded_skin->NumLoadedSkinLayersPerm = 0;
    return 1;
}

void __fastcall CarLoader_LoadedSkinCallback(DWORD* CarLoader, void *EDX_Unused, LoadedSkin* loaded_skin)
{
    if (loaded_skin->LoadStatePerm == 1)
    {
        loaded_skin->LoadStatePerm = 2;
        CarLoader_LoadedSkinLayers(CarLoader, loaded_skin->LoadedSkinLayersPerm, loaded_skin->NumLoadedSkinLayersPerm);
    }
    if (loaded_skin->LoadStateTemp == 1)
    {
        loaded_skin->LoadStateTemp = 2;
        CarLoader_LoadedSkinLayers(CarLoader, loaded_skin->LoadedSkinLayersTemp, loaded_skin->NumLoadedSkinLayersTemp);
    }
    CarLoader[5] = 0;
    CarLoader_ServiceLoading(CarLoader);
}