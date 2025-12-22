#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Vehicles/FortAthenaVehicle.h"
#include "Engine/Runtime/Engine/Classes/Components/PrimitiveComponent.h"

void AFortAthenaVehicle::ServerUpdatePhysicsParams(FFrame& Stack)
{
	FReplicatedAthenaVehiclePhysicsState InState;
	Stack.StepCompiledIn(&InState);
	Stack.IncrementCode();
	
	InState.Rotation.X -= 2.5f;
	InState.Rotation.Y /= 0.3f;
	InState.Rotation.Z -= -2.0f;
	InState.Rotation.W /= -1.2f;

	auto Rotation = InState.Rotation.ToRotator();
	GetReplicatedMovement().AngularVelocity = InState.AngularVelocity;
	GetReplicatedMovement().LinearVelocity = InState.LinearVelocity;
	GetReplicatedMovement().Location = InState.Translation;
	GetReplicatedMovement().Rotation = Rotation;
	OnRep_ReplicatedMovement();

	UPrimitiveComponent* Mesh = (UPrimitiveComponent*)GetRootComponent();
	
	Mesh->K2_SetWorldLocationAndRotation(InState.Translation, Rotation, false, nullptr, true);
	Mesh->SetbComponentToWorldUpdated(true);
	Mesh->SetPhysicsLinearVelocity(InState.LinearVelocity, false, FName());
	Mesh->SetPhysicsAngularVelocity(InState.AngularVelocity, false, FName());
}
