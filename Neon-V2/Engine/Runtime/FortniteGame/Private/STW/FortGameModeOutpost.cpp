#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/STW/FortGameModeOutpost.h"

#include "Engine/Runtime/Engine/Classes/World.h"

char AFortMission::InitializeActorsForMission(AFortMission* a1, AFortMission* Mission, int TagQuery, __int64 OutActorsInitialized)
{
	auto GameState = GetWorld()->GetGameState();

	if (!GameState)
	{
		UE_LOG(LogTemp, Error, "No GameState!");
		return NULL;
	}

	auto WorldManager = GameState->GetWorldManager();
	auto& Manager = WorldManager->GetMissionManagerRecord();
	
	auto MissionGen = StaticFindObject<UFortMissionGenerator>("/Game/World/MissionGens/MissionGen_TheOutpost_PvE_01.MissionGen_TheOutpost_PvE_01");
	
	FFortMissionRecord Record{};
	Record.MissionEntry.MissionInfo = StaticLoadObject<UFortMissionInfo>("/Game/Missions/Primary/Outpost/MissionInfo_Outpost_PvE_01.MissionInfo_Outpost_PvE_01");
	Record.MissionEntry.MissionGenerator = MissionGen;
//	Record.MissionGuid = Mission->GetMissionGuid();

	static int32 MissionRecordSize = StaticClassImpl("FortMissionRecord")->GetSize();
	
	Manager.MissionRecords.Add(Record, MissionRecordSize);
	
	Mission->SetMissionGenerator(MissionGen);
	
	return InitializeActorsForMissionOG(a1, Mission, TagQuery, OutActorsInitialized);
}
