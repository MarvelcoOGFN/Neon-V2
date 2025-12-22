#pragma once
#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Mutators/FortAthenaMutator.h"


class AFortAthenaMutator_InfiniteResources : public AFortAthenaMutator // AFortGameplayMutator
{
public:
    DEFINE_BOOL(AFortAthenaMutator_InfiniteResources, bInfiniteResources)


public:
    DECLARE_STATIC_CLASS(AFortAthenaMutator_InfiniteResources);
    DECLARE_DEFAULT_OBJECT(AFortAthenaMutator_InfiniteResources);
};