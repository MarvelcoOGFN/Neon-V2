#pragma once
#include "pch.h"

#include "BehaivorTree.h"
#include "Engine/Runtime/AIModule/Classes/BrainComponent.h"

class UBehaviorTreeComponent : public UBrainComponent {
public:
    DEFINE_PTR(UBehaviorTree, UBehaviorTreeComponent, DefaultBehaviorTreeAsset)
public:
    DECLARE_STATIC_CLASS(UBehaviorTreeComponent)
    DECLARE_DEFAULT_OBJECT(UBehaviorTreeComponent)
};
