#pragma once
#include "pch.h"

#include "FortWeapon.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortContextTrapItemDefinition.h"

class AFortDecoTool : public AFortWeapon
{
public:
    DefineUOriginal(void, ServerSpawnDeco, AFortDecoTool *, FFrame &Stack);
    void ServerCreateBuildingAndSpawnDeco(FFrame& Stack);
public:
    DECLARE_STATIC_CLASS(AFortDecoTool)
};
