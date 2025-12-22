#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"

struct FSoftClassPath final : public FSoftObjectPath
{
};

struct alignas(0x04) FNavAgentSelector final
{
public:
	uint8                                         bSupportsAgent0 : 1;                               // 0x0000(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent1 : 1;                               // 0x0000(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent2 : 1;                               // 0x0000(0x0001)(BitIndex: 0x02, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent3 : 1;                               // 0x0000(0x0001)(BitIndex: 0x03, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent4 : 1;                               // 0x0000(0x0001)(BitIndex: 0x04, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent5 : 1;                               // 0x0000(0x0001)(BitIndex: 0x05, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent6 : 1;                               // 0x0000(0x0001)(BitIndex: 0x06, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent7 : 1;                               // 0x0000(0x0001)(BitIndex: 0x07, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent8 : 1;                               // 0x0001(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent9 : 1;                               // 0x0001(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent10 : 1;                              // 0x0001(0x0001)(BitIndex: 0x02, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent11 : 1;                              // 0x0001(0x0001)(BitIndex: 0x03, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent12 : 1;                              // 0x0001(0x0001)(BitIndex: 0x04, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent13 : 1;                              // 0x0001(0x0001)(BitIndex: 0x05, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent14 : 1;                              // 0x0001(0x0001)(BitIndex: 0x06, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bSupportsAgent15 : 1;                              // 0x0001(0x0001)(BitIndex: 0x07, PropSize: 0x0001 (Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         Pad_2[0x2];                                        // 0x0002(0x0002)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UNavigationSystemConfig : public UObject
{
public:
	FSoftClassPath                         NavigationSystemClass;                             // 0x0028(0x0018)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FNavAgentSelector                      SupportedAgentsMask;                               // 0x0040(0x0004)(Edit, NoDestructor, NativeAccessSpecifierPublic)
	class FName                                   DefaultAgentName;                                  // 0x0044(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         bIsOverriden : 1;                                  // 0x004C(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, EditConst, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         Pad_4D[0x3];                                       // 0x004D(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UNavigationSystemModuleConfig : public UNavigationSystemConfig
{
public:
	uint8                                         bStrictlyStatic : 1;                               // 0x0050(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bCreateOnClient : 1;                               // 0x0050(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bAutoSpawnMissingNavData : 1;                      // 0x0050(0x0001)(BitIndex: 0x02, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bSpawnNavDataInNavBoundsLevel : 1;                 // 0x0050(0x0001)(BitIndex: 0x03, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         Pad_51[0x7];                                       // 0x0051(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UFortNavSystemConfig : public UNavigationSystemModuleConfig
{
public:
	uint8                                         bAllowAutoRebuild : 1;                             // 0x0058(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bRebuildOnInitialUnlock : 1;                       // 0x0058(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bUsesStreamedInNavLevel : 1;                       // 0x0058(0x0001)(BitIndex: 0x02, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         Pad_59[0x7];                                       // 0x0059(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UAthenaNavSystemConfig final : public UFortNavSystemConfig
{
public:
	TArray<struct FNavDataSetVariantSettings>     NavDataSetVariants;                                // 0x0060(0x0010)(Edit, ZeroConstructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	uint8                                         bUseNavDataSetVariants : 1;                        // 0x0070(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bUseBuildingGridAsNavigableSpace : 1;              // 0x0070(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bDiscardNavDataFromSublevels : 1;                  // 0x0070(0x0001)(BitIndex: 0x02, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bUseNavigationInvokers : 1;                        // 0x0070(0x0001)(BitIndex: 0x03, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bLazyOctree : 1;                                   // 0x0070(0x0001)(BitIndex: 0x04, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bUseNavOctTreeInclusionBounds : 1;                 // 0x0070(0x0001)(BitIndex: 0x05, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bPrioritizeNavigationAroundSpawners : 1;           // 0x0070(0x0001)(BitIndex: 0x06, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bResetDirtyAreasOnInitialBuildingRelease : 1;      // 0x0070(0x0001)(BitIndex: 0x07, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         bSupportRuntimeNavmeshDisabling : 1;               // 0x0071(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected))
	uint8                                         Pad_72[0x6];                                       // 0x0072(0x0006)(Fixing Struct Size After Last Property [ Dumper-7 ])
public:
	DefineUOriginal(void, CreateAndConfigureNavigationSystem, UAthenaNavSystemConfig*, UWorld*);
};