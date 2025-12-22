#pragma once
#include "pch.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"

class USceneComponent : public UObject
{
public:
    void K2_SetWorldTransform(const struct FTransform &NewTransform, bool bSweep, struct FHitResult *SweepHitResult, bool bTeleport)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("SceneComponent", "K2_SetWorldLocationAndRotation");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct SceneComponent_K2_SetWorldLocationAndRotation final
        {
        public:
            struct FTransform NewTransform; // 0x0000(0x0030)(ConstParm, Parm, OutParm, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
            bool bSweep;                    // 0x0030(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_28C[0x7];             // 0x0031(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
            bool bTeleport;                 // 0x00C0(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_28D[0xF];
        };

        SceneComponent_K2_SetWorldLocationAndRotation Params;
        Params.NewTransform = std::move(NewTransform);
        Params.bSweep = bSweep;
        Params.bTeleport = bTeleport;

        this->ProcessEvent(Func, &Params);
    }

    void SetPhysicsLinearVelocity(const struct FVector &NewVel, bool bAddToCurrent, class FName BoneName)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("PrimitiveComponent", "SetPhysicsLinearVelocity");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct PrimitiveComponent_SetPhysicsLinearVelocity final
        {
        public:
            struct FVector NewVel;
            bool bAddToCurrent;
            uint8 Pad_2CA[0x3];
            class FName BoneName;
        };

        PrimitiveComponent_SetPhysicsLinearVelocity Params;
        Params.NewVel = std::move(NewVel);
        Params.bAddToCurrent = bAddToCurrent;
        Params.BoneName = BoneName;

        this->ProcessEvent(Func, &Params);
    }

    struct t final
    {
    public:
        uint8_t Padding[0x88]; // 0xe8 on LWC builds
    };

    void K2_SetWorldLocationAndRotation(const struct FVector &NewLocation, const struct FRotator &NewRotation, bool bSweep, struct t *SweepHitResult, bool bTeleport)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("SceneComponent", "K2_SetWorldLocationAndRotation");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct SceneComponent_K2_SetWorldLocationAndRotation final
        {
        public:
            struct FVector NewLocation;  // 0x0000(0x000C)(Parm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            struct FRotator NewRotation; // 0x000C(0x000C)(Parm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
            bool bSweep;                 // 0x0018(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_19[0x7];           // 0x0019(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
            t SweepHitResult;            // 0x0020(0x0088)(Parm, OutParm, IsPlainOldData, NoDestructor, ContainsInstancedReference, NativeAccessSpecifierPublic)
            bool bTeleport;              // 0x00A8(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_A9[0x7];           // 0x00A9(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
        };

        SceneComponent_K2_SetWorldLocationAndRotation Params;
        Params.NewLocation = std::move(NewLocation);
        Params.NewRotation = std::move(NewRotation);
        Params.bSweep = bSweep;
        Params.bTeleport = bTeleport;

        this->ProcessEvent(Func, &Params);
    }

    void SetPhysicsAngularVelocity(const struct FVector &NewAngVel, bool bAddToCurrent, class FName BoneName)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("PrimitiveComponent", "SetPhysicsAngularVelocity");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct PrimitiveComponent_SetPhysicsAngularVelocity final
        {
        public:
            struct FVector NewAngVel;
            bool bAddToCurrent;
            uint8 Pad_2C7[0x3];
            class FName BoneName;
        };

        PrimitiveComponent_SetPhysicsAngularVelocity Params;
        Params.NewAngVel = std::move(NewAngVel);
        Params.bAddToCurrent = bAddToCurrent;
        Params.BoneName = BoneName;

        this->ProcessEvent(Func, &Params);
    }

public:
    DEFINE_PTR(USceneComponent, USceneComponent, RootComponent);
    DEFINE_MEMBER(uint8, USceneComponent, bComponentToWorldUpdated);
    DEFINE_MEMBER(FVector, USceneComponent, RelativeLocation);
    DEFINE_MEMBER(FRotator, USceneComponent, RelativeRotation);
    DECLARE_DEFAULT_OBJECT(USceneComponent)
    DECLARE_STATIC_CLASS(USceneComponent)
};