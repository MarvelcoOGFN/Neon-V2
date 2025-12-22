#pragma once
#include "pch.h"

#include "Controller.h"
#include "Engine/Runtime/Engine/Core/Public/Math/Transform.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortGameStateAthena.h"


class AGameSession : public UObject
{
public:
    DEFINE_MEMBER(int32, AGameSession, MaxPlayers);
public:
    DECLARE_STATIC_CLASS(AGameSession)
    DECLARE_DEFAULT_OBJECT(AGameSession)
};

class AGameModeBase : public UObject
{
public:
    DEFINE_PTR(AGameSession, AGameModeBase, GameSession);
    DEFINE_PTR(AFortGameStateAthena, AGameModeBase, GameState);
    DEFINE_MEMBER(TSubclassOf<class AGameSession>, AGameModeBase, GameSessionClass)
public:
    void RestartPlayer(AController* Controller)
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("GameModeBase", "RestartPlayer").Func;
        if (!Func) return;
    
        struct { AController* NewPlayer; } Params{ Controller };
        this->ProcessEvent(Func, &Params);
    }

    APawn* SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("GameModeBase", "SpawnDefaultPawnAtTransform").Func;
        if (!Func) return nullptr;
    
        struct { AController* NewPlayer; FTransform SpawnTransform; APawn* ReturnValue; } Params{ NewPlayer, SpawnTransform };
        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }
public:
    DECLARE_STATIC_CLASS(AGameModeBase)
    DECLARE_DEFAULT_OBJECT(AGameModeBase)
};

class AGameMode : public AGameModeBase
{
public:
    DEFINE_MEMBER(FName, AGameMode, MatchState);
public:
    DECLARE_STATIC_CLASS(AGameMode)
    DECLARE_DEFAULT_OBJECT(AGameMode)
};