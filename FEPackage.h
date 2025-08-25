#include "stdio.h"
#include "InGameFunctions.h"
#include "Helpers.h"
#include "includes\ini.h"

char ObjectName[64], ObjectPrefix[64];
char CloneName[64], ClonePrefix[64];
char FNGFixName[8], FNGChildName[8];

// 0x5557D4
bool __fastcall CloneObjectstoShowMoreItemsInMenu(DWORD* FEPackage, void* edx_unused, DWORD* FEGameInterface)
{
	bool result = FEPackage_Startup(FEPackage, FEGameInterface);
	int i;
	DWORD* CloneTarget, * CloneDest, * CloneTargetChild, * CloneChild, * CloneChildLast;

	auto FNGFixesPath = CurrentWorkingDirectory / "UnlimiterData" / "_FNGFixes.ini";
	mINI::INIFile FNGFixesINIFile(FNGFixesPath.string());
	mINI::INIStructure FNGFixesINI;
	FNGFixesINIFile.read(FNGFixesINI);

	int FNGFixesCount = mINI_ReadInteger(FNGFixesINI, "FNGFixes", "NumberOfFNGFixes", -1);
	if (FNGFixesCount == -1) return result;


	for (int FNGFixID = 1; FNGFixID <= FNGFixesCount; FNGFixID++)
	{
		sprintf(FNGFixName, "FNG%d", FNGFixID);
		if (stricmp((char*)FEPackage[3], mINI_ReadString(FNGFixesINI, FNGFixName, "FNGName", "")) == 0)
		{
			int NewNumberOfObjects = mINI_ReadInteger(FNGFixesINI, FNGFixName, "NumberOfObjects", -1);
			bool HasNumberSuffix = mINI_ReadInteger(FNGFixesINI, FNGFixName, "HasNumberSuffix", 1) != 0;

			sprintf(ObjectPrefix, mINI_ReadString(FNGFixesINI, FNGFixName, "ObjectPrefix", ""));

			if (HasNumberSuffix)
			{
				strcat(ObjectPrefix, "%d");

				// Get Last available object
				for (i = 1; i <= NewNumberOfObjects; i++)
				{
					sprintf(ObjectName, ObjectPrefix, i);
					CloneTarget = FEPackage_FindObjectByHash(FEPackage, bStringHash(ObjectName));
					if (!CloneTarget)
					{
						i--;
						sprintf(ObjectName, ObjectPrefix, i);
						CloneTarget = FEPackage_FindObjectByHash(FEPackage, bStringHash(ObjectName));
						break;
					}
				}
			}
			else
			{
				sprintf(ObjectName, ObjectPrefix);
				CloneTarget = FEPackage_FindObjectByHash(FEPackage, bStringHash(ObjectName));
			}

			// Make enough clones of that object
			if (CloneTarget)
			{
				for (int j = 1; (i + j) <= NewNumberOfObjects; j++)
				{
					CloneDest = (*(DWORD * (__thiscall**)(DWORD*, bool))(*(DWORD*)CloneTarget + 4))(CloneTarget, false); // FE...::Clone

					if (HasNumberSuffix) sprintf(ObjectName, ObjectPrefix, i + j);

					CloneDest[4] = bStringHash(ObjectName); // FEObject -> NameHash
					CloneDest[22] = 0; // FEObject -> Cached (Fix crash at FERenderObject::Clear)
					//FEMinList_AddNode(&(FEPackage[17]), (DWORD*)(FEPackage[20]), CloneDest); // &FEPackage -> Objects, FEPackage -> Objects.Tail

					CloneDest[2] = (DWORD)CloneTarget; // FEobject -> Prev
					CloneDest[1] = CloneTarget[1]; // FEObject -> Next
					CloneTarget[1] = (DWORD)CloneDest; // FEObject -> Next

					// FEObject -> objects.NumElements
					FEPackage[18]++;

					// Check for children if it's a group
					if (CloneDest[0] == _FEGroup_vtbl)
					{
						int ChildrenCount = CloneDest[24]; // FEGroup -> children.NumElements
						CloneChild = (DWORD*)CloneDest[25]; // FEGroup -> children.Head
						CloneTargetChild = (DWORD*)CloneTarget[25]; // FEGroup -> children.Head
						CloneChildLast = (DWORD*)CloneDest[26]; // FEGroup -> children.Tail

						for (int c = 1; c <= ChildrenCount; c++)
						{
							if (CloneChild)
							{
								sprintf(FNGChildName, "Child%d", c);

								sprintf(ClonePrefix, mINI_ReadString(FNGFixesINI, FNGFixName, FNGChildName, ""));
								if (strcmp(ClonePrefix, ""))
								{
									strcat(ClonePrefix, "%d");
									sprintf(CloneName, ClonePrefix, i + j);
									CloneChild[4] = bStringHash(CloneName); // FEObject -> NameHash
								}

								if (CloneChild == CloneChildLast) break;

								CloneChild = (DWORD*)CloneChild[1]; // FEObject -> Next
								CloneTargetChild = (DWORD*)CloneTargetChild[1]; // FEObject -> Next
							}
						}
					}

					// Clone the new object for the next one
					CloneTarget = CloneDest;
				}
			}

			FEPackage_ConnectObjectResources(FEPackage);
			//FEPackage_BuildMouseObjectStateList(FEPackage);
		}
	}

	return result;
}