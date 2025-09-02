#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall SelectCarCameraMover_GetAndMoveToNewDefaultOrientation(DWORD* SelectCarCameraMover, int CarTypeID, float FoV, float XEye, float XLook, float YEye, float XRot, float Zoom, float YRot, float AnimTime, float Focus)
{
    StaticCameraInfo* StCam = GetStaticCameraInfo(CarTypeID);
    
    if (SelectCarCameraMover)
    { 
        if (StCam)
        {
            SelectCarCameraMover_MoveToNewDefaultOrientation(
                SelectCarCameraMover,
                StCam->FoV,
                StCam->XEye,
                StCam->XLook,
                StCam->YEye,
                StCam->XRot,
                StCam->Zoom,
                StCam->YRot,
                StCam->AnimTime,
                StCam->Focus);
        }
        else
        {
            SelectCarCameraMover_MoveToNewDefaultOrientation(
                SelectCarCameraMover,
                FoV,
                XEye,
                XLook,
                YEye,
                XRot,
                Zoom,
                YRot,
                AnimTime,
                Focus);
        }
    }
}

StaticCameraInfo* __fastcall SelectCarCameraMover_GetAndSetDefaultOrientation(DWORD* SelectCarCameraMover, int CarTypeID, float FoV, float XEye, float XLook, float YEye, float XRot, float Zoom, float YRot, float AnimTime, float Focus)
{
    StaticCameraInfo* StCam = GetStaticCameraInfo(CarTypeID);
    
    if (SelectCarCameraMover)
    {
        if (StCam)
        {
            SelectCarCameraMover_SetDefaultOrientation(
                SelectCarCameraMover,
                StCam->FoV,
                StCam->XEye,
                StCam->XLook,
                StCam->YEye,
                StCam->XRot,
                StCam->Zoom,
                StCam->YRot,
                StCam->AnimTime,
                StCam->Focus);
        }
        else
        {
            SelectCarCameraMover_SetDefaultOrientation(
                SelectCarCameraMover,
                FoV,
                XEye,
                XLook,
                YEye,
                XRot,
                Zoom,
                YRot,
                AnimTime,
                Focus);
        }
        
    }

    return StCam;
}