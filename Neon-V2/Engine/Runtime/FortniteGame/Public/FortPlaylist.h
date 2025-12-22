#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"

class UFortPlaylist : public UObject
{
public:
    DEFINE_MEMBER(float, UFortPlaylist, GarbageCollectionFrequency); // 0x0280(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    DEFINE_MEMBER(TSoftObjectPtr<UDataTable>, UFortPlaylist, LootPackages); // 0x0280(0x0028)(Edit, DisableEditOnInstance, Protected, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierProtected)
    DEFINE_MEMBER(TSoftObjectPtr<UDataTable>, UFortPlaylist, LootTierData); // 0x0280(0x0028)(Edit, DisableEditOnInstance, Protected, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierProtected)DEFINE_MEMBER(FName, UFortPlaylist, PlaylistName);
    DEFINE_MEMBER(TSoftObjectPtr<class UCurveTable>, UFortPlaylist, GameData);
    DEFINE_MEMBER(int32, UFortPlaylist, MaxTeamSize);
    DEFINE_MEMBER(int32, UFortPlaylist, MaxSquadSize);
    DEFINE_MEMBER(int32, UFortPlaylist, MaxTeamCount);
    DEFINE_MEMBER(int32, UFortPlaylist, MaxPlayers);
public:
    DECLARE_DEFAULT_OBJECT(UFortPlaylist)
    DECLARE_STATIC_CLASS(UFortPlaylist)
};
