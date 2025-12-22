#pragma once
#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerPawn.h"

struct FFortSearchBounceData final
{
public:
    struct FVector BounceNormal; // 0x0000(0x000C)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint32 SearchAnimationCount; // 0x000C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class ABuildingContainer : public AActor
{
public:
    DEFINE_MEMBER(FRotator, ABuildingContainer, LootTossDirection_Athena);
    DEFINE_MEMBER(float, ABuildingContainer, LootTossSpeed_Athena);
    DEFINE_MEMBER(float, ABuildingContainer, LootTossConeHalfAngle_Athena);
    DEFINE_MEMBER(int32, ABuildingContainer, ReplicatedLootTier);
    DEFINE_MEMBER(FVector, ABuildingContainer, LootSpawnLocation_Athena);
    DEFINE_BOOL(ABuildingContainer, bAlreadySearched);
    DEFINE_MEMBER(FName, ABuildingContainer, SearchLootTierGroup);

public:
    void OnRep_LootTier()
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingContainer", "OnRep_LootTier");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    bool BP_SpawnLoot(AFortPlayerPawn *PlayerPawn)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingContainer", "BP_SpawnLoot");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        struct BuildingContainer_BP_SpawnLoot final
        {
        public:
            AFortPlayerPawn *PlayerPawn;
            bool ReturnValue;
        };
        BuildingContainer_BP_SpawnLoot Params;
        Params.PlayerPawn = PlayerPawn;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    bool SpawnLoot();

public:
    DECLARE_STATIC_CLASS(ABuildingContainer)
    DECLARE_DEFAULT_OBJECT(ABuildingContainer)
};
