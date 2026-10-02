#pragma once

struct eModel : bTNode<eModel>
{
	unsigned int NameHash;
	eSolid* Solid;
	DWORD/*eReplacementTextureTable*/* pReplacementTextureTable;
	int NumReplacementTextures;

    void ReplaceLightMaterial(int NameHash, int LightMaterial)
    {
        DWORD* LightMaterialTable; // eax
        int NumLightMaterials; // ecx

        if (Solid)
        {
            if (LightMaterial)
            {
                LightMaterialTable = Solid->LightMaterialTable;
                NumLightMaterials = Solid->NumLightMaterials;
                if (NumLightMaterials > 0)
                {
                    do
                    {
                        if (*LightMaterialTable == NameHash)
                            LightMaterialTable[1] = LightMaterial;
                        LightMaterialTable += 2;
                        --NumLightMaterials;
                    } while (NumLightMaterials);
                }
            }
        }
    }
};

void __fastcall eModel_ReplaceLightMaterial(eModel* deze, void* EDX_Unused, int NameHash, int LightMaterial)
{
    if (deze) // Check if there is actually a model to begin with (to work around crashes)
    {
        deze->ReplaceLightMaterial(NameHash, LightMaterial);
    }
}