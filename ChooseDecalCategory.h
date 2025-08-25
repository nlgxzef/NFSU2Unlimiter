#include "stdio.h"
#include "InGameFunctions.h"

void __fastcall ChooseDecalCategory_Setup(DWORD* ChooseDecalCategory, void* EDX_Unused)
{
	int LayoutID;
	int v3;
	DWORD* WideBodyPart;
	DWORD* QuarterPart;
	int v5;
	int ChooseDecalCategoryPackage;
	int PaintSetupPrice;
	DWORD v9;
	DWORD v10;
	DWORD v11;
	DWORD v12;
	int NumDecalSlots;
	char DecalMenuTexture[64];
	char DecalMenuString[64];

	ChooseDecalCategory[10] = CT_bStringHash("HELP_GRAPHICS_SHOP_DECAL_SELECT_CATEGORY");
	if ((DWORD*)ChooseDecalCategory[20] == ChooseDecalCategory + 20)
	{
		LayoutID = ChooseDecalCategory[85];
		
		switch (LayoutID)
		{
		case 0x20006: // Left Quarter Layout 1
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 57); // DECAL_LEFT_QUARTER
			if (!NumDecalSlots) NumDecalSlots = 1;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 136 + i, 1, CT_bStringHash("DECAL_LREAR_QP_LAYOUT2"), bStringHash(DecalMenuString));
			}

			break;

		case 0x20007: // Left Quarter Layout 2
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 57); // DECAL_LEFT_QUARTER
			if (!NumDecalSlots) NumDecalSlots = 2;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuTexture, "DECAL_LREAR_QP_L1_SLOT%d", i);
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 136 + i, 1, bStringHash(DecalMenuTexture), bStringHash(DecalMenuString));
			}

			break;

		case 0x20008: // Right Quarter Layout 1
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 58); // DECAL_RIGHT_QUARTER
			if (!NumDecalSlots) NumDecalSlots = 1;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 144 + i, 1, CT_bStringHash("DECAL_RREAR_QP_LAYOUT2"), bStringHash(DecalMenuString));
			}

			break;

		case 0x20009: // Right Quarter Layout 2
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 58); // DECAL_RIGHT_QUARTER
			if (!NumDecalSlots) NumDecalSlots = 2;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuTexture, "DECAL_RREAR_QP_L1_SLOT%d", i);
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 144 + i, 1, bStringHash(DecalMenuTexture), bStringHash(DecalMenuString));
			}

			break;

		case 0x2000A: // Hood Layout 1
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 52); // DECAL_HOOD
			if (!NumDecalSlots) NumDecalSlots = 4;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuTexture, "DECAL_HOOD_L1_SLOT%d", i);
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 96 + i, 1, bStringHash(DecalMenuTexture), bStringHash(DecalMenuString));
			}

			break;

		case 0x2000B: // Hood Layout 2
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 52); // DECAL_HOOD
			if (!NumDecalSlots) NumDecalSlots = 4;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuTexture, "DECAL_HOOD_L2_SLOT%d", i);
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 96 + i, 1, bStringHash(DecalMenuTexture), bStringHash(DecalMenuString));
			}

			break;

		case 0x10001: // Left Door
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 55); // DECAL_LEFT_DOOR
			if (!NumDecalSlots) NumDecalSlots = 6;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuTexture, "DECAL_LEFTDOOR_SLOT%d", i);
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 120 + i, 1, bStringHash(DecalMenuTexture), bStringHash(DecalMenuString));
			}

			break;

		case 0x10002: // Right Door
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 56); // DECAL_RIGHT_DOOR
			if (!NumDecalSlots) NumDecalSlots = 6;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuTexture, "DECAL_RIGHTDOOR_SLOT%d", i);
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 128 + i, 1, bStringHash(DecalMenuTexture), bStringHash(DecalMenuString));
			}
			break;


		case 0x10003: // Left Panel Layout Selection
			ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x20006, 0, CT_bStringHash("DECAL_LREAR_QP_LAYOUT2"), CT_bStringHash("DECAL_LAYOUT1"));
			WideBodyPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 6);
			if (!WideBodyPart)
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x20007, 0, CT_bStringHash("DECAL_LREAR_QP_LAYOUT1"), CT_bStringHash("DECAL_LAYOUT2"));
			else if ((*((BYTE*)WideBodyPart + 5) & 0xE0) == 0)
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x20007, 0, CT_bStringHash("DECAL_LREAR_QP_LAYOUT1"), CT_bStringHash("DECAL_LAYOUT2"));
			ChooseDecalCategory[10] = CT_bStringHash("HELP_DECAL_SUBMENU");
			break;

		case 0x10004: // Right Panel Layout Selection
			ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x20008, 0, CT_bStringHash("DECAL_RREAR_QP_LAYOUT2"), CT_bStringHash("DECAL_LAYOUT1"));
			WideBodyPart = RideInfo_GetPart((DWORD*)gTheRideInfo, 6);
			if (!WideBodyPart)
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x20009, 0, CT_bStringHash("DECAL_RREAR_QP_LAYOUT1"), CT_bStringHash("DECAL_LAYOUT2"));
			else if ((*((BYTE*)WideBodyPart + 5) & 0xE0) == 0)
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x20009, 0, CT_bStringHash("DECAL_RREAR_QP_LAYOUT1"), CT_bStringHash("DECAL_LAYOUT2"));
			ChooseDecalCategory[10] = CT_bStringHash("HELP_DECAL_SUBMENU");
			break;

		case 0x10005: // Hood Layout Selection
			ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x2000A, 0, CT_bStringHash("DECAL_HOOD_LAYOUT1"), CT_bStringHash("DECAL_LAYOUT1"));
			ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 0x2000B, 0, CT_bStringHash("DECAL_HOOD_LAYOUT2"), CT_bStringHash("DECAL_LAYOUT2"));
			ChooseDecalCategory[10] = CT_bStringHash("HELP_DECAL_SUBMENU");
			break;

		case 0x10006: // Windshield
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 53); // DECAL_FRONT_WINDOW
			if (!NumDecalSlots) NumDecalSlots = 1;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 104 + i, 1, CT_bStringHash("DECAL_ZONE_WINDSHIELD"), bStringHash(DecalMenuString));
			}
			ChooseDecalCategory[10] = CT_bStringHash("HELP_DECAL_SUBMENU");
			break;

		case 0x10007: // Rear Window
			NumDecalSlots = CarCustomizeManager_GetNumDecalsForInstalledLayoutPart((DWORD*)gCarCustomizeManager, 54); // DECAL_REAR_WINDOW
			if (!NumDecalSlots) NumDecalSlots = 1;

			for (int i = 1; i <= NumDecalSlots; i++)
			{
				sprintf(DecalMenuString, "DECAL_SLOT%d", i);
				ChooseDecalCategory_AddCategoryOption(ChooseDecalCategory, 112 + i, 1, CT_bStringHash("DECAL_ZONE_REARWINDOW"), bStringHash(DecalMenuString));
			}
			ChooseDecalCategory[10] = CT_bStringHash("HELP_DECAL_SUBMENU");
			break;

		case 0: // Decals Main
		default:
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10006, 0, CT_bStringHash("DECAL_ZONE_WINDSHIELD"), CT_bStringHash("DECAL_ZONE_WINDSHIELD"));
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10007, 0, CT_bStringHash("DECAL_ZONE_REARWINDOW"), CT_bStringHash("DECAL_ZONE_REARWINDOW"));
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10005, 0, CT_bStringHash("DECAL_ZONE_HOOD"), CT_bStringHash("DECAL_ZONE_HOOD"));
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10001, 0, CT_bStringHash("DECAL_ZONE_LEFTDOOR"), CT_bStringHash("DECAL_ZONE_LEFTDOOR"));
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10002, 0, CT_bStringHash("DECAL_ZONE_RIGHTDOOR"), CT_bStringHash("DECAL_ZONE_RIGHTDOOR"));
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10003, 0, CT_bStringHash("DECAL_ZONE_LEFTPANEL"), CT_bStringHash("DECAL_ZONE_LEFTPANEL"));
			ChooseDecalCategory_AddCategoryOptionLock(ChooseDecalCategory, 0x10004, 0, CT_bStringHash("DECAL_ZONE_RIGHTPANEL"), CT_bStringHash("DECAL_ZONE_RIGHTPANEL"));
			break;
		}

		(*(void(__thiscall**)(DWORD*, DWORD))(ChooseDecalCategory[19] + 32))(
			ChooseDecalCategory + 19,
			*((unsigned __int8*)ChooseDecalCategory + *((__int16*)ChooseDecalCategory + 171) + 356));
		if (*(DWORD*)0x850078 == 1)
		{
			ChooseDecalCategory_ShowTotalPaintCost(ChooseDecalCategory);
			ChooseDecalCategory_UpdateTotalPaintCost(ChooseDecalCategory);
		}
		else
		{
			ChooseDecalCategory_HideTotalPaintCost(ChooseDecalCategory);
		}
		(*(void(__thiscall**)(DWORD*))(*ChooseDecalCategory + 16))(ChooseDecalCategory);
		v9 = ChooseDecalCategory[20];               // IsHead
		if (v9)
			v10 = v9 - 4;
		else
			v10 = 0;
		if (ChooseDecalCategory[22] == v10)
		{
			FEngSetInvisible((DWORD*)ChooseDecalCategory[51]);
		}
		else
		{
			v11 = ChooseDecalCategory[21];            // IsTail
			if (v11)
				v12 = v11 - 4;
			else
				v12 = 0;
			if (ChooseDecalCategory[22] == v12)
			{
				FEngSetInvisible((DWORD*)ChooseDecalCategory[52]);
			}
			else
			{
				FEngSetVisible((DWORD*)ChooseDecalCategory[51]);
				FEngSetVisible((DWORD*)ChooseDecalCategory[52]);
			}
		}
	}
}