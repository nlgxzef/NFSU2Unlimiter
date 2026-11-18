#pragma once

#include "CarPhysicsInfo.h"

#pragma pack(push, 1)
struct PhysicsUpgradeSpecification
{
	float UpgradeLevel[10];
	int JunkmanPart[10];
	char PerfPartInstalled[68];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CustomPhysicsSpec
{
	int CarType;
	int field_04;
	PhysicsUpgradeSpecification* pUpgrades;
	CarPhysicsInfo* pCarPhysicsInfo;
	int NumActiveTunings;
	unsigned __int8 TuningValues[41];
	char _pad3D[3];
	int field_40[23];
	int field_9C[9];
	int field_C0[9];
	int field_E4;
};
#pragma pack(pop)
