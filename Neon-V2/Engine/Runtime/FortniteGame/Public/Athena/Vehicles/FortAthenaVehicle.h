#pragma once
#include "pch.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPawn.h"

struct FReplicatedAthenaVehiclePhysicsState final
{
public:
	struct FVector                                Translation;                                       // 0x0000(0x000C)(IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_C[0x4];                                        // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	struct FQuat                                  Rotation;                                          // 0x0010(0x0010)(IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	struct FVector                                LinearVelocity;                                    // 0x0020(0x000C)(IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector                                AngularVelocity;                                   // 0x002C(0x000C)(IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint16_t                                        SyncKey;                                           // 0x0038(0x0002)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_3A[0x6];                                       // 0x003A(0x0006)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class AFortAthenaVehicle : public APawn
{
public:
	DefineUOriginal(void, ServerUpdatePhysicsParams, AFortAthenaVehicle*, FFrame& Stack)
public:
	DECLARE_STATIC_CLASS(AFortAthenaVehicle)
	DECLARE_DEFAULT_OBJECT(AFortAthenaVehicle)
};