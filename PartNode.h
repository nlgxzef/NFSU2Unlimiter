#pragma once

struct PartNode;

typedef void(__thiscall* PartNode_dtor_t)(PartNode* that);
PartNode_dtor_t PartNode_dtor = (PartNode_dtor_t)0x4AEB70;

struct PartNode : bTNode<PartNode>
{
	CarPart* part; // 0x0C
	DWORD* FEngObject; // FEImage*
	float XPos;
	float YPos;
	Color OriginalColor;
	bool IsGreyOut;
	bool field_0x21;
	bool field_0x22;
	bool field_0x23;
	float OrigWidth;
	float OrigHeight;
	CarPart* part_2; // ?

	virtual ~PartNode()
	{
		PartNode_dtor(this);
	}
};