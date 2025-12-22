#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerState.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"
#include <Engine/Runtime/FortniteGame/Public/Components/FortLevelSaveComponent.h>


bool AFortPlayerStateAthena::Server_SetCanEditCreativeIsland(bool bCanEdit, TArray<class FString> WhiteList)
{
	Server_SetCanEditCreativeIslandOG(this, &bCanEdit, WhiteList);

	auto PC = Cast<AFortPlayerControllerAthena>(this->GetOwner());
	if (!PC->IsValidLowLevelFast())
		return 1;

	if (!PC->GetCurrentVolume()->IsValidLowLevelFast())
		return 1;
	
	auto LevelSaveComponent = (UFortLevelSaveComponent*)PC->GetCurrentVolume()->GetComponentByClass(UFortLevelSaveComponent::StaticClass());

	LevelSaveComponent->GetPlotPermissions().Permission = bCanEdit ? EFortCreativePlotPermission::Public : EFortCreativePlotPermission::Private;
	static auto FN1 = StaticFindObject<UFunction>("/Script/FortniteGame.FortLevelSaveComponent.OnRep_PlotPermissionsChanged");
	LevelSaveComponent->ProcessEvent(FN1, nullptr);


	return 1;
}