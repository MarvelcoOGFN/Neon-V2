#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"

class UFortControllerComponent_Aircraft : public UActorComponent
{
public:
	void ServerAttemptAircraftJump(FFrame& Stack);
public:
    DECLARE_STATIC_CLASS(UFortControllerComponent_Aircraft)
    DECLARE_DEFAULT_OBJECT(UFortControllerComponent_Aircraft)
};
