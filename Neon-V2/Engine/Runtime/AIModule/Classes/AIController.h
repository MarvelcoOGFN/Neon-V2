#pragma once
#include "pch.h"

#include "BrainComponent.h"
#include "BehaivorTree/BehaviorTreeComponent.h"
#include "BehaivorTree/BlackboardComponent.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"

class AAIController : public AController
{
public:
    DEFINE_PTR(UPathFollowingComponent, AAIController, PathFollowingComponent);
    DEFINE_PTR(UBrainComponent, AAIController, BrainComponent);
    DEFINE_PTR(UBlackboardComponent, AAIController, Blackboard);
private:
    struct AIController_UseBlackboard final
    {
    public:
        class UBlackboardData* BlackboardAsset;
        class UBlackboardComponent* BlackboardComponent;
        bool ReturnValue;
    };

    struct AIController_OnUsingBlackBoard final
    {
    public:
        class UBlackboardComponent* BlackboardComp;
        class UBlackboardData* BlackboardAsset;
    };
public:
    bool UseBlackboard(class UBlackboardData* BlackboardAsset, class UBlackboardComponent** BlackboardComponent) {
        static class UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("AIController", "UseBlackboard");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        AIController_UseBlackboard Params;
		Params.BlackboardAsset = BlackboardAsset;
        if (BlackboardComponent != nullptr)
            Params.BlackboardComponent = *BlackboardComponent;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    void OnUsingBlackBoard(class UBlackboardComponent* BlackboardComp, class UBlackboardData* BlackboardAsset) {
        static class UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("AIController", "OnUsingBlackBoard");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        AIController_OnUsingBlackBoard Params;
		Params.BlackboardComp = BlackboardComp;
        Params.BlackboardAsset = BlackboardAsset;

        this->ProcessEvent(Func, &Params);
    }

    bool RunBehaviorTree(UBehaviorTree* BTAsset)
    {
        if (!BTAsset) return false;

        bool bSuccess = true;

        UBlackboardComponent* BlackboardComp = GetBlackboard();
        if (BTAsset->GetBlackboardAsset() && !BlackboardComp)
        {
            bSuccess = UseBlackboard(BTAsset->GetBlackboardAsset(), &BlackboardComp);
            if (bSuccess) {
                OnUsingBlackBoard(BlackboardComp, BTAsset->GetBlackboardAsset());
            }
        } 

        if (bSuccess)
        {
            UBehaviorTreeComponent* BTComp = (UBehaviorTreeComponent*)GetBrainComponent();
            if (!BTComp)
            {
                BTComp = (UBehaviorTreeComponent*)UGameplayStatics::SpawnObject(UBehaviorTreeComponent::StaticClass(), this);
//                ((void (*)(UObject * Component, UObject * World))(uintptr_t)UKismetMemLibrary::GetAddress<uintptr_t>(TEXT("RegisterComponentWithWorld")))(BTComp, GetWorld());
                ((void (*)(UObject * Component, UObject * World))(IMAGEBASE + 0x486D890))(BTComp, GetWorld());
            }
            SetBrainComponent(BTComp);
            BTComp->SetDefaultBehaviorTreeAsset(BTAsset);
            BTComp->StartLogic();
        }

        return bSuccess;
    }
};
