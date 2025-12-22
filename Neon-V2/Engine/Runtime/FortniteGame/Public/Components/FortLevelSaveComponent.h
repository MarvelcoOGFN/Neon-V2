#pragma once
#include "pch.h"


enum class EFortCreativePlotPermission : uint8
{
    Private = 0,
    Public = 1,
    EFortCreativePlotPermission_MAX = 2,
};

struct FFortCreativePlotPermissionData final
{
public:
    EFortCreativePlotPermission                   Permission;                                        // 0x0000(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         Pad_1[0x7];                                        // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
    TArray<class FString>                         WhiteList;                                         // 0x0008(0x0010)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, NativeAccessSpecifierPublic)
};

class UFortLevelSaveComponent : public UActorComponent
{
public:
    DEFINE_BOOL(UFortLevelSaveComponent, bIsLoaded)
    DEFINE_MEMBER(FFortCreativePlotPermissionData, UFortLevelSaveComponent, PlotPermissions)
    DEFINE_MEMBER(FUniqueNetIdRepl, UFortLevelSaveComponent, AccountIdOfOwner)
    DECLARE_STATIC_CLASS(UFortLevelSaveComponent)
    DECLARE_DEFAULT_OBJECT(UFortLevelSaveComponent)
};
