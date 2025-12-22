#pragma once
#include "pch.h"

#include "ItemDefinition.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthenaXPComponent.h"

class UFortAccoladeItemDefinition : public UFortItemDefinition
{
public:
    DEFINE_MEMBER(FScalableFloat, UFortAccoladeItemDefinition, XpRewardAmount);
    DEFINE_MEMBER(TArray<TSoftObjectPtr<class UFortAccoladeItemDefinition>>, UFortAccoladeItemDefinition, AccoladeToReplace);
    DEFINE_MEMBER(EXPEventPriorityType, UFortAccoladeItemDefinition, Priority);
    DECLARE_STATIC_CLASS(UFortAccoladeItemDefinition)
    DECLARE_DEFAULT_OBJECT(UFortAccoladeItemDefinition)
};
