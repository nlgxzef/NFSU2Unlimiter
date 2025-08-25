#include "stdio.h"
#include "InGameFunctions.h"

int __cdecl GetIconHashForVinylGroup(unsigned int group)
{
    int result = 0; // eax
    int NumVinylGroups = VinylGroups.size();

    if (group > NumVinylGroups) return 0;
    
    for (int i = 0; i < NumVinylGroups; i++)
    {
        if (VinylGroups[i].Index == group)
        {
            result = VinylGroups[i].TextureHash;
            break;
        }
    }

    return result;
}