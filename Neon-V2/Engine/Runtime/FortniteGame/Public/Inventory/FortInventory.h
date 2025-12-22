#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/NetSerialization.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/Engine/Core/Public/Misc/Guid.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/ItemDefinition.h"
#include "Engine/Runtime/GameplayAbilities/Public/GameplayAbilitySpec.h"

struct FFortItemEntry : public FFastArraySerializerItem
{
    DEFINE_MEMBER(int32, FFortItemEntry, PhantomReserveAmmo)
    DEFINE_MEMBER(TArray<FFortItemEntryStateValue>, FFortItemEntry, StateValues)
    DEFINE_MEMBER(int32, FFortItemEntry, Count)
    DEFINE_MEMBER(int32, FFortItemEntry, LoadedAmmo)
    DEFINE_MEMBER(int32, FFortItemEntry, Level)
    DEFINE_MEMBER(FGuid, FFortItemEntry, ItemGuid)
    DEFINE_MEMBER(FGuid, FFortItemEntry, TrackerGuid)
    DEFINE_PTR(UFortItemDefinition, FFortItemEntry, ItemDefinition)
    DEFINE_MEMBER(float, FFortItemEntry, Durability)
    DEFINE_MEMBER(FGameplayAbilitySpecHandle, FFortItemEntry, GameplayAbilitySpecHandle)
};

class UFortItem : public UObject
{
public:
    class AFortPlayerController* GetOwningController()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortItem", "GetOwningController");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        struct FortItem_GetOwningController final
        {
        public:
            class AFortPlayerController*                  ReturnValue;                                       // 0x0000(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }
    
    void SetOwningControllerForTemporaryItem(class AFortPlayerController* InController)
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortItem", "SetOwningControllerForTemporaryItem");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct FortItem_SetOwningControllerForTemporaryItem final
        {
        public:
            class AFortPlayerController*                  InController;                                      // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{InController};

        this->ProcessEvent(Func, &Params);
    }

    inline bool CanBeDropped()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortItem", "CanBeDropped");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        struct
        {
        public:
            bool                                          ReturnValue;                                       // 0x0000(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};
        
        this->ProcessEvent(Func, &Params);
    
        return Params.ReturnValue;
    }
};

class UFortWorldItem : public UFortItem
{
public:

    
    DEFINE_MEMBER(FFortItemEntry, UFortWorldItem, ItemEntry)
public:
    DECLARE_STATIC_CLASS(UFortWorldItem)
    DECLARE_DEFAULT_OBJECT(UFortWorldItem)
};


struct FFortItemList : public FFastArraySerializer
{
public:
    
    
public:
    
    DEFINE_MEMBER(TArray<UFortWorldItem*>, FFortItemList, ItemInstances)
    DEFINE_MEMBER(TArray<FFortItemEntry>, FFortItemList, ReplicatedEntries)
};

enum class EFortPickupSpawnSource : uint8
{
    Unset = 0,
    PlayerElimination = 1,
    Chest = 2,
    SupplyDrop = 3,
    AmmoBox = 4,
    Drone = 5,
    ItemSpawner = 6
};

enum class EFortPickupSourceTypeFlag : uint8
{
    Other = 0,
    Player = 1,
    Destruction = 2,
    Container = 3,
    AI = 4,
    Tossed = 5,
    FloorLoot = 6
};

enum class EFortPickupTossState : uint8
{
    NotTossed                                = 0,
    InProgress                               = 1,
    AtRest                                   = 2,
    EFortPickupTossState_MAX                 = 3,
};

struct FFortPickupLocationData final
{
public:
	class AFortPawn*                              PickupTarget;                                      // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AFortPickup*                            CombineTarget;                                     // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AFortPawn*                              ItemOwner;                                         // 0x0010(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector                  LootInitialPosition;                               // 0x0018(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector                  LootFinalPosition;                                 // 0x0024(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         FlyTime;                                           // 0x0030(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector_NetQuantizeNormal              StartDirection;                                    // 0x0034(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector                  FinalTossRestLocation;                             // 0x0040(0x000C)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	EFortPickupTossState                          TossState;                                         // 0x004C(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                          bPlayPickupSound;                                  // 0x004D(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_4E[0x2];                                       // 0x004E(0x0002)(Fixing Size After Last Property [ Dumper-7 ])
	struct FGuid                                  PickupGuid;                                        // 0x0050(0x0010)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
};

class UProjectileMovementComponent : public UObject
{
public:
    DECLARE_STATIC_CLASS(UProjectileMovementComponent)
    DECLARE_DEFAULT_OBJECT(UProjectileMovementComponent)
};

class AFortPickup : public AActor
{
public:
    void OnRep_PrimaryPickupItemEntry()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPickup", "OnRep_PrimaryPickupItemEntry");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        if (!this) 
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_bPickedUp()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPickup", "OnRep_bPickedUp");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        if (!this) 
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_PickupLocationData()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPickup", "OnRep_PickupLocationData");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        if (!this) 
            return;

        this->ProcessEvent(Func, nullptr);
    }
    
    void OnRep_TossedFromContainer()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPickup", "OnRep_TossedFromContainer");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        if (!this) 
            return;

        auto Flgs = Func->FunctionFlags();
        Func->FunctionFlags() |= 0x400;
    
        this->ProcessEvent(Func, nullptr);

        Func->FunctionFlags() = Flgs;
    }

    void TossPickup(const struct FVector& FinalLocation, class AFortPawn* ItemOwner, int32 OverrideMaxStackCount, bool bToss, bool bShouldCombinePickupsWhenTossCompletes, const EFortPickupSourceTypeFlag InPickupSourceTypeFlags, const EFortPickupSpawnSource InPickupSpawnSource)
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPickup", "TossPickup");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        if (!this) 
            return;

        struct FortPickup_TossPickup final
        {
        public:
            struct FVector FinalLocation;
            class AFortPawn* ItemOwner;
            int32 OverrideMaxStackCount;
            bool bToss;
            bool bShouldCombinePickupsWhenTossCompletes;
            EFortPickupSourceTypeFlag InPickupSourceTypeFlags;
            EFortPickupSpawnSource InPickupSpawnSource;
        };
        FortPickup_TossPickup Params{};

        Params.FinalLocation = std::move(FinalLocation);
        Params.ItemOwner = ItemOwner;
        Params.OverrideMaxStackCount = OverrideMaxStackCount;
        Params.bToss = bToss;
        Params.bShouldCombinePickupsWhenTossCompletes = bShouldCombinePickupsWhenTossCompletes;
        Params.InPickupSourceTypeFlags = InPickupSourceTypeFlags;
        Params.InPickupSpawnSource = InPickupSpawnSource;
        
        this->ProcessEvent(Func, &Params);
    }
public:
    DEFINE_PTR(UProjectileMovementComponent, AFortPickup, MovementComponent);
    DEFINE_PTR(class AFortPlayerPawn, AFortPickup, PawnWhoDroppedPickup);
    DEFINE_BOOL(AFortPickup, bTossedFromContainer)
    DEFINE_BOOL(AFortPickup, bPickedUp);
    DEFINE_BOOL(AFortPickup, bRandomRotation);
    DEFINE_MEMBER(FFortPickupLocationData, AFortPickup, PickupLocationData);
    DEFINE_MEMBER(FFortItemEntry, AFortPickup, PrimaryPickupItemEntry);
public:
    DECLARE_STATIC_CLASS(AFortPickup)
    DECLARE_DEFAULT_OBJECT(AFortPickup)
};

class AFortPickupAthena : public AFortPickup
{
public:
    DECLARE_STATIC_CLASS(AFortPickupAthena)
    DECLARE_DEFAULT_OBJECT(AFortPickupAthena)
};

struct FFortBaseWeaponStats 
{
    DEFINE_MEMBER(int32, FFortBaseWeaponStats, InitialClips);
    DEFINE_MEMBER(int, FFortBaseWeaponStats, ClipSize);
};

struct FFortRangedWeaponStats : public FFortBaseWeaponStats
{
    
};

class AFortInventory : public AActor
{
public:
    DEFINE_MEMBER(FFortItemList, AFortInventory, Inventory)
    DEFINE_BOOL(AFortInventory, bRequiresLocalUpdate)
public:
    void HandleInventoryLocalUpdate();
    void Update(FFortItemEntry* Entry);
    UObject* GiveItem(UFortItemDefinition* Def, int32 Count, int LoadedAmmo, int32 Level, int Slot = 0, bool bAthena = true);
    void ReplaceEntry(FFortItemEntry&);
    void Remove(FGuid Guid);
    AFortPickupAthena* SpawnPickup(FVector Loc, FFortItemEntry* Entry, EFortPickupSourceTypeFlag SourceTypeFlag, EFortPickupSpawnSource SpawnSource, class AFortPlayerPawn* Pawn, int OverrideCount, bool Toss, bool RandomRotation, bool bCombine);
    AFortPickupAthena* SpawnPickup(FVector, UFortItemDefinition*, int, int, EFortPickupSourceTypeFlag SourceTypeFlag = EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource SpawnSource = EFortPickupSpawnSource::Unset, AFortPlayerPawn* Pawn = nullptr, bool Toss = true);
    static AFortPickupAthena* SpawnPickupStatic(FVector Loc, FFortItemEntry* Entry, EFortPickupSourceTypeFlag SourceTypeFlag, EFortPickupSpawnSource SpawnSource, class AFortPlayerPawn* Pawn, int OverrideCount, bool Toss, bool RandomRotation, bool bCombine);
    FFortItemEntry* MakeItemEntry(UFortItemDefinition*, int32, int32);
    FFortRangedWeaponStats* GetStats(class UFortWeaponItemDefinition* Def);
    static FFortRangedWeaponStats* GetStatsStatic(class UFortWeaponItemDefinition* Def);
    FFortItemEntry* FindItemEntryDef(UObject* Object);
    UFortWorldItem* FindItemInstance(FGuid Guid);
    FFortItemEntry* FindReplicatedEntry(FGuid Guid);
    UFortItem* GetPickaxeInstance();
    void ClearInventory();
public:
    DECLARE_STATIC_CLASS(AFortInventory)
    DECLARE_DEFAULT_OBJECT(AFortInventory)
};
