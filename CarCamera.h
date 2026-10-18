#pragma once

struct CarCamera
{
    short id;
    WORD Angle;
    float Lag;
    float Height;
    float LatOffset;
};

struct CarCameraInfo // not a real name
{
    int NumPlayerCameraInfos;
    int NumAICameraInfos;
    unsigned int pad[2];
    CarCamera PlayerCameras[6];
    CarCamera AICameras[6];
};