#include "pch.h"
#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"

UObject *UGameplayStatics::SpawnObject(TSubclassOf<class UObject> ObjectClass, class UObject *Param_Outer)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("GameplayStatics", "SpawnObject");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return nullptr;

    struct
    {
        UClass *ObjectClass;  // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        UObject *Outer;       // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        UObject *ReturnValue; // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    } UGameplayStatics_SpawnObject_Params{ObjectClass, Param_Outer};

    static UObject *Object = nullptr;
    if (!Object)
        Object = SDK::StaticClassImpl("GameplayStatics")->GetClassDefaultObject();

    Object->ProcessEvent(Func, &UGameplayStatics_SpawnObject_Params);

    return UGameplayStatics_SpawnObject_Params.ReturnValue;
}
AActor *UGameplayStatics::FinishSpawningActor(AActor *Actor, FTransform SpawnTransform)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("GameplayStatics", "FinishSpawningActor");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return nullptr;

    struct
    {
        AActor *Actor;
        FTransform SpawnTransform;
        AActor *ReturnValue;
    } UGameplayStatics_FinishSpawningActor_Params{Actor, SpawnTransform};

    static UObject *Object = nullptr;
    if (!Object)
        Object = SDK::StaticClassImpl("GameplayStatics")->GetClassDefaultObject();

    Object->ProcessEvent(Func, &UGameplayStatics_FinishSpawningActor_Params);

    return UGameplayStatics_FinishSpawningActor_Params.ReturnValue;
}

AActor *UGameplayStatics::BeginSpawningActorFromClass(UObject *WorldContextObject, UClass *ActorClass, FTransform SpawnTransform, bool bNoCollisionFail, AActor *Owner)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("GameplayStatics", "BeginSpawningActorFromClass");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return nullptr;

    struct
    {
        UObject *WorldContextObject;
        UClass *ActorClass;
        FTransform SpawnTransform;
        bool bNoCollisionFail;
        AActor *Owner;
        AActor *ReturnValue;
    } params(WorldContextObject, ActorClass, SpawnTransform, bNoCollisionFail, Owner);

    static UObject *Object = nullptr;
    if (!Object)
        Object = SDK::StaticClassImpl("GameplayStatics")->GetClassDefaultObject();

    Object->ProcessEvent(Func, &params);

    return params.ReturnValue;
}

float UGameplayStatics::GetTimeSeconds(UWorld *World)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("GameplayStatics", "GetTimeSeconds");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return 0.f;

    struct
    {
        UObject *WorldContextObject;
        float TimeSeconds;
    } GetTimeSecondsParams{World};

    static UObject *Object = nullptr;
    if (!Object)
        Object = SDK::StaticClassImpl("GameplayStatics")->GetClassDefaultObject();

    Object->ProcessEvent(Func, &GetTimeSecondsParams);

    return GetTimeSecondsParams.TimeSeconds;
}

TArray<AActor *> UGameplayStatics::GetAllActorsOfClass(const UObject *World, UClass *ActorClass)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("GameplayStatics", "GetAllActorsOfClass");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return TArray<AActor *>();

    struct
    {
        const UObject *WorldContextObject;
        UClass *ActorClass;
        TArray<AActor *> OutActors;
    } UGameplayStatics_GetAllActorsOfClass_Params{World, ActorClass};

    static UObject *Object = nullptr;
    if (!Object)
        Object = SDK::StaticClassImpl("GameplayStatics")->GetClassDefaultObject();

    Object->ProcessEvent(Func, &UGameplayStatics_GetAllActorsOfClass_Params);

    return UGameplayStatics_GetAllActorsOfClass_Params.OutActors;
}