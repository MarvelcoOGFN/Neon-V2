#pragma once
#include "pch.h"

#include "Engine/Runtime/AIModule/Classes/BehaivorTree/BehaivorTree.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortAthenaMutator_Bots.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerPawn.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTag.h"


class AFortAIDirector : public AActor
{
public:
    DECLARE_STATIC_CLASS(AFortAIDirector);
    DECLARE_DEFAULT_OBJECT(AFortAIDirector);
};

class AFortAIGoalManager : public AActor
{
public:
    DECLARE_STATIC_CLASS(AFortAIGoalManager)
    DECLARE_DEFAULT_OBJECT(AFortAIGoalManager)
};

struct FFortAthenaAIBotRunTimeCustomizationData final
{
public:
    FGameplayTag                           PredefinedCosmeticSetTag;                          // 0x0000(0x0008)(Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float                                         CullDistanceSquared;                               // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    bool                                          bCheckForOverlaps;                                 // 0x000C(0x0001)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         bHasCustomSquadId : 1;                             // 0x000D(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
    uint8                                         CustomSquadId;                                     // 0x000E(0x0001)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         Pad_1C77[0x1];                                     // 0x000F(0x0001)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UFortBotNameSettings : public UObject
{
    
};

class UFortAthenaAIBotCharacterCustomization : public UObject
{
public:
    DEFINE_MEMBER(FFortAthenaLoadout, UFortAthenaAIBotCharacterCustomization, CustomizationLoadout)
};

class UFortAthenaAIBotInventoryItems final : public UObject
{
public:
    DEFINE_MEMBER(TArray<UFortItemDefinition *>, UFortAthenaAIBotInventoryItems, Items);
};

struct FConstructionBuildingInfo final
{
public:
    TSubclassOf<class ABuildingSMActor>           BuildingActorClass[0x3];                           // 0x0000(0x0008)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class UFortAthenaAIBotCustomizationData final : public UObject
{
public:
    DEFINE_BOOL(UFortAthenaAIBotCharacterCustomization, bOverrideConstructionBuildingInfo)
    DEFINE_MEMBER(FConstructionBuildingInfo, UFortAthenaAIBotCustomizationData, OverrideConstructionBuildingInfo)
    DEFINE_PTR(UFortBotNameSettings, UFortAthenaAIBotCustomizationData, BotNameSettings);   
    DEFINE_PTR(UFortAthenaAIBotInventoryItems, UFortAthenaAIBotCustomizationData, StartupInventory);
    DEFINE_MEMBER(TSubclassOf<class AFortPlayerPawn>, UFortAthenaAIBotCustomizationData, PawnClass)
    DEFINE_PTR(UBehaviorTree, UFortAthenaAIBotCustomizationData, BehaviorTree)
    DEFINE_PTR(UFortAthenaAIBotCharacterCustomization, UFortAthenaAIBotCustomizationData, CharacterCustomization)
    DEFINE_MEMBER(float, UFortAthenaAIBotCustomizationData, SkillLevel)
    DEFINE_MEMBER(uint8, UFortAthenaAIBotCharacterCustomization, CustomSquadId)
    DEFINE_BOOL(UFortAthenaAIBotCharacterCustomization, bOverrideBotNameSettings)
    DEFINE_BOOL(UFortAthenaAIBotCharacterCustomization, bOverrideStartupInventory)
    DEFINE_BOOL(UFortAthenaAIBotCharacterCustomization, bHasCustomSquadId)
    DEFINE_MEMBER(FString, UFortAthenaAIBotCharacterCustomization, BotAnalyticsSuffix)
    DEFINE_BOOL(UFortAthenaAIBotCharacterCustomization, bOverrideBotIDAnalyticsSuffix)
    DEFINE_BOOL(UFortAthenaAIBotCharacterCustomization, bRequiresUniqueNetId)
};

class UAthenaAIPopulationTracker : public UObject
{
public:
    DECLARE_STATIC_CLASS(UAthenaAIPopulationTracker)
    DECLARE_DEFAULT_OBJECT(UAthenaAIPopulationTracker)
};

class UFortServerBotManagerAthena : public UObject
{
public:
    static void Init();
    static void SpawnAI();
    DefineUOriginal(AFortPlayerPawn*, SpawnBot, UFortServerBotManagerAthena *BotManager, FVector SpawnLoc, FRotator SpawnRot, UFortAthenaAIBotCustomizationData *BotData, FFortAthenaAIBotRunTimeCustomizationData &RuntimeBotData);
public:
    DEFINE_MEMBER(FConstructionBuildingInfo, UFortServerBotManagerAthena, ConstructionBuildingInfo)
    DEFINE_PTR(AFortGameModeAthena, UFortServerBotManagerAthena, CachedGameMode)
    DEFINE_PTR(AFortGameStateAthena, UFortServerBotManagerAthena, CachedGameState)
    DEFINE_PTR(AFortAthenaMutator_Bots, UFortServerBotManagerAthena, CachedBotMutator)
    DEFINE_PTR(UAthenaAIPopulationTracker, UFortServerBotManagerAthena, CachedAIPopulationTracker)
public:
    DECLARE_STATIC_CLASS(UFortServerBotManagerAthena)
    DECLARE_DEFAULT_OBJECT(UFortServerBotManagerAthena)
};
