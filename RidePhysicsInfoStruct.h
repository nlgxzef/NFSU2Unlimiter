#pragma once

#include "PhysicsUpgradeSpecification.h"
#include "Table.h"

// Kept apart from RidePhysicsInfo.h so RideInfo.h can embed it (RideInfo::mPhysics) while the
// RidePhysicsInfo hooks, which read the RideInfo around them, still see the full RideInfo.
#pragma pack(push, 1)
struct RidePhysicsInfo
{
    int Type;
    unsigned __int8 CarClass;
    char _pad005[3];
    float field_008; // RebuildPhysicsInfo: area under (acceleration table - drag/mass) curve; CarClass is bucketed from it
    int field_00C;
    int MiscStats[5];
    int field_024[3];
    CarPhysicsInfo mCarPhysicsInfo;
    PhysicsUpgradeSpecification mUpgrades;
    CustomPhysicsSpec mCustomSpec;
    float AccelerationTableData[63];
    int field_558;
    Table AccelerationTable;
};
#pragma pack(pop)

static_assert(offsetof(CarPhysicsInfo, Body) == 0x110, "CarPhysicsInfo::Body");
static_assert(offsetof(CarPhysicsInfo, Aero) == 0x260, "CarPhysicsInfo::Aero");
static_assert(offsetof(RidePhysicsInfo, mCarPhysicsInfo) == 0x30, "RidePhysicsInfo::mCarPhysicsInfo");
static_assert(offsetof(RidePhysicsInfo, mUpgrades) == 0x2E0, "RidePhysicsInfo::mUpgrades");
static_assert(offsetof(RidePhysicsInfo, mCustomSpec) == 0x374, "RidePhysicsInfo::mCustomSpec");
static_assert(offsetof(RidePhysicsInfo, AccelerationTable) == 0x55C, "RidePhysicsInfo::AccelerationTable");
static_assert(sizeof(RidePhysicsInfo) == 0x570, "RidePhysicsInfo size");
