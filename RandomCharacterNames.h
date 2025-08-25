#pragma once
#include "stdio.h"
#include "InGameFunctions.h"
#include "UserCalls.h"

using namespace std;

int NumberOfNames;
char const** QuickRaceCharacterNames;
bool STRFirstTime = 1;

// Read the names from RACERNAME_%03d strings (like NFSMW+)
int CountRandomNameStrings()
{
    if (!STRFirstTime)
    {
        return NumberOfNames;
    }

    int i;
    char OppNameBuffer[15];

    for (i = 0; i <= 999; i++)
    {
        sprintf(OppNameBuffer, "RACERNAME_%03d", i);
        if (!MakeUserCall(SearchForString, 1, edx, bStringHash(OppNameBuffer))) break;
    }

    if (i > 0) NumberOfNames = i;
    else NumberOfNames = -1;

    STRFirstTime = 0;
    return i;
}

void GetRandomCharacterNames(const char** Names, int NumberOfOpponents)
{
    int* NameUsed; // esi
    int i; // ebx
    int v4; // edi
    int ID; // eax
    char RandomNameBuffer[16];
    int RandomNameHash;
    bool IsUsingStringImplementation = 0;

    NumberOfNames = CountRandomNameStrings();

    // Fall back to game's implementation if the conditions are not met
    if ((!RandomNameHook || !NumberOfNames) && (NumberOfNames < NumberOfOpponents)) return GetRandomCharacterNames_Game(Names, NumberOfOpponents);

    // String Implementation
    NameUsed = (int*)j__malloc(NumberOfNames * sizeof(int));
    memset(NameUsed, 0, NumberOfNames * sizeof(int));
    for (i = 0; i < NumberOfOpponents; ++i)
    {
        v4 = 0;
        while (1)
        {
            ID = bRandom(NumberOfNames);
            if (!NameUsed[ID])
                break;
            if (++v4 >= 100)
                goto LABEL_7;
        }
       
        sprintf(RandomNameBuffer, "RACERNAME_%03d", ID);
        RandomNameHash = bStringHash(RandomNameBuffer);
        Names[i] = GetLocalizedString(RandomNameHash);
        NameUsed[ID] = 1;

    LABEL_7:
        ;
    }
    j__free(NameUsed);
}