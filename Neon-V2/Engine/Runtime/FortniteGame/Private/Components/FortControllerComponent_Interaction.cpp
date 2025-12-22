#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Components/FortControllerComponent_Interaction.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingContainer.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"

void UFortControllerComponent_Interaction::ServerAttemptInteract(FFrame& Stack)
{
    enum class ETInteractionType : uint8 { IT_NoInteraction=0, IT_Simple=1, IT_LongPress=2, IT_BuildingEdit=3, IT_BuildingImprovement=4, IT_TrapPlacement=5, IT_MAX=6 };
    enum class EInteractionBeingAttempted : uint8 { FirstInteraction=0, SecondInteraction=1, AllInteraction=2, EInteractionBeingAttempted_MAX=3 };

    AActor* ReceivingActor;
    UPrimitiveComponent* InteractComponent;
    ETInteractionType InteractType;
    UObject* OptionalObjectData;
    EInteractionBeingAttempted InteractionBeingAttempted{};
    int32 RequestId{};

    Stack.StepCompiledIn(&ReceivingActor);
    Stack.StepCompiledIn(&InteractComponent);
    Stack.StepCompiledIn(&InteractType);
    Stack.StepCompiledIn(&OptionalObjectData);
    if (Fortnite_Version >= 11.00)
    {
        Stack.StepCompiledIn(&InteractionBeingAttempted);
        Stack.StepCompiledIn(&RequestId);
    }
    Stack.IncrementCode();

    if (ReceivingActor->IsA(ABuildingContainer::StaticClass()) && Fortnite_Version <= 10.20)
    {
        auto Container = Cast<ABuildingContainer>(ReceivingActor);
        if (!Container->GetbAlreadySearched()) Container->SpawnLoot();
        else goto callOriginal;
    }

callOriginal:
    if (Fortnite_Version >= 11.00)
    {
        struct Params
        {
            AActor* ReceivingActor;
            UPrimitiveComponent* InteractComponent;
            ETInteractionType InteractType;
            uint8 Pad_11[0x7];
            UObject* OptionalObjectData;
            EInteractionBeingAttempted InteractionBeingAttempted;
            uint8 Pad_21[0x3];
            int32 RequestId;
        } Params{};

        Params.ReceivingActor = ReceivingActor;
        Params.InteractComponent = InteractComponent;
        Params.InteractType = InteractType;
        Params.OptionalObjectData = OptionalObjectData;
        Params.InteractionBeingAttempted = InteractionBeingAttempted;
        Params.RequestId = RequestId;

        callExecOG(this, "/Script/FortniteGame.FortControllerComponent_Interaction", ServerAttemptInteract, Params);
    }
    else
    {
        struct Params
        {
            AActor* ReceivingActor;
            UPrimitiveComponent* InteractComponent;
            ETInteractionType InteractType;
            uint8 Pad_11[0x7];
            UObject* OptionalObjectData;
        } Params{};

        Params.ReceivingActor = ReceivingActor;
        Params.InteractComponent = InteractComponent;
        Params.InteractType = InteractType;
        Params.OptionalObjectData = OptionalObjectData;

        callExecOG(this, "/Script/FortniteGame.FortControllerComponent_Interaction", ServerAttemptInteract, Params);
    }

    if (ReceivingActor->IsA(ABuildingActor::StaticClass()))
    {
        ABuildingActor* BuildingActor = (ABuildingActor*)ReceivingActor;
        if (!IsValidPointer(BuildingActor)) return;
        FGameplayTagContainer SourceTags;
        FGameplayTagContainer TargetTags;
        FGameplayTagContainer ContextTags;
        UFortQuestManager* QuestManager = ((AFortPlayerControllerAthena*)GetOwner())->GetQuestManager(ESubGame::Athena);
        QuestManager->GetSourceAndContextTags(&SourceTags, &ContextTags);
        TargetTags.AppendTags(BuildingActor->GetStaticGameplayTags());
        
        QuestManager->SendStatEvent(SourceTags, TargetTags, nullptr, nullptr, 1, EFortQuestObjectiveStatEvent::Interact, ContextTags);
    }
}