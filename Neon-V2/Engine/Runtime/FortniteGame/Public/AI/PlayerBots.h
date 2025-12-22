#pragma once
#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/FortAthenaMutator_Bots.h"

struct alignas(0x08) FMMRSpawningInfo final
{
    public:
    float BotSpawningDataInfoTargetELO;
    float BotSpawningDataInfoWeight;
    int NumBotsToSpawn;
    class TSubclassOf<class UFortAthenaAISpawnerData> AISpawnerData;
    uint8_t SpawnedCallbacks[0x10];
};

struct FMMRSpawningPlayerBotsRuntimeInfo final : public FMMRSpawningBaseRuntimeInfo
{
public:
    uint8                                         Pad_10[0x10];                                      // 0x0010(0x0010)(Fixing Size After Last Property [ Dumper-7 ])
    TArray<struct FMMRSpawningInfo>               SpawningInfos;                                     // 0x0020(0x0010)(ZeroConstructor, Transient, NativeAccessSpecifierPublic)
    uint8                                         Pad_30[0x8];                                       // 0x0030(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UAthenaAIServicePlayerBots : public UObject
{
public:
    DEFINE_PTR(UObject, UAthenaAIServicePlayerBots, DefaultAISpawnerDataComponentList);
    DEFINE_MEMBER(EAthenaGamePhase, UAthenaAIServicePlayerBots, GamePhaseToStartSpawning)
    DEFINE_BOOL(UAthenaAIServicePlayerBots, bWaitForNavmeshToBeLoaded)
    DEFINE_MEMBER(TSubclassOf<class UFortAthenaAISpawnerData>, UAthenaAIServicePlayerBots, DefaultBotAISpawnerData)
    DEFINE_MEMBER(FMMRSpawningPlayerBotsRuntimeInfo, UAthenaAIServicePlayerBots, CachedMMRSpawningInfo)
public:
    DECLARE_STATIC_CLASS(UAthenaAIServicePlayerBots)
    DECLARE_DEFAULT_OBJECT(UAthenaAIServicePlayerBots)
};