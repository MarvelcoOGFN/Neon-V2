#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortPlaysetItemDefinition.h"

class AB_Prj_Athena_PlaysetGrenade_C : public AActor
{
public:
    void OnLoaded_3645F4484F4ECED813C69D92F55C7A1F(UObject* Loaded);
public:
    DEFINE_PTR(UFortPlaysetItemDefinition, AB_Prj_Athena_PlaysetGrenade_C, CachedPlayset)
    DEFINE_PTR(UClass, AB_Prj_Athena_PlaysetGrenade_C, PlayspaceActorToSpawn)
    DECLARE_STATIC_CLASS(AB_Prj_Athena_PlaysetGrenade_C)
    DECLARE_DEFAULT_OBJECT(AB_Prj_Athena_PlaysetGrenade_C)
};
