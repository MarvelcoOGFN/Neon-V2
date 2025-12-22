#pragma once
#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerState.h"

class UAthenaCharacterItemDefinition : public UObject 
{
public:
    DEFINE_PTR(UFortHeroType, UAthenaCharacterItemDefinition, HeroDefinition);
public:
    DECLARE_STATIC_CLASS(UAthenaCharacterItemDefinition)
    DECLARE_DEFAULT_OBJECT(UAthenaCharacterItemDefinition)
};
