#pragma once
#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/FortPlaylist.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UFortPlaylistAthena : public UFortPlaylist
{
public:
    DEFINE_BOOL(UFortPlaylistAthena, bAutoAcquireSpawnChip)
    DEFINE_MEMBER(FString, UFortPlaylistAthena, RatingType)
    DEFINE_BOOL(UFortPlaylistAthena, bIsTournament)
    DEFINE_MEMBER(int32,UFortPlaylistAthena, PlaylistId);
    DEFINE_BOOL(UFortPlaylistAthena, bLimitedTimeMode)
    DEFINE_MEMBER(FName, UFortPlaylistAthena, PlaylistName);
    DEFINE_MEMBER( FGameplayTagContainer,UFortPlaylist,                  GameplayTagContainer)
    DEFINE_MEMBER(uint8, UFortPlaylistAthena, AirCraftBehavior);
public:
    DECLARE_STATIC_CLASS(UFortPlaylistAthena);
    DECLARE_DEFAULT_OBJECT(UFortPlaylistAthena)
};
