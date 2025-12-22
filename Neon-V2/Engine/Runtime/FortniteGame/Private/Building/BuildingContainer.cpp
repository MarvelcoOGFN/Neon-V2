#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingContainer.h"

#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"
#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"


bool ABuildingContainer::SpawnLoot()
{
    auto World = GetWorld();
    if (!World) {
        return false;
    }
    
    auto GameState = World->GetGameState();
    auto GameMode = World->GetAuthorityGameMode();
    if (!GameState || !GameMode || GetbAlreadySearched())
    {
        return false;
    }

    FName LootTierGroupToUse = GetSearchLootTierGroup();

    if (Fortnite_Version >= 10.00)
    {
        for (auto& [SupportTierGroup, Redirect] : GameMode->GetRedirectAthenaLootTierGroups()) {
            if (LootTierGroupToUse == SupportTierGroup) {
                LootTierGroupToUse = Redirect;
                break;
            }
        }
    } else
    {
        static auto Loot_TreasureFName = UKismetStringLibrary::Conv_StringToName(L"Loot_Treasure");
        static auto Loot_AmmoFName = UKismetStringLibrary::Conv_StringToName(L"Loot_Ammo");

        if (LootTierGroupToUse == Loot_TreasureFName)
            LootTierGroupToUse = UKismetStringLibrary::Conv_StringToName(L"Loot_AthenaTreasure");

        if (LootTierGroupToUse == Loot_AmmoFName)
            LootTierGroupToUse = UKismetStringLibrary::Conv_StringToName(L"Loot_AthenaAmmoLarge");
    }
    
    TArray<FFortItemEntry*> LootItems = UFortKismetLibrary::PickLootDrops(LootTierGroupToUse, -1, -1);
    int32 SpawnedCount = 0;
    
    for (FFortItemEntry* Item : LootItems) {
        if (!IsValidPointer(Item->GetItemDefinition())) {
            continue;
        }

        FVector Loc = K2_GetActorLocation() + GetActorRightVector() * 70.0f + FVector{0,0,  50};
        
        static AFortInventory* DefaultObj = Cast<AFortInventory>(AFortInventory::StaticClass()->GetClassDefaultObject());
        
        DefaultObj->SpawnPickupStatic(
            Loc, 
            Item, 
            EFortPickupSourceTypeFlag::Container, 
            EFortPickupSpawnSource::Unset, 
            nullptr, 
    -1, false, true, true);
        
        SpawnedCount++;
    }
    
    SetbAlreadySearched(true);
    CallFunc<void>("BuildingContainer", "OnRep_bAlreadySearched");
    Get<FFortSearchBounceData>("BuildingContainer", "SearchBounceData").SearchAnimationCount++;
    CallFunc<void>("BuildingContainer", "BounceContainer");
    
    return true;
}
