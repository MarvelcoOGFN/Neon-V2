#include "pch.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"

FTransform AActor::GetTransform()
{
    static SDK::UFunction *Func = SDK::PropLibrary->GetFunctionByName("Actor", "GetTransform").Func;
    if (!Func)
        return FTransform();

    struct
    {
        FTransform ReturnValue;
    } Params;
    ProcessEvent(Func, &Params);
    return Params.ReturnValue;
}

FVector AActor::K2_GetActorLocation()
{
    static SDK::UFunction *Func = SDK::PropLibrary->GetFunctionByName("Actor", "K2_GetActorLocation").Func;
    if (!Func)
        return FVector();

    struct
    {
        FVector ReturnValue;
    } Params;
    ProcessEvent(Func, &Params);
    return Params.ReturnValue;
}

void AActor::K2_DestroyActor()
{
    static SDK::UFunction *Func = SDK::PropLibrary->GetFunctionByName("Actor", "K2_DestroyActor").Func;
    if (!Func)
        return;

    ProcessEvent(Func, nullptr);
}

FVector AActor::GetActorForwardVector()
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "GetActorForwardVector");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return FVector();

    struct Actor_K2_GetActorLocation final
    {
    public:
        FVector ReturnValue;
    };
    Actor_K2_GetActorLocation Params;

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}

void AActor::K2_AttachToComponent(class USceneComponent *Parent, class FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "K2_AttachToComponent");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return;

    struct Actor_K2_AttachToComponent final
    {
    public:
        class USceneComponent *Parent; // 0x0000(0x0008)(Parm, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        class FName SocketName;        // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        EAttachmentRule LocationRule;  // 0x0010(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        EAttachmentRule RotationRule;  // 0x0011(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        EAttachmentRule ScaleRule;     // 0x0012(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        bool bWeldSimulatedBodies;     // 0x0013(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        uint8 Pad_14[0x4];             // 0x0014(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
    } Params{};

    Params.Parent = Parent;
    Params.SocketName = SocketName;
    Params.LocationRule = LocationRule;
    Params.RotationRule = RotationRule;
    Params.ScaleRule = ScaleRule;
    Params.bWeldSimulatedBodies = bWeldSimulatedBodies;

    this->ProcessEvent(Func, &Params);
}

FVector AActor::GetActorUpVector()
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "GetActorUpVector");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return FVector();

    struct Actor_K2_GetActorLocation final
    {
    public:
        FVector ReturnValue;
    };
    Actor_K2_GetActorLocation Params;

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}

float AActor::GetDistanceTo(AActor *Actor)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "GetDistanceTo");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return false;

    struct Actor_GetDistanceTo final
    {
    public:
        const class AActor *OtherActor; // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        float ReturnValue;              // 0x0008(0x0004)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        uint8 Pad_C[0x4];               // 0x000C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
    };
    Actor_GetDistanceTo Params;
    Params.OtherActor = std::move(Actor);

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}

FVector AActor::GetActorRightVector()
{
    return this->CallFunc<FVector>("Actor", "GetActorRightVector");
}

FRotator AActor::K2_GetActorRotation()
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "K2_GetActorRotation");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return FRotator();

    struct Actor_K2_GetActorLocation final
    {
    public:
        FRotator ReturnValue;
    };
    Actor_K2_GetActorLocation Params;

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}

bool AActor::K2_SetActorRotation(FRotator Rot, bool bTeleportPhysics)
{
    static SDK::UFunction* Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "K2_SetActorRotation");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return 0;

    struct Actor_K2_SetActorRotation final
    {
    public:
        struct FRotator                               NewRotation;                                       // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
        bool                                          bTeleportPhysics;                                  // 0x000C(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        bool                                          ReturnValue;                                       // 0x000D(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        uint8                                         Pad_E[0x2];                                        // 0x000E(0x0002)(Fixing Struct Size After Last Property [ Dumper-7 ])
    };
    Actor_K2_SetActorRotation Params;
    Params.bTeleportPhysics = bTeleportPhysics;
    Params.NewRotation = Rot;

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
    
}

UActorComponent *AActor::GetComponentByClass(UClass *ComponentClass)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "GetComponentByClass");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return nullptr;

    struct
    {
        class UClass *ComponentClass; // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        UActorComponent *ReturnValue; // (ExportObject, Parm, OutParm, ZeroConstructor, ReturnParm, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    } Params{ComponentClass};

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}

bool AActor::K2_TeleportTo(FVector DestLocation, FRotator DestRotation)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("Actor", "K2_TeleportTo");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return false;

    struct Actor_K2_TeleportTo final
    {
    public:
        struct FVector DestLocation;  // 0x0000(0x000C)(Parm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        struct FRotator DestRotation; // 0x000C(0x000C)(Parm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
        bool ReturnValue;             // 0x0018(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        uint8 Pad_19[0x3];            // 0x0019(0x0003)(Fixing Struct Size After Last Property [ Dumper-8 ])
    };

    Actor_K2_TeleportTo Params;
    Params.DestLocation = std::move(DestLocation);
    Params.DestRotation = std::move(DestRotation);

    this->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}
