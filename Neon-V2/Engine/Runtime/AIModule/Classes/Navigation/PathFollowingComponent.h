#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"

class ANavigationData : public AActor
{
public:
    DECLARE_STATIC_CLASS(ANavigationData);
    DECLARE_DEFAULT_OBJECT(ANavigationData);
};

class UPathFollowingComponent : public UObject
{
public:
    DEFINE_PTR(ANavigationData, UPathFollowingComponent, MyNavData)
public:
    DECLARE_STATIC_CLASS(UPathFollowingComponent)
    DECLARE_DEFAULT_OBJECT(UPathFollowingComponent)
};