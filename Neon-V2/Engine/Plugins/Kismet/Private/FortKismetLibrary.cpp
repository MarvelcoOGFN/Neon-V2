#include "pch.h"
#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"

#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"
#include "Engine/Plugins/Kismet/Public/KismetSystemLibrary.h"
#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"
#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortGameStateAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortPlaylistAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingContainer.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponMeleeItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

static TArray<FFortLootTierData*> TierDataAllGroups;
static TArray<FFortLootPackageData*> LPGroupsAll;

FFortItemEntry* MakeItemEntry(UFortItemDefinition* ItemDefinition, int32 Count, int32 Level) {
    if (!ItemDefinition) return nullptr;
    
    static int32 FortItemEntrySize = StaticClassImpl("FortItemEntry")->GetSize();
    
    FFortItemEntry* IE = (FFortItemEntry*)malloc(FortItemEntrySize);
    if (!IE) return nullptr;
    
    memset(IE, 0, FortItemEntrySize);
    new(IE) FFortItemEntry();
    
    IE->MostRecentArrayReplicationKey = -1;
    IE->ReplicationID = -1;
    IE->ReplicationKey = -1;

    IE->SetItemDefinition(ItemDefinition);
    IE->SetCount(Count);

    static UClass* Weapon = UFortWeaponMeleeItemDefinition::StaticClass();
    
    if (ItemDefinition->IsA(Weapon)) {
        UFortWeaponItemDefinition* WeaponDef = (UFortWeaponItemDefinition*)(ItemDefinition);
        int32 ClipSize = AFortInventory::GetStatsStatic(WeaponDef) ? AFortInventory::GetStatsStatic(WeaponDef)->GetClipSize() : 0;
        IE->SetLoadedAmmo(ClipSize);
    } else {
        IE->SetLoadedAmmo(0);
    }
    
    IE->SetDurability(1.f);
    IE->SetGameplayAbilitySpecHandle(FGameplayAbilitySpecHandle(-1));
    IE->SetLevel(Level);

    return IE;
}

AFortAIDirector* UFortKismetLibrary::GetAIDirector(FFrame& Stack, AFortAIDirector** Ret)
{
    Stack.IncrementCode();
    
    UObject* WorldContextObject = nullptr;

    Stack.StepCompiledIn(&WorldContextObject);

    *Ret = GetWorld()->GetAuthorityGameMode()->GetAIDirector();
    return *Ret;
}


AFortAIGoalManager* UFortKismetLibrary::GetAIGoalManager(FFrame& Stack, AFortAIGoalManager** Ret)
{
    Stack.IncrementCode();
    
    UObject* WorldContextObject = nullptr;

    Stack.StepCompiledIn(&WorldContextObject);

    *Ret = GetWorld()->GetAuthorityGameMode()->GetAIGoalManager();
    return *Ret;
}


static void SetupLootDropsForPackage(TArray<FFortItemEntry*>& LootDrops, FName Package, int i, FName TierGroup, int WorldLevel, int RecursionDepth) {
    if (RecursionDepth > 3 || LootDrops.Num() > 20)
    {
        UE_LOG(LogKismet, Log, TEXT("SetupLootDropsForPackage: Recursion depth exceeded or too many loot drops!"));
        return;
    }
    
    if (LPGroupsAll.Num() == 0) {
        auto World = GetWorld();
        if (!World || !World->GetGameState()) return;
        
        auto GameState = Cast<AFortGameStateAthena>(World->GetGameState());
        if (!GameState) return;
        
        UFortPlaylistAthena* Playlist = nullptr;

        if (Fortnite_Version >= 6.10)
        {
            static int CurrentPlaylistInfoOffset = UKismetMemLibrary::GetOffset(GameState, "CurrentPlaylistInfo");
            FPlaylistPropertyArray& CurrentPlaylistInfoPtr = *reinterpret_cast<FPlaylistPropertyArray*>(__int64(GameState) + CurrentPlaylistInfoOffset);

            Playlist = CurrentPlaylistInfoPtr.GetBasePlaylist();
        }
        else
        {
            Playlist = GameState->GetCurrentPlaylistData();
        }

        UDataTable* LootPackages = nullptr;
        
        if (Fortnite_Version >= 6.10)
        {
            static int LootPackagesOffset = UKismetMemLibrary::GetOffset(Playlist, "LootPackages");
            static auto ErmOk = *(TSoftObjectPtr<UDataTable>*)(__int64(Playlist) + LootPackagesOffset);

            LootPackages = ErmOk.Get(UCompositeDataTable::StaticClass(), true);
            if (!LootPackages) LootPackages = Playlist->GetLootPackages().Get(UDataTable::StaticClass(), true);
        }

        if (!LootPackages)
        {
            LootPackages = StaticLoadObject<UDataTable>("/Game/Items/DataTables/AthenaLootPackages_Client.AthenaLootPackages_Client");
        }
        
        auto AddPackages = [&](const TMap<FName, uint8*>& RowMap) {
            for (const auto& RowPair : RowMap) {
                if (auto Val = reinterpret_cast<FFortLootPackageData*>(RowPair.Value); IsValidPointer(Val)) {
                    LPGroupsAll.Add(Val);
                }
            }
        };
        
        AddPackages(LootPackages->GetRowMap());
        
        if (auto CompositeTable = Cast<UCompositeDataTable>(LootPackages)) {
            for (auto ParentTable : CompositeTable->GetParentTables()) {
                if (ParentTable) AddPackages(ParentTable->GetRowMap());
            }
        }
    }

    TArray<FFortLootPackageData*> LPGroups;
    for (auto Val : LPGroupsAll) {
        if (!Val || Val->GetLootPackageID().GetComparisonIndex() != Package.GetComparisonIndex()) continue;
        if (i != -1 && Val->GetLootPackageCategory() != i) continue;
        if (WorldLevel >= 0 && ((Val->GetMaxWorldLevel() >= 0 && WorldLevel > Val->GetMaxWorldLevel()) || 
                               (Val->GetMinWorldLevel() >= 0 && WorldLevel < Val->GetMinWorldLevel()))) continue;
        if (Val->GetCount() <= 0) continue;
        LPGroups.Add(Val);
    }
    
    if (LPGroups.Num() == 0) return;

    auto LootPackage = PickWeighted(LPGroups, [](float Total) { return ((float)rand() / 32767.f) * Total; });
    if (!LootPackage)
    {
        UE_LOG(LogKismet, Log, TEXT("SetupLootDropsForPackage: PickWeighted returned null!"));
        return;
    }
    
    auto LootPackageCall = LootPackage->GetLootPackageCall();
    if (LootPackageCall.GetData().Num() > 0) {
        for (int idx = 0; idx < LootPackage->GetCount(); idx++) {
            SetupLootDropsForPackage(LootDrops, UKismetStringLibrary::Conv_StringToName(LootPackageCall), 0, TierGroup, WorldLevel, RecursionDepth + 1);
        }
        return;
    }
    
    auto AddLoot = [&](UFortItemDefinition* Item, int Count) {
        for (auto& LootDrop : LootDrops) {
            if (LootDrop->GetItemDefinition() == Item) {
                LootDrop->GetCount() += Count;
                if (LootDrop->GetCount() > Item->GetMaxStackSize()) {
                    int32 Overflow = LootDrop->GetCount() - Item->GetMaxStackSize();
                    LootDrop->GetCount() = Item->GetMaxStackSize();
                    if (GetQuickbar(Item) == EFortQuickBars::Secondary) {
                        FFortItemEntry* OverflowItem = MakeItemEntry(Item, Overflow, std::clamp(1, Item->GetMinLevel(), Item->GetMaxLevel()));
                        if (OverflowItem) LootDrops.Add(OverflowItem);
                    }
                }
                
                return GetQuickbar(Item) == EFortQuickBars::Secondary;
            }
        }
        FFortItemEntry* I = MakeItemEntry(Item, Count, std::clamp(1, Item->GetMinLevel(), Item->GetMaxLevel()));
        if (Item) LootDrops.Add(I);
        
        return false;
    };

    auto ItemDefinition = Cast<UFortItemDefinition>(LootPackage->GetItemDefinition().Get());
    if (!IsValidPointer(ItemDefinition)) return;
    
    AddLoot(ItemDefinition, LootPackage->GetCount());

    if (Fortnite_Version.Season() > 11) // i think??
    {
        if (auto WeaponItemDefinition = Cast<UFortWeaponItemDefinition>(ItemDefinition)) {
            if (auto AmmoData = WeaponItemDefinition->GetAmmoData().Get()) {
                if (auto AmmoItemDefinition = Cast<UFortWorldItemDefinition>(AmmoData)) {
                    if (int AmmoCount = AmmoData->GetDropCount(); AmmoCount > 0) {
                        AddLoot(AmmoItemDefinition, AmmoCount);
                    }
                }
            }
        }
    }
}

TArray<FFortItemEntry*> UFortKismetLibrary::PickLootDrops(FName TierGroup, int LootTier, int WorldLevel)
{
    auto World = GetWorld();
    if (!World) {
        return {};
    }
    
    auto GameState = Cast<AFortGameStateAthena>(World->GetGameState());
    if (!GameState) {
        return {};
    }
    
    if (WorldLevel == -1) {
        WorldLevel = GameState->GetWorldLevel();
    }
    
    if (TierDataAllGroups.Num() == 0) {
        UDataTable* LootTierData = nullptr;
        
        static UFortPlaylistAthena* Playlist = nullptr;
        
        if (Fortnite_Version >= 6.10)
        {
            static int CurrentPlaylistInfoOffset = UKismetMemLibrary::GetOffset(GameState, "CurrentPlaylistInfo");
            FPlaylistPropertyArray& CurrentPlaylistInfoPtr = *reinterpret_cast<FPlaylistPropertyArray*>(__int64(GameState) + CurrentPlaylistInfoOffset);

            Playlist = CurrentPlaylistInfoPtr.GetBasePlaylist();
        }
        else
        {
            Playlist = GameState->GetCurrentPlaylistData();
        }
        // i dont fucking know man
        if (Fortnite_Version >= 6.10)
        {
            static int LootTierDataOffset = UKismetMemLibrary::GetOffset(Playlist, "LootTierData");
            static auto ErmOk2 = *(TSoftObjectPtr<UDataTable>*)(__int64(Playlist) + LootTierDataOffset);

            LootTierData = ErmOk2.Get(UCompositeDataTable::StaticClass(), true);
        
            if (!LootTierData) {
                LootTierData = Cast<UFortPlaylistAthena>(Playlist)->GetLootTierData().Get(UDataTable::StaticClass(), true);
            }
        }
        
        if (!LootTierData)
        {
            LootTierData = StaticLoadObject<UDataTable>("/Game/Items/DataTables/AthenaLootTierData_Client.AthenaLootTierData_Client");
        }
        
        if (auto CompositeTable = Cast<UCompositeDataTable>(LootTierData)) {
            auto& ParentTables = CompositeTable->GetParentTables();
            for (auto& ParentTable : ParentTables) {
                if (ParentTable) {
                    auto& RowMap = ParentTable->GetRowMap();
                    for (const auto& RowPair : RowMap) {
                        FFortLootTierData* Val = reinterpret_cast<FFortLootTierData*>(RowPair.Value);
                        if (IsValidPointer(Val)) {
                            TierDataAllGroups.Add(Val);
                        }
                    }
                }
            }
        }
        
        auto& MainRowMap = LootTierData->GetRowMap();
        for (const auto& RowPair : MainRowMap) {
            FFortLootTierData* Val = reinterpret_cast<FFortLootTierData*>(RowPair.Value);
            if (IsValidPointer(Val)) {
                TierDataAllGroups.Add(Val);
            }
        }
    }

    TArray<FFortLootTierData*> TierDataGroups;
    for (auto const& Val : TierDataAllGroups) {
        if (!Val) continue;
        
        if (Val->GetTierGroup().GetComparisonIndex() == TierGroup.GetComparisonIndex() && 
            (LootTier == -1 ? true : LootTier == Val->GetLootTier())) {
            TierDataGroups.Add(Val);
        }
    }
    
    if (TierDataGroups.Num() == 0) {
        UE_LOG(LogKismet, Log, ("PickLootDrops: No TierDataGroups found for TierGroup %s and LootTier %d!"), TierGroup.ToString().ToString().c_str(), LootTier);
        return {};
    }
    
    auto LootTierData = PickWeighted(TierDataGroups, [](float Total) { return ((float)rand() / 32767.f) * Total; });
    if (!LootTierData) {
        UE_LOG(LogKismet, Log, TEXT("PickLootDrops: LootTierData is null!"));
        return {};
    }

    float DropCount = 0;
    if (LootTierData->GetNumLootPackageDrops() > 0) {
        DropCount = LootTierData->GetNumLootPackageDrops() < 1 ? 1 : 
                   (float)((int32)((LootTierData->GetNumLootPackageDrops() * 2) - .5f) >> 1);

        if (LootTierData->GetNumLootPackageDrops() > 1) {
            float idk = LootTierData->GetNumLootPackageDrops() - DropCount;
            if (idk > 0.0000099999997f) {
                DropCount += idk >= ((float)rand() / 32767);
            }
        }
    }

    float AmountOfLootDrops = 0;

    for (auto& Min : LootTierData->GetLootPackageCategoryMinArray()) {
        AmountOfLootDrops += Min;
    }

    int32 SumWeights = 0;

    for (int i = 0; i < LootTierData->GetLootPackageCategoryWeightArray().Num(); ++i) {
        if (LootTierData->GetLootPackageCategoryWeightArray()[i] > 0 && 
            LootTierData->GetLootPackageCategoryMaxArray()[i] != 0) {
            SumWeights += LootTierData->GetLootPackageCategoryWeightArray()[i];
        }
    }

    while (SumWeights > 0) {
        AmountOfLootDrops++;

        if (AmountOfLootDrops >= LootTierData->GetNumLootPackageDrops()) {
            break;
        }

        SumWeights--;
    }

    if (!AmountOfLootDrops) {
        AmountOfLootDrops = 1;
    }
        
    TArray<FFortItemEntry*> LootDrops;

    auto& CategoryMinArray = LootTierData->GetLootPackageCategoryMinArray();
    
    for (int i = 0; i < AmountOfLootDrops && i < CategoryMinArray.Num(); i++) {
        for (int j = 0; j < CategoryMinArray[i] && CategoryMinArray[i] >= 1; j++) {
            SetupLootDropsForPackage(LootDrops, LootTierData->GetLootPackage(), i, TierGroup, WorldLevel, 0);
        }
    }

    return LootDrops;
}

void InternalSpawnLoot(FVector Loc, const TArray<FFortItemEntry*>& PrecomputedLoot) {
    int32 SpawnedCount = 0;
    
    for (FFortItemEntry* Item : PrecomputedLoot)
    {
        if (!IsValidPointer(Item->GetItemDefinition())) {
            UE_LOG(LogKismet, Log, TEXT("InternalSpawnLoot: ItemDefinition is not valid!"));
            continue;
        }

        FVector SpawnLoc = Loc;
        SpawnLoc.X += (rand() % 130 - 65);
        SpawnLoc.Y += (rand() % 130 - 65);

        static AFortInventory* DefaultObj = Cast<AFortInventory>(AFortInventory::StaticClass()->GetClassDefaultObject());
        
        DefaultObj->SpawnPickupStatic(
            SpawnLoc, 
            Item, 
            EFortPickupSourceTypeFlag::Container, 
            EFortPickupSpawnSource::Unset, 
            nullptr, 
-1, false, true, true);
        
        SpawnedCount++;
    }
}

AFortPickup* UFortKismetLibrary::K2_SpawnPickupInWorld(FFrame& Stack, AFortPickup** Ret)
{
    UObject* WorldContextObject;                                       // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	UFortWorldItemDefinition* ItemDefinition;                                           // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int                                                NumberToSpawn;                                            // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector                                     Position;                                                 // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector                                     Direction;                                                // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int                                                OverrideMaxStackCount;                                    // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                               bToss;                                                    // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                               bRandomRotation;                                          // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                               bBlockedFromAutoPickup;                                   // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int                                                PickupInstigatorHandle;                                   // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                          SourceType;                                               // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                             Source;                                                   // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	AFortPlayerController* OptionalOwnerPC;                                          // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                               bPickupOnlyRelevantToOwner;                               // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)

	Stack.StepCompiledIn(&WorldContextObject);
	Stack.StepCompiledIn(&ItemDefinition);
	Stack.StepCompiledIn(&NumberToSpawn);
	Stack.StepCompiledIn(&Position);
	Stack.StepCompiledIn(&Direction);
	Stack.StepCompiledIn(&OverrideMaxStackCount);
	Stack.StepCompiledIn(&bToss);
	Stack.StepCompiledIn(&bRandomRotation);
	Stack.StepCompiledIn(&bBlockedFromAutoPickup);
	Stack.StepCompiledIn(&PickupInstigatorHandle);
	Stack.StepCompiledIn(&SourceType);
	Stack.StepCompiledIn(&Source);
	Stack.StepCompiledIn(&OptionalOwnerPC);
	Stack.StepCompiledIn(&bPickupOnlyRelevantToOwner);
    
    auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());
    if (!GameState) return nullptr;

    auto NewPickup = AFortInventory::SpawnPickupStatic(
        Position, 
        MakeItemEntry(ItemDefinition, NumberToSpawn, 1), 
        (EFortPickupSourceTypeFlag)SourceType, 
        (EFortPickupSpawnSource)Source, 
        nullptr, 
        OverrideMaxStackCount, 
        bToss, 
        bRandomRotation, 
        bBlockedFromAutoPickup);
    
    K2_SpawnPickupInWorldOG(this, Stack, Ret);
    *Ret = NewPickup;
    return *Ret;
}

class FScriptInterface
{
public:
    UObject* ObjectPointer = nullptr;
    void* InterfacePointer = nullptr;

    FORCEINLINE UObject*& GetObjectRef()
    {
        return ObjectPointer;
    }
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
};

void UFortKismetLibrary::GiveItemToInventoryOwner(FFrame& Stack)
{
    static UFunction* Func = PropLibrary->GetFunctionByName("FortKismetLibrary", "K2_GetResourceItemDefinition").Func;
    
    if (!Func)
        return GiveItemToInventoryOwnerOG(this, Stack);
    
    static auto ItemLevelOffset = UKismetMemLibrary::GetOffset(Func, "ItemLevel");
    static auto PickupInstigatorHandleOffset = UKismetMemLibrary::GetOffset(Func, "PickupInstigatorHandle");
    static auto ItemVariantGuidOffset = UKismetMemLibrary::GetOffset(Func, "ItemVariantGuid");

    TScriptInterface<UObject> InventoryOwner; 
    UFortWorldItemDefinition* ItemDefinition = nullptr; 
    FGuid ItemVariantGuid;
    int NumberToGive; 
    bool bNotifyPlayer; 
    int ItemLevel;
    int PickupInstigatorHandle; 

    Stack.StepCompiledIn(&InventoryOwner);
    Stack.StepCompiledIn(&ItemDefinition);
    if (ItemVariantGuidOffset != -1) Stack.StepCompiledIn(&ItemVariantGuid);
    Stack.StepCompiledIn(&NumberToGive);
    Stack.StepCompiledIn(&bNotifyPlayer);
    
    if (ItemLevelOffset != -1)
        Stack.StepCompiledIn(&ItemLevel);

    if (PickupInstigatorHandleOffset != -1)
        Stack.StepCompiledIn(&PickupInstigatorHandle);
    
    auto InterfacePointer = InventoryOwner.InterfacePointer;
    auto ObjectPointer = InventoryOwner.ObjectPointer;
    if (!InterfacePointer || !ItemDefinition || !ObjectPointer)
        return GiveItemToInventoryOwnerOG(this, Stack);

    UE_LOG(LogTemp, Log, "GiveItemToInventoryOwner called: %s", ItemDefinition->GetFName().ToString().ToString().c_str());

    auto PlayerController = Cast<AFortPlayerController>(ObjectPointer);

    if (!PlayerController)
        return GiveItemToInventoryOwnerOG(this, Stack);

    int32 LoadedAmmo = 1;
    if (ItemDefinition->IsA<UFortWeaponItemDefinition>())
    {
        auto WeaponStats = PlayerController->GetWorldInventory()->GetStats((UFortWeaponItemDefinition*)ItemDefinition);
        if (WeaponStats)
        {
            LoadedAmmo = WeaponStats->GetClipSize();
        }
    }

    PlayerController->GetWorldInventory()->GiveItem(ItemDefinition, NumberToGive, LoadedAmmo, 0, PlayerController->IsA(AFortPlayerControllerAthena::StaticClass()));
    return GiveItemToInventoryOwnerOG(this, Stack);
}

void UFortKismetLibrary::K2_GiveItemToPlayer(FFrame& Stack)
{
    AFortPlayerController* PlayerController = nullptr;
    UFortWorldItemDefinition* ItemDefinition = nullptr;
    int NumberToGive;
    bool bNotifyPlayer;

    Stack.StepCompiledIn(&PlayerController);
    Stack.StepCompiledIn(&ItemDefinition);
    Stack.StepCompiledIn(&NumberToGive);
    Stack.StepCompiledIn(&bNotifyPlayer);

    if (!PlayerController || !ItemDefinition)
        return K2_GiveItemToPlayerOG(this, Stack);

    PlayerController->GetWorldInventory()->GiveItem(ItemDefinition, NumberToGive, -1, 0, PlayerController->IsA(AFortPlayerControllerAthena::StaticClass()));

    return K2_GiveItemToPlayerOG(this, Stack);
}

void UFortKismetLibrary::SpawnBGAConsumables() {
    auto Containers = UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABGAConsumableSpawner::StaticClass());

    int32 TotalItemsSpawned = 0;

    for (auto& BuildingContainer : Containers) {
        auto Container = Cast<ABGAConsumableSpawner>(BuildingContainer);
        if (!Container) {
            UE_LOG(LogKismet, Log, TEXT("SpawnFloorLootForContainer: Actor is not a BuildingContainer!"));
            continue;
        }
        
        if (Fortnite_Version.Season() <= 10)
        {
            FVector SpawnLocation = Container->K2_GetActorLocation();
        
            FName TierGroup = Container->GetSpawnLootTierGroup();
            TArray<FFortItemEntry*> LootItems;

            if (TierGroup.ToString() == TEXT("Loot_ForagedItem_GravityRocks"))
            {
                auto Class = StaticFindObject<UClass>("/Script/FortniteGame.BGAConsumableWrapperItemDefinition");
                auto LowGravityItemDefinition = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/ForagedItems/LowGravity/Athena_Foraged_LowGravity.Athena_Foraged_LowGravity", Class);

                if (!LowGravityItemDefinition)
                {
                    UE_LOG(LogKismet, Error, TEXT("Failed to load Athena_Foraged_LowGravity!"));
                    continue;
                }

                auto Entry = MakeItemEntry(LowGravityItemDefinition, 1, 0);
                LootItems.Add(Entry);
            }
            else
            {
                LootItems = PickLootDrops(TierGroup, -1, -1);
            }


            for (auto Entry : LootItems)
            {
                auto ConsumableItemDef = Cast<UBGAConsumableWrapperItemDefinition>(Entry->GetItemDefinition());

                if (!ConsumableItemDef) continue;

                auto StrongConsumableClass = ConsumableItemDef->GetConsumableClass().Get(UBlueprintGeneratedClass::StaticClass(), true);

                if (!StrongConsumableClass) continue;

                FVector Start = SpawnLocation + FVector(0.f, 0.f, 500.f);
                FVector End   = SpawnLocation - FVector(0.f, 0.f, 5000.f);

                TArray<AActor*> IgnoreActors;

                ETraceTypeQuery TraceType = ETraceTypeQuery::TraceTypeQuery1;
                FHitResult HitResult;

                bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceType, false, IgnoreActors, EDrawDebugTrace::None, &HitResult, true, FLinearColor::Red(), FLinearColor::Green(), 0.f);
                
                if (bHit)
                {
                    FVector GroundLocation = HitResult.ImpactPoint;

                    UE_LOG(LogTemp, Log, "HIT! Spawning");
                    
                    GetWorld()->SpawnActor(StrongConsumableClass, GroundLocation);
                }
            }
            
            BuildingContainer->K2_DestroyActor();
        } 
    }
    
    Containers.Free();
}

void UFortKismetLibrary::SpawnFloorLootForContainer(UBlueprintGeneratedClass* ContainerType) {
    auto Containers = UGameplayStatics::GetAllActorsOfClass(GetWorld(), ContainerType);

    int32 TotalItemsSpawned = 0;

    for (auto& BuildingContainer : Containers) {
        auto Container = Cast<ABuildingContainer>(BuildingContainer);
        if (!Container) {
            UE_LOG(LogKismet, Log, TEXT("SpawnFloorLootForContainer: Actor is not a BuildingContainer!"));
            continue;
        }
        
        if (Fortnite_Version.Season() <= 10)
        {
            FVector SpawnLocation = Container->K2_GetActorLocation() + 
                                   Container->GetActorForwardVector() * Container->GetLootSpawnLocation_Athena().X + 
                                   Container->GetActorRightVector() * Container->GetLootSpawnLocation_Athena().Y + 
                                   Container->GetActorUpVector() * Container->GetLootSpawnLocation_Athena().Z;
        
            FName TierGroup = Container->GetSearchLootTierGroup();
            TArray<FFortItemEntry*> LootItems = PickLootDrops(TierGroup, -1, -1);
            int32 ItemsForThisContainer = LootItems.Num();
            TotalItemsSpawned += ItemsForThisContainer;
        
            InternalSpawnLoot(SpawnLocation, LootItems);
            BuildingContainer->K2_DestroyActor();
        } else
        {
            Container->BP_SpawnLoot(nullptr);
            Container->K2_DestroyActor();
        }
    }
    
    Containers.Free();
}

bool UFortKismetLibrary::PickLootDropsHook(FFrame& Stack)
{
    static auto WorldContextObjectOffset = UKismetMemLibrary::GetOffsetStruct("PickLootDrops", "WorldContextObject");
    
    UObject* WorldContextObject;
    TArray<FFortItemEntry> OutLootToDropTemp;
    FName TierGroupName;
    int WorldLevel;
    int ForcedLootTier;

    if (WorldContextObjectOffset != -1)
        Stack.StepCompiledIn(&WorldContextObject);

    auto& OutLootToDrop = Stack.StepCompiledInRef<TArray<FFortItemEntry>>(&OutLootToDropTemp);
    Stack.StepCompiledIn(&TierGroupName);
    Stack.StepCompiledIn(&WorldLevel);
    Stack.StepCompiledIn(&ForcedLootTier);

    UE_LOG(LogTemp, Log, "PickLootDropsHook: %s", TierGroupName.ToString().ToString().c_str());

    auto LootDrops = UFortKismetLibrary::PickLootDrops(TierGroupName, ForcedLootTier, WorldLevel);

    for (auto& LootDrop : LootDrops)
        OutLootToDrop.Add(*LootDrop);
    
    return true;
}
