#pragma once
#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Mutators/FortAthenaMutator.h"


class AFortAthenaMutator_SiphonValues : public AFortAthenaMutator
{
public:
    DEFINE_MEMBER(float, AFortAthenaMutator_SiphonValues, HealthSiphonValue)
        DEFINE_MEMBER(float, AFortAthenaMutator_SiphonValues, WoodSiphonValue)
        DEFINE_MEMBER(float, AFortAthenaMutator_SiphonValues, StoneSiphonValue)
        DEFINE_MEMBER(float, AFortAthenaMutator_SiphonValues, MetalSiphonValue)


public:
    DECLARE_STATIC_CLASS(AFortAthenaMutator_SiphonValues);
    DECLARE_DEFAULT_OBJECT(AFortAthenaMutator_SiphonValues);
};