#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/Engine/Core/Public/Misc/Guid.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponMeleeItemDefinition.h"

class AFortWeapon : public AActor
{
public:
    DefineUOriginal(void, UseSpareAmmo, AFortWeapon*, int AmountToUse)
public:
    DEFINE_MEMBER(FGuid, AFortWeapon, ItemEntryGuid)
    DEFINE_PTR(UFortWeaponMeleeItemDefinition, AFortWeapon, WeaponData)
    DEFINE_MEMBER(int32, AFortWeapon, AmmoCount)
public:
    DECLARE_STATIC_CLASS(AFortWeapon)
    DECLARE_DEFAULT_OBJECT(AFortWeapon)
};
