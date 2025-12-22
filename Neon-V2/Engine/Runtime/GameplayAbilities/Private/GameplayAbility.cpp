#include "pch.h"
#include "Engine/Runtime/GameplayAbilities/Public/GameplayAbility.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/ItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerState.h"

__int64 UFortGameplayAbility::ApplyCost(void* a2, void* a3, void* a4)
{
    if (GetFName().ToString().ToString().starts_with("GA_Athena_AppleSun_Passive_C_"))
    {
        auto Def = StaticFindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/AppleSun/WID_Athena_AppleSun.WID_Athena_AppleSun");
        auto ASC = GetActivatingAbilityComponent();
        AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena *)ASC->GetOwner();
        AFortPlayerPawn* Pawn = PlayerState->CallFunc<AFortPlayerPawn*>("FortPlayerState", "GetCurrentPawn");
        AFortPlayerControllerAthena* Controller = (AFortPlayerControllerAthena *)Pawn->GetOwner();

        static int32 FortItemEntrySize = StaticClassImpl("FortItemEntry")->GetSize();
        auto Entry = Controller->GetWorldInventory()->GetInventory().GetReplicatedEntries().Search([Def](FFortItemEntry &Entry)
                                                                            { return Entry.GetItemDefinition() == Def; }, FortItemEntrySize);
        Entry->SetCount( Entry->GetCount() - 1 ) ;
        if (Entry->GetCount() <= 0)
        {
            Controller->GetWorldInventory()->Remove(Entry->GetItemGuid());
        }
        Controller->GetWorldInventory()->ReplaceEntry(*Entry);
    }
    return ApplyCostOG(this, a2, a3, a4);
}
