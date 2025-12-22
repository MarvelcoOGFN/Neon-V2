#pragma once
#include "pch.h"

#include "Engine/Runtime/AIModule/Classes/AIController.h"
#include "Engine/Runtime/AIModule/Classes/BehaivorTree/BehaivorTree.h"

class UFortAthenaAIBotAimingDigestedSkillSet : public UObject
{
public:
    DEFINE_PTR(class AFortWeapon, UFortAthenaAIBotAimingDigestedSkillSet, CachedWeaponUsedToCalculateAccuracy);
public:
    DECLARE_STATIC_CLASS(UFortAthenaAIBotAimingDigestedSkillSet)
    DECLARE_DEFAULT_OBJECT(UFortAthenaAIBotAimingDigestedSkillSet)
};

class AFortAthenaAIBotController : public AAIController
{
public:
    void BlueprintOnBehaviorTreeStarted() {
        static class SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaAIBotController", "BlueprintOnBehaviorTreeStarted");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
public:
    DEFINE_PTR(UFortAthenaAIBotAimingDigestedSkillSet, AFortAthenaAIBotController, CacheAimingDigestedSkillSet)
    DEFINE_PTR(class AFortInventory, AFortAthenaAIBotController, Inventory)
    DEFINE_PTR(class UFortAthenaAIBotInventoryItems, AFortAthenaAIBotController, StartupInventory)
    DEFINE_PTR(UBehaviorTree, AFortAthenaAIBotController, BehaviorTree)
    DEFINE_MEMBER(float, AFortAthenaAIBotController, Skill)
public:
    DECLARE_STATIC_CLASS(AFortAthenaAIBotController)
    DECLARE_DEFAULT_OBJECT(AFortAthenaAIBotController)
};
