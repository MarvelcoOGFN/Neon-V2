#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerPawn.h"

struct alignas(0x08) FMMRSpawningBaseRuntimeInfo
{
public:
    uint8 Pad_0[0x10]; // 0x0000(0x0010)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UFortAthenaMutator_SpawningPolicyData : public UDataAsset
{
public:
    DEFINE_MEMBER(FScalableFloat, UFortAthenaMutator_SpawningPolicyData, NumItemsToSpawn); // 0x0000(0x0020)(Edit, DisableEditOnInstance, NativeAccessSpecifierPrivate)
public:
    DECLARE_STATIC_CLASS(UFortAthenaMutator_SpawningPolicyData)
    DECLARE_DEFAULT_OBJECT(UFortAthenaMutator_SpawningPolicyData)
};

class UFortAthenaMutator_PlayerBotSpawningPolicyData : public UFortAthenaMutator_SpawningPolicyData
{
public:
    DECLARE_STATIC_CLASS(UFortAthenaMutator_PlayerBotSpawningPolicyData)
    DECLARE_DEFAULT_OBJECT(UFortAthenaMutator_PlayerBotSpawningPolicyData)
};

class UBotELOSpawningInfo final : public UFortAthenaMutator_SpawningPolicyData
{
public:
    DEFINE_PTR(UFortAthenaMutator_SpawningPolicyData, UBotELOSpawningInfo, BotSpawningDataInfo);
    DEFINE_PTR(class AFortGameModeAthena, UBotELOSpawningInfo, CachedGameMode);

public:
    DECLARE_STATIC_CLASS(UBotELOSpawningInfo)
    DECLARE_DEFAULT_OBJECT(UBotELOSpawningInfo)
};

struct FMMRSpawningBotsRuntimeInfo final : public FMMRSpawningBaseRuntimeInfo
{
public:
    uint8 Pad_8[0x10];                                    // 0x0008(0x0010)(Fixing Size After Last Property [ Dumper-7 ])
    TArray<class UBotELOSpawningInfo *> ELOSpawningInfos; // 0x0018(0x0010)(ZeroConstructor, Transient, NativeAccessSpecifierPublic)
    uint8 Pad_28[0x8];                                    // 0x0028(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FSpawningInfo
{
public:
    uint8 Pad_4477[0x8];                                        // Fixing Size After Last Property  [ Dumper-7 ]
    class UFortAthenaMutator_SpawningPolicyData *ItemSpawnData; // 0x8(0x8)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class AFortAthenaMutator_SpawningPolicyBase : public AActor
{
public:
    DEFINE_MEMBER(EAthenaGamePhase, AFortAthenaMutator_SpawningPolicyBase, GamePhaseToStartSpawning)
    DEFINE_BOOL(AFortAthenaMutator_SpawningPolicyBase, bWaitForNavmeshToBeLoaded);
    DEFINE_MEMBER(TArray<struct FSpawningInfo>, AFortAthenaMutator_SpawningPolicyBase, QueuedSpawnInfo)
public:
    DECLARE_STATIC_CLASS(AFortAthenaMutator_SpawningPolicyBase)
    DECLARE_DEFAULT_OBJECT(AFortAthenaMutator_SpawningPolicyBase)
};

class AFortAthenaMutator_Bots : public AFortAthenaMutator_SpawningPolicyBase
{
public:
    DEFINE_MEMBER(TArray<class UFortAthenaMutator_PlayerBotSpawningPolicyData *>, AFortAthenaMutator_Bots, BotSpawningDataItems)
    DEFINE_MEMBER(FMMRSpawningBotsRuntimeInfo, AFortAthenaMutator_Bots, CachedMMRSpawningInfo);
    AFortPlayerPawn *SpawnBot(TSubclassOf<class AFortPlayerPawn> BotPawnClass, const class AActor *InSpawnLocator, const struct FVector &InSpawnLocation, const struct FRotator &InSpawnRotation, const bool bSnapToGround)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaMutator_Bots", "SpawnBot");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        struct FortAthenaMutator_Bots_SpawnBot final
        {
        public:
            TSubclassOf<class AFortPlayerPawn> BotPawnClass; // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            const class AActor *InSpawnLocator;              // 0x0008(0x0008)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FVector InSpawnLocation;                  // 0x0010(0x000C)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FRotator InSpawnRotation;                 // 0x001C(0x000C)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
            bool bSnapToGround;                              // 0x0028(0x0001)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            class AFortPlayerPawn *ReturnValue;              // 0x0030(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } FortAthenaMutator_Bots_SpawnBot_Params{BotPawnClass, InSpawnLocator, InSpawnLocation, InSpawnRotation, bSnapToGround};

        this->ProcessEvent(Func, &FortAthenaMutator_Bots_SpawnBot_Params);

        return FortAthenaMutator_Bots_SpawnBot_Params.ReturnValue;
    }

public:
    DECLARE_STATIC_CLASS(AFortAthenaMutator_Bots)
    DECLARE_DEFAULT_OBJECT(AFortAthenaMutator_Bots)
};

DefineOriginal(inline void, MutatorInitializeMMRInfos, AFortAthenaMutator_Bots *a1);
inline void MutatorInitializeMMRInfos(AFortAthenaMutator_Bots *a1)
{
    a1->GetBotSpawningDataItems().Add(StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C"));
    UBotELOSpawningInfo *PhoebeMMRInfo = (UBotELOSpawningInfo *)UBotELOSpawningInfo::GetDefaultObj();
   // PhoebeMMRInfo->SetCachedGameMode(GetWorld()->GetAuthorityGameMode());
    PhoebeMMRInfo->SetNumItemsToSpawn(FScalableFloat(1400.0f));
    PhoebeMMRInfo->SetBotSpawningDataInfo(StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C"));
    a1->GetCachedMMRSpawningInfo().ELOSpawningInfos.Add(PhoebeMMRInfo);
    *(int32 *)(IMAGEBASE + 0x8C00991) = 1400.f;
    *(int32 *)(IMAGEBASE + 0x680F388) = 70;
    *(int32 *)(IMAGEBASE + 0x6E82A83) = 1;
    a1->SetbWaitForNavmeshToBeLoaded(false);
    a1->SetGamePhaseToStartSpawning(EAthenaGamePhase::Warmup);
    FSpawningInfo Info{};
    Info.ItemSpawnData = StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C");

    a1->GetQueuedSpawnInfo().Add(Info);

    return MutatorInitializeMMRInfosOG(a1);
}

inline UFortAthenaMutator_PlayerBotSpawningPolicyData *Test()
{
    static UFortAthenaMutator_PlayerBotSpawningPolicyData *Ok = StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C");
    return Ok;
}

DefineOriginal(inline void, AFortAthenaMutatorOnSafeZoneUpdated, AFortAthenaMutator_Bots *Bots);
inline void AFortAthenaMutatorOnSafeZoneUpdated(AFortAthenaMutator_Bots *a1)
{
    UE_LOG(LogTemp, Log, "Huh");
    static bool firstCall = true;
    // if (firstCall)
    {
        firstCall = false;
        a1->GetBotSpawningDataItems().Add(StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C"));
        UBotELOSpawningInfo *PhoebeMMRInfo = (UBotELOSpawningInfo *)UBotELOSpawningInfo::GetDefaultObj();
     //   PhoebeMMRInfo->SetCachedGameMode(GetWorld()->GetAuthorityGameMode());
        PhoebeMMRInfo->SetNumItemsToSpawn(FScalableFloat(70.0f));
        PhoebeMMRInfo->SetBotSpawningDataInfo(StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C"));
        a1->GetCachedMMRSpawningInfo().ELOSpawningInfos.Add(PhoebeMMRInfo);
        *(int32 *)(IMAGEBASE + 0x680F38C) = 1400.f;
        *(int32 *)(IMAGEBASE + 0x680F388) = 70;
        *(int32 *)(IMAGEBASE + 0x6E82A83) = 1;
        a1->SetbWaitForNavmeshToBeLoaded(false);
        a1->SetGamePhaseToStartSpawning(EAthenaGamePhase::Warmup);
        FSpawningInfo Info{};
        Info.ItemSpawnData = StaticLoadObject<UFortAthenaMutator_PlayerBotSpawningPolicyData>("/Game/Athena/AI/Phoebe/BP_PhoebeSpawningItemData.BP_PhoebeSpawningItemData_C");

        a1->GetQueuedSpawnInfo().Add(Info);

        ((void (*)(AFortAthenaMutator_Bots *))(IMAGEBASE + 0x1654BB0))(a1);
    }

    AFortAthenaMutatorOnSafeZoneUpdatedOG(a1);
}
