#pragma once

struct AcidEmitterData : bTNode<AcidEmitterData>
{
	DWORD CollectionNameHash;
	DWORD TextureNameHash;
	DWORD GroupNameHash;
	int field_0x14;
	bool SpreadAsDisc;
	BYTE ContactSheetW;
	BYTE ContactSheetH;
	BYTE AnimFPS;
	BYTE RandomStartFrame;
	BYTE RandomRotationDirection;
	BYTE MotionLive;
	BYTE Padding;
	int OnCycle;
	float OnCycleVariance;
	int OffCycle;
	float OffCycleVariance;
	int NumParticles;
	float NumParticlesVariance;
	float Life;
	float LifeVariance;
	float Speed;
	float SpeedVariance;
	float InitialAngleRange;
	float SpreadAngle;
	float MotionInherit;
	float MotionInheritVariance;
	int CarPosition;
	int EmitterID;
	float VolumeCenterX;
	float VolumeCenterY;
	float VolumeCenterZ;
	float VolumeCenterW;
	float VolumeExtentX;
	float VolumeExtentY;
	float VolumeExtentZ;
	float VolumeExtentW;
	float FarClip;
	float Gravity;
	float Drag;
	float MaxPixelSize;
	float KeyPosition1;
	float KeyPosition2;
	float KeyPosition3;
	float KeyPosition4;
	bMatrix4 ColorMatrix;
	float SizeX;
	float SizeY;
	float SizeZ;
	float SizeW;
	float RelativeAngleX;
	float RelativeAngleY;
	float RelativeAngleZ;
	float RelativeAngleW;
	bMatrix4 ColorBasis;
	bMatrix4 ExtraBasis;
	char CollectionName[0x40];
	char TextureName[0x30];
	char GroupName[0x30];
};

struct AcidEmitter : bTNode<AcidEmitter>
{
	DWORD CollectionNameHash;
	DWORD field_0x0C;
	DWORD field_0x10;
	DWORD field_0x14;
	DWORD field_0x18;
	AcidEmitterData* Data; // 0x1C
	DWORD* WorldEffects;
	DWORD field_0x24;
	DWORD field_0x28;
	DWORD field_0x2C;
	bMatrix4* ColorMatrix; // 0x30
	DWORD field_0x34;
	DWORD field_0x38;
	DWORD field_0x3C;
	float field_0x40[16];
	DWORD field_0x80;
	DWORD field_0x84;
	DWORD field_0x88;
	DWORD field_0x8C;
	char CollectionName[0x40]; // 0x90
};

void(*aFxGetEmitters)(bTList<AcidEmitter>* EmitterList, DWORD EmitterGroup) = (void(*)(bTList<AcidEmitter> *, DWORD))0x48B8B0;
void(*afxShiftEmitterColours)(bTList<AcidEmitter>* EmitterList, bMatrix4* matrix, float hue, float sat, float lum) = (void(*)(bTList<AcidEmitter> *, bMatrix4*, float, float, float))0x48B920;
