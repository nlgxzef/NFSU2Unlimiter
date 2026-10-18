#pragma once

struct WheelInfo
{
    bVector3 Position;
    float Radius;
    float Width;
    int WheelNumber;
    float YRenderOffset;
    float WideBodyYRenderOffset;
    float pad[3];
};

struct TireInfo
{
    float MaxSlipAngle;
    float MaxLoad;
    float ForceScale;
    float StaticFrictionCoefficient;
    float DynamicFrictionCoefficient;
    float dummy[3];
};

struct RigidBodyInfo
{
    float Mass;
    float Xdim;
    float Ydim;
    float Zdim;
    bMatrix4 InertiaTensor;
    float InertiaTensorZExtend;
    float pad[3];
};

struct SuspensionInfo
{
    float Progression;
    float Stiffness;
    float Damping[2];
    float SwayBarStiffness;
    float BumpStop[2];
    float MaxRenderingCompression;
};

struct DriveTrainInfo
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

struct EngineInfo
{
    float IdleRPM;
    float RedLineRPM;
    float MaxRPM;
    float MomentOfInertia;
    float TorqueCurveTable[9];
    float TurboTorqueCurve[9];
    float EngineBrakingTable[3];
    float pad1;
    float pad2;
    float pad3;
};

struct AeroInfo
{
    float Drag;
    float FrontDownforce;
    float RearDownforce;
    float AutoBalanceDownforce;
};

struct ControlInfo
{
    float Steering;
    float Brake;
    float EBrake;
    float BrakeBias;
    float YawControl;
    float YawControlNFS;
    float MaxZMomentAcc;
    float MaxZMomentAccExtreme;
    float SpeedLimiter;
    float dummy[3];
};

struct NitrousInfo
{
    float TotalNOS;
    int NumBursts;
    float RechargeTime;
    float TorqueAdditive;
};

struct CarPhysicsInfo
{
    bVector3 CenterOfGravity;
    WheelInfo Wheels[4];
    TireInfo Tires[2];
    RigidBodyInfo Body;
    SuspensionInfo Suspension[2];
    DriveTrainInfo DriveTrain;
    EngineInfo Engine;
    AeroInfo Aero;
    ControlInfo Control;
    NitrousInfo Nitrous;
};