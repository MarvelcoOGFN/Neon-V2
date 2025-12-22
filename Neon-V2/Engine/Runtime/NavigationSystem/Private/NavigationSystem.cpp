#include "pch.h"
#include "Engine/Runtime/NavigationSystem/Public/NavigationSystem.h"

void UNavigationSystemV1::InitializeForWorld(UWorld* World, uint8 Mode)
{
    SetbAutoCreateNavigationData(true);
    GetSupportedAgentsMask().bSupportsAgent3 = 1;
    SetbSpawnNavDataInNavBoundsLevel(true);
 
    return InitializeForWorldOG(this, World, Mode);
}
