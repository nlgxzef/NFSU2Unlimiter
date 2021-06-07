#pragma once
#include "stdio.h"
#include "InGameFunctions.h"

#define MAX_RANDOM_STRINGS 99

int SUVRandomCnt = 0;

void CountRandomEngageStrings()
{
	unsigned int StringHashToLookup;

	for (int i = 1; i < MAX_RANDOM_STRINGS; i++)
	{
		StringHashToLookup = FEngHashString("ENGAGE_SUV_RANDOM_%d", i);
		char const* StringData = (char const* )MakeUserCall(SearchForString, 1, edx, StringHashToLookup);
		if (!StringData) break;
		SUVRandomCnt++;
	}
}

unsigned int __fastcall CareerEventData_GetEngageDialogBody(BYTE* CareerEventData, void* EDX_Unused)
{
    unsigned int result; // eax
    const char* RandomRaceString; // esi
    int DriftRandomCnt; // eax
    int SUVRandom; // eax
    BYTE EventIcon; // dl
    int SponsoredCircuitRandom; // eax
    int OpenWorldRandom; // eax
    int SponsoredStreetXRandom; // eax
    int SponsoredDragRandom; // eax
    int SponsoredDriftRandom; // eax
    int URLRandom; // eax
    int CircuitRandom; // eax
    int SprintRandom; // eax
    int StreetXRandom; // eax
    int DragRandom; // eax
    int DriftRandom; // eax

    DWORD CareerStringTable = *(DWORD*)_CareerStringTable;

    // If the event has a specific engage string, return its hash.
    // Example: "ENGAGE_STAGE_0_SPRINT_0"
    result = FEngHashString("ENGAGE_%s", (const char*)(CareerStringTable + *(unsigned __int16*)CareerEventData));
    char const* StringData = (char const*)MakeUserCall(SearchForString, 1, edx, result);
    if (StringData) return result;

    // Else, return a random one.
    RandomRaceString = 0;
    DriftRandomCnt = 0;
    if (CareerEventData[13] == 1)                          // IsSUVRace
    {
        SUVRandom = bRandom(9);
        result = FEngHashString("ENGAGE_SUV_RANDOM_%d", SUVRandom + 1);
    }
    else
    {
        EventIcon = CareerEventData[52];                       // EventIcon
        if (EventIcon == 1)                       // Sponsor
        {
            switch (CareerEventData[15])
            {
            case 0u:
                SponsoredCircuitRandom = bRandom(11);
                result = FEngHashString("ENGAGE_SPON_CIRCUIT_RANDOM_%d", SponsoredCircuitRandom + 1);
                break;
            case 1u:
                RandomRaceString = "ENGAGE_SPON_SPRINT_RANDOM_%d";
                goto LABEL_9;
            case 2u:
                SponsoredStreetXRandom = bRandom(10);
                result = FEngHashString("ENGAGE_SPON_STREET_RANDOM_%d", SponsoredStreetXRandom + 1);
                break;
            case 4u:
                SponsoredDragRandom = bRandom(8);
                result = FEngHashString("ENGAGE_SPON_DRAG_RANDOM_%d", SponsoredDragRandom + 1);
                break;
            case 5u:
                SponsoredDriftRandom = bRandom(11);
                result = FEngHashString("ENGAGE_SPON_DRIFT_RANDOM_%d", SponsoredDriftRandom + 1);
                break;
            default:
                goto LABEL_21;
            }
        }
        else if (EventIcon == 2)                  // URL
        {
            URLRandom = bRandom(12);
            result = FEngHashString("ENGAGE_URL_RANDOM_%d", URLRandom + 1);
        }
        else
        {
            switch (CareerEventData[15])                       // Regular
            {
            case 0u:
                CircuitRandom = bRandom(18);
                result = FEngHashString("ENGAGE_CIRCUIT_RANDOM_%d", CircuitRandom + 1);
                break;
            case 1u:
                SprintRandom = bRandom(15);
                result = FEngHashString("ENGAGE_SPRINT_RANDOM_%d", SprintRandom + 1);
                break;
            case 2u:
                StreetXRandom = bRandom(14);
                result = FEngHashString("ENGAGE_STREET_RANDOM_%d", StreetXRandom + 1);
                break;
            case 3u:
            LABEL_9:
                OpenWorldRandom = bRandom(10);
                result = FEngHashString(RandomRaceString, OpenWorldRandom + 1);
                break;
            case 4u:
                DragRandom = bRandom(12);
                result = FEngHashString("ENGAGE_DRAG_RANDOM_%d", DragRandom + 1);
                break;
            case 5u:
                RandomRaceString = "ENGAGE_DRIFT_RANDOM_%d";
                DriftRandomCnt = 13;
                //goto LABEL_21;
            default:
            LABEL_21:
                DriftRandom = bRandom(DriftRandomCnt);
                result = FEngHashString(RandomRaceString, DriftRandom + 1);
                break;
            }
        }
    }
    return result;
}