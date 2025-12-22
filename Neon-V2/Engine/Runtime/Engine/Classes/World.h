#pragma once
#include "pch.h"

#include "GameInstance.h"
#include "NetDriver.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UGameViewportClient : public UObject
{
public:
    DEFINE_PTR(class UWorld, UGameViewportClient, World);
public:
    DECLARE_STATIC_CLASS(UGameViewportClient);
    DECLARE_DEFAULT_OBJECT(UGameViewportClient);
};

struct FURL
{
public:
    FString Protocol;
    FString Host;
    int32 Port;
    int32 Valid;
    FString Map;
    FString RedirectUrl;
    TArray<FString> Op;
    FString Portal;
};

struct FLevelCollection
{
    BYTE CollectionType[1];
    bool bIsVisible;
    AFortGameStateAthena* GameState;
    UNetDriver* NetDriver;
    void* DemoNetDriver;
    void* PersistentLevel;
    uint8_t Levels[0x50];
};

class ULevelStreaming : public UObject
{
public:
    DEFINE_MEMBER(FName, ULevelStreaming, PackageNameToLoad);
    
public:
    DECLARE_DEFAULT_OBJECT(ULevelStreaming);
    DECLARE_STATIC_CLASS(ULevelStreaming);
};

class ULevel : public UObject
{
public:
    DEFINE_MEMBER(TArray<class AActor*>, ULevel, Actors)
    DEFINE_PTR(UWorld, ULevel, OwningWorld);
public:
    static ULevel* GetLevel(AActor* Actor) {
        auto Outer = Actor->GetOuter();

        while (Outer)
        {
            if (Outer->GetClass() == ULevel::StaticClass())
                return (ULevel*)Outer;
            else
                Outer = Outer->GetOuter();
        }
        return nullptr;
    }
public:
    DECLARE_STATIC_CLASS(ULevel);
    DECLARE_DEFAULT_OBJECT(ULevel);
};

class UWorld : public UObject
{
public:
    DEFINE_PTR(ULevel, UWorld, PersistentLevel)
    DECLARE_STATIC_CLASS(UWorld);
    DECLARE_DEFAULT_OBJECT(UWorld);
    DEFINE_PTR(UGameInstance, UWorld, OwningGameInstance);
    DEFINE_PTR(UNetDriver, UWorld, NetDriver);
    DEFINE_PTR(AFortGameStateAthena, UWorld, GameState);
    DEFINE_PTR(AFortGameModeAthena, UWorld, AuthorityGameMode);
    DEFINE_MEMBER(TArray<FLevelCollection>, UWorld, LevelCollections);
    DEFINE_PTR(class UAthenaAISystem, UWorld, AISystem)
    DEFINE_MEMBER(TArray<ULevelStreaming*>, UWorld, StreamingLevels);
public:
    AActor* SpawnActor(UClass* Class, FVector Loc, FRotator Rot = FRotator(0,0,0), AActor* Owner = nullptr);
    
    template <typename T>
    T* SpawnActor(UClass* Class, FVector Loc, FRotator Rot = FRotator(0,0,0), AActor* Owner = nullptr)
    {
        return static_cast<T*>(SpawnActor(Class, Loc, Rot, Owner));
    }
    
    void Listen(FURL URL);

    DefineUOriginal(void, BeginPlay, UWorld*);
};

static inline UWorld* GetWorld()
{
    static UEngine* GEngine = nullptr;

    if (!GEngine)
        GEngine = UEngine::GetEngine();

    if (UObject* GameViewportClient = GEngine->GetGameViewport())
    {
        return ((UGameViewportClient*)GameViewportClient)->GetWorld();
    }

    return nullptr;
}

class AFortAthenaPatrolPath : UObject
{
public:
    DEFINE_MEMBER(TArray<AActor*>, AFortAthenaPatrolPath, PatrolPoints);
    DEFINE_MEMBER(FGameplayTagContainer, AFortAthenaPatrolPath, GameplayTags)
    DECLARE_STATIC_CLASS(AFortAthenaPatrolPath)
    DECLARE_DEFAULT_OBJECT(AFortAthenaPatrolPath)
};

class AFortAthenaPatrolPathPointProvider : public UObject
{
public:
    DEFINE_MEMBER(FGameplayTagContainer, AFortAthenaPatrolPathPointProvider, FiltersTags)
    DEFINE_PTR(AFortAthenaPatrolPath, AFortAthenaPatrolPathPointProvider, AssociatedPatrolPath)
public:
    DECLARE_DEFAULT_OBJECT(AFortAthenaPatrolPathPointProvider);
    DECLARE_STATIC_CLASS(AFortAthenaPatrolPathPointProvider);
};

class UFortAthenaAISpawnerData : public UObject
{
public:
    static class UObject* CreateComponentListFromClass(const TSubclassOf<class UFortAthenaAISpawnerData> AISpawnerDataClass, class UObject* OuterObject)
    {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaAISpawnerData", "CreateComponentListFromClass");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        struct FortAthenaAISpawnerData_CreateComponentListFromClass final
        {
        public:
            TSubclassOf<class UFortAthenaAISpawnerData>   AISpawnerDataClass;                                // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            class UObject*                                OuterObject;                                       // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            class UObject*  ReturnValue;                                       // 0x0010(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{AISpawnerDataClass, OuterObject};

        static UObject* DefaultObj = StaticClassImpl("FortAthenaAISpawnerData")->GetClassDefaultObject();
        DefaultObj->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }
};

class UAthenaAISpawner : public UObject
{
public:
    int32 RequestSpawn(class UObject* AISpawnerComponentList, const struct FTransform& SpawnTransform)
    {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("AthenaAISpawner", "RequestSpawn");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return 0;

        struct AthenaAISpawner_RequestSpawn final
        {
        public:
            class UObject*  AISpawnerComponentList;                            // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8                                         Pad_8[0x8];                                        // 0x0008(0x0008)(Fixing Size After Last Property [ Dumper-7 ])
            struct FTransform                             SpawnTransform;                                    // 0x0010(0x0060)(ConstParm, Parm, OutParm, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
            int32                                         ReturnValue;                                       // 0x0070(0x0004)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8                                         Pad_74[0xC];                                       // 0x0074(0x000C)(Fixing Struct Size After Last Property [ Dumper-7 ])
        } Params{};

        Params.SpawnTransform = SpawnTransform;
        Params.AISpawnerComponentList = AISpawnerComponentList;

        ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }
public:
    DECLARE_STATIC_CLASS(UAthenaAISpawner)
    DECLARE_DEFAULT_OBJECT(UAthenaAISpawner)
};

class UAthenaAISystem : public UObject
{
public:
    void SpawnAI(std::string NPCName, std::string Season);
public:
    DEFINE_PTR(UAthenaAISpawner, UAthenaAISystem, AISpawner)
    DECLARE_STATIC_CLASS(UAthenaAISystem)
    DECLARE_DEFAULT_OBJECT(UAthenaAISystem)
};