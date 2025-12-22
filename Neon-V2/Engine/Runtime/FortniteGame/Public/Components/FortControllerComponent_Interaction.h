#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"

class UFortControllerComponent_Interaction : public UActorComponent
{
public:
    DefineUOriginal(void, ServerAttemptInteract, UFortControllerComponent_Interaction* Component, FFrame& Stack);
public:
    DECLARE_STATIC_CLASS(UFortControllerComponent_Interaction)
    DECLARE_DEFAULT_OBJECT(UFortControllerComponent_Interaction)
};
