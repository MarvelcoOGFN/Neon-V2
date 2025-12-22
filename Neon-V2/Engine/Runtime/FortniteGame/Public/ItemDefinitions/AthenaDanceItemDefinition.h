#pragma once
#include "pch.h"

#include "FortMontageItemDefinitionBase.h"

class UAthenaDanceItemDefinition : public UFortMontageItemDefinitionBase
{
public:
    DEFINE_BOOL(UAthenaDanceItemDefinition, bMovingEmote);
    DEFINE_BOOL(UAthenaDanceItemDefinition, bMoveForwardOnly);
    DEFINE_BOOL(UAthenaDanceItemDefinition, bMoveFollowingOnly);
    DEFINE_MEMBER(float, UAthenaDanceItemDefinition, WalkForwardSpeed);
    DEFINE_PTR(UClass, UAthenaDanceItemDefinition, CustomDanceAbility);
public:
    DECLARE_STATIC_CLASS(UAthenaDanceItemDefinition)
    DECLARE_DEFAULT_OBJECT(UAthenaDanceItemDefinition)
};
