#pragma once
#include "pch.h"
#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"


class UFortMutatorListComponent final : public UActorComponent
{
public:
    DEFINE_MEMBER(TArray<class AActor*> ,UFortMutatorListComponent, Mutators);
public:
    DECLARE_STATIC_CLASS(UFortMutatorListComponent);
    DECLARE_DEFAULT_OBJECT(UFortMutatorListComponent);
};