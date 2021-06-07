#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall RidePhysicsInfo_RebuildPhysicsInfo(float* RidePhysicsInfo, void* EDX_Unused, int PhysicsType, bool ShouldComputeMiscStats, float a4)
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
    DWORD AttrVal;

    float* PerformanceClassRange = (float*)_PerformanceClassRange;
    float flt_7A1A80 = *(float*)0x7A1A80;

    if (*(DWORD*)RidePhysicsInfo != -1)
    {
        CarTypeInfo = GetCarTypeInfo(*(DWORD*)RidePhysicsInfo);

        _ccpi = (float*)(CarTypeInfo + 68);// UnknownVectorVal + WheelPos...
        memcpy(CompleatCarPhysicsInfo, _ccpi, 480 * sizeof(float)); // Create a copy of physics values

        // get wheel offsets from rideinfo + custom attr
        DWORD* RideInfo = (DWORD*)RidePhysicsInfo - 4;
        DWORD* FenderPart = (DWORD*)RideInfo[356 + 23]; // FENDER
        DWORD* QuarterPart = (DWORD*)RideInfo[356 + 24]; // QUARTER
        DWORD* WideBodyPart = (DWORD*)RideInfo[356 + 6]; // WIDE_BODY

        // Check our custom attributes for track width
        float FrontTireOffset = 0;
        float RearTireOffset = 0;
        if (WideBodyPart && (*((BYTE*)RideInfo + 2104 + 6) == 1)) // check has WIDE_BODY and its visibility
        {
            AttrVal = CarPart_GetAppliedAttributeUParam(WideBodyPart, bStringHash("FRONT_TIRE_OFFSET"), 0);
            FrontTireOffset = *(float*)&AttrVal;
            AttrVal = CarPart_GetAppliedAttributeUParam(WideBodyPart, bStringHash("REAR_TIRE_OFFSET"), 0);
            RearTireOffset = *(float*)&AttrVal;
        }
        else
        {
            if (FenderPart && (*((BYTE*)RideInfo + 2104 + 23) == 1)) // check has FENDER and its visibility
            {
                AttrVal = CarPart_GetAppliedAttributeUParam(FenderPart, bStringHash("FRONT_TIRE_OFFSET"), 0);
                FrontTireOffset = *(float*)&AttrVal;
            }
            if (QuarterPart && (*((BYTE*)RideInfo + 2104 + 24) == 1)) // check has QUARTER and its visibility
            {
                AttrVal = CarPart_GetAppliedAttributeUParam(QuarterPart, bStringHash("REAR_TIRE_OFFSET"), 0);
                RearTireOffset = *(float*)&AttrVal;
            }
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

        CompleatCarPhysicsInfo_BuildCarPhysicsInfo(CompleatCarPhysicsInfo, RidePhysicsInfo + 12, RidePhysicsInfo + 184, a4);
        CompleatCarPhysicsInfo_ApplyJunkmanUpgrades(CompleatCarPhysicsInfo, RidePhysicsInfo + 12, RidePhysicsInfo + 184);
        if (CustomPhysicsSpec_GetNumActiveTunings((DWORD*)RidePhysicsInfo + 221) > 0)
            CustomPhysicsSpec_RebuildCustomTunings((DWORD*)RidePhysicsInfo + 221);
        MakeAccelerationTable(RidePhysicsInfo + 343, RidePhysicsInfo);
        if (CarTypeInfo[529])                     // CarUsageType
        {
            *((BYTE*)RidePhysicsInfo + 4) = 3;
        }
        else
        {
            v11 = 0.0;
            a4a = RidePhysicsInfo[344];
            if (a4a <= RidePhysicsInfo[345])
            {
                do
                {
                    v6 = Table_GetValue(RidePhysicsInfo + 343, a4a);
                    v7 = a4a * RidePhysicsInfo[164] * a4a / RidePhysicsInfo[80];
                    if (v6 < v7)
                        break;
                    v11 = (v6 - v7) * flt_7A1A80 + v11;
                    v8 = flt_7A1A80 + a4a;
                    a4a = v8;
                } while (v8 <= RidePhysicsInfo[345]);
            }
            RidePhysicsInfo[2] = v11;
            v9 = 3;
            v10 = 3;
            while (v11 <= PerformanceClassRange[v10])
            {
                if (--v10 < 0)
                    goto LABEL_13;
            }
            v9 = 3 - v10;
        LABEL_13:
            *((BYTE*)RidePhysicsInfo + 4) = v9;                // CalculateCarClass
            if (ShouldComputeMiscStats)
                RidePhysicsInfo_ComputeMiscStats((DWORD*)RidePhysicsInfo);
        }
        switch (PhysicsType)
        {
        case 3:
            CompleatCarPhysicsInfo_TuneForDrift(CompleatCarPhysicsInfo, RidePhysicsInfo + 12, RidePhysicsInfo + 184);
            MakeAccelerationTable(RidePhysicsInfo + 343, RidePhysicsInfo);
            break;
        case 10:
        case 5:
            CompleatCarPhysicsInfo_TuneForShortTrack(CompleatCarPhysicsInfo, RidePhysicsInfo + 12, RidePhysicsInfo + 184);
            MakeAccelerationTable(RidePhysicsInfo + 343, RidePhysicsInfo);
            break;
        case 2:
            CompleatCarPhysicsInfo_TuneForDragRace(CompleatCarPhysicsInfo, RidePhysicsInfo + 12, RidePhysicsInfo + 184);
            MakeAccelerationTable(RidePhysicsInfo + 343, RidePhysicsInfo);
            break;
        }
    }
}