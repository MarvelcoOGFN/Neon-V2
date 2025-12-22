#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Math/Transform.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"

enum class EDynamicFoundationEnabledState : uint8
{
    Unknown = 0,
    Enabled = 1,
    Disabled = 2,
    EDynamicFoundationEnabledState_MAX = 3,
};

// Enum FortniteGame.EDynamicFoundationType
// NumValues: 0x0005
enum class EDynamicFoundationType : uint8
{
    Static = 0,
    StartEnabled_Stationary = 1,
    StartEnabled_Dynamic = 2,
    StartDisabled = 3,
    EDynamicFoundationType_MAX = 4,
};

// 0x001C (0x001C - 0x0000)
struct FDynamicBuildingFoundationRepData final
{
public:
    struct FRotator Rotation;                    // 0x0000(0x000C)(ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
    struct FVector Translation;                  // 0x000C(0x000C)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    EDynamicFoundationEnabledState EnabledState; // 0x0018(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8 Pad_1D3B[0x3];                         // 0x0019(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FBuildingFoundationStreamingData final
{
public:
    DEFINE_MEMBER(FVector, FBuildingFoundationStreamingData, FoundationLocation);
};

class ABuildingFoundation : public UObject
{
public:
    DEFINE_MEMBER(FTransform, ABuildingFoundation, DynamicFoundationTransform);
    DEFINE_MEMBER(FBuildingFoundationStreamingData, ABuildingFoundation, StreamingData);
    DEFINE_MEMBER(TArray<TSoftObjectPtr<class UWorld>>, ABuildingFoundation, AdditionalWorlds);
    DEFINE_MEMBER(EDynamicFoundationType, ABuildingFoundation, DynamicFoundationType);
    DEFINE_MEMBER(bool, ABuildingFoundation, bServerStreamedInLevel);
    DEFINE_MEMBER(FDynamicBuildingFoundationRepData, ABuildingFoundation, DynamicFoundationRepData);
    DEFINE_MEMBER(EDynamicFoundationEnabledState, ABuildingFoundation, FoundationEnabledState);
    DECLARE_DEFAULT_OBJECT(ABuildingFoundation)
    DECLARE_STATIC_CLASS(ABuildingFoundation)
public:
    void SetDynamicFoundationEnabled(bool bEnabled)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingFoundation", "SetDynamicFoundationEnabled");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct BuildingFoundation_SetDynamicFoundationEnabled final
        {
        public:
            bool bEnabled;
        };
        BuildingFoundation_SetDynamicFoundationEnabled Params;
        Params.bEnabled = bEnabled;

        this->ProcessEvent(Func, &Params);
    }

    void OnRep_ServerStreamedInLevel()
    {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingFoundation", "OnRep_ServerStreamedInLevel");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_DynamicFoundationRepData()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BuildingFoundation", "OnRep_DynamicFoundationRepData");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    DefineUOriginal(void, SetDynamicFoundationEnabled, ABuildingFoundation *, FFrame &);
    DefineUOriginal(void, SetDynamicFoundationTransform, ABuildingFoundation *, FFrame &);
};