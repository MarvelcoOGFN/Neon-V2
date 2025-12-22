#pragma once
#include "pch.h"

#include "FortWeapon.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortContextTrapItemDefinition.h"

class AFortDecoTool_ContextTrap : public AFortWeapon
{
public:
    DEFINE_PTR(UFortContextTrapItemDefinition, AFortDecoTool_ContextTrap, ContextTrapItemDefinition);
public:
    DECLARE_STATIC_CLASS(AFortDecoTool_ContextTrap)
};
