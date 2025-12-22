#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/GameMode.h"

class AFortGameMode : public AGameMode
{
public:
    DEFINE_BOOL(AFortGameMode, bDBNOEnabled)
    DEFINE_MEMBER(FString, AFortGameMode, CurrentBucketId);
    DEFINE_MEMBER(int32, AFortGameMode, CurrentPlaylistId);
    DEFINE_MEMBER(FName, AFortGameMode, CurrentPlaylistName);
    DEFINE_BOOL(AFortGameMode, bWorldIsReady);
    DEFINE_BOOL(AFortGameMode, bEnableReplicationGraph)
};
