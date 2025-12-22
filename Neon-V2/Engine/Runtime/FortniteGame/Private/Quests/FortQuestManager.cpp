#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Quests/FortQuestManager.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/FortPlaylistAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/AI/FortAthenaAIBotController.h"

static std::unordered_map<std::string, std::vector<FFluxBroadcastQuestProgress>> PlayerFluxBroadcastQuestProgress;

std::vector<FFluxBroadcastQuestProgress> UFortQuestManager::GetFluxBroadcastQuestProgress(AFortPlayerControllerAthena* PlayerController)
{
	if (!PlayerController) return {};
	
	std::string AccountID = PlayerController->GetPlayerState()->GetAccountID();
	auto it = PlayerFluxBroadcastQuestProgress.find(AccountID);
	if (it != PlayerFluxBroadcastQuestProgress.end()) return it->second;
	
	return {};
}

void UFortQuestManager::SendComplexCustomStatEvent(UObject* TargetObj, FGameplayTagContainer& SourceTags, FGameplayTagContainer& TargetTags, bool* QuestActive, bool* QuestCompleted, int32 Count)
{
	UE_LOG(LogTemp, Log, __FUNCTION__);
    FGameplayTagContainer ContextTags;
    SendStatEvent(SourceTags, TargetTags, nullptr, nullptr, 1, EFortQuestObjectiveStatEvent::Custom, ContextTags);
    return SendComplexCustomStatEventOG(this, TargetObj, SourceTags, TargetTags, QuestActive, QuestCompleted, Count);
}

void UFortMinigameManager::SendStatEventWithTags(EFortQuestObjectiveStatEvent Type, UObject* TargetObject, FGameplayTagContainer& TargetTags, FGameplayTagContainer& SourceTags, FGameplayTagContainer& ContextTags, int32 Count)
{
	SendStatEventWithTagsOG(this, Type, TargetObject, TargetTags, SourceTags, ContextTags, Count);
	auto Controller = (AFortPlayerControllerAthena*)this->GetPlayerController();
	if (!Controller) return;
	
	Controller->GetQuestManager(ESubGame::Athena)->SendStatEvent(TargetTags, SourceTags, nullptr, nullptr, Count, Type, ContextTags);
}

static void ProgressQuest(AFortPlayerControllerAthena* PlayerController, UFortQuestManager* QuestManager, UFortQuestItem* QuestItem, UFortQuestItemDefinition* QuestDef, FFortMcpQuestObjectiveInfo* Objective, int32 IncrementCount)
{
	static std::unordered_map<AFortPlayerControllerAthena*, std::vector<FFortMcpQuestObjectiveInfo>> CompletedObjectives;
	
	std::string AccountID = PlayerController->GetPlayerState()->GetAccountID();
	
	auto Count = Fortnite_Version >= 8.51 ? QuestManager->GetObjectiveCompletionCount(QuestDef, Objective->GetBackendName()) : QuestDef->CallFunc<int32>("FortQuestItemDefinition", "GetPartialObjectiveCompletionCount");
	if (Count == -1) Count = 0;
	UE_LOG(LogTemp, Log, "Count: %d IncrementCount: %d", Count, IncrementCount);
	int32 NewCount = Count + IncrementCount;
	QuestDef->SetObjectiveCompletionCount(NewCount);
	std::string BackendName = std::string(Objective->GetBackendName().ToString().ToString());
	
	auto& ProgressList = PlayerFluxBroadcastQuestProgress[AccountID];
	bool found = false;
	for (auto& entry : ProgressList) {
		if (entry.BackendName == BackendName) {
			entry.Count = NewCount;
			found = true;
			break;
		}
	}

	if (!found) {
		FFluxBroadcastQuestProgress newEntry;
		newEntry.BackendName = BackendName;
		newEntry.Count = NewCount;
		ProgressList.push_back(newEntry);
	}
    
	bool objectiveCompleted = NewCount >= Objective->GetCount();
	bool allObjectivesCompleted = objectiveCompleted && QuestDef->GetObjectives().Num() == 1;
    
	if (objectiveCompleted && QuestDef->GetObjectives().Num() > 1) {
		bool alreadyExists = false;
		for (auto& CompObj : CompletedObjectives[PlayerController]) {
			if (CompObj.GetBackendName() == Objective->GetBackendName()) {
				alreadyExists = true;
				break;
			}
		}
		if (!alreadyExists) CompletedObjectives[PlayerController].push_back(*Objective);
        
		int32 CompletionCount = 0;
		for (auto& QuestObj : QuestDef->GetObjectives()) {
			for (auto& CompObj : CompletedObjectives[PlayerController]) {
				if (QuestObj.GetBackendName() == CompObj.GetBackendName()) {
					CompletionCount++;
					break;
				}
			}
		}
		allObjectivesCompleted = (CompletionCount == QuestDef->GetObjectives().Num());
	}
    
	auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->GetPlayerState());
	if (!PlayerState) return;

		bool foundExisting = false;
		for (auto& UpdatedStat : PlayerController->GetUpdatedObjectiveStats()) {
			if (UpdatedStat.BackendName == Objective->GetBackendName()) {
				UpdatedStat.Quest = QuestDef;
				UpdatedStat.CurrentStage++;
				UpdatedStat.StatDelta = Count;
				UpdatedStat.StatValue = QuestDef->GetObjectiveCompletionCount();
				PlayerController->OnRep_UpdatedObjectiveStats();
				foundExisting = true;
				break;
			}
		}

		if (!foundExisting) {
			static void* NewStatVoid = nullptr;
			static int StatSize = 0;
			if (!NewStatVoid) {
				StatSize = StaticClassImpl("FortUpdatedObjectiveStat")->GetSize();
				NewStatVoid = (void*)malloc(StatSize);
				memset(NewStatVoid, 0, StatSize);
			}
			if (StatSize == 0x18) {
				auto NewStat = (FFortUpdatedObjectiveStatOther*)NewStatVoid; 
				NewStat->BackendName = Objective->GetBackendName();
				NewStat->Quest = QuestDef;
				NewStat->StatValue = QuestDef->GetObjectiveCompletionCount();
				static int UpdatedObjectiveStatsOffset = UKismetMemLibrary::GetOffset(PlayerController, "UpdatedObjectiveStats");
				auto UpdatedObjectiveStats = reinterpret_cast<TArray<FFortUpdatedObjectiveStatOther>*>(__int64(PlayerController) + UpdatedObjectiveStatsOffset);
				UpdatedObjectiveStats->Add(*NewStat);
			} else
			{
				auto NewStat = (FFortUpdatedObjectiveStat*)NewStatVoid;
				NewStat->BackendName = Objective->GetBackendName();
				NewStat->Quest = QuestDef;
				NewStat->CurrentStage++;
				NewStat->StatDelta = Count;
				NewStat->StatValue = QuestDef->GetObjectiveCompletionCount();
				PlayerController->GetUpdatedObjectiveStats().Add(*NewStat, StatSize);
			}
			PlayerController->OnRep_UpdatedObjectiveStats();
		}

		QuestManager->CallFunc<void>("FortQuestManager", "SelfCompletedUpdatedQuest", PlayerController, QuestDef, Objective->GetBackendName(), NewCount, IncrementCount, PlayerState, objectiveCompleted, allObjectivesCompleted);
		QuestManager->CallFunc<void>("FortQuestManager", "HandleQuestUpdated", PlayerController, QuestDef, Objective->GetBackendName(), NewCount, IncrementCount,  PlayerState, objectiveCompleted, allObjectivesCompleted);
	
/*	for (const auto& TeamMember : PlayerState->GetPlayerTeam()->GetTeamMembers()) {
		auto TeamController = (AFortPlayerControllerAthena*)TeamMember;
		if (Fortnite_Version >= 10.40)
		{
			if (TeamController->IsA<AFortAthenaAIBotController>())
				continue;
		}
    	
		bool foundExisting = false;
		for (auto& UpdatedStat : PlayerController->GetUpdatedObjectiveStats()) {
			if (UpdatedStat.BackendName == Objective->GetBackendName()) {
				UpdatedStat.Quest = QuestDef;
				UpdatedStat.CurrentStage++;
				UpdatedStat.StatDelta = Count;
				UpdatedStat.StatValue = QuestDef->GetObjectiveCompletionCount();
				PlayerController->OnRep_UpdatedObjectiveStats();
				foundExisting = true;
				break;
			}
		}

		if (!foundExisting) {
			static void* NewStatVoid = nullptr;
			static int StatSize = 0;
			if (!NewStatVoid) {
				StatSize = StaticClassImpl("FortUpdatedObjectiveStat")->GetSize();
				NewStatVoid = (void*)malloc(StatSize);
				memset(NewStatVoid, 0, StatSize);
			}
			if (StatSize == 0x18) {
				auto NewStat = (FFortUpdatedObjectiveStatOther*)NewStatVoid; 
				NewStat->BackendName = Objective->GetBackendName();
				NewStat->Quest = QuestDef;
				NewStat->StatValue = QuestDef->GetObjectiveCompletionCount();
				static int UpdatedObjectiveStatsOffset = UKismetMemLibrary::GetOffset(PlayerController, "UpdatedObjectiveStats");
				auto UpdatedObjectiveStats = reinterpret_cast<TArray<FFortUpdatedObjectiveStatOther>*>(__int64(PlayerController) + UpdatedObjectiveStatsOffset);
				UpdatedObjectiveStats->Add(*NewStat);
			} else
			{
				auto NewStat = (FFortUpdatedObjectiveStat*)NewStatVoid;
				NewStat->BackendName = Objective->GetBackendName();
				NewStat->Quest = QuestDef;
				NewStat->CurrentStage++;
				NewStat->StatDelta = Count;
				NewStat->StatValue = QuestDef->GetObjectiveCompletionCount();
				PlayerController->GetUpdatedObjectiveStats().Add(*NewStat, StatSize);
			}
			PlayerController->OnRep_UpdatedObjectiveStats();
		}

		if (TeamController == PlayerController) {
			QuestManager->CallFunc<void>("FortQuestManager", "SelfCompletedUpdatedQuest", TeamController, QuestDef, Objective->GetBackendName(), NewCount, IncrementCount, PlayerState, objectiveCompleted, allObjectivesCompleted);
		} else {
			QuestManager->CallFunc<void>("FortQuestManager", "AssistedPlayerUpdatedQuest", TeamController, QuestDef, Objective->GetBackendName(), NewCount, IncrementCount, PlayerState, objectiveCompleted, allObjectivesCompleted);
		}

		QuestManager->CallFunc<void>("FortQuestManager", "HandleQuestUpdated", TeamController, QuestDef, Objective->GetBackendName(), NewCount, IncrementCount, TeamController == PlayerController ? nullptr : PlayerState, objectiveCompleted, allObjectivesCompleted);
	} */

	if (allObjectivesCompleted) {
		std::cout << "allObjectivesCompleted" << std::endl;
		CompletedObjectives[PlayerController].clear();

		if (Fortnite_Version >= 11.00) {
			int32 XPReward = 0;
			if (auto RewardsTable = QuestDef->GetRewardsTable()) {
				static auto DefaultName = UKismetStringLibrary::Conv_StringToName(L"Default");
				for (const auto& [RowName, RowPtr] : RewardsTable->GetRowMap()) {
					if (RowName == DefaultName) {
						XPReward = ((FFortQuestRewardTableRow*)RowPtr)->GetQuantity();
						break;
					}
				}
			}

			if (XPReward > 0)
			{
				for (const auto& TeamMember : PlayerState->GetPlayerTeam()->GetTeamMembers()) {
					auto TeamController = (AFortPlayerControllerAthena*)TeamMember;
					if (!TeamController) continue;
					if (TeamController->IsA<AFortAthenaAIBotController>())
						continue;
                
					auto XPComponent = TeamController->GetXPComponent();
					if (!XPComponent) continue;

					FXPEventInfo XPEvent{};
					XPEvent.EventXpValue = XPReward;
					XPEvent.RestedValuePortion = XPReward;
					XPEvent.RestedXPRemaining = XPReward;
					XPEvent.TotalXpEarnedInMatch = XPComponent->GetTotalXpEarned() + XPReward;
					XPEvent.Priority = EXPEventPriorityType::XPBarOnly;
					XPEvent.SimulatedText = UKismetStringLibrary::Conv_StringToText(L"Objective Completed");
					XPEvent.EventName = UKismetStringLibrary::Conv_StringToName(L"Objective Completed");
					XPEvent.SeasonBoostValuePortion = 20;

					XPComponent->SetChallengeXp(XPComponent->GetChallengeXp() + XPReward);
					XPComponent->SetMatchXp(XPComponent->GetMatchXp() + XPReward);
					XPComponent->SetTotalXpEarned(XPComponent->GetTotalXpEarned() + XPReward);
					XPComponent->OnXPEvent(XPEvent);
				}
			}
		}
	}
}

void UFortQuestManager::SendStatEvent(FGameplayTagContainer& SourceTags, FGameplayTagContainer& TargetTags, bool* QuestActive, bool* QuestCompleted, int32 Count, EFortQuestObjectiveStatEvent StatEvent, FGameplayTagContainer& ContextTags)
{
	FGameplayTagContainer PSourceTags;
	FGameplayTagContainer PContextTags;
	GetSourceAndContextTags(&PSourceTags, &ContextTags);
	UFortPlaylistAthena* Playlist = nullptr;
	if (Fortnite_Version >= 6.10)
	{
		static int CurrentPlaylistInfoOffset = UKismetMemLibrary::GetOffset(GetWorld()->GetGameState(), "CurrentPlaylistInfo");
		FPlaylistPropertyArray& CurrentPlaylistInfoPtr = *reinterpret_cast<FPlaylistPropertyArray*>(__int64(GetWorld()->GetGameState()) + CurrentPlaylistInfoOffset);
		Playlist = CurrentPlaylistInfoPtr.GetBasePlaylist();
	} else Playlist = GetWorld()->GetGameState()->GetCurrentPlaylistData();
	
	ContextTags.AppendTags(PContextTags);
	ContextTags.AppendTags(Playlist->GetGameplayTagContainer());
	SourceTags.AppendTags(PSourceTags);
	ContextTags.AppendTags(ContextTags);
	
	auto& CurrentQuests = GetCurrentQuests();
	for (int i = 0; i < CurrentQuests.Num(); i++)
	{
		if (!CurrentQuests.IsValidIndex(i) || !CurrentQuests[i]) continue;
		
		auto CurrentQuest = CurrentQuests[i];
		if (CurrentQuest->HasCompletedQuest()) continue;
		
		auto QuestDef = CurrentQuest->GetQuestDefinitionBP();
		if (!QuestDef) continue;
		if (HasCompletedQuest(QuestDef)) continue;

		auto Controller = (AFortPlayerControllerAthena*)CurrentQuest->GetOwningController();
		if (!Controller || !Controller->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, "No Controller");
			return;
		}
		
		auto& Objectives = QuestDef->GetObjectives();
		static int32 FFortMcpQuestObjectiveInfoSize = StaticClassImpl("FortMcpQuestObjectiveInfo")->GetSize();
		for (int j = 0; j < Objectives.Num(); j++)
		{
			if (!Objectives.IsValidIndex(j)) continue;
			FFortMcpQuestObjectiveInfo* Objective = (FFortMcpQuestObjectiveInfo*)((uint8*)Objectives.GetData() + (j * FFortMcpQuestObjectiveInfoSize));
			
			if (HasCompletedObjectiveWithName(QuestDef, Objective->GetBackendName()) ||
					HasCompletedObjective(QuestDef, Objective->GetObjectiveStatHandle()) ||
					CurrentQuest->HasCompletedObjectiveWithName(Objective->GetBackendName()) ||
					CurrentQuest->HasCompletedObjective(Objective->GetObjectiveStatHandle())) continue;
				
			auto StatTable = Objective->GetObjectiveStatHandle().DataTable;
			auto& StatRowName = Objective->GetObjectiveStatHandle().RowName;

			if (!StatTable && Fortnite_Version >= 9.00 || StatRowName.GetComparisonIndex() < 0 && Fortnite_Version >= 9.00)
			{
				auto& InlineStats = Objective->GetInlineObjectiveStats();
				static int32 FFortQuestObjectiveStatSize = StaticClassImpl("FortQuestObjectiveStat")->GetSize();
				for (int k = 0; k < InlineStats.Num(); k++)
				{
					if (!InlineStats.IsValidIndex(k)) continue;
					FFortQuestObjectiveStat* ObjectiveStat = (FFortQuestObjectiveStat*)((uint8*)InlineStats.GetData() + (k * FFortQuestObjectiveStatSize));
					if (ObjectiveStat->GetType() != StatEvent) continue;

					bool bFoundQuest = true; 
					auto& TagConditions = ObjectiveStat->GetTagConditions();
					static int32 FInlineObjectiveStatTagCheckEntrySize = StaticClassImpl("InlineObjectiveStatTagCheckEntry")->GetSize();
					for (int l = 0; l < TagConditions.Num(); l++)
					{
						if (!TagConditions.IsValidIndex(l)) continue;
						
						auto* TagCondition = (FInlineObjectiveStatTagCheckEntry*)((uint8*)TagConditions.GetData() + (l * FInlineObjectiveStatTagCheckEntrySize));
						if (!TagCondition->GetRequire() || !bFoundQuest) continue;

						auto CheckTag = [&](bool bHasTags, const FGameplayTagContainer& Tags)
						{
							if (bHasTags && !Tags.HasTag(TagCondition->GetTag()))
								bFoundQuest = false;
						};

						switch (TagCondition->GetType())
						{
						case EInlineObjectiveStatTagCheckEntryType::Target:
							CheckTag(ObjectiveStat->GetbHasInclusiveTargetTags(), TargetTags);
							break;
						case EInlineObjectiveStatTagCheckEntryType::Source:
							CheckTag(ObjectiveStat->GetbHasInclusiveSourceTags(), SourceTags);
							break;
						case EInlineObjectiveStatTagCheckEntryType::Context:
							CheckTag(ObjectiveStat->GetbHasInclusiveContextTags(), ContextTags);
							break;
						default:
							break;
						}
					}

					if (bFoundQuest)
						ProgressQuest(Controller, this, CurrentQuest, QuestDef, Objective, Count); 
				}
			}
			else if (StatTable)
			{
				auto& RowMap = StatTable->GetRowMap();

				for (const auto& [RowName, RowPtr] : RowMap)
				{
					if (RowName == StatRowName)
					{
						auto Row = (FFortQuestObjectiveStatTableRow*)RowPtr;
						if (Row->Type != StatEvent) continue;
						UE_LOG(LogTemp, Log, "Found Stat Row: %s", RowName.ToString().ToString().c_str());
				//		if (!TargetTags.HasAll(Row->TargetTagContainer)) continue;
			//			if (!SourceTags.HasAll(Row->GetSourceTagContainer())) continue;
			//			if (!ContextTags.HasAll(Row->GetContextTagContainer())) continue;

						ProgressQuest(Controller, this, CurrentQuest, QuestDef, Objective, Count);
					}
				}
			}
		}
	}
}

void UFortQuestManager::SendCustomStatEvent(struct FDataTableRowHandle& ObjectiveStat, int32 Count, bool bForceFlush)
{
	auto& CurrentQuests = GetCurrentQuests();
	for (int i = 0; i < CurrentQuests.Num(); i++)
	{
		if (!CurrentQuests.IsValidIndex(i) || !CurrentQuests[i]) continue;
		
		auto CurrentQuest = CurrentQuests[i];
		if (CurrentQuest->HasCompletedQuest()) continue;
		
		auto QuestDef = CurrentQuest->GetQuestDefinitionBP();
		if (!QuestDef) continue;
		if (HasCompletedQuest(QuestDef)) continue;

		auto Controller = (AFortPlayerControllerAthena*)CurrentQuest->GetOwningController();
		if (!Controller || !Controller->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, "No Controller");
			return;
		}
		
		auto StatTable = ObjectiveStat.DataTable;
		auto& StatRowName = ObjectiveStat.RowName;

		auto Objectives = QuestDef->GetObjectives();
		
		static int32 FFortMcpQuestObjectiveInfoSize = StaticClassImpl("FortMcpQuestObjectiveInfo")->GetSize();
		FGameplayTagContainer SourceTags;
		FGameplayTagContainer ContextTags;
		GetSourceAndContextTags(&SourceTags, &ContextTags);

		auto ObjectiveInfo = CurrentQuest->GetObjectiveInfo(ObjectiveStat);
		
		for (int j = 0; j < Objectives.Num(); j++)
		{
			if (!Objectives.IsValidIndex(j))
				continue;
				
			FFortMcpQuestObjectiveInfo* Objective = (FFortMcpQuestObjectiveInfo*)((uint8*)Objectives.GetData() + (j * FFortMcpQuestObjectiveInfoSize));
			if (Objective->GetObjectiveStatHandle().RowName == StatRowName)
			{
				if (StatTable)
				{
					auto& RowMap = StatTable->GetRowMap();

					for (const auto& [RowName, RowPtr] : RowMap)
					{
						if (RowName == StatRowName)
						{
							auto Row = (FFortQuestObjectiveStatTableRow*)RowPtr;
							if (Row->Type != ObjectiveInfo->GetStatEvent()) continue;
							if (!SourceTags.HasAll(Row->SourceTagContainer)) continue;
							if (!ContextTags.HasAll(Row->ContextTagContainer)) continue;
					
							ProgressQuest(Controller, this, CurrentQuest, QuestDef, Objective, Count);
							break;
						}
					}
				}
			}
		}
	}
}
