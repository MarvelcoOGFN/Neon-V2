#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Weapons/FortDecoTool.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

enum class EBuildingAttachmentType : uint8
{
    ATTACH_Floor                             = 0,
    ATTACH_Wall                              = 1,
    ATTACH_Ceiling                           = 2,
    ATTACH_Corner                            = 3,
    ATTACH_All                               = 4,
    ATTACH_WallThenFloor                     = 5,
    ATTACH_FloorAndStairs                    = 6,
    ATTACH_CeilingAndStairs                  = 7,
    ATTACH_None                              = 8,
    ATTACH_MAX                               = 9,
};

void AFortDecoTool::ServerSpawnDeco(FFrame& Stack)
{
    FVector Location;
    FRotator Rotation;
    ABuildingSMActor* AttachedActor;
    EBuildingAttachmentType InBuildingAttachmentType;
    Stack.StepCompiledIn(&Location);
    Stack.StepCompiledIn(&Rotation);
    Stack.StepCompiledIn(&AttachedActor);
    Stack.StepCompiledIn(&InBuildingAttachmentType);
    Stack.IncrementCode();
    
    struct FortDecoTool_ServerSpawnDeco final
    {
    public:
        struct FVector                                Location;                                          // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        struct FRotator                               Rotation;                                          // 0x000C(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
        class ABuildingSMActor*                       AttachedActor;                                     // 0x0018(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        EBuildingAttachmentType                       InBuildingAttachmentType;                          // 0x0020(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        uint8                                         Pad_1D87[0x7];                                     // 0x0021(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
    } Params{Location, Rotation, AttachedActor, InBuildingAttachmentType};

    callExecOG(this, "/Script/FortniteGame.FortDecoTool", ServerSpawnDeco, Params);
    auto Pawn = (APawn*)GetOwner();
    if (!Pawn) return;
    auto PlayerController = (AFortPlayerControllerAthena*)Pawn->GetController();
    if (!PlayerController) return;

    auto TrapPtr = AttachedActor->GetAttachedBuildingActors().Search([&](ABuildingSMActor*& Actor) {
        return Actor->IsA<ABuildingTrap>() && Actor->GetTeamIndex() != ((AFortPlayerStateAthena*)PlayerController->GetPlayerState())->GetTeamIndex();
        });
    
    if (TrapPtr) {
        (*TrapPtr)->SetTeamIndex(((AFortPlayerStateAthena*)PlayerController->GetPlayerState())->GetTeamIndex());
        (*TrapPtr)->SetTeam(EFortTeam((*TrapPtr)->GetTeamIndex()));
    }
}

void AFortDecoTool::ServerCreateBuildingAndSpawnDeco(FFrame& Stack)
{
    FVector_NetQuantize10 BuildLoc;
    FRotator BuildRot;
    FVector_NetQuantize10 Location;
    FRotator Rotation;
    EBuildingAttachmentType InBuildingAttachmentType;
    Stack.StepCompiledIn(&BuildLoc);
    Stack.StepCompiledIn(&BuildRot);
    Stack.StepCompiledIn(&Location);
    Stack.StepCompiledIn(&Rotation);
    Stack.StepCompiledIn(&InBuildingAttachmentType);
    Stack.IncrementCode();

    auto Pawn = (APawn*)GetOwner();
    if (!Pawn) return;
    auto PlayerController = (AFortPlayerControllerAthena*)Pawn->GetController();
    if (!PlayerController) return;

    static auto WoodFloorClass = StaticLoadObject<UClass>("/Game/Building/ActorBlueprints/Player/Wood/L1/PBWA_W1_Floor.PBWA_W1_Floor_C");
    static auto BrickFloorClass = StaticLoadObject<UClass>("/Game/Building/ActorBlueprints/Player/Stone/L1/PBWA_S1_Floor.PBWA_S1_Floor_C");
    static auto MetalFloorClass = StaticLoadObject<UClass>("/Game/Building/ActorBlueprints/Player/Metal/L1/PBWA_M1_Floor.PBWA_M1_Floor_C");

    UClass* BuildingClass = nullptr;
    auto RemoteBuildingMaterial = PlayerController->GetBroadcastRemoteClientInfo()->GetRemoteBuildingMaterial();
        
    switch (RemoteBuildingMaterial)
    {
    case EFortResourceType::Wood:
        BuildingClass = WoodFloorClass;
        break;
    case EFortResourceType::Stone:
        BuildingClass = BrickFloorClass;
        break;
    case EFortResourceType::Metal:
        BuildingClass = MetalFloorClass;
        break;
    default:
        BuildingClass = WoodFloorClass;
        break;
    }
    
    TArray<AActor*> ExistingBuildings;
    char PlacementResultFlag;
    static auto CantBuild = UKismetMemLibrary::Get<__int64 (*)(UWorld*, UObject*, FVector, FRotator, bool, TArray<AActor*>*, char*)>(L"CantBuild");
    
    if (CantBuild(GetWorld(), BuildingClass, BuildLoc, BuildRot, false, &ExistingBuildings, &PlacementResultFlag)) {
        ExistingBuildings.Free(); return;
    }
    
    for (AActor* Building : ExistingBuildings) if (Building) Building->K2_DestroyActor();
    
    static map<UClass*, EFortResourceType> ResourceMap;
    if (!ResourceMap.contains(BuildingClass))
        ResourceMap[BuildingClass] = ((ABuildingSMActor*)BuildingClass->GetClassDefaultObject())->GetResourceType();
    
    auto* Resource = UFortKismetLibrary::K2_GetResourceItemDefinition(ResourceMap[BuildingClass]);
    FFortItemEntry* ItemEntry = nullptr;
    auto WorldInventory = PlayerController->GetWorldInventory();

    for (UFortWorldItem* Item : WorldInventory->GetInventory().GetItemInstances())
        if (Item->GetItemEntry().GetItemDefinition() == Resource) { ItemEntry = &Item->GetItemEntry(); break; }
    if (!PlayerController->CanBuildFree())
    {
        int32 count = ItemEntry->GetCount();
        if (!ItemEntry || count < 10) return;
        ItemEntry->SetCount(count - 10);
        WorldInventory->ReplaceEntry(*ItemEntry);
    }

    auto Building = GetWorld()->SpawnActor<ABuildingSMActor>(BuildingClass, BuildLoc, BuildRot, PlayerController);
    
    Building->SetbPlayerPlaced(true);
    Building->SetMirrored(false);
    Building->InitializeKismetSpawnedBuildingActor(Building, PlayerController, true);
    
    if (auto* PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->GetPlayerState())) {
        uint8 TeamIndex = PlayerState->GetTeamIndex();
        if (Fortnite_Version >= 12.00) Building->Set("BuildingActor", "TeamIndex", TeamIndex) ;
        Building->Set("BuildingActor", "Team", EFortTeam(TeamIndex));
    }

    CallFunc<void>("FortDecoTool", "ServerSpawnDeco", Location, Rotation, Building, InBuildingAttachmentType);
}
