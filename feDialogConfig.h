#pragma once

#include "stdio.h"
#include "InGameFunctions.h"

struct feDialogConfig {
    // total size: 0x338
    char BlurbString[768]; // offset 0x0, size 0x300
    unsigned int Button1TextHash; // offset 0x300, size 0x4
    unsigned int Button1PressedMessage; // offset 0x304, size 0x4
    unsigned int Button2TextHash; // offset 0x308, size 0x4
    unsigned int Button2PressedMessage; // offset 0x30C, size 0x4
    unsigned int Button3TextHash; // offset 0x310, size 0x4
    unsigned int Button3PressedMessage; // offset 0x314, size 0x4
    unsigned int DialogCancelledMessage; // offset 0x318, size 0x4
    unsigned int FirstButton; // offset 0x31C, size 0x4
    const char* ParentPackage; // offset 0x320, size 0x4
    const char* DialogPackage; // offset 0x324, size 0x4
    int NumButtons; // offset 0x328, size 0x4
    bool bIsDismissable; // offset 0x32C, size 0x1
    bool bDetectController; // offset 0x32D, size 0x1
    bool bBlurbIsUTF8; // offset 0x32E, size 0x1
    bool DismissedByOtherDialogMsg; // offset 0x32F, size 0x1
    int pad1;
    int pad2;
};