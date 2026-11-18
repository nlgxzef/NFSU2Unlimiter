#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

#include "RidePhysicsInfoStruct.h"
#include "RideInfo.h" // RidePhysicsInfo lives at RideInfo::mPhysics

inline RideInfo* RidePhysicsInfo_GetRideInfo(RidePhysicsInfo* ride_physics_info)
{
    return (RideInfo*)((BYTE*)ride_physics_info - offsetof(RideInfo, mPhysics));
}


void __fastcall RidePhysicsInfo_RebuildPhysicsInfo(RidePhysicsInfo* ride_physics_info, void* EDX_Unused, int PhysicsType, bool ShouldComputeMiscStats, float a4)
{
    DWORD* CarTypeInfo; // edi
    float v6; // st7
    float v7; // st6
    float v8; // st7
    char v9; // cl
    int v10; // edx
    float v11; // [esp+4h] [ebp-8h]
    float* _ccpi; // [esp+8h] [ebp-4h]
    float CompleatCarPhysicsInfo[480];
    float a4a; // [esp+18h] [ebp+Ch]

    float* PerformanceClassRange = (float*)_PerformanceClassRange;
    float flt_7A1A80 = *(float*)0x7A1A80;

    if (ride_physics_info->Type != -1)
    {
        CarTypeInfo = GetCarTypeInfo(ride_physics_info->Type);

        _ccpi = (float*)(CarTypeInfo + 68);// UnknownVectorVal + WheelPos...
        memcpy(CompleatCarPhysicsInfo, _ccpi, 480 * sizeof(float)); // Create a copy of physics values

        // get wheel offsets from rideinfo + custom attr
        RideInfo* ride_info = RidePhysicsInfo_GetRideInfo(ride_physics_info);
        CarPart* FenderPart = ride_info->mPartsTable[CARSLOTID_FENDER]; // FENDER
        CarPart* QuarterPart = ride_info->mPartsTable[CARSLOTID_QUARTER]; // QUARTER
        CarPart* WideBodyPart = ride_info->mPartsTable[CARSLOTID_WIDE_BODY]; // WIDE_BODY

        // Check our custom attributes for track width
        float FrontTireOffset = 0;
        float RearTireOffset = 0;
        float FrontTireWidth = CompleatCarPhysicsInfo[9]; // fl, 21 = fr
        float RearTireWidth = CompleatCarPhysicsInfo[33]; // rr, 45 = rl
        float FrontTireRadius = CompleatCarPhysicsInfo[8]; // fl, 20 = fr
        float RearTireRadius = CompleatCarPhysicsInfo[32]; // rr, 44 = rl

		bool HasWideBody = WideBodyPart && (ride_info->mPartsEnabled[CARSLOTID_WIDE_BODY] == 1);

        if (HasWideBody) // check has WIDE_BODY and its visibility
        {
            FrontTireOffset = CarPart_GetAppliedAttributeFParam(WideBodyPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_OFFSET"), 0.0f);
            RearTireOffset = CarPart_GetAppliedAttributeFParam(WideBodyPart, EDX_Unused, CT_bStringHash("REAR_TIRE_OFFSET"), 0.0f);

            FrontTireWidth = CarPart_GetAppliedAttributeFParam(WideBodyPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_WIDTH"), -1.0f);
            RearTireWidth = CarPart_GetAppliedAttributeFParam(WideBodyPart, EDX_Unused, CT_bStringHash("REAR_TIRE_WIDTH"), -1.0f);

            FrontTireRadius = CarPart_GetAppliedAttributeFParam(WideBodyPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_RADIUS"), -1.0f);
            RearTireRadius = CarPart_GetAppliedAttributeFParam(WideBodyPart, EDX_Unused, CT_bStringHash("REAR_TIRE_RADIUS"), -1.0f);
        }

        bool TakeBodyOffsets = AccumulateTireOffsets || !HasWideBody;
        
        if (TakeBodyOffsets && FenderPart && (ride_info->mPartsEnabled[CARSLOTID_FENDER] == 1)) // check has FENDER and its visibility
        {
            FrontTireOffset += CarPart_GetAppliedAttributeFParam(FenderPart, EDX_Unused, CT_bStringHash("FRONT_TIRE_OFFSET"), 0.0f);
        }
        if (TakeBodyOffsets && QuarterPart && (ride_info->mPartsEnabled[CARSLOTID_QUARTER] == 1)) // check has QUARTER and its visibility
        {
            RearTireOffset += CarPart_GetAppliedAttributeFParam(QuarterPart, EDX_Unused, CT_bStringHash("REAR_TIRE_OFFSET"), 0.0f);
        }
        

        // Set wheel offsets
        CompleatCarPhysicsInfo[11] += FrontTireOffset;
        CompleatCarPhysicsInfo[12] += FrontTireOffset;
        CompleatCarPhysicsInfo[23] -= FrontTireOffset;
        CompleatCarPhysicsInfo[24] -= FrontTireOffset;
        CompleatCarPhysicsInfo[35] -= RearTireOffset;
        CompleatCarPhysicsInfo[36] -= RearTireOffset;
        CompleatCarPhysicsInfo[47] += RearTireOffset;
        CompleatCarPhysicsInfo[48] += RearTireOffset;

		// Set tire widths
        if (FrontTireWidth != -1.0f)
        {
            CompleatCarPhysicsInfo[9] = FrontTireWidth; // fl
            CompleatCarPhysicsInfo[21] = FrontTireWidth; // fr
        }
        if (RearTireWidth != -1.0f)
        {
            CompleatCarPhysicsInfo[33] = RearTireWidth; // rr
            CompleatCarPhysicsInfo[45] = RearTireWidth; // rl
        }
        
		// Set tire radius
        if (FrontTireRadius != -1.0f)
        {
            CompleatCarPhysicsInfo[8] = FrontTireRadius; // fl
            CompleatCarPhysicsInfo[20] = FrontTireRadius; // fr
        }
        if (RearTireRadius != -1.0f)
        {
            CompleatCarPhysicsInfo[32] = RearTireRadius; // rr
            CompleatCarPhysicsInfo[44] = RearTireRadius; // rl
        }
		

        float* CarPhysicsInfoPtr = (float*)&ride_physics_info->mCarPhysicsInfo;
        float* UpgradesPtr = (float*)&ride_physics_info->mUpgrades;
        float* AccelerationTablePtr = (float*)&ride_physics_info->AccelerationTable;

        CompleatCarPhysicsInfo_BuildCarPhysicsInfo(CompleatCarPhysicsInfo, CarPhysicsInfoPtr, UpgradesPtr, a4);
        CompleatCarPhysicsInfo_ApplyJunkmanUpgrades(CompleatCarPhysicsInfo, CarPhysicsInfoPtr, UpgradesPtr);
        if (CustomPhysicsSpec_GetNumActiveTunings((DWORD*)&ride_physics_info->mCustomSpec) > 0)
            CustomPhysicsSpec_RebuildCustomTunings((DWORD*)&ride_physics_info->mCustomSpec);
        MakeAccelerationTable(AccelerationTablePtr, (float*)ride_physics_info);
        if (CarTypeInfo[529])                     // CarUsageType
        {
            ride_physics_info->CarClass = 3;
        }
        else
        {
            v11 = 0.0;
            a4a = ride_physics_info->AccelerationTable.__base.MinArg;
            if (a4a <= ride_physics_info->AccelerationTable.__base.MaxArg)
            {
                do
                {
                    v6 = Table_GetValue(AccelerationTablePtr, a4a);
                    v7 = a4a * ride_physics_info->mCarPhysicsInfo.Aero.Drag * a4a / ride_physics_info->mCarPhysicsInfo.Body.Mass;
                    if (v6 < v7)
                        break;
                    v11 = (v6 - v7) * flt_7A1A80 + v11;
                    v8 = flt_7A1A80 + a4a;
                    a4a = v8;
                } while (v8 <= ride_physics_info->AccelerationTable.__base.MaxArg);
            }
            ride_physics_info->field_008 = v11;
            v9 = 3;
            v10 = 3;
            while (v11 <= PerformanceClassRange[v10])
            {
                if (--v10 < 0)
                    goto LABEL_13;
            }
            v9 = 3 - v10;
        LABEL_13:
            ride_physics_info->CarClass = v9;                // CalculateCarClass
            if (ShouldComputeMiscStats)
                RidePhysicsInfo_ComputeMiscStats((DWORD*)ride_physics_info);
        }
        switch (PhysicsType)
        {
        case 3:
            CompleatCarPhysicsInfo_TuneForDrift(CompleatCarPhysicsInfo, CarPhysicsInfoPtr, UpgradesPtr);
            MakeAccelerationTable(AccelerationTablePtr, (float*)ride_physics_info);
            break;
        case 10:
        case 5:
            CompleatCarPhysicsInfo_TuneForShortTrack(CompleatCarPhysicsInfo, CarPhysicsInfoPtr, UpgradesPtr);
            MakeAccelerationTable(AccelerationTablePtr, (float*)ride_physics_info);
            break;
        case 2:
            CompleatCarPhysicsInfo_TuneForDragRace(CompleatCarPhysicsInfo, CarPhysicsInfoPtr, UpgradesPtr);
            MakeAccelerationTable(AccelerationTablePtr, (float*)ride_physics_info);
            break;
        }
    }
}

float __fastcall RidePhysicsInfo_GetCamberPercent(RidePhysicsInfo* ride_physics_info, void* EDX_Unused)
{
    int CarTypeID = ride_physics_info->Type;
    float result = .0f;

	RideInfo* ride_info = RidePhysicsInfo_GetRideInfo(ride_physics_info);
	BYTE* PhysUpgSpec = (BYTE*)&ride_physics_info->mUpgrades;

	// Check body kit if it has a custom camber attribute
	CarPart* BodyKitPart = ride_info->mPartsTable[CARSLOTID_WIDE_BODY];
    if (BodyKitPart && (ride_info->mPartsEnabled[CARSLOTID_WIDE_BODY] == 1)) // check has WIDE_BODY and its visibility
    {
        result += CarPart_GetAppliedAttributeFParam(BodyKitPart, EDX_Unused, CT_bStringHash("CAMBER"), 0.0f);
	}

    // Check performance parts
    result += CarConfigs[CarTypeID].Physics.CamberStock;
    if (PhysicsUpgradeSpecification_IsPerfPartInstalled(PhysUpgSpec, PERF_PART_SU_CAMBER_KIT))
        result += CarConfigs[CarTypeID].Physics.CamberKit;
    if (PhysicsUpgradeSpecification_IsPerfPartInstalled(PhysUpgSpec, PERF_PART_SU_COIL_OVER_SUSPENSION_SYSTEM))
        result += CarConfigs[CarTypeID].Physics.CamberCoilOver;

    return result;
}