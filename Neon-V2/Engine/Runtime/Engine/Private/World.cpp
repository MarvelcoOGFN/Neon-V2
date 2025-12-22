#include "pch.h"

#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Plugins/Kismet/Public/KismetMemLibrary.h"
#include "Engine/Plugins/Kismet/Public/KismetStringLibrary.h"
#include "Engine/Plugins/Neon/Public/Neon.h"
#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"

void UWorld::Listen(FURL URL)
{
    UE_LOG(LogTemp, Log, __FUNCTION__);
    FName NetDriverName = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");

    auto CreateNetDriver = UKismetMemLibrary::Get<
        UNetDriver* (*)(UEngine*, UWorld*, FName)>(L"CreateNetDriver");
    
    UNetDriver* NetDriver = nullptr;
    if (Fortnite_Version >= 16.00)
    {
        static void* (*GetWorldContextFromObject)(UEngine*, UWorld*) = decltype(GetWorldContextFromObject)(UKismetMemLibrary::GetAddress<uintptr_t>(L"GetWorldContextFromObject"));
        void* WorldContext = GetWorldContextFromObject(UEngine::GetEngine(), GetWorld());
        static UNetDriver* (*CreateNetDriver_Local)(UEngine*, void* WorldContext, FName) = decltype(CreateNetDriver_Local)(UKismetMemLibrary::GetAddress<uintptr_t>(L"CreateNamedNetDriver"));
        NetDriver = CreateNetDriver_Local(UEngine::GetEngine(), WorldContext, NetDriverName);
    } else
    {
        NetDriver = CreateNetDriver(UEngine::GetEngine(), this, NetDriverName);
    }
    
    if (!NetDriver) return;

    NetDriver->SetNetDriverName(NetDriverName); 
    NetDriver->SetWorld(this);

    if (NetDriver->GetMaxInternetClientRate() < NetDriver->GetMaxClientRate())
        if (NetDriver->GetMaxInternetClientRate() > 2500) NetDriver->SetMaxClientRate(NetDriver->GetMaxInternetClientRate());

    UKismetMemLibrary::Get<void (*)(UNetDriver*, UWorld*)>(L"SetWorld")(NetDriver, GetWorld());
    SetNetDriver(NetDriver);

    FString Error;
    UKismetMemLibrary::Get<
        bool (*)(UNetDriver*, UWorld*, FURL&, bool, FString&)>(L"InitListen")(
            NetDriver, GetWorld(), URL, false, Error);
    UKismetMemLibrary::Get<void (*)(UNetDriver*, UWorld*)>(L"SetWorld")(NetDriver, GetWorld());

    for (auto& Collection : GetLevelCollections())
        Collection.NetDriver = NetDriver;

    GNeon->ChangeState((std::wstring(L"Listening on Port: ") + std::to_wstring(URL.Port)).c_str());
    GNeon->UpdatePort(URL.Port);
}

enum class ESpawnActorCollisionHandlingMethod : uint8
{
    Undefined,
    AlwaysSpawn,
    AdjustIfPossibleButAlwaysSpawn,
    AdjustIfPossibleButDontSpawnIfColliding,
    DontSpawnIfColliding
};

struct FActorSpawnParameters
{
public:
    FName Name;

    AActor* Template;
    AActor* Owner;
    APawn* Instigator;
    UObject* OverrideLevel;
    ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

private:
    uint8	bRemoteOwned : 1;
public:
    uint8	bNoFail : 1;
    uint8	bDeferConstruction : 1;
    uint8	bAllowDuringConstructionScript : 1;
    EObjectFlags ObjectFlags;
};

void UAthenaAISystem::SpawnAI(std::string NPCName, std::string Season)
{
    auto Path = "/BattlepassS" + Season + "/AI/NPCs/" + NPCName + "/" + "BP_AIBotSpawnerData_" + NPCName + "." + "BP_AIBotSpawnerData_" + NPCName + "_C";
    UE_LOG(LogTemp, Log, "Path: %s", Path.c_str());
    auto Class = StaticLoadObject<UClass>(Path.c_str());
    if (!Class) return;
    
    TArray<AFortAthenaPatrolPath*> PossibleSpawnPaths;
    for (auto* Actor : UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFortAthenaPatrolPathPointProvider::StaticClass()))
    {
        AFortAthenaPatrolPathPointProvider* Path = (AFortAthenaPatrolPathPointProvider*)Actor;   
        if (Path->GetFiltersTags().GameplayTags.Num() == 0)
            continue;
        auto PathName = Path->GetFiltersTags().GameplayTags[0].TagName.ToString().ToString();
        if (PathName.substr(PathName.rfind(L'.') + 1) == NPCName)
            PossibleSpawnPaths.Add(Path->GetAssociatedPatrolPath());
    }
    for (auto* Actor : UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFortAthenaPatrolPath::StaticClass()))
    {
        AFortAthenaPatrolPath* PatrolPath = (AFortAthenaPatrolPath*)Actor;   
        if (PatrolPath->GetGameplayTags().GameplayTags.Num() == 0)
            continue;
        auto PathName = PatrolPath->GetGameplayTags().GameplayTags[0].TagName.ToString().ToString();
        if (PathName.substr(PathName.rfind(L'.') + 1) == NPCName)
            PossibleSpawnPaths.Add(PatrolPath);
    }

    if (PossibleSpawnPaths.Num() > 0)
    {
        auto PatrolPath = PossibleSpawnPaths[(int)ceil((float)rand() / RAND_MAX) * PossibleSpawnPaths.Num() - 1];
        auto ComponentList = UFortAthenaAISpawnerData::CreateComponentListFromClass(Class, GetWorld());
    
        auto Transform = IsValidPointer(PatrolPath->GetPatrolPoints()[0]) ? PatrolPath->GetPatrolPoints()[0]->GetTransform() : FTransform{};
        GetAISpawner()->RequestSpawn(ComponentList, Transform);
    } else
    {
        UE_LOG_DISPATCH(LogTemp, Log, "No Location for UAthenaAISystem::SpawnAI");
    }
}

AActor* UWorld::SpawnActor(UClass* Class, FVector Loc, FRotator Rot, AActor* Owner)
{
    FActorSpawnParameters addr{};

    addr.Owner = Owner;
    addr.bDeferConstruction = false;
    addr.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    static auto fn = reinterpret_cast<
        AActor* (*)(UWorld*, UClass*, FVector const*, FRotator const*, FActorSpawnParameters*)
    >(
        UKismetMemLibrary::Get<uintptr_t>(TEXT("SpawnActor"))
    );
    
    return fn(this, Class, &Loc, &Rot, &addr);
}

void UWorld::BeginPlay()
{
    this->GetAuthorityGameMode()->SetGameSessionClass(AFortGameSessionDedicated::StaticClass());

    BeginPlayOG(this);
}
