#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"
#include "Engine/Runtime/GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

struct alignas(0x08) FHitResult final
{
public:
    uint8 bBlockingHit : 1;                               // 0x0000(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
    uint8 bStartPenetrating : 1;                          // 0x0000(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
    uint8 Pad_1[0x3];                                     // 0x0001(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
    int32 FaceIndex;                                      // 0x0004(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float Time;                                           // 0x0008(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float Distance;                                       // 0x000C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    struct FVector Location;                              // 0x0010(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
    struct FVector ImpactPoint;                           // 0x001C(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
    struct FVector_NetQuantizeNormal Normal;              // 0x0028(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
    struct FVector_NetQuantizeNormal ImpactNormal;        // 0x0034(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
    struct FVector TraceStart;                            // 0x0040(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
    struct FVector TraceEnd;                              // 0x004C(0x000C)(NoDestructor, NativeAccessSpecifierPublic)
    float PenetrationDepth;                               // 0x0058(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    int32 Item;                                           // 0x005C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    TWeakObjectPtr<class UPhysicalMaterial> PhysMaterial; // 0x0060(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    TWeakObjectPtr<class AActor> Actor;                   // 0x0068(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    TWeakObjectPtr<class UPrimitiveComponent> Component;  // 0x0070(0x0008)(ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    class FName BoneName;                                 // 0x0078(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    class FName MyBoneName;                               // 0x0080(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};
class ABuildingActor : public AActor
{
public:
    float GetHealth()
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingActor", "GetHealth");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0.f;

        struct BuildingActor_GetHealth final
        {
        public:
            float ReturnValue;
        };
        BuildingActor_GetHealth Params;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    float GetMaxHealth()
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingActor", "GetMaxHealth");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0.f;

        struct BuildingActor_GetMaxHealth final
        {
        public:
            float ReturnValue;
        };
        BuildingActor_GetMaxHealth Params;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    void OnRep_Team()
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingActor", "OnRep_Team");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;
        this->ProcessEvent(Func, nullptr);
    }

    void OnSetTeam(uint8 InTeam)
    {
                static class UFunction *Func = nullptr;
                static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingActor", "SetTeam");
        
                if (Func == nullptr)
                    Func = Info.Func;
                if (!Func)
                    return;

        struct BuildingActor_SetTeam final
        {
        public:
            uint8                                         InTeam;                                            // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};
        Params.InTeam = InTeam;
        this->ProcessEvent(Func, &Params);
    }
    
    void InitializeKismetSpawnedBuildingActor(class ABuildingActor *BuildingOwner, class AFortPlayerController *SpawningController, bool bUsePlayerBuildAnimations)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingActor", "InitializeKismetSpawnedBuildingActor");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct BuildingActor_InitializeKismetSpawnedBuildingActor final
        {
        public:
            class ABuildingActor *BuildingOwner;             // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            class AFortPlayerController *SpawningController; // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            bool bUsePlayerBuildAnimations;                  // 0x0010(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_11[0x7];                               // 0x0011(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
        } Params{};

        Params.BuildingOwner = BuildingOwner;
        Params.SpawningController = SpawningController;
        Params.bUsePlayerBuildAnimations = bUsePlayerBuildAnimations;

        this->ProcessEvent(Func, &Params);
    }

public:
    DEFINE_MEMBER(EFortTeam, ABuildingActor, Team)
    DEFINE_MEMBER(uint8, ABuildingActor, TeamIndex)
    DEFINE_MEMBER(FGameplayTagContainer, ABuildingActor, StaticGameplayTags)
    DEFINE_MEMBER(bool, ABuildingActor, bPlayerPlaced)
};

enum EFortResourceType : uint8
{
    Wood = 0,
    Stone = 1,
    Metal = 2,
    Permanite = 3,
    GoldCurrency = 4,
    Ingredient = 5,
    None = 6,
    EFortResourceType_MAX = 7,
};

class ABuildingSMActor : public ABuildingActor
{
public:
    DEFINE_MEMBER(TArray<class ABuildingSMActor*>, ABuildingSMActor, AttachedBuildingActors)
    DEFINE_MEMBER(FCurveTableRowHandle, ABuildingSMActor, BuildingResourceAmountOverride)
    DEFINE_MEMBER(EFortResourceType, ABuildingSMActor, ResourceType)
public:
    void SetMirrored(bool bIsMirrored)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingSMActor", "SetMirrored");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct BuildingSMActor_SetMirrored final
        {
        public:
            bool bIsMirrored; // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};

        Params.bIsMirrored = bIsMirrored;

        this->ProcessEvent(Func, &Params);
    }

    DefineUOriginal(void, OnDamageServer, ABuildingSMActor *, float, FGameplayTagContainer, FVector, FHitResult, AActor *, AActor *, FGameplayEffectContextHandle);

public:
    DECLARE_STATIC_CLASS(ABuildingSMActor)
    DECLARE_DEFAULT_OBJECT(ABuildingSMActor)
};

class ABuildingTrap : public ABuildingSMActor
{
public:
    DECLARE_STATIC_CLASS(ABuildingTrap)
    DECLARE_DEFAULT_OBJECT(ABuildingTrap)
};