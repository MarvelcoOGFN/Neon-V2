#pragma once
#include "pch.h"

#include "WorldItemDefinition.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"

class UFortWeaponItemDefinition : public UFortWorldItemDefinition
{
public:
    DEFINE_BOOL(UFortWeaponItemDefinition, bUsesPhantomReserveAmmo)
    DEFINE_MEMBER(FDataTableRowHandle, UFortWeaponItemDefinition, WeaponStatHandle);
    DEFINE_MEMBER(TSoftObjectPtr<UFortWorldItemDefinition>, UFortWeaponItemDefinition, AmmoData);
public:
    DECLARE_STATIC_CLASS(UFortWeaponItemDefinition)
    DECLARE_DEFAULT_OBJECT(UFortWeaponItemDefinition)
};
