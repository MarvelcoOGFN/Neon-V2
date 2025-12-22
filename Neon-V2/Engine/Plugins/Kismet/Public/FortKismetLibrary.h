#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortAmmoItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortBuildingItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortResourceItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortTrapItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponMeleeItemDefinition.h"

struct FFortLootTierData
{
    DEFINE_MEMBER(float, FFortLootPackageData, Weight);
    DEFINE_MEMBER(FName, FFortLootTierData, LootPackage);
    DEFINE_MEMBER(int32, FFortLootTierData, LootTier);
    DEFINE_MEMBER(FName, FFortLootTierData, TierGroup);
    DEFINE_MEMBER(TArray<int32>, FFortLootTierData, LootPackageCategoryMaxArray)
    DEFINE_MEMBER(float, FFortLootTierData, NumLootPackageDrops);
    DEFINE_MEMBER(TArray<int32>, FFortLootTierData, LootPackageCategoryWeightArray)
    DEFINE_MEMBER(TArray<int32>, FFortLootTierData, LootPackageCategoryMinArray)
};

struct FFortLootPackageData
{
    DEFINE_MEMBER(float, FFortLootPackageData, Weight);
    DEFINE_MEMBER(TSoftObjectPtr<class UFortItemDefinition>, FFortLootPackageData, ItemDefinition);
    DEFINE_MEMBER(FName, FFortLootPackageData, LootPackageID);
    DEFINE_MEMBER(int32, FFortLootPackageData, LootPackageCategory);
    DEFINE_MEMBER(int32, FFortLootPackageData, MinWorldLevel);
    DEFINE_MEMBER(int32, FFortLootPackageData, Count);
    DEFINE_MEMBER(int32, FFortLootPackageData, MaxWorldLevel);
    DEFINE_MEMBER(FString, FFortLootPackageData, LootPackageCall);
};

template <typename T>
inline T *PickWeighted(TArray<T *> &Map, float (*RandFunc)(float), bool bCheckZero = true)
{
    if (Map.Num() == 0)
    {
        return nullptr;
    }

    float TotalWeight = 0.0f;
    for (auto &p : Map)
    {
        if (p != nullptr)
        {
            TotalWeight += p->GetWeight();
        }
    }

    if (TotalWeight <= 0.0f)
    {
        return nullptr;
    }

    float RandomNumber = RandFunc(TotalWeight);

    for (auto &Element : Map)
    {
        if (!Element)
        {
            continue;
        }

        float Weight = Element->GetWeight();
        if (bCheckZero && Weight <= 0)
            continue;

        if (RandomNumber <= Weight)
        {
            return Element;
        }

        RandomNumber -= Weight;
    }

    return nullptr;
}

enum class EFortQuickBars : uint8
{
    Primary = 0,
    Secondary = 1,
    Creative = 2,
    Max_None = 3,
    EFortQuickBars_MAX = 4,
};

static EFortQuickBars GetQuickbar(UFortItemDefinition *ItemDefinition)
{
    if (!ItemDefinition)
        return EFortQuickBars::Max_None;
    if (!ItemDefinition->IsA<UFortWeaponMeleeItemDefinition>() &&
        !ItemDefinition->IsA<UFortBuildingItemDefinition>() &&
        !ItemDefinition->IsA<UFortAmmoItemDefinition>() &&
        !ItemDefinition->IsA<UFortResourceItemDefinition>() &&
        !ItemDefinition->IsA<UFortTrapItemDefinition>())
    {
        return EFortQuickBars::Primary;
    }

    return EFortQuickBars::Secondary;
}

class UBlueprintGeneratedClass : public UClass
{
public:
    DECLARE_STATIC_CLASS(UBlueprintGeneratedClass)
    DECLARE_DEFAULT_OBJECT(UBlueprintGeneratedClass)
};

class ABGAConsumableSpawner : public AActor
{
public:
    DEFINE_MEMBER(FName, ABGAConsumableSpawner, SpawnLootTierGroup);
    DECLARE_STATIC_CLASS(ABGAConsumableSpawner)
    DECLARE_DEFAULT_OBJECT(ABGAConsumableSpawner)
};

class UBGAConsumableWrapperItemDefinition : public UFortItemDefinition
{
public:
    DEFINE_MEMBER(TSoftClassPtr<class UClass>, UBGAConsumableWrapperItemDefinition, ConsumableClass)

public:
    DECLARE_STATIC_CLASS(UBGAConsumableWrapperItemDefinition)
    DECLARE_DEFAULT_OBJECT(UBGAConsumableWrapperItemDefinition)
};

class UFortKismetLibrary : public UObject
{
public:
    static void SpawnFloorLootForContainer(UBlueprintGeneratedClass *ContainerType);
    static TArray<FFortItemEntry *> PickLootDrops(FName TierGroup, int LootTier, int WorldLevel);
    static void SpawnBGAConsumables();
    DefineUOriginal(void, GiveItemToInventoryOwner, UFortKismetLibrary *, FFrame &Stack);
    DefineUOriginal(void, K2_GiveItemToPlayer, UFortKismetLibrary *, FFrame &Stack);
    DefineUOriginal(AFortPickup *, K2_SpawnPickupInWorld, UFortKismetLibrary *, FFrame &Stack, AFortPickup **Ret);

    static void UpdatePlayerCustomCharacterPartsVisualization(class AFortPlayerStateAthena *PlayerState)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortKismetLibrary", "UpdatePlayerCustomCharacterPartsVisualization");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("FortKismetLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &PlayerState);
    }

    static class UFortWorldItemDefinition *K2_GetResourceItemDefinition(const enum EFortResourceType ResourceType)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortKismetLibrary", "K2_GetResourceItemDefinition");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        struct
        {
            EFortResourceType type;
            class UFortWorldItemDefinition *ret;
        } Params{ResourceType};
        static UObject *Object = nullptr;
        if (!Object)
            Object = SDK::StaticClassImpl("FortKismetLibrary")->GetClassDefaultObject();

        Object->ProcessEvent(Func, &Params);

        return Params.ret;
    }

    bool PickLootDropsHook(FFrame &Stack);
DefineUOriginal(class AFortAIDirector *, GetAIDirector, UFortKismetLibrary *, FFrame &Stack, AFortAIDirector **Ret)
    DefineUOriginal(class AFortAIGoalManager *, GetAIGoalManager, UFortKismetLibrary *, FFrame &Stack, AFortAIGoalManager **Ret) public : DECLARE_STATIC_CLASS(UFortKismetLibrary);
    DECLARE_DEFAULT_OBJECT(UFortKismetLibrary);
};
