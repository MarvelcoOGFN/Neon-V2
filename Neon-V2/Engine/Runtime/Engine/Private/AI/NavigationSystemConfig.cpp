#include "pch.h"
#include "Engine/Runtime/Engine/Classes/AI/NavigationSystemConfig.h"

void UAthenaNavSystemConfig::CreateAndConfigureNavigationSystem(UWorld* World)
{
    UE_LOG(LogTemp, Log, __FUNCTION__);
    bPrioritizeNavigationAroundSpawners = true;
    bAutoSpawnMissingNavData = true;
    bAllowAutoRebuild = true;
    bSupportRuntimeNavmeshDisabling = false; // main fixes for nav
    bUsesStreamedInNavLevel = true;
    
    return CreateAndConfigureNavigationSystemOG(this, World);
}
