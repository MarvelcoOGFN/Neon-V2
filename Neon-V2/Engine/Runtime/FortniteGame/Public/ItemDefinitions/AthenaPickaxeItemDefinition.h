#pragma once
#include "pch.h"

#include "WeaponMeleeItemDefinition.h"

class UAthenaPickaxeItemDefinition : public UObject
{
public:
    DEFINE_PTR(UFortWeaponMeleeItemDefinition, UAthenaPickaxeItemDefinition, WeaponDefinition);
public:
    DECLARE_STATIC_CLASS(UAthenaPickaxeItemDefinition)
    DECLARE_DEFAULT_OBJECT(UAthenaPickaxeItemDefinition)
};
