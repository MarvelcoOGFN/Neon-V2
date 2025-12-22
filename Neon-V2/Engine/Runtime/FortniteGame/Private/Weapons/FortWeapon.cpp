#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/AI/FortAthenaAIBotController.h"
#include "Engine/Runtime/FortniteGame/Public/Weapons/FortWeapon.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

void AFortWeapon::UseSpareAmmo(int AmountToUse) 
{
    AActor* Owner = Get<AActor*>("Actor", "Owner");
    AController* C = Owner ? Owner->Get<AController*>("Pawn", "Controller") : nullptr;
    AFortPlayerControllerAthena* Controller = Cast<AFortPlayerControllerAthena>(C);
    if (!Controller)
        return UseSpareAmmoOG(this, AmountToUse);
 
    AFortInventory* Inventory = nullptr;
    if (!Inventory)
        Inventory = Controller->GetWorldInventory();

    if (!Inventory)
        return UseSpareAmmoOG(this, AmountToUse);

    UFortWeaponItemDefinition* WeaponData = GetWeaponData();
    UFortWorldItemDefinition* AmmoDefinition = WeaponData ? WeaponData->GetAmmoData().Get() : nullptr;

    FFortItemList& InventoryList = Inventory->GetInventory();
    TArray<UFortWorldItem*>& ItemInstances = InventoryList.GetItemInstances();

    FGuid WeaponGuid = GetItemEntryGuid();

    FFortItemEntry* WeaponEntry = nullptr;
    for (UFortWorldItem* Item : ItemInstances)
    {
        if (Item && Item->GetItemEntry().GetItemGuid() == WeaponGuid)
        {
            WeaponEntry = &Item->GetItemEntry();
            break;
        }
    }

    if (!WeaponEntry)
        return UseSpareAmmoOG(this, AmountToUse);

    FFortItemEntry* AmmoEntry = nullptr;
    for (UFortWorldItem* Item : ItemInstances)
    {
        if (Item && Item->GetItemEntry().GetItemDefinition() == AmmoDefinition)
        {
            AmmoEntry = &Item->GetItemEntry();
            break;
        }
    }

    if (!AmmoEntry)
        return UseSpareAmmoOG(this, AmountToUse);

    int32 NewAmmoCount = AmmoEntry->GetCount() - AmountToUse;
    AmmoEntry->SetCount(NewAmmoCount);

    if (NewAmmoCount <= 0)
        Inventory->Remove(AmmoEntry->GetItemGuid());
    else
        Inventory->ReplaceEntry(*AmmoEntry);

    WeaponEntry->SetLoadedAmmo(WeaponEntry->GetLoadedAmmo() + AmountToUse);
    Inventory->ReplaceEntry(*WeaponEntry);

    return UseSpareAmmoOG(this, AmountToUse);
}
