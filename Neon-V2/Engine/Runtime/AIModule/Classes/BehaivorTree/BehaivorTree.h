#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"

class UBlackboardData : public UDataAsset {
public:
    DECLARE_STATIC_CLASS(UBlackboardData);
    DECLARE_DEFAULT_OBJECT(UBlackboardData);
};

class UBehaviorTree : public UObject {
public:
    DEFINE_PTR(UBlackboardData, UBehaviorTree, BlackboardAsset);
public:
    DECLARE_STATIC_CLASS(UBehaviorTree);
    DECLARE_DEFAULT_OBJECT(UBehaviorTree);
};
