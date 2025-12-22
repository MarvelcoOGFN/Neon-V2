#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"
#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortGameStateAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTag.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

void ABuildingSMActor::OnDamageServer(
    float Damage,
    FGameplayTagContainer DamageTags,
    FVector Momentum,
    FHitResult HitInfo,
    AActor* InstigatedBy,
    AActor* DamageCauser,
    FGameplayEffectContextHandle Context)
{
    if (!InstigatedBy)
        return OnDamageServerOG(this, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, Context);

    AFortPlayerController* Controller = Cast<AFortPlayerController>(InstigatedBy);
    static UCurveTable* ResourceCurveTable = nullptr;
    int ResourceAmount = 0;
    static const UClass* Weapon = AFortWeapon::StaticClass();
    static const UClass* MeleeWeapon = UFortWeaponMeleeItemDefinition::StaticClass();

    if (!DamageCauser || !DamageCauser->IsA(Weapon) || !static_cast<AFortWeapon*>(DamageCauser)->GetWeaponData()->IsA(MeleeWeapon))
        return OnDamageServerOG(this, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, Context);

    auto ResourceDefinition = UFortKismetLibrary::K2_GetResourceItemDefinition(GetResourceType());
    static FName PickaxeTag = UKismetStringLibrary::Conv_StringToName(L"Weapon.Melee.Impact.Pickaxe");
    FGameplayTag* DamageTagEntry = nullptr;
    for (FGameplayTag& Entry : DamageTags.GameplayTags)
        if (Entry.TagName == PickaxeTag) { DamageTagEntry = &Entry; break; }

    if (!DamageTagEntry || !ResourceDefinition)
        return OnDamageServerOG(this, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, Context);

    int MaxStackSize = ResourceDefinition->GetMaxStackSize();
    if (GetBuildingResourceAmountOverride().RowName.GetComparisonIndex() > 0) {
        if (!ResourceCurveTable)
            ResourceCurveTable = GetBuildingResourceAmountOverride().CurveTable;
        if (!ResourceCurveTable)
            ResourceCurveTable = StaticFindObject<UCurveTable>("/Game/Athena/Balance/DataTables/AthenaResourceRates.AthenaResourceRates");
        float Out = UDataTableFunctionLibrary::EvaluateCurveTableRow(ResourceCurveTable, GetBuildingResourceAmountOverride().RowName, 0.0f);
        ResourceAmount = round(Out / (GetMaxHealth() / Damage));
    }

    if (ResourceAmount <= 0)
        return OnDamageServerOG(this, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, Context);

    const FVector PlayerLocation = Controller->GetPawn()->K2_GetActorLocation();
    AFortInventory* Inventory = Controller->GetWorldInventory();
    if (!Inventory)
        return OnDamageServerOG(this, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, Context);

    FFortItemList& IInventory = Inventory->GetInventory();
    TArray<UFortWorldItem*>& ItemInstances = IInventory.GetItemInstances();
    UFortWorldItem* WorldItem = nullptr;
    FFortItemEntry* InventoryEntry = nullptr;
    for (UFortWorldItem* Item : ItemInstances)
        if (Item && Item->GetItemEntry().GetItemDefinition() == ResourceDefinition) { WorldItem = Item; InventoryEntry = &Item->GetItemEntry(); break; }

    if (InventoryEntry) {
        int32 NewCount = InventoryEntry->GetCount() + ResourceAmount;
        InventoryEntry->SetCount(NewCount);
        if (NewCount > MaxStackSize) {
            int32 OverflowAmount = NewCount - MaxStackSize;
            InventoryEntry->SetCount(MaxStackSize);
            Inventory->SpawnPickup(PlayerLocation, ResourceDefinition, OverflowAmount, 0, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, Controller->GetMyFortPawn());
        }
        WorldItem->SetItemEntry(*InventoryEntry);
        Inventory->ReplaceEntry(*InventoryEntry);
        if (InventoryEntry->GetCount() <= 0) Inventory->Remove(InventoryEntry->GetItemGuid());
    } else if (ResourceAmount > 0) {
        if (ResourceAmount > MaxStackSize) {
            Inventory->SpawnPickup(PlayerLocation, ResourceDefinition, ResourceAmount - MaxStackSize, 0, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, Controller->GetMyFortPawn());
            ResourceAmount = MaxStackSize;
        }
        Inventory->GiveItem(ResourceDefinition, ResourceAmount, 0, 0);
    }
    
    Controller->ClientReportDamagedResourceBuilding(this, GetResourceType(), ResourceAmount, false, Damage == 100.f);
    return OnDamageServerOG(this, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, Context);
}