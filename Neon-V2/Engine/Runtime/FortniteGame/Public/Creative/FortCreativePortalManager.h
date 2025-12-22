#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortAthenaCreativePortal.h"


class AFortCreativePortalManager final : public AActor
{
public:
    AFortAthenaCreativePortal* ClaimPortal();
    


public:
    DEFINE_MEMBER(TArray<class AFortAthenaCreativePortal*>, AFortCreativePortalManager, UsedPortals);
    DEFINE_MEMBER(TArray<class AFortAthenaCreativePortal*>, AFortCreativePortalManager, AvailablePortals);
    DEFINE_MEMBER(TArray<class AFortAthenaCreativePortal*>, AFortCreativePortalManager, AllPortals);

public:
    DECLARE_STATIC_CLASS(AFortCreativePortalManager)
    DECLARE_DEFAULT_OBJECT(AFortCreativePortalManager)
};
