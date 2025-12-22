#pragma once
#include "pch.h"

class UFortBuildingItemDefinition : public UFortWorldItemDefinition
{
public:
    DEFINE_MEMBER(TSoftObjectPtr<class UObject>, UFortBuildingItemDefinition, BuildingMetaData);
public:
    DECLARE_STATIC_CLASS(UFortBuildingItemDefinition);
    DECLARE_DEFAULT_OBJECT(UFortBuildingItemDefinition);
};