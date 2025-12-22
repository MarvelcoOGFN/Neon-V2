#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPawn.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

void AFortPawn::MovingEmoteStopped(FFrame& Stack)
{
    SetbMovingEmote(false);
    if (Fortnite_Version >= 8.00) SetbMovingEmoteForwardOnly(false);
    return MovingEmoteStoppedOG(this, Stack);
}

void AFortPawn::NetMulticast_Athena_BatchedDamageCues(FFrame& Stack)
{
    AFortPlayerControllerAthena* PlayerController = Cast<AFortPlayerControllerAthena>(GetController());

    if (!PlayerController) return;
    if (!GetCurrentWeapon()) return;

    UFortWorldItem* CurrentWeaponInstance = PlayerController->GetWorldInventory()->FindItemInstance(GetCurrentWeapon()->GetItemEntryGuid());
    if (!CurrentWeaponInstance) return;

    FFortItemEntry& CurrentWeaponEntry = CurrentWeaponInstance->GetItemEntry();

    if (CurrentWeaponEntry.GetLoadedAmmo() != GetCurrentWeapon()->GetAmmoCount())
    {
        CurrentWeaponEntry.SetLoadedAmmo(GetCurrentWeapon()->GetAmmoCount());
        PlayerController->GetWorldInventory()->ReplaceEntry(CurrentWeaponEntry);
    }

    NetMulticast_Athena_BatchedDamageCuesOG(this, Stack);
}
