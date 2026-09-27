#pragma once

struct eSolid;

struct eModel : bTNode<eModel>
{
	unsigned int NameHash;
	eSolid* Solid;
	DWORD/*eReplacementTextureTable*/* pReplacementTextureTable;
	int NumReplacementTextures;
};
