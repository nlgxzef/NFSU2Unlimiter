#pragma once

struct WeightReductionUpgradeInfo
{
    float FractionOfBaseWeight;
    float ForwardCOGAdjustment;
    float InertiaTensorZExtend;
    float pad1;
};

struct TransmissionUpgradeInfo
{
    float ShiftUpPoint;
    float ShiftDownRPM;
    float FinalDrive[2];
    float TorqueSplit;
    float ClutchStiffness;
    int NumGears;
    float ShiftSpeed;
    float GearRatios[8];
};

struct EngineUpgradeInfo
{
    float IdleRPMAdjust;
    float RedLineRPMAdjust;
    float MaxRPMAdjust;
    float MomentOfInertiaAdjust;
    float TorqueCurveAdjust[9];
    float EngineBrakingAdjust[3];
};

struct TurboUpgradeInfo
{
    float TorqueAdditive[9];
    float pad1;
    float pad2;
    float pad3;
};

struct TireUpgradeInfo
{
    TireInfo FrontTires;
    TireInfo RearTires;
};

struct NitrousUpgradeInfo
{
    NitrousInfo NOS;
};

struct SpoilerUpgradeInfo
{
    float FrontDownforceAdjust;
    float RearDownforceAdjust;
    float AutoBalanceAdjust;
    float pad1;
};

struct GroundEffectsUpgradeInfo
{
    float FrontDownforceAdjust;
    float RearDownforceAdjust;
    float AutoBalanceAdjust;
    float pad1;
};

struct BrakeUpgradeInfo
{
    float Brake;
    float EBrake;
    float BrakeBias;
    float Steering;
};

struct SuspensionUpgradeInfo
{
    SuspensionInfo FrontSuspension;
    SuspensionInfo RearSuspension;
    float COGLowering;
    float FrontLowering;
    float RearLowering;
    float pad2;
};

struct JunkmanUpgradeInfo
{
    float WeightReduction;
    float DriveTrainTorqueSplit;
    float NOSTorqueAdditive;
    float BrakeAdditive;
    float StaticFrictionAdditive[2];
    float DynamicFrictionAdditive[2];
    float EngineTorqueAdditive[9];
    float ECUTorqueAdditive[9];
    float TurboTorqueAdditive[9];
    float pad;
};

struct CompleatCarPhysicsInfo
{
    CarPhysicsInfo BasePhysicsInfo;
    JunkmanUpgradeInfo JunkmanUpgrades;
    WeightReductionUpgradeInfo WeightReductionUpgrades;
    TransmissionUpgradeInfo TransmissionUpgrades[3];
    EngineUpgradeInfo EngineUpgrades[3];
    EngineUpgradeInfo ECUUpgrades;
    TurboUpgradeInfo TurboUpgrades;
    TireUpgradeInfo TireUpgrades;
    NitrousUpgradeInfo NitrousUpgrades;
    SpoilerUpgradeInfo SpoilerUpgrades;
    GroundEffectsUpgradeInfo GroundEffectsUpgrades;
    BrakeUpgradeInfo BrakeUpgrades;
    SuspensionUpgradeInfo SuspensionUpgrades;
};