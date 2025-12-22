#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/PlayerState.h"

class AFortTeamInfo : public UObject
{
public:
    DEFINE_MEMBER(TArray<class AFortPlayerControllerAthena*>, AFortTeamInfo, TeamMembers); // 0x0280(0x0010)(ZeroConstructor, Protected, UObjectWrapper, NativeAccessSpecifierProtected)
};

class AFortPlayerState : public APlayerState
{
public:

public:
    DEFINE_PTR(class UAbilitySystemComponent, AFortPlayerState, AbilitySystemComponent);
    DEFINE_BOOL(AFortPlayerState,bHasStartedPlaying)
    DEFINE_PTR(AFortTeamInfo, AFortPlayerState, PlayerTeam); 
};

class AFortPlayerStateZone : public AFortPlayerState
{
public:
    DECLARE_STATIC_CLASS(AFortPlayerStateZone)
    DECLARE_DEFAULT_OBJECT(AFortPlayerStateZone)
};
