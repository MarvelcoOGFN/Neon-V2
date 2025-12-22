#pragma once
#include "pch.h"

#include "ItemDefinition.h"

class UFortWorldItemDefinition : public UFortItemDefinition
{
public:
    DEFINE_MEMBER(uint8, UFortWorldItemDefinition, NumberOfSlotsToTake);
    DEFINE_MEMBER(int32, UFortWorldItemDefinition, MaxLevel);
    DEFINE_MEMBER(int32, UFortWorldItemDefinition, MinLevel);
    DEFINE_BOOL(UFortWorldItemDefinition, bCanBeDropped)
    DEFINE_BOOL(UFortWorldItemDefinition, bForceIntoOverflow)
    DEFINE_MEMBER(int, UFortWorldItemDefinition, DropCount)
public:
    
public:
    DECLARE_STATIC_CLASS(UFortWorldItemDefinition)
    DECLARE_DEFAULT_OBJECT(UFortWorldItemDefinition)
};
