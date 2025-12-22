#pragma once
#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Mutators/FortAthenaMutator.h"


class AFortAthenaMutator_HealthAndShield : public AFortAthenaMutator // AFortGameplayMutator
{
public:
    DEFINE_MEMBER(float, AFortAthenaMutator_HealthAndShield, MaxHealth)
    DEFINE_MEMBER(float, AFortAthenaMutator_HealthAndShield, StartingHealth)
    DEFINE_MEMBER(float, AFortAthenaMutator_HealthAndShield, MaxShield)
    DEFINE_MEMBER(float, AFortAthenaMutator_HealthAndShield, StartingShield)


public:
    DECLARE_STATIC_CLASS(AFortAthenaMutator_HealthAndShield);
    DECLARE_DEFAULT_OBJECT(AFortAthenaMutator_HealthAndShield);
};