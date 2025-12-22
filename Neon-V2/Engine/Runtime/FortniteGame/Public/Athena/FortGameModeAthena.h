#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/NetDriver.h"
#include "Engine/Runtime/FortniteGame/Public/FortGameMode.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"

struct FItemAndCount final
{
public:
    int32                                         Count;                                             // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         Pad_1BE8[0x4];                                     // 0x0004(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
    class UFortItemDefinition*                    Item;                                              // 0x0008(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class AFortSafeZoneIndicator : public UObject
{
public:
    DEFINE_MEMBER(float, AFortSafeZoneIndicator, Radius);
    DEFINE_MEMBER(float, AFortSafeZoneIndicator, SafeZoneStartShrinkTime);
    DEFINE_MEMBER(float, AFortSafeZoneIndicator, SafeZoneFinishShrinkTime);
    DEFINE_MEMBER(FVector_NetQuantize100, AFortSafeZoneIndicator, NextRadius)
    DEFINE_MEMBER(FVector_NetQuantize100, AFortSafeZoneIndicator, LastCenter)
    DEFINE_MEMBER(FVector_NetQuantize100, AFortSafeZoneIndicator, NextCenter)
public:
    DECLARE_STATIC_CLASS(AFortSafeZoneIndicator)
    DECLARE_DEFAULT_OBJECT(AFortSafeZoneIndicator)
};



struct FGameMemberInfo final : public FFastArraySerializerItem
{
public:
    uint8                                         SquadId;                                           // 0x000C(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         TeamIndex;                                         // 0x000D(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         Pad_1554[0x2];                                     // 0x000E(0x0002)(Fixing Size After Last Property [ Dumper-7 ])
    struct FUniqueNetIdRepl                       MemberUniqueId;                                    // 0x0010(0x0028)(HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class AFortGameSessionDedicated : public AActor
{
public:
    DECLARE_STATIC_CLASS(AFortGameSessionDedicated)
    DECLARE_DEFAULT_OBJECT(AFortGameSessionDedicated)
};

class AFortGameModeEmptyDedicated : public AFortGameMode
{
public:
    DECLARE_STATIC_CLASS(AFortGameModeEmptyDedicated)
    DECLARE_DEFAULT_OBJECT(AFortGameModeEmptyDedicated)
};

class ANavigationObjectBase : public AActor {

};

class APlayerStart : public ANavigationObjectBase {

};

class AFortPlayerStartCreative : public APlayerStart
{
public:
    DECLARE_STATIC_CLASS(AFortPlayerStartCreative)
    DECLARE_DEFAULT_OBJECT(AFortPlayerStartCreative)
};

class AFortPlayerStartWarmup : public APlayerStart
{
public:
    DECLARE_STATIC_CLASS(AFortPlayerStartWarmup)
    DECLARE_DEFAULT_OBJECT(AFortPlayerStartWarmup)
};

class AFortAthenaSpawningPolicyManager : public UObject
{
public:
    DEFINE_PTR(class AFortGameModeAthena, AFortAthenaSpawningPolicyManager, GameModeAthena);
    DEFINE_PTR(class AFortGameStateAthena, AFortAthenaSpawningPolicyManager, GameStateAthena);
public:
    DECLARE_STATIC_CLASS(AFortAthenaSpawningPolicyManager)
    DECLARE_DEFAULT_OBJECT(AFortAthenaSpawningPolicyManager)
};

class AFortGameModeAthena : public AFortGameMode
{
public:
    typedef TMap<FName, FName> Redirect;
    DEFINE_BOOL(AFortGameModeAthena, bPlaylistHotfixChangedGCDisabling)
    DEFINE_BOOL(AFortGameModeAthena, bDisableGCOnServerDuringMatch)
        DEFINE_BOOL(AFortGameModeAthena, bSafeZoneActive)

    DEFINE_MEMBER(Redirect, AFortGameModeAthena, RedirectAthenaLootTierGroups);
    DEFINE_PTR(class AFortAIGoalManager, AFortGameModeAthena, AIGoalManager);
    DEFINE_PTR(class AFortAIDirector, AFortGameModeAthena, AIDirector);
    DEFINE_MEMBER(TSubclassOf<class UFortServerBotManagerAthena>, AFortGameModeAthena, ServerBotManagerClass);
    DEFINE_PTR(UFortServerBotManagerAthena, AFortGameModeAthena, ServerBotManager);
    DEFINE_PTR(AFortSafeZoneIndicator, AFortGameModeAthena, SafeZoneIndicator)
    DEFINE_MEMBER(TArray<FItemAndCount>, AFortGameModeAthena, StartingItems)
    DECLARE_DEFAULT_OBJECT(AFortGameModeAthena)
    DECLARE_STATIC_CLASS(AFortGameModeAthena)
    DEFINE_MEMBER(int, AFortGameModeAthena, SafeZonePhase)
    DEFINE_MEMBER(TArray<struct FVector>, AFortGameModeAthena, SafeZoneLocations);
    DEFINE_PTR(AFortAthenaSpawningPolicyManager, AFortGameModeAthena, SpawningPolicyManager)
    DEFINE_MEMBER(TArray<class AFortPlayerControllerAthena*>, AFortGameModeAthena, AlivePlayers);
    DEFINE_MEMBER(TArray<class AFortAthenaAIBotController*>, AFortGameModeAthena, AliveBots);
public:
    DefineUOriginal(void, StartAircraftPhase, AFortGameModeAthena*, char a2);
    DefineUOriginal(EFortTeam, PickTeam, AFortGameModeAthena*, uint8_t PreferredTeam, AFortPlayerControllerAthena* Controller)
    DefineOriginal(bool, ReadyToStartMatch, FFrame& Stack, bool* Result)
    DefineOriginal(APawn*, SpawnDefaultPawnFor, APlayerController* NewPlayer, AActor* StartSpot)
    static UClass** GetGameSessionClass(AFortGameModeAthena*, UClass** Result);
    DefineUOriginal(void, StartNewSafeZonePhase, AFortGameModeAthena*, int NewSafeZonePhase)
    DefineUOriginal(void, HandleStartingNewPlayer, AFortGameModeAthena*, APlayerController* NewPlayer)
};

class AFortAthenaVehicleSpawner : public AActor
{
public:
    DECLARE_STATIC_CLASS(AFortAthenaVehicleSpawner)
    DECLARE_DEFAULT_OBJECT(AFortAthenaVehicleSpawner)
};
