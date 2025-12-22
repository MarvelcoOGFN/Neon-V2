#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Quests/FortQuestManager.h"

#include "Engine/Runtime/FortniteGame/Public/Components/FortMutatorListComponent.h"
#include "Engine/Runtime/FortniteGame/Public/Mutators/FortAthenaMutator_CreativeInfiniteResources.h"
#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"
#include "Engine/Plugins/Kismet/Public/KismetMathLibrary.h"
#include "Engine/Plugins/Neon/Public/Neon.h"
#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingContainer.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/AthenaSprayItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortContextTrapItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortDecoItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortMontageItemDefinitionBase.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponRangedItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Weapons/FortDecoTool_ContextTrap.h"
#include "Engine/Runtime/FortniteGame/Public/Weapons/FortWeap_BuildingTool.h"
#include "Engine/Runtime/FortniteGame/Public/Weapons/FortWeap_EditingTool.h"

void AFortPlayerController::ServerExecuteInventoryItem(FGuid ItemGuid)
{
    if ( !this || !GetMyFortPawn() ) return;
    
    FFortItemList& Inventory = GetWorldInventory()->GetInventory();
    TArray<FFortItemEntry>& ReplicatedEntries = Inventory.GetReplicatedEntries();
    
    FFortItemEntry* Entry = nullptr;
    uint8* Data = (uint8*)ReplicatedEntries.GetData();
    int32 Count = ReplicatedEntries.Num();
    static int32 FortItemEntrySize = StaticClassImpl("FortItemEntry")->GetSize();
    
    for (int32 i = 0; i < Count; ++i) {
        auto Item = ( FFortItemEntry* )( Data + ( i * FortItemEntrySize ) );
        if (Item->GetItemGuid() == ItemGuid) { Entry = Item; break; }
    }

    if ( Entry && GetMyFortPawn() )
    {
        auto ItemDefinition = Entry->GetItemDefinition();
        if (ItemDefinition->IsA(UFortDecoItemDefinition::StaticClass()))
        {
            auto DecoItemDefinition = Cast<UFortDecoItemDefinition>(ItemDefinition);
            if (!DecoItemDefinition) return;
    
            GetMyFortPawn()->PickUpActor(nullptr, DecoItemDefinition);
            GetMyFortPawn()->GetCurrentWeapon()->SetItemEntryGuid(ItemGuid);

            if (auto ContextTrap = Cast<AFortDecoTool_ContextTrap>(GetMyFortPawn()->GetCurrentWeapon()))
            {
                ContextTrap->SetContextTrapItemDefinition(Cast<UFortContextTrapItemDefinition>(DecoItemDefinition));
            }
            return;
        }
        
        if (Fortnite_Version <= 14.60)
        {
           GetMyFortPawn()->EquipWeaponDefinition(ItemDefinition, Entry->GetItemGuid()); 
        } else if (Fortnite_Version >= 15.00 && Fortnite_Version <= 18.40)
        {
            GetMyFortPawn()->EquipWeaponDefinition(ItemDefinition, Entry->GetItemGuid(), Entry->GetTrackerGuid()); 
        }
    }
    
    AFortWeapon* CurrentWeapon = GetMyFortPawn() ? GetMyFortPawn()->GetCurrentWeapon() : nullptr;
    if ( !CurrentWeapon ) return;
    if ( CurrentWeapon->IsA(AFortWeap_BuildingTool::StaticClass())
        && !IsA(AFortPlayerControllerAthena::StaticClass()) )
    {
        AFortWeap_BuildingTool* Tool = (AFortWeap_BuildingTool*)CurrentWeapon;
        Tool->SetDefaultMetadata(((UFortBuildingItemDefinition*)Tool->GetWeaponData())->GetBuildingMetaData().Get());
        Tool->OnRep_DefaultMetadata(Tool->GetDefaultMetadata()); 
    }
}

void AFortPlayerController::ServerLoadingScreenDropped(FFrame& Stack)
{
    Stack.IncrementCode();
    
    static UFortAbilitySet* AbilitySet = Fortnite_Version > 8.30 ? StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer")
        : StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");
    
    AFortPlayerState* PlayerStateAthena = Cast<AFortPlayerState>(GetPlayerState());
    PlayerStateAthena->GetAbilitySystemComponent()->GiveAbilitySet(AbilitySet);
}

std::vector<struct FItemAndCount> AFortPlayerController::GetLategameLoadout()
{
    static std::vector<FItemAndCount> Shotguns{
        FItemAndCount(1, {},    StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03")), // pump 
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03")), // pump 
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03")), // pump 
    };

    static std::vector<FItemAndCount> AssaultRifles{
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03")), // scar epic
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03")), // scar gold 
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_VR_Ore_T03.WID_Assault_Suppressed_Athena_VR_Ore_T03")), // suppresed epic 
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_SR_Ore_T03.WID_Assault_Suppressed_Athena_SR_Ore_T03")), // suppresed gold
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_R_Ore_T03.WID_Assault_Heavy_Athena_R_Ore_T03")), // gold p90
    };

    static std::vector<FItemAndCount> Snipers {
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03")), // heavy sniper
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/LTM/WID_Sniper_NoScope_Athena_VR_Ore_T03.WID_Sniper_NoScope_Athena_VR_Ore_T03")), // hunting
        FItemAndCount(1, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03")), // bolt sniper
    };

    static std::vector<FItemAndCount> Heals{
        FItemAndCount(3, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")), // big pots
        FItemAndCount(6, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall")), // minis
        FItemAndCount(6, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Consumables/ShockwaveGrenade/Athena_ShockGrenade.Athena_ShockGrenade")), // shockwave
        FItemAndCount(2, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff")), // Slurp Juice
        FItemAndCount(4, {}, StaticFindObject<UFortWeaponRangedItemDefinition>("/Game/Athena/Items/Consumables/ChillBronco/Athena_ChillBronco.Athena_ChillBronco")), // Slurp Juice
     };

    int f = rand() % Heals.size();
    int s;
    do {
        s = rand() % Heals.size();
    } while (s == f && Heals.size() > 1);

    if (!Heals[s].Item) s = (f + 1) % Heals.size();

    return {
        Shotguns[rand() % Shotguns.size()],
        AssaultRifles[rand() % AssaultRifles.size()],
        Snipers[rand() % Snipers.size()],
        Heals[f],
        Heals[s]
    };
}

void AFortPlayerController::ServerAttemptAircraftJump(FRotator Rotation)
{
    auto GameMode = GetWorld()->GetAuthorityGameMode();
    auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());
    
    if (GameMode)
    {
        AFortInventory* WorldInventory = GetWorldInventory();

        if (WorldInventory)
        {
            FFortItemList& Inventory = WorldInventory->GetInventory();
            for (int i = Inventory.GetItemInstances().Num() - 1; i >= 0; --i)
            {
                UFortWorldItem* ItemInstance = Inventory.GetItemInstances()[i];

                if (!ItemInstance || !ItemInstance->CanBeDropped()) continue;
                
                WorldInventory->Remove(Inventory.GetItemInstances()[i]->GetItemEntry().GetItemGuid());
            }
        }
        
        GameMode->RestartPlayer(this);
        ClientSetRotation(Rotation, false);
        auto SpawnLocation = GameState->GetAircrafts()[0]->K2_GetActorLocation();
        SpawnLocation.Z -= UKismetMathLibrary::RandomIntegerInRange(370, 700);
        SpawnLocation.X += UKismetMathLibrary::RandomIntegerInRange(200, 750);
        SpawnLocation.X -= UKismetMathLibrary::RandomIntegerInRange(200, 400);

        GetMyFortPawn()->K2_TeleportTo(SpawnLocation, {});
        GetMyFortPawn()->BeginSkydiving(true);

        GameState->SetGamePhase(EAthenaGamePhase::SafeZones);
        GameState->CallFunc<void>("FortGameModeAthena", "OnRep_GamePhase", EAthenaGamePhase::Count);
        
        if (GNeon->bLategame)
        {
            auto Loadout = GetLategameLoadout();
            for (auto Item : Loadout)
            {
                if (!Item.Item) continue;
                WorldInventory->GiveItem(Item.Item, Item.Count, 0, 1);
            }

            static auto Assault = StaticFindObject<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
            static auto Shotgun = StaticFindObject<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells");
            static auto Submachine = StaticFindObject<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
            static auto Rocket = StaticFindObject<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets");
            static auto Sniper = StaticFindObject<UFortAmmoItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");

            WorldInventory->GiveItem(Assault, 120, 0, 1);
            WorldInventory->GiveItem(Shotgun, 30, 0, 1);
            WorldInventory->GiveItem(Submachine, 180, 0, 1);
            WorldInventory->GiveItem(Rocket, 6, 0, 1);
            WorldInventory->GiveItem(Sniper, 18, 0, 1);

            WorldInventory->GiveItem(UFortKismetLibrary::K2_GetResourceItemDefinition(Wood), 999, 999, 1);
            WorldInventory->GiveItem(UFortKismetLibrary::K2_GetResourceItemDefinition(Stone), 999, 999, 1);
            WorldInventory->GiveItem(UFortKismetLibrary::K2_GetResourceItemDefinition(Metal), 999, 999, 1);
        }
    }
}

void AFortPlayerController::GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation)
{
    if ( GetStateName().ToString() == L"Spectating" ) {
        OutViewLocation = GetLastSpectatorSyncLocation();
        OutViewRotation = GetLastSpectatorSyncRotation();
        return;
    }
    else if ( auto MyFortPawn = GetMyFortPawn() ) {
        OutViewLocation = MyFortPawn->K2_GetActorLocation();
        OutViewRotation = GetControlRotation();
        return;
    }
    
    return GetPlayerViewPointOG(this, OutViewLocation, OutViewRotation);
}

bool AFortPlayerController::CanBuildFree()
{
    if (GNeon->bFreeBuilds) return true;
    if (!GNeon->bCreative) return false;
    auto PC = Cast<AFortPlayerControllerAthena>(this);
    static auto fn1 = StaticFindObject<UFunction>("/Script/FortniteGame.FortPlayerControllerAthena.IsPlayerInAMinigame");
    static auto ir = AFortAthenaMutator_InfiniteResources::StaticClass();
    static auto cir = AFortAthenaMutator_CreativeInfiniteResources::StaticClass();
    struct FortPlayerControllerAthena_IsPlayerInAMinigame
    {
    public:
        bool                                          ReturnValue;                                       // 0x0000(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    };
    FortPlayerControllerAthena_IsPlayerInAMinigame ret{};
    PC->ProcessEvent(fn1, &ret);
    if (!ret.ReturnValue)
    {
        return true;
    }
    auto vol = PC->GetCurrentVolume();
    auto cmpnt = (UFortMutatorListComponent*)vol->GetComponentByClass(UFortMutatorListComponent::StaticClass());
    auto Mutators = cmpnt->GetMutators();
    for (int32 i = 0; i < Mutators.Num(); i++)
    {
        auto mut = Mutators[i];
        if (mut->IsA(ir) || mut->IsA(cir))
        {
            auto castmut = Cast<AFortAthenaMutator_InfiniteResources>(mut);
            return castmut->GetbInfiniteResources();
        }
    }
    return false;
}

void AFortPlayerController::ServerRepairBuildingActor(FFrame& Stack)
{
    ABuildingSMActor* Building = nullptr;
    Stack.StepCompiledIn(&Building);
    Stack.IncrementCode();
    if (!Building) return;

    int32 Price = (int32)std::floor(10.f * (1.f - Building->CallFunc<float>("BuildingActor", "GetHealthPercent")) * 0.75f);
    UFortItemDefinition* Resource = UFortKismetLibrary::K2_GetResourceItemDefinition(Building->GetResourceType());

    AFortInventory* Inv = GetWorldInventory();
    if (!Inv) return;

    FFortItemList& Items = Inv->GetInventory();
    FFortItemEntry* Entry = nullptr;
    for (UFortWorldItem* Item : Items.GetItemInstances())
        if (Item->GetItemEntry().GetItemDefinition() == Resource) { Entry = &Item->GetItemEntry(); break; }

    if (Entry && Entry->GetCount() >= Price)
    {
        Entry->SetCount(Entry->GetCount() - Price);
        Inv->ReplaceEntry(*Entry);
        Building->CallFunc<void>("BuildingSMActor", "RepairBuilding", this, Price);
    }
}

void AFortPlayerController::ServerAttemptInventoryDrop(FFrame& Stack)
{
    FGuid ItemGuid;
    int32 Count;
    Stack.StepCompiledIn(&ItemGuid);
    Stack.StepCompiledIn(&Count);
    Stack.IncrementCode();

    static int32 FFortItemEntrySize = StaticClassImpl("FortItemEntry")->GetSize();

    AFortInventory* WorldInventory = GetWorldInventory();
    FFortItemList& Inventory = WorldInventory->GetInventory();
    TArray<FFortItemEntry>& ReplicatedEntries = Inventory.GetReplicatedEntries();
    FFortItemEntry* Entry = nullptr;
    
    for (int32 i = 0; i < ReplicatedEntries.Num(); ++i) {
        auto E = (FFortItemEntry*) ((uint8*) ReplicatedEntries.GetData() + (i * FFortItemEntrySize));

        if (E->GetItemGuid() == ItemGuid) {
            Entry = E;
            break;
        }
    }

    if (!Entry || (Entry->GetCount() - Count) < 0)
    {
        return;
    }
	
    Entry->SetCount(Entry->GetCount() - Count);
    AFortInventory::SpawnPickupStatic(GetMyFortPawn()->K2_GetActorLocation() + GetMyFortPawn()->GetActorForwardVector() * 70.f + FVector(0, 0, 50), Entry, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, GetMyFortPawn(), Count, true, true, true);
    if (Entry->GetCount() == 0)
        WorldInventory->Remove(ItemGuid);
    else
        WorldInventory->ReplaceEntry(*Entry);
}


void AFortPlayerController::ServerAttemptInteract(FFrame& Stack)
{
    enum class ETInteractionType : uint8
    {
        IT_NoInteraction                         = 0,
        IT_Simple                                = 1,
        IT_LongPress                             = 2,
        IT_BuildingEdit                          = 3,
        IT_BuildingImprovement                   = 4,
        IT_TrapPlacement                         = 5,
        IT_MAX                                   = 6,
    };
    
    class AActor* ReceivingActor;
    class UPrimitiveComponent* InteractComponent;
    ETInteractionType InteractType;
    class UObject* OptionalObjectData;
    Stack.StepCompiledIn(&ReceivingActor);
    Stack.StepCompiledIn(&InteractComponent);
    Stack.StepCompiledIn(&InteractType);
    Stack.StepCompiledIn(&OptionalObjectData);
    Stack.IncrementCode();

    if (ReceivingActor->IsA(ABuildingContainer::StaticClass()))
    {
        ABuildingContainer* Container = Cast<ABuildingContainer>(ReceivingActor);
        
        if (Container->GetbAlreadySearched()) {
            goto CallOriginal;
        }
        
        Container->SpawnLoot();
    }

    CallOriginal:
    struct FortPlayerController_ServerAttemptInteract final
    {
    public:
        class AActor*                                 ReceivingActor;                                    // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        class UPrimitiveComponent*                    InteractComponent;                                 // 0x0008(0x0008)(Parm, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        ETInteractionType                             InteractType;                                      // 0x0010(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        uint8                                         Pad_11[0x7];                                       // 0x0011(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
        class UObject*                                OptionalObjectData;                                // 0x0018(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    } Params{};

    Params.ReceivingActor = ReceivingActor;
    Params.InteractComponent = InteractComponent;
    Params.InteractType = InteractType;
    Params.OptionalObjectData = OptionalObjectData;
    
    callExecOG(this, "/Script/FortniteGame.FortPlayerController", ServerAttemptInteract, Params);
    
    if (ReceivingActor->IsA(ABuildingActor::StaticClass()))
    {
        ABuildingActor* BuildingActor = (ABuildingActor*)ReceivingActor;
        if (!IsValidPointer(BuildingActor)) return;
        FGameplayTagContainer SourceTags;
        FGameplayTagContainer TargetTags;
        FGameplayTagContainer ContextTags;
        UFortQuestManager* QuestManager = GetQuestManager(ESubGame::Athena);
        QuestManager->GetSourceAndContextTags(&SourceTags, &ContextTags);
        TargetTags.AppendTags(BuildingActor->GetStaticGameplayTags());
        
        QuestManager->SendStatEvent(SourceTags, TargetTags, nullptr, nullptr, 1, EFortQuestObjectiveStatEvent::Interact, ContextTags);
    }
}

bool CanBePlacedByPlayer(UClass* BuildClass) {
    for (auto& Pair : GetWorld()->GetGameState()->GetAllPlayerBuildableClassesIndexLookup())
        if (Pair.Key == BuildClass) { BuildClass = Pair.Key; break; }
    if (!BuildClass) return false;

    return true;
}

void AFortPlayerController::ServerCreateBuildingActor(FFrame& Stack)
{
    UClass* BuildingClass = nullptr;
    FVector BuildLoc;
    FRotator BuildRot;
    bool bMirrored;
    
    if (Fortnite_Version >= 8.30) {
        struct { uint32_t BuildingClassHandle; FVector BuildLoc; FRotator BuildRot; bool bMirrored; 
                 unsigned char Unk2[0x3]; float SyncKey; unsigned char Unk[0x4]; FBuildingClassData BuildingClassData; } Data{};
        Stack.StepCompiledIn(&Data);
        Stack.IncrementCode();
        BuildLoc = Data.BuildLoc; BuildRot = Data.BuildRot; bMirrored = Data.bMirrored;
        
        if (Fortnite_Version >= 7.40) {
            for (auto& Pair : GetWorld()->GetGameState()->GetAllPlayerBuildableClassesIndexLookup())
                if (Pair.Value == Data.BuildingClassHandle) { BuildingClass = Pair.Key; break; }
            if (!BuildingClass) return;
        } else BuildingClass = Data.BuildingClassData.BuildingClass;
    } else {
        FBuildingClassData BuildingClassData{};
        Stack.StepCompiledIn(&BuildingClassData);
        Stack.StepCompiledIn(&BuildLoc);
        Stack.StepCompiledIn(&BuildRot);
        Stack.StepCompiledIn(&bMirrored);
        Stack.IncrementCode();
        BuildingClass = BuildingClassData.BuildingClass;
    }
    
    TArray<AActor*> ExistingBuildings;
    char PlacementResultFlag;
    static auto CantBuild = UKismetMemLibrary::Get<__int64 (*)(UWorld*, UObject*, FVector, FRotator, bool, TArray<AActor*>*, char*)>(L"CantBuild");
    
    if (CantBuild(GetWorld(), BuildingClass, BuildLoc, BuildRot, bMirrored, &ExistingBuildings, &PlacementResultFlag)) {
        ExistingBuildings.Free(); return;
    }
    
    for (AActor* Building : ExistingBuildings) if (Building) Building->K2_DestroyActor();
    
    static map<UClass*, EFortResourceType> ResourceMap;
    if (!ResourceMap.contains(BuildingClass))
        ResourceMap[BuildingClass] = ((ABuildingSMActor*)BuildingClass->GetClassDefaultObject())->GetResourceType();
    
    auto* Resource = UFortKismetLibrary::K2_GetResourceItemDefinition(ResourceMap[BuildingClass]);
    FFortItemEntry* ItemEntry = nullptr;
    auto WorldInventory = GetWorldInventory();

    for (UFortWorldItem* Item : WorldInventory->GetInventory().GetItemInstances())
        if (Item->GetItemEntry().GetItemDefinition() == Resource) { ItemEntry = &Item->GetItemEntry(); break; }
    if (!CanBuildFree())
    {
        int32 count = ItemEntry->GetCount();
        if (!ItemEntry || count < 10) return;
        ItemEntry->SetCount(count - 10);
        WorldInventory->ReplaceEntry(*ItemEntry);
    }

    auto Building = GetWorld()->SpawnActor<ABuildingSMActor>(BuildingClass, BuildLoc, BuildRot, this);

    Building->SetbPlayerPlaced(true);
    Building->SetMirrored(bMirrored);
    Building->InitializeKismetSpawnedBuildingActor(Building, this, true);
    
    if (auto* PlayerState = Cast<AFortPlayerStateAthena>(GetPlayerState())) {
        uint8 TeamIndex = PlayerState->GetTeamIndex();
        if (Fortnite_Version >= 12.00) Building->SetTeamIndex(TeamIndex) ;
        Building->SetTeam(EFortTeam(TeamIndex));
        Building->OnRep_Team();
        Building->OnSetTeam(TeamIndex);
    }
}

void AFortPlayerController::ServerBeginEditingBuildingActor(FFrame& Stack)
{
    ABuildingSMActor* BuildingSMActor; 
    Stack.StepCompiledIn(&BuildingSMActor); 
    Stack.IncrementCode(); 
    if (!BuildingSMActor || !GetMyFortPawn()) return; 
    
    AFortPlayerStateAthena* PlayerStateAthena = (AFortPlayerStateAthena*)Cast<AFortPlayerState>(GetPlayerState());
    uint8 BuildingTeam = Fortnite_Version >= 12.00 ? BuildingSMActor->GetTeamIndex(): (uint8)BuildingSMActor->GetTeam();
    if (IsA(AFortPlayerControllerAthena::StaticClass()) && BuildingTeam != PlayerStateAthena->GetTeamIndex()) return;
    
    static auto EditTool = StaticFindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool"); 
    FFortItemEntry* ItemEntry = GetWorldInventory()->FindItemEntryDef(EditTool); 
    if (!ItemEntry) return;     
    
    static auto SetEditingPlayer = UKismetMemLibrary::Get<void(*)(ABuildingSMActor*, AFortPlayerStateAthena*)>(TEXT("SetEditingPlayer")); 
    SetEditingPlayer(BuildingSMActor, PlayerStateAthena); 

    AFortWeap_EditingTool* EditingTool = nullptr;
    
    if (Fortnite_Version <= 14.60)
    {
        EditingTool = (AFortWeap_EditingTool*)(GetMyFortPawn()->EquipWeaponDefinition(EditTool, ItemEntry->GetItemGuid())); 
    } else if (Fortnite_Version >= 15.00 && Fortnite_Version <= 18.40)
    {
        EditingTool = (AFortWeap_EditingTool*)(GetMyFortPawn()->EquipWeaponDefinition(EditTool, ItemEntry->GetItemGuid(), ItemEntry->GetTrackerGuid())); 
    }

    if (IsValidPointer(EditingTool)) { 
        EditingTool->SetEditActor(BuildingSMActor); 
        EditingTool->OnRep_EditActor(); 
    }
}

void AFortPlayerController::ServerEmote(FFrame& Stack)
{
    FName AssetName;
    Stack.StepCompiledIn(&AssetName);
    Stack.IncrementCode();
    
    UFortMontageItemDefinitionBase* Emote = (UFortMontageItemDefinitionBase*)GUObjectArray.FindObject(AssetName.ToString().ToString());
    if (!Emote) return;
    
    static auto GenericAbility = StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");
    static auto EmoteAbility = GenericAbility->GetClassDefaultObject();
    UGameplayAbility* Ability = Cast<UGameplayAbility>(EmoteAbility);
    UAbilitySystemComponent* AbilitySystemComponent = Cast<AFortPlayerState>(GetPlayerState())->GetAbilitySystemComponent();
    
    if (Ability)
    {
        static FGameplayAbilitySpec* StaticSpec = nullptr;
        static bool bInitialized = false;

        if (!bInitialized)
        {
            int32 GameplayAbilitySpecSize = StaticClassImpl("GameplayAbilitySpec")->GetSize();
            StaticSpec = (FGameplayAbilitySpec*)malloc(GameplayAbilitySpecSize);
            if (!StaticSpec)
                return;
            memset(StaticSpec, 0, GameplayAbilitySpecSize);
            new (StaticSpec) FGameplayAbilitySpec();
            bInitialized = true;
        }

        if (!StaticSpec)
            return;
        
        StaticSpec->MostRecentArrayReplicationKey = -1;
        StaticSpec->ReplicationID = -1;
        StaticSpec->ReplicationKey = -1;
        StaticSpec->GetHandle().Handle = rand();
        StaticSpec->SetAbility(Ability);
        StaticSpec->SetSourceObject(Emote);
        StaticSpec->SetInputID(-1);
        StaticSpec->SetLevel(1);
        StaticSpec->SetRemoveAfterActivation(true);
        
        FGameplayAbilitySpecHandle Handle = StaticSpec->GetHandle();

        UKismetMemLibrary::Get<FGameplayAbilitySpecHandle(__fastcall*)(
            UAbilitySystemComponent*,
            FGameplayAbilitySpecHandle*, 
            const FGameplayAbilitySpec&
        )>(L"GiveAbility")(AbilitySystemComponent, &Handle, *StaticSpec);

        UGameplayAbility* InstancedAbility = NULL;
        FPredictionKey PredictionKey{};
        
        UKismetMemLibrary::Get<bool (*)(
            UAbilitySystemComponent*, 
            FGameplayAbilitySpecHandle, 
            FPredictionKey, 
            UGameplayAbility**, 
            void*, 
            const FGameplayEventData*
        )>(L"InternalTryActivateAbility")(AbilitySystemComponent, Handle, PredictionKey, &InstancedAbility, nullptr, nullptr);
    }
}

void AFortPlayerController::ServerPlayEmoteItem(FFrame& Stack)
{
    UFortMontageItemDefinitionBase* Asset;
    Stack.StepCompiledIn(&Asset);
    Stack.IncrementCode();
    
    if (!GetMyFortPawn() || !Asset) return;
	
    UAbilitySystemComponent* AbilitySystemComponent = Cast<AFortPlayerStateAthena>(GetPlayerState())->GetAbilitySystemComponent();
    UGameplayAbility* Ability = nullptr;

    static auto GenericAbility = StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");
    static auto SprayAbility = StaticLoadObject<UClass>("/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C");

    if (Cast<UAthenaSprayItemDefinition>(Asset))
    {
        static auto Spray = SprayAbility->GetClassDefaultObject();
        Ability = Cast<UGameplayAbility>(Spray);
    }
    
    if (auto Dance = Cast<UAthenaDanceItemDefinition>(Asset))
    {
        GetMyFortPawn()->SetbMovingEmote(Dance->GetbMovingEmote());
        if (Fortnite_Version >= 8.00) GetMyFortPawn()->SetbMovingEmoteForwardOnly(Dance->GetbMoveForwardOnly());
        GetMyFortPawn()->SetEmoteWalkSpeed(Dance->GetWalkForwardSpeed());
        static auto EmoteAbility = GenericAbility->GetClassDefaultObject();
        Ability = Cast<UGameplayAbility>(EmoteAbility);
    }
    
    if (Ability)
    {
        static FGameplayAbilitySpec* StaticSpec = nullptr;
        static bool bInitialized = false;

        if (!bInitialized)
        {
            int32 GameplayAbilitySpecSize = StaticClassImpl("GameplayAbilitySpec")->GetSize();
            StaticSpec = (FGameplayAbilitySpec*)malloc(GameplayAbilitySpecSize);
            if (!StaticSpec)
                return;
            memset(StaticSpec, 0, GameplayAbilitySpecSize);
            new (StaticSpec) FGameplayAbilitySpec();
            bInitialized = true;
        }

        if (!StaticSpec)
            return;
        
        StaticSpec->MostRecentArrayReplicationKey = -1;
        StaticSpec->ReplicationID = -1;
        StaticSpec->ReplicationKey = -1;
        StaticSpec->GetHandle().Handle = rand();
        StaticSpec->SetAbility(Ability);
        StaticSpec->SetSourceObject(Asset);
        StaticSpec->SetInputID(-1);
        StaticSpec->SetLevel(1);
        StaticSpec->SetRemoveAfterActivation(true);
        
        FGameplayAbilitySpecHandle Handle = StaticSpec->GetHandle();

        UKismetMemLibrary::Get<FGameplayAbilitySpecHandle(__fastcall*)(
            UAbilitySystemComponent*,
            FGameplayAbilitySpecHandle*, 
            const FGameplayAbilitySpec&
        )>(L"GiveAbility")(AbilitySystemComponent, &Handle, *StaticSpec);

        UGameplayAbility* InstancedAbility = NULL;
        FPredictionKey PredictionKey{};
        
        UKismetMemLibrary::Get<bool (*)(
            UAbilitySystemComponent*, 
            FGameplayAbilitySpecHandle, 
            FPredictionKey, 
            UGameplayAbility**, 
            void*, 
            const FGameplayEventData*
        )>(L"InternalTryActivateAbility")(AbilitySystemComponent, Handle, PredictionKey, &InstancedAbility, nullptr, nullptr);
    }
}

void AFortPlayerController::ServerEditBuildingActor(FFrame& Stack)
{
    ABuildingSMActor* BuildingSMActor; 
    TSubclassOf<ABuildingSMActor> NewClass; 
    uint8 RotationIterations; 
    bool bMirrored; 
    Stack.StepCompiledIn(&BuildingSMActor); 
    Stack.StepCompiledIn(&NewClass); 
    Stack.StepCompiledIn(&RotationIterations); 
    Stack.StepCompiledIn(&bMirrored); 
    Stack.IncrementCode(); 
    if (!BuildingSMActor || !GetMyFortPawn() || !CanBePlacedByPlayer(NewClass)) return; 
    
    AFortPlayerStateAthena* PlayerStateAthena = (AFortPlayerStateAthena*)Cast<AFortPlayerState>(GetPlayerState());
    uint8 BuildingTeam = Fortnite_Version >= 12.00 ? BuildingSMActor->GetTeamIndex(): (uint8)BuildingSMActor->GetTeam();
    if (!GetMyFortPawn() && IsA(AFortPlayerControllerAthena::StaticClass()) && BuildingTeam != PlayerStateAthena->GetTeamIndex()) return;
    
    static auto SetEditingPlayer = UKismetMemLibrary::Get<void(*)(ABuildingSMActor*, AFortPlayerStateAthena*)>(TEXT("SetEditingPlayer")); 
    SetEditingPlayer(BuildingSMActor, nullptr); 
    
    static auto ReplaceBuildingActor = UKismetMemLibrary::Get<ABuildingSMActor*(*)(ABuildingSMActor*, unsigned int, UObject*, unsigned int, int, bool, AFortPlayerController*)>(TEXT("ReplaceBuildingActor")); 
    ABuildingSMActor* NewBuild = ReplaceBuildingActor(BuildingSMActor, 1, NewClass, BuildingSMActor->Get<int32>("BuildingActor", "CurrentBuildingLevel"), RotationIterations, bMirrored, this); 
    if (NewBuild) { 
        if (Fortnite_Version >= 12.00) NewBuild->Set("BuildingActor", "TeamIndex", PlayerStateAthena->GetTeamIndex());
        NewBuild->Set("BuildingActor", "Team", EFortTeam(PlayerStateAthena->GetTeamIndex()));
        NewBuild->SetbPlayerPlaced(true); 
    }
}

void AFortPlayerController::ServerEndEditingBuildingActor(FFrame& Stack)
{
    ABuildingSMActor* BuildingSMActor; 
    Stack.StepCompiledIn(&BuildingSMActor); 
    Stack.IncrementCode(); 
    AFortPlayerStateAthena* PlayerStateAthena = (AFortPlayerStateAthena*)Cast<AFortPlayerState>(GetPlayerState());
    if (!BuildingSMActor || !GetMyFortPawn()) return;
    uint8 BuildingTeam = Fortnite_Version >= 12.00 ? BuildingSMActor->GetTeamIndex(): (uint8)BuildingSMActor->GetTeam();
    if (!GetMyFortPawn() && IsA(AFortPlayerControllerAthena::StaticClass()) && BuildingTeam != PlayerStateAthena->GetTeamIndex()) return;
    
    static auto SetEditingPlayer = UKismetMemLibrary::Get<void(*)(ABuildingSMActor*, AFortPlayerStateAthena*)>(TEXT("SetEditingPlayer")); 
    SetEditingPlayer(BuildingSMActor, nullptr); 
    
    AFortWeap_EditingTool* EditingTool = (AFortWeap_EditingTool*)(GetMyFortPawn()->GetCurrentWeapon()); 
    if (IsValidPointer(EditingTool)) { 
        EditingTool->SetbEditConfirmed(true); 
        EditingTool->SetEditActor(nullptr); 
        EditingTool->OnRep_EditActor(); 
    }
}
