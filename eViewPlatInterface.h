#pragma once

// one 60 byte mesh entry, the runtime face of a shading group
#define eStripEntry_TextureIndex(m)        (*(BYTE*)((BYTE*)(m) + 0x1C))
#define eStripEntry_LightMaterialIndex(m)  (*(BYTE*)((BYTE*)(m) + 0x20))

bool eLightMaterial_ValidPtr(void* p)
{
	uintptr_t v = (uintptr_t)p;
	return v >= 0x00010000 && v <= 0xC0000000 && !(v & 3);
}

int ShouldReplaceByMaterial = 0;
DWORD* CurrCarRenderInfo = nullptr;

DWORD* eSolid_ReplaceTextureByMaterial(eSolid* solid, eStripEntry* entry, DWORD* orig)
{
	DWORD* result = orig;

	if (!CurrCarRenderInfo) return result;
	CarRenderInfoExtra* extra = CarRenderInfo_GetExtra(CurrCarRenderInfo);
	if (!extra) return result;

	BYTE Index = eStripEntry_LightMaterialIndex(entry);
	if (Index == 0xFF) return result;

	int Count = solid->NumLightMaterials;
	DWORD* Table = solid->LightMaterialTable;

	if (Count <= 0 || Index >= Count || !eLightMaterial_ValidPtr(Table)) return result;

	DWORD SourceMaterialHash = Table[Index * 2];

	switch (SourceMaterialHash)
	{
	case CT_bStringHash("RUBBER"):
		result = extra->pTireTextureInfo ? extra->pTireTextureInfo : orig;
		break;
	default:
		break;
	}

	return result;
}

void __declspec(naked) eViewPlatInterface_Render_MatBasedReplacementCodeCave()
{
	__asm
	{
		mov ecx, [eax + 0x2C];
		mov ebp, [ecx + edx * 8 + 4];

		mov ecx, ShouldReplaceByMaterial;
		test ecx, ecx;
		jz keep;

		push eax;
		push edx;

		push ebp;		// the texture this mesh would have drawn with
		push esi;		// the mesh entry
		push eax;		// the eSolid
		call eSolid_ReplaceTextureByMaterial;
		add esp, 0x0C;

		mov ebp, eax;

		pop edx;
		pop eax;

	keep:
		push 0x5C5A9B;
		retn
	}
}

void(__thiscall* eViewPlatInterface_Render)(DWORD* view, eModel* model, bMatrix4* local_to_world, DWORD* light_context, unsigned int flags, bMatrix4* blending_matricies) = (void(__thiscall*)(DWORD*, eModel*, bMatrix4*, DWORD*, unsigned int, bMatrix4*))0x5C5930;
void __fastcall eViewPlatInterface_RenderWheel(DWORD* view, DWORD* CarRenderInfo, eModel* model, bMatrix4* local_to_world, DWORD* light_context, unsigned int flags, bMatrix4* blending_matricies)
{
	ShouldReplaceByMaterial = 1;
	eViewPlatInterface_Render(view, model, local_to_world, light_context, flags, blending_matricies);
	ShouldReplaceByMaterial = 0;
}

// 0x6162FD
void __declspec(naked) CarRenderInfo_RenderFast_GetCurrCRICodeCave()
{
	_asm
	{
		mov ecx, ebx
		mov dword ptr ds: [esp + 0x70], edi // CRI
		mov CurrCarRenderInfo, edi

		push 0x616303
		retn
	}
}

// 0x620AF8
void __declspec(naked) CarRenderInfo_Render_GetCurrCRICodeCave()
{
	_asm
	{
		push edi
		mov dword ptr ds : [esp + 0x58] , esi // CRI
		mov CurrCarRenderInfo, esi

		push 0x620AFD
		retn
	}
}