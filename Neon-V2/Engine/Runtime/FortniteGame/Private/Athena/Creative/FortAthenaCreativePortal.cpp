#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Creative/FortAthenaCreativePortal.h"

#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponRangedItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerPawn.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

void AFortAthenaCreativePortal::TeleportPlayerToLinkedVolume(FFrame& Stack)
{
    AFortPlayerPawn* PlayerPawn;
    bool bUseSpawnTags;
    Stack.StepCompiledIn(&PlayerPawn);
    Stack.StepCompiledIn(&bUseSpawnTags);
    Stack.IncrementCode();

    if (!PlayerPawn)
        return;

    auto Location = GetLinkedVolume()->K2_GetActorLocation();
    Location.Z = 15000.f;

    PlayerPawn->K2_TeleportTo(Location, FRotator());
    PlayerPawn->BeginSkydiving(false);
}
