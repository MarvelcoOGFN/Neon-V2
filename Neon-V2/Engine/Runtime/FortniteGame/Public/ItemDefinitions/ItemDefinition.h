#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/UObjectGlobals.h"

enum class EFortItemEntryState : uint8_t 
{
    NoneState = 0,
    NewItemCount = 1,
    ShouldShowItemToast = 2,
    DurabilityInitialized = 3,
    DoNotShowSpawnParticles = 4,
    FromRecoveredBackpack = 5,
    FromGift = 6,
    PendingUpgradeCriteriaProgress = 7,
    OwnerBuildingHandle = 8,
    FromDroppedPickup = 9,
    JustCrafted = 10,
    CraftAndSlotTarget = 11,
    GenericAttributeValueSet = 12,
    PickupInstigatorHandle = 13,
    CreativeUserPrefabHasContent = 14,
    EFortItemEntryState_MAX = 15
};

struct FFortItemEntryStateValue final
{
public:
    int32                                         IntValue;                                          // 0x0000(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
    class FName                                   NameValue;                                         // 0x0004(0x0008)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
    EFortItemEntryState                           StateType;                                         // 0x000C(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
    uint8                                         Pad_D[0x3];                                        // 0x000D(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FSimpleCurveKey
{
    float                                              Time;                                                     // 0x0000(0x0004) (Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float                                              Value;                                                    // 0x0004(0x0004) (Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

struct FIndexedCurve
{

};

struct FRealCurve : public FIndexedCurve
{
    
};

struct FSimpleCurve : public FRealCurve
{
public:
    DEFINE_MEMBER(TArray<FSimpleCurveKey>, FSimpleCurve, Keys); 
};

struct FScalableFloat
{
public:
    float                                        Value;                                             // 0x0(0x4)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                        Pad_3BF0[0x4];                                     // Fixing Size After Last Property  [ Dumper-7 ]
    FCurveTableRowHandle                  Curve;                                             // 0x8(0x10)(Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class UFortItemDefinition : public UObject
{
public:
    class UFortWorldItem* CreateTemporaryItemInstanceBP(int32 Count, int32 Level)
    {
        static UFunction* Func = StaticFindObject<UFunction>("/Script/FortniteGame.FortItemDefinition.CreateTemporaryItemInstanceBP");

        struct FortItemDefinition_CreateTemporaryItemInstanceBP final
        {
        public:
            int32                                         Count;                                             // 0x0000(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            int32                                         Level;                                             // 0x0004(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            UFortWorldItem* ReturnValue;                                       // 0x0008(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };

        FortItemDefinition_CreateTemporaryItemInstanceBP Parms{};
        Parms.Count = Count;
        Parms.Level = Level;

        auto Flgs = Func->FunctionFlags();
        Func->FunctionFlags() |= 0x400;

        this->ProcessEvent(Func, &Parms);

        Func->FunctionFlags() = Flgs;

        return Parms.ReturnValue;
    }

    bool IsStackable()
    {
        static UFunction* Func = StaticFindObject<UFunction>("/Script/FortniteGame.FortItemDefinition.IsStackable");
        struct FortItemDefinition_IsStackable final
        {
        public:
            bool                                          ReturnValue;                                       // 0x0000(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};

        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }

    /*int GetMaxStackSize()
    {
        bool bIsScalableFloat = Fortnite_Version >= 12.00; 

        if (!bIsScalableFloat)
        {
            static auto MaxStackSize = this->Get<int>("FortItemDefinition", "MaxStackSize");
            return MaxStackSize;
        }
    
        auto MaxStackSize = this->Get<FScalableFloat>("FortItemDefinition", "MaxStackSize");
    
        static auto AthenaGameData = StaticFindObject<UDataTable>("/Game/Athena/Balance/DataTables/AthenaGameData.AthenaGameData");

        auto& RowMap = AthenaGameData->GetRowMap();

        if (MaxStackSize.Curve.RowName == 0)
            return MaxStackSize.Value;

        FSimpleCurve* Curve = nullptr;

        for (auto& Pair : RowMap)
        {
            if (Pair.Key == MaxStackSize.Curve.RowName)
            {
                Curve = (FSimpleCurve*)Pair.Value;
                break;
            }
        }

        if (!Curve)
            return 1;

        return Curve->GetKeys()[0].Value;
    }*/

    int GetMaxStackSize()
    {
        static int PropOffset = -1;
        if (PropOffset == -1)
        {
            PropOffset = SDK::PropLibrary->GetPropertyByName("FortItemDefinition", "MaxStackSize").Offset;
        }

        static bool IsScalable = Fortnite_Version >= 12.00;
        
        if (IsScalable) {
            auto* AsScalable = reinterpret_cast<FScalableFloat*>(uintptr_t(this) + PropOffset);

            if (!AsScalable->Curve.CurveTable)
                return static_cast<int>(AsScalable->Value);
            else
                return static_cast<int>(UDataTableFunctionLibrary::EvaluateCurveTableRow(AsScalable->Curve.CurveTable, AsScalable->Curve.RowName, 0));
        } else {
            return *reinterpret_cast<int*>(uintptr_t(this) + PropOffset);
        }
    }
public:
    DEFINE_MEMBER(int32, UFortWorldItemDefinition, MinLevel);
    DEFINE_MEMBER(int32, UFortWorldItemDefinition, MaxLevel);
    DEFINE_BOOL(UFortItemDefinition, bInventorySizeLimited)
    DEFINE_BOOL(UFortItemDefinition, bAllowMultipleStacks);
public:
    DECLARE_STATIC_CLASS(UFortItemDefinition)
    DECLARE_DEFAULT_OBJECT(UFortItemDefinition)
};
