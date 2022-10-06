# NFSU2 Unlimiter
NFSU2 Unlimiter is a script mod which fixes some issues and makes some improvements for added cars and events.

Features:
+ Fixes visibility and crash issues for added cars.
+ Fixes disappearing wheels issue.
+ Expands memory pools.
+ Fixes a crash issue related with world map markers.
+ Adds options to change car render information. (See UnlimiterData\_General.ini file.)
	- LinkLicensePlateToTrunk: Attaches license plate to trunk for part animations.
	- ShowTrunkUnderInFE: Shows inside of the trunk while in menus.
	- RemoveCentreBrakeWithCustomSpoiler: Removes centre brakelight position marker when a custom spoiler is installed.
	- HasSunroof: Swaps KIT00_ROOF with KIT00_FULLROOF when a roof scoop is installed.
+ Fixes missing objects for options and stuff in FNGs by cloning them.
+ Allows the events to have specific engage messages in any stage. (The game did it only for Stage 0 by default.)
+ Adds new customization items to the menu. (Fully configurable via car-specific or general ini files.) (Parts should get added by modders.)
+ Restores cut customization options from the game: Trunk Audio Paint, Cabin Neon, Hood Decals, etc.
+ Lets modders add custom rim brands. (Fully configurable via ini file.)
	- [RimBrands] RemoveRimSizeRestrictions: Removes the outer radius restriction to make any rim available for all cars.
	- [BrandX] NoRimSize: Disables rim size checks for a specific brand.
	- [BrandX] HideBrandName: Hides brand name on rim selection screen for a specific brand.
+ Adds custom attributes to allow even more customization options: (Extra Customization needs to be enabled to make custom attributes work.)
	- SHOWENGINE (Boolean) : Allows the player to see engine and neon through the hood.
	- SHOWTRUNK (Boolean) : Allows the player to see trunk audio components and neon through the trunk.
	- TRUNKUNDER (Key) : Sets the under part of the trunk. Works like the HOODUNDER attribute, which is available for hoods.
	- NUMSLOTS (Integer) : Declares the number of component slots a Trunk Audio Layout has. If not present, the "default" hardcoded values from the game will be used.
	- FRONT_TIRE_OFFSET (Floating) : Adds offset (spacers) to the front wheels. This attribute can be added to Fender and Wide Body parts.
	- REAR_TIRE_OFFSET (Floating) : Adds offset (spacers) to the rear wheels. This attribute can be added to Quarter Panel and Wide Body parts.
+ Implements use of strings for AI Opponent names like later games and adds 150+ names from credits.

Also check out:
- [NFS Mods with Unlimiter (v3+) & Extended Customization Support](https://docs.google.com/spreadsheets/d/1BYqui01raMMtRGrJ63-2B-Agh9ag6RdPB-bd07pDIKI/edit#gid=0)


# Technical Information
If you want everything to work properly:

1) You will need to add these textures in your FrontEnd\\FrontB.lzc, FrontEndTextures.tpk:
```
63694594 - PAINT_MOD_PART_TRUNK_AUDIO
F9BB4F22 - VISUAL_PART_ATTACHMENT1
F9BB4F23 - VISUAL_PART_ATTACHMENT2
F9BB4F24 - VISUAL_PART_ATTACHMENT3
F9BB4F25 - VISUAL_PART_ATTACHMENT4
F9BB4F26 - VISUAL_PART_ATTACHMENT5
541B24A5 - VISUAL_PART_CABIN
7BCFBDE4 - VISUAL_PART_FRONT_FENDER
A882A4B3 - VISUAL_PART_LICENSE_PLATES
0294EC5E - VISUAL_PART_ROOF
5558691C - VISUAL_PART_TRUNK
0DEB2F1C - VISUAL_RIMS_BRAND_ADVAN
0EE6D551 - VISUAL_RIMS_BRAND_OASIS
```

2) You will need to add these strings into your LANGUAGES\\(LanguageName).bin:
```
7BCFBDE4 - VISUAL_PART_FRONT_FENDER
AEBD0675 - VISUAL_PART_REAR_QUARTER
0294EC5E - VISUAL_PART_ROOF
5558691C - VISUAL_PART_TRUNK
541B24A5 - VISUAL_PART_CABIN
DABD392C - PAINT_AUDIO
740E3098 - CUSTOMIZE_CANT_PAINT_STOCK_TRUNK
7D2BCFD7 - UI_ICE_LICENSE_PLATES
5BB4E255 - UI_ICE_NEON_CABIN_LAYOUT
282FF99B - STYLE00_CABIN_NEON
592356FC - STYLE01_CABIN_NEON
8A16B45D - STYLE02_CABIN_NEON
BB0A11BE - STYLE03_CABIN_NEON
F9BB4F22 - VISUAL_PART_ATTACHMENT1
F9BB4F23 - VISUAL_PART_ATTACHMENT2
F9BB4F24 - VISUAL_PART_ATTACHMENT3
F9BB4F25 - VISUAL_PART_ATTACHMENT4
F9BB4F26 - VISUAL_PART_ATTACHMENT5
2D4C819A - RIMS_BRAND_CUSTOM
2FBD9289 - RIMS_BRAND_ADVAN
XXXXXXXX - RACERNAME_001-XXX (To make random AI Opponent names in Quick Race mode work)
```

Note: Binary scripts which will do these automatically for you are available in the release pack.

# Download
You can [download Unlimiter](https://github.com/nlgxzef/NFSU2Unlimiter/releases) from Releases page, or from [NFSMods.xyz](https://www.nfsmods.xyz).
If you want to compile it yourself, you can download the source code from the green Clone or Download button up there.
