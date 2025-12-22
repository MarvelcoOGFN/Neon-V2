#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"

struct FFortMcpQuestObjectiveInfo final
{
    DEFINE_MEMBER(int32, FFortMcpQuestObjectiveInfo, Count);
    DEFINE_MEMBER(TArray<struct FFortQuestObjectiveStat> , FFortMcpQuestObjectiveInfo, InlineObjectiveStats);
    DEFINE_MEMBER(FName, FFortMcpQuestObjectiveInfo, BackendName);
    DEFINE_MEMBER(FDataTableRowHandle, FFortMcpQuestObjectiveInfo, ObjectiveStatHandle);
};

class UFortQuestItemDefinition : public UObject
{
public:
    DEFINE_PTR(UDataTable, UFortQuestItemDefinition, RewardsTable);
    DEFINE_MEMBER(int32, UFortQuestItemDefinition, ObjectiveCompletionCount);
    DEFINE_MEMBER(TArray<struct FFortMcpQuestObjectiveInfo>,UFortQuestItemDefinition,     Objectives);
public:
    DECLARE_DEFAULT_OBJECT(UFortQuestItemDefinition)
    DECLARE_STATIC_CLASS(UFortQuestItemDefinition)
};
