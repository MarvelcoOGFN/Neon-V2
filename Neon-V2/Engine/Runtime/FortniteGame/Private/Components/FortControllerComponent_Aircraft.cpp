#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Components/FortControllerComponent_Aircraft.h"
#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"

void UFortControllerComponent_Aircraft::ServerAttemptAircraftJump(FFrame& Stack)
{
    FRotator Rotation;
    Stack.StepCompiledIn(&Rotation);
    Stack.IncrementCode();
    
    Cast<AFortPlayerController>(GetOwner())->ServerAttemptAircraftJump(Rotation);
}
