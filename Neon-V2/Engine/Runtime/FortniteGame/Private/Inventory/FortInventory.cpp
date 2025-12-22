#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"

#include "Engine/Plugins/Kismet/Public/KismetMathLibrary.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"
#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponMeleeItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponRangedItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerPawn.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

void AFortInventory::HandleInventoryLocalUpdate()
{
    static UFunction* Func = nullptr;
    if (!Func) Func = PropLibrary->GetFunctionByName("FortInventory", "HandleInventoryLocalUpdate").Func;
    if (!Func) return;
    
    ProcessEvent(Func, nullptr);
}

void AFortInventory::Update(FFortItemEntry* Entry)
{
    SetbRequiresLocalUpdate(true);
    HandleInventoryLocalUpdate();
        
    if (auto OwningPlayer = Cast<AFortPlayerControllerAthena>(GetOwner()))
    {
        OwningPlayer->HandleWorldInventoryLocalUpdate();
        OwningPlayer->ClientForceUpdateQuickbar(EFortQuickBars::Primary);
        OwningPlayer->ClientForceUpdateQuickbar(EFortQuickBars::Secondary);
    }

    Entry ? GetInventory().MarkItemDirty(Entry) : GetInventory().MarkArrayDirty();
}

UObject* AFortInventory::GiveItem(UFortItemDefinition* Def, int32 Count, int LoadedAmmo, int32 Level, int Slot, bool bAthena)
{
    if ( !Def || Count == 0 ) return nullptr;

    UFortWorldItem* BP = Def->CreateTemporaryItemInstanceBP(Count, Level);
    if ( !BP ) return nullptr;
    
    auto& ItemEntry = BP->GetItemEntry();
    ItemEntry.SetLoadedAmmo(LoadedAmmo);
    
    if (auto Weapon = Cast<UFortWeaponItemDefinition>(Def))
    {
        auto Stats = GetStats(Weapon);

        if (LoadedAmmo == 0 && Stats)
            ItemEntry.SetLoadedAmmo(Stats->GetClipSize());

        if (Fortnite_Version.Season() >= 27) // ??
        {
            if (Weapon->GetbUsesPhantomReserveAmmo())
                ItemEntry.SetPhantomReserveAmmo(Stats->GetInitialClips() - Stats->GetClipSize());
        }
    }
    
    FFortItemList& Inventory = GetInventory();
    TArray<FFortItemEntry>& ReplicatedEntriesOffsetPtr = Inventory.GetReplicatedEntries();
    TArray<UFortWorldItem*>& ItemInstancesOffsetPtr = Inventory.GetItemInstances();
    
    static int StructSize = StaticClassImpl("FortItemEntry")->GetSize();
    ItemInstancesOffsetPtr.Add(BP);

    FFortItemEntry* ReplicatedEntryPtr = (FFortItemEntry*)((uint8*)ReplicatedEntriesOffsetPtr.GetData() + (ReplicatedEntriesOffsetPtr.Add(BP->GetItemEntry(), StructSize)) * StructSize);
    Inventory.MarkItemDirty(&ItemEntry);      
    Inventory.MarkItemDirty(ReplicatedEntryPtr);

    if (!bAthena)
    {
        auto ReturnQuickbarSlot = [](AFortPlayerController* Controller) -> int {
            if (!Controller || !Controller->GetQuickBars()) return 0;

            auto& Slots = Controller->GetQuickBars()->GetPrimaryQuickBar().Slots;
            if (Slots.Num() == 0) return -1;

            for (int i = 0; i < Slots.Num(); i++) {
                if (!Slots[i].Items.GetData()) {
                    return i;
                }
            }

            return -1;
        };

        int FinalSlot = Slot;

        if (Slot != 0)
        {
            FinalSlot = ReturnQuickbarSlot(Cast<AFortPlayerController>(GetOwner()));
        }

        auto QuickBar = GetQuickbar(ItemEntry.GetItemDefinition());
            
        if (QuickBar == EFortQuickBars::Secondary)
        {
            Cast<AFortPlayerController>(GetOwner())->GetQuickBars()->ServerAddItemInternal(ItemEntry.GetItemGuid(), QuickBar, Slot);
        }
        else
        {
            Cast<AFortPlayerController>(GetOwner())->GetQuickBars()->ServerAddItemInternal(ItemEntry.GetItemGuid(), QuickBar, FinalSlot);
        }
    }
    
    Update(&ItemEntry);
    
    return BP;
}

FFortItemEntry* AFortInventory::FindItemEntryDef(UObject* Object)
{
    FFortItemList& Inventory = GetInventory();
    TArray<UFortWorldItem*>& ItemInstancesOffsetPtr = Inventory.GetItemInstances();
    for (int32 i = 0; i < ItemInstancesOffsetPtr.Num(); ++i)
    {
        if (ItemInstancesOffsetPtr[i]->GetItemEntry().GetItemDefinition() == Object)
        {
            return &ItemInstancesOffsetPtr[i]->GetItemEntry();
        }
    }

    return nullptr;
}
UFortWorldItem* AFortInventory::FindItemInstance(FGuid Guid)
{
    FFortItemList& Inventory = GetInventory();
    TArray<UFortWorldItem*>& ItemInstancesOffsetPtr = Inventory.GetItemInstances();
    for (int32 i = 0; i < ItemInstancesOffsetPtr.Num(); ++i)
    {
        if (ItemInstancesOffsetPtr[i]->GetItemEntry().GetItemGuid() == Guid)
        {
            return ItemInstancesOffsetPtr[i];
        }
    }

    return nullptr;
}
FFortItemEntry* AFortInventory::FindReplicatedEntry(FGuid Guid)
{
    FFortItemList& Inventory = GetInventory();
    TArray<FFortItemEntry>& ItemInstancesOffsetPtr = Inventory.GetReplicatedEntries();
    for (int32 i = 0; i < ItemInstancesOffsetPtr.Num(); ++i)
    {
        if (ItemInstancesOffsetPtr[i].GetItemGuid() == Guid)
        {
            return &ItemInstancesOffsetPtr[i];
        }
    }

    return nullptr;
}

UFortItem* AFortInventory::GetPickaxeInstance()
{
    static auto FortWeaponMeleeItemDefinitionClass = StaticFindObject<UClass>("/Script/FortniteGame.FortWeaponMeleeItemDefinition");

    auto& ItemInstances = GetInventory().GetItemInstances();

    for (int i = 0; i < ItemInstances.Num(); ++i)
    {
        auto ItemInstance = ItemInstances[i];

        if (&ItemInstance->GetItemEntry() && ItemInstance->GetItemEntry().GetItemDefinition() &&
            ItemInstance->GetItemEntry().GetItemDefinition()->IsA(FortWeaponMeleeItemDefinitionClass)
            )
        {
            return ItemInstance;
        }
    }
    return nullptr;
}

void AFortInventory::ClearInventory()
{
    static auto FortEditToolItemDefinitionClass = StaticFindObject<UClass>("/Script/FortniteGame.FortEditToolItemDefinition");
    static auto FortBuildingItemDefinitionClass = StaticFindObject<UClass>("/Script/FortniteGame.FortBuildingItemDefinition");

    std::vector<std::pair<FGuid, int>> GuidsAndCountsToRemove;
    const auto& ItemInstances = this->GetInventory().GetItemInstances();
    auto PickaxeInstance = this->GetPickaxeInstance();

    for (int i = 0; i < ItemInstances.Num(); ++i)
    {
        auto ItemInstance = ItemInstances[i];
        const auto ItemDefinition = Cast<UFortWorldItemDefinition>(ItemInstance->GetItemEntry().GetItemDefinition());

        if (!ItemDefinition->IsA(FortBuildingItemDefinitionClass)
            && !ItemDefinition->IsA(FortEditToolItemDefinitionClass)
            && ItemInstance != PickaxeInstance)
        {
            GuidsAndCountsToRemove.push_back({
                ItemInstance->GetItemEntry().GetItemGuid(),
                ItemInstance->GetItemEntry().GetCount()
                });
        }
    }

    for (auto& [Guid, Count] : GuidsAndCountsToRemove)
    {
        this->Remove(Guid);
    }
}

AFortPickupAthena* AFortInventory::SpawnPickupStatic(FVector Loc, FFortItemEntry* Entry, EFortPickupSourceTypeFlag SourceTypeFlag, EFortPickupSpawnSource SpawnSource, AFortPlayerPawn* Pawn, int OverrideCount, bool Toss, bool RandomRotation, bool bCombine)
{
    if (!Entry) return nullptr;

    static auto PickupClass = AFortPickupAthena::StaticClass();
    AFortPickupAthena* NewPickup = GetWorld()->SpawnActor<AFortPickupAthena>(PickupClass, Loc);
    if (NewPickup != nullptr && Entry != nullptr && Entry->GetItemDefinition() != nullptr)
    {
        static auto Offset = UKismetMemLibrary::GetAddress<uintptr_t>(L"PickupInit");
        if (Offset != 0)
        {
            ((FVector* (*)(AFortPickup*, FFortItemEntry*, TArray<FFortItemEntry>, bool))(Offset))(NewPickup, Entry, TArray<FFortItemEntry>(), false);
        }
        
        NewPickup->SetbRandomRotation(RandomRotation);
        NewPickup->GetPrimaryPickupItemEntry().SetItemDefinition(Entry->GetItemDefinition());
        NewPickup->GetPrimaryPickupItemEntry().SetLoadedAmmo(Entry->GetLoadedAmmo());
        NewPickup->GetPrimaryPickupItemEntry().SetCount(OverrideCount != -1 ? OverrideCount : Entry->GetCount());
        NewPickup->SetPawnWhoDroppedPickup(Pawn);
        NewPickup->OnRep_PrimaryPickupItemEntry();

        NewPickup->TossPickup(Loc, Pawn, -1, Toss, true, SourceTypeFlag, SpawnSource);
        NewPickup->SetMovementComponent((UProjectileMovementComponent*)UGameplayStatics::SpawnObject(UProjectileMovementComponent::StaticClass(), NewPickup));
    }
    
    return NewPickup;
}

AFortPickupAthena* AFortInventory::SpawnPickup(FVector Loc, FFortItemEntry* Entry, EFortPickupSourceTypeFlag SourceTypeFlag, EFortPickupSpawnSource SpawnSource, AFortPlayerPawn* Pawn, int OverrideCount, bool Toss, bool RandomRotation, bool bCombine)
{
    if (!Entry) return nullptr;

    static auto PickupClass = AFortPickupAthena::StaticClass();
    
    Loc.Y += UKismetMathLibrary::RandomIntegerInRange(50, 150);
    Loc.X += UKismetMathLibrary::RandomIntegerInRange(50, 150);
    
    AFortPickupAthena* NewPickup = GetWorld()->SpawnActor<AFortPickupAthena>(PickupClass, Loc);
    if (NewPickup != nullptr && Entry != nullptr && Entry->GetItemDefinition() != nullptr)
    {
        NewPickup->SetbRandomRotation(RandomRotation);
        NewPickup->GetPrimaryPickupItemEntry().SetItemDefinition(Entry->GetItemDefinition());
        NewPickup->GetPrimaryPickupItemEntry().SetLoadedAmmo(Entry->GetLoadedAmmo());
        NewPickup->GetPrimaryPickupItemEntry().SetCount(OverrideCount != -1 ? OverrideCount : Entry->GetCount());
        NewPickup->SetPawnWhoDroppedPickup(Pawn);
        NewPickup->OnRep_PrimaryPickupItemEntry();
            
        if (!Pawn) NewPickup->TossPickup(Loc, Pawn, -1, Toss, true, SourceTypeFlag, SpawnSource);
    }
    
    return NewPickup;
}

void AFortInventory::ReplaceEntry(FFortItemEntry& Entry) {
    FFortItemList& Inventory = GetInventory();
    TArray<UFortWorldItem*>& ItemInstances = Inventory.GetItemInstances();
    TArray<FFortItemEntry>& ReplicatedEntries = Inventory.GetReplicatedEntries();
    FGuid TargetGuid = Entry.GetItemGuid();
    int32 ItemIndex = -1, ReplicatedIndex = -1;

    for (int32 i = 0; i < ItemInstances.Num(); ++i)
        if (ItemInstances[i] && ItemInstances[i]->GetItemEntry().GetItemGuid() == TargetGuid) { ItemIndex = i; break; }

    static int StructSize = StaticClassImpl("FortItemEntry")->GetSize();
    for (int32 i = 0; i < ReplicatedEntries.Num(); ++i)
        if (((FFortItemEntry*)((uint8*)ReplicatedEntries.GetData() + i * StructSize))->GetItemGuid() == TargetGuid) { ReplicatedIndex = i; break; }

    if (ItemIndex != -1 && ReplicatedIndex != -1 && Entry.GetItemDefinition()) {
        UFortWorldItem* ExistingItem = ItemInstances[ItemIndex];
        if (ExistingItem) {
            FFortItemEntry& ExistingEntry = ExistingItem->GetItemEntry();
            ExistingEntry.SetCount(Entry.GetCount());
            ExistingEntry.SetLoadedAmmo(Entry.GetLoadedAmmo());
            ExistingEntry.SetLevel(Entry.GetLevel());
            ExistingEntry.SetDurability(Entry.GetDurability());
            ExistingEntry.SetItemDefinition(Entry.GetItemDefinition());
            auto RepEntry = (FFortItemEntry*)((uint8*)ReplicatedEntries.GetData() + ReplicatedIndex * StructSize);
            memcpy(RepEntry, &ExistingEntry, StructSize);
            Inventory.MarkItemDirty(&ExistingEntry);
            Inventory.MarkItemDirty(RepEntry);
            
            Update(&ExistingEntry);
        }
    } else if (Entry.GetItemDefinition()) {
        if (ReplicatedIndex != -1) ReplicatedEntries.Remove(ReplicatedIndex);
        if (ItemIndex != -1) ItemInstances.Remove(ItemIndex);
        Update(nullptr);
        GiveItem(Entry.GetItemDefinition(), Entry.GetCount(), Entry.GetLoadedAmmo(), Entry.GetLevel());
    }
}

void AFortInventory::Remove(FGuid Guid)
{
    FFortItemList& Inventory = GetInventory();
    TArray<UFortWorldItem*>& ItemInstances = Inventory.GetItemInstances();
    TArray<FFortItemEntry>& ReplicatedEntries = Inventory.GetReplicatedEntries();
    bool bItemRemoved = false;

    for (int32 i = ItemInstances.Num() - 1; i >= 0; --i) {
        UFortWorldItem* Item = ItemInstances[i];
        if (Item && Item->GetItemEntry().GetItemGuid() == Guid) {
            UFortWorldItem** Data = ItemInstances.GetData();
            for (int32 j = i; j < ItemInstances.Num() - 1; ++j)
                Data[j] = Data[j + 1];
            *((int32*)((uint8*)&ItemInstances + 8)) -= 1;
            bItemRemoved = true;
            break;
        }
    }

    static int StructSize = StaticClassImpl("FortItemEntry")->GetSize();
    for (int32 i = ReplicatedEntries.Num() - 1; i >= 0; --i) {
        auto Entry = (FFortItemEntry*)((uint8*)ReplicatedEntries.GetData() + i * StructSize);
        if (Entry->GetItemGuid() == Guid) {
            uint8* Cur = (uint8*)Entry;
            uint8* Next = Cur + StructSize;
            int32 MoveCount = ReplicatedEntries.Num() - 1 - i;
            if (MoveCount > 0) memmove(Cur, Next, MoveCount * StructSize);
            *((int32*)((uint8*)&ReplicatedEntries + 8)) -= 1;
            bItemRemoved = true;
            break;
        }
    }

    if (bItemRemoved) {
        (ItemInstances.Num() > 0) ? ItemInstances.ResizeGrow(sizeof(UFortWorldItem*)) : ItemInstances.Free();
        (ReplicatedEntries.Num() > 0) ? ReplicatedEntries.ResizeGrow(StructSize) : ReplicatedEntries.Free();
        Update(nullptr);
    }
}

AFortPickupAthena* AFortInventory::SpawnPickup(FVector Loc, UFortItemDefinition* ItemDefinition, int Count, int LoadedAmmo, EFortPickupSourceTypeFlag SourceTypeFlag, EFortPickupSpawnSource SpawnSource, AFortPlayerPawn* Pawn, bool Toss)
{
    FFortItemEntry* ItemEntry = MakeItemEntry(ItemDefinition, Count, 0);
    return SpawnPickup(Loc, ItemEntry, SourceTypeFlag, SpawnSource, Pawn, -1, Toss, true, true);
}

FFortRangedWeaponStats* AFortInventory::GetStatsStatic(UFortWeaponItemDefinition* Def)
{
    if (!Def || !Def->GetWeaponStatHandle().DataTable)
        return nullptr;

    if (!Def->IsA<UFortWeaponRangedItemDefinition>()) 
        return nullptr;

    auto& WeaponStatHandle = Def->GetWeaponStatHandle();
    auto Table = WeaponStatHandle.DataTable;
    auto& RowMap = Table->GetRowMap();
    
    void* Row = nullptr;
    for (auto& Pair : RowMap)
    {
        if (WeaponStatHandle.RowName.ToString().ToString() == Pair.Key.ToString().ToString())
        {
            Row = Pair.Value;
            break;
        }
    }
    
    return Row ? (FFortRangedWeaponStats*)Row : nullptr;
}

FFortRangedWeaponStats* AFortInventory::GetStats(UFortWeaponItemDefinition* Def)
{
    if (!Def || !Def->GetWeaponStatHandle().DataTable)
        return nullptr;

    if (!Def->IsA<UFortWeaponRangedItemDefinition>()) 
        return nullptr;

    auto& WeaponStatHandle = Def->GetWeaponStatHandle();
    auto Table = WeaponStatHandle.DataTable;
    auto& RowMap = Table->GetRowMap();
    
    void* Row = nullptr;
    for (auto& Pair : RowMap)
    {
        if (WeaponStatHandle.RowName.ToString().ToString() == Pair.Key.ToString().ToString())
        {
            Row = Pair.Value;
            break;
        }
    }
    
    return Row ? (FFortRangedWeaponStats*)Row : nullptr;
}

FFortItemEntry* AFortInventory::MakeItemEntry(UFortItemDefinition* ItemDefinition, int32 Count, int32 Level) {
    if (!ItemDefinition)
    {
        UE_LOG(LogTemp, Warning, "MakeItemEntry: Invalid ItemDefinition");
        return nullptr;
    }
    
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
        int32 ClipSize = GetStats(WeaponDef) ? GetStats(WeaponDef)->GetClipSize() : 0;
        IE->SetLoadedAmmo(ClipSize);
    } else {
        IE->SetLoadedAmmo(0);
    }
    
    IE->SetDurability(1.f);
    IE->SetGameplayAbilitySpecHandle(FGameplayAbilitySpecHandle(-1));
    IE->SetLevel(Level);

    return IE;
}