#pragma once
#include "pch.h"

#include "World.h"
#include "GameFramework/Actor.h"

class UGameplayStatics : public UObject
{
public:
	static TArray<AActor*> GetAllActorsOfClass(const UObject* World, UClass* ActorClass);
	static float GetTimeSeconds(UWorld* World);
	static UObject* SpawnObject(TSubclassOf<class UObject> ObjectClass, class UObject* Param_Outer);
	static AActor* FinishSpawningActor(AActor* Actor, FTransform SpawnTransform);
	static AActor* BeginSpawningActorFromClass(UObject* WorldContextObject, UClass* ActorClass, FTransform SpawnTransform, bool bNoCollisionFail = false, AActor* Owner = NULL);
public:
	DECLARE_STATIC_CLASS(UGameplayStatics);
	DECLARE_DEFAULT_OBJECT(UGameplayStatics);
};