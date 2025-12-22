#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/AI/NavigationSystemConfig.h"

class UNavigationSystemV1 : public UObject
{
public:
    DEFINE_BOOL(UNavigationSystemV1, bSpawnNavDataInNavBoundsLevel)
    DEFINE_MEMBER(FNavAgentSelector, UNavigationSystemV1, SupportedAgentsMask);
    DEFINE_BOOL(UNavigationSystemV1, bAutoCreateNavigationData);
    DEFINE_BOOL(UNavigationSystemV1, bAllowClientSideNavigation)
    DEFINE_BOOL(UNavigationSystemV1, bSupportRebuilding);
public:
    DefineUOriginal(void, InitializeForWorld, UNavigationSystemV1*, UWorld*, uint8);
public:
    DECLARE_DEFAULT_OBJECT(UNavigationSystemV1)
    DECLARE_STATIC_CLASS(UNavigationSystemV1)
};
