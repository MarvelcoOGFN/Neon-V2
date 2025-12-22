#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Math/Transform.h"
//#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"

enum class ERotatorQuantization : uint8
{
    ByteComponents = 0,
    ShortComponents = 1,
    ERotatorQuantization_MAX = 2,
};

enum class EVectorQuantization : uint8
{
    RoundWholeNumber = 0,
    RoundOneDecimal = 1,
    RoundTwoDecimals = 2,
    EVectorQuantization_MAX = 3,
};

struct FRepMovement
{
public:
    struct FVector LinearVelocity;                  // 0x0000(0x000C)(Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    struct FVector AngularVelocity;                 // 0x000C(0x000C)(Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    struct FVector Location;                        // 0x0018(0x000C)(Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    struct FRotator Rotation;                       // 0x0024(0x000C)(Transient, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
    uint8 bSimulatedPhysicSleep : 1;                // 0x0030(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
    uint8 bRepPhysics : 1;                          // 0x0030(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
    EVectorQuantization LocationQuantizationLevel;  // 0x0031(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    EVectorQuantization VelocityQuantizationLevel;  // 0x0032(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    ERotatorQuantization RotationQuantizationLevel; // 0x0033(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

enum class ENetRole : uint8
{
    ROLE_None = 0,
    ROLE_SimulatedProxy = 1,
    ROLE_AutonomousProxy = 2,
    ROLE_Authority = 3,
    ROLE_MAX = 4,
};

enum class EAttachmentRule : uint8
{
    KeepRelative = 0,
    KeepWorld = 1,
    SnapToTarget = 2,
    EAttachmentRule_MAX = 3,
};

class AActor : public UObject
{
public:
    DEFINE_MEMBER(FRepMovement, AActor, ReplicatedMovement);
    DEFINE_PTR(AActor, AActor, Owner);
    DEFINE_PTR(class USceneComponent, AActor, RootComponent);
    DEFINE_MEMBER(ENetRole, AActor, Role);
    FTransform GetTransform();
    FVector K2_GetActorLocation();
    void K2_DestroyActor();
    FVector GetActorForwardVector();
    FVector GetActorUpVector();
    FVector GetActorRightVector();
    FRotator K2_GetActorRotation();
    bool K2_SetActorRotation(FRotator Rot, bool bTeleportPhysics);
    class UActorComponent* GetComponentByClass(UClass* ComponentClass);
    void K2_AttachToComponent(class USceneComponent* Parent, class FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies);
    bool K2_TeleportTo(FVector DestLocation, FRotator DestRotation);
    float GetDistanceTo(AActor *Actor);
    void OnRep_ReplicatedMovement()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "OnRep_ReplicatedMovement");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

public:
    DEFINE_BOOL(AActor, bHidden);

public:
    DECLARE_STATIC_CLASS(AActor);
    DECLARE_DEFAULT_OBJECT(AActor);
};