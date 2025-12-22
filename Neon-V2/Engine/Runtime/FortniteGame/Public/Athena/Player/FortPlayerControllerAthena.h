#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/FortPlaylistAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"
#include "Engine/Runtime/FortniteGame/Public/Quests/FortQuestManager.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

struct FFortPlayerDeathReport final
{
public:
	float                                         ServerTimeForRespawn;                              // 0x0000(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         ServerTimeForResurrect;                            // 0x0004(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         LethalDamage;                                      // 0x0008(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1506[0x4];                                     // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	class AFortPlayerState*                       KillerPlayerState;                                 // 0x0010(0x0008)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AFortPawn*                              KillerPawn;                                        // 0x0018(0x0008)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AActor*                                 DamageCauser;                                      // 0x0020(0x0008)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         bDroppedBackpack : 1;                              // 0x0028(0x0001)(BitIndex: 0x00, PropSize: 0x0001 (BlueprintVisible, BlueprintReadOnly, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         bNotifyUI : 1;                                     // 0x0028(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (BlueprintVisible, BlueprintReadOnly, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
	uint8                                         Pad_1507[0x7];                                     // 0x0029(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	struct FGameplayTagContainer                  Tags;                                              // 0x0030(0x0020)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
};

enum class ERewardSource : uint8
{
	Invalid = 0,
	MinutesPlayed = 1,
	FirstKill = 2,
	TeamKills = 3,
	Placement = 4,
	Medals = 5,
	FirstWin = 6,
	SeasonLevelUp = 7,
	BookLevelUp = 8,
	MAX_COUNT = 9,
	ERewardSource_MAX = 10,
};

struct FAthenaAwardGroup final
{
public:
	ERewardSource RewardSource; // 0x0000(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_1[0x7];			// 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	uint8 Pad_21[0xA];
	int32 Score;	   // 0x0018(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float SeasonXp;	   // 0x001C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 BookXp;	   // 0x0020(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_24[0x4]; // 0x0024(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FAthenaRewardResult final
{
public:
	DEFINE_MEMBER(int32, FAthenaRewardResult, LevelsGained);
	DEFINE_MEMBER(int32, FAthenaRewardResult, BookLevelsGained);
	DEFINE_MEMBER(int32, FAthenaRewardResult, TotalSeasonXpGained);
	DEFINE_MEMBER(int32, FAthenaRewardResult, TotalBookXpGained);
	DEFINE_MEMBER(int32, FAthenaRewardResult, PrePenaltySeasonXpGained);
	DEFINE_MEMBER(TArray<struct FAthenaAwardGroup>, FAthenaRewardResult, Rewards);
	uint8 Pad_14[0x4];
	DEFINE_MEMBER(float, FAthenaRewardResult, AntiAddictionMultiplier);
	uint8 Pad_3C[0x4];
};

struct FAthenaMatchStats final
{
public:
	DEFINE_MEMBER(FString, FAthenaMatchStats, StatBucket);
	DEFINE_MEMBER(FString, FAthenaMatchStats, MatchID);
	DEFINE_MEMBER(FString, FAthenaMatchStats, MatchEndTime);
	DEFINE_MEMBER(FString, FAthenaMatchStats, MatchPlatform);
	int32 Stats[0x14];
	DEFINE_BOOL(FAthenaMatchStats, bIsValid);
	uint8 Pad_D5[0x3];
	DEFINE_MEMBER(FString, FAthenaMatchStats, FactionTag);
};

struct FAthenaMatchTeamStats final
{
public:
	DEFINE_MEMBER(int32, FAthenaMatchTeamStats, Place);
	DEFINE_MEMBER(int32, FAthenaMatchTeamStats, TotalPlayers);
};

class UAthenaPlayerMatchReport : public UObject
{
public:
	DEFINE_MEMBER(FAthenaRewardResult, UAthenaPlayerMatchReport, EndOfMatchResults); // 0x0000(0x0038)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
	DEFINE_MEMBER(FAthenaMatchTeamStats, UAthenaPlayerMatchReport, TeamStats);		 // 0x0000(0x0028)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
	DEFINE_MEMBER(FAthenaMatchStats, UAthenaPlayerMatchReport, MatchStats);			 // 0x0028(0x00A0)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
public:
	DECLARE_STATIC_CLASS(UAthenaPlayerMatchReport);
	DECLARE_DEFAULT_OBJECT(UAthenaPlayerMatchReport);
};

struct FFortUpdatedObjectiveStat final
{
public:
	class UFortQuestItemDefinition*               Quest;                                             // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FName                                   BackendName;                                       // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         StatValue;                                         // 0x0010(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         StatDelta;                                         // 0x0014(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         CurrentStage;                                      // 0x0018(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_144E[0x4];                                     // 0x001C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};
struct FFortUpdatedObjectiveStatOther final
{
public:
	class UFortQuestItemDefinition *Quest; // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FName BackendName;			   // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 StatValue;					   // 0x0010(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_14[0x4];					   // 0x0014(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FAthenaQuickChatActiveEntry final
{
public:
	class UObject *Bank;		  // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UObject *ContextObject; // 0x0008(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint16_t ContextValue;		  // 0x0010(0x0002)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Index;				  // 0x0012(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_13[0x5];			  // 0x0013(0x0005)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class AFortBroadcastRemoteClientInfo : public AActor
{
public:
	DEFINE_MEMBER(EFortResourceType, AFortBroadcastRemoteClientInfo, RemoteBuildingMaterial);
	DEFINE_MEMBER(TSubclassOf<class ABuildingSMActor>, AFortBroadcastRemoteClientInfo, RemoteBuildableClass);
public:
	DECLARE_STATIC_CLASS(AFortBroadcastRemoteClientInfo)
	DECLARE_DEFAULT_OBJECT(AFortBroadcastRemoteClientInfo)
};

class AFortPlayerControllerAthena : public AFortPlayerController
{
public:
	DEFINE_PTR(AFortBroadcastRemoteClientInfo, AFortPlayerControllerAthena, BroadcastRemoteClientInfo)
	DEFINE_PTR(AActor, AFortPlayerControllerAthena, WarmupPlayerStart)
	DEFINE_MEMBER(TArray<struct FFortUpdatedObjectiveStat>, AFortPlayerController, UpdatedObjectiveStats)
	DEFINE_PTR(class UFortPlayerControllerAthenaXPComponent, AFortPlayerControllerAthena, XPComponent)
	DEFINE_PTR(UAthenaPlayerMatchReport, AFortPlayerControllerAthena, MatchReport)
	DEFINE_BOOL(AFortPlayerControllerAthena, bMarkedAlive)
	DEFINE_PTR(class AFortVolume, AFortPlayerControllerAthena, CreativePlotLinkedVolume);
	DEFINE_PTR(AFortAthenaCreativePortal, AFortPlayerControllerAthena, OwnedPortal);

public:
	void ServerTeleportToPlaygroundLobbyIsland(FFrame &Stack);
	void ServerClientIsReadyToRespawn(FFrame &Stack);
	void ServerGiveCreativeItem(FFrame &Stack);
	void ServerAcknowledgePossession(FFrame &Stack);
	void ServerPlaySquadQuickChatMessage(__int64 ChatEntry, __int64 SenderID);
	DefineUOriginal(void, ClientOnPawnDied, AFortPlayerControllerAthena *, FFortPlayerDeathReport &DeathReport);
	bool ServerRemoveInventoryItem(FGuid ItemGuid, int Count, bool bForceRemoveFromQuickBars, bool bForceRemoval);

	void ClientSendEndBattleRoyaleMatchForPlayer(bool bSuccess, const FAthenaRewardResult Result)
	{
		this->CallFunc<void>("FortPlayerControllerAthena", "ClientSendEndBattleRoyaleMatchForPlayer", bSuccess, Result);
	}

	void ClientStartRespawnPreparation(FVector RespawnLoc, FRotator RespawnRot, float RespawnCameraDist, FName InRespawnCameraBehavior, FText HUDReasonText)
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "ClientStartRespawnPreparation").Func;
		if (!Func)
			return;
		struct
		{
			FVector RespawnLoc;
			FRotator RespawnRot;
			float RespawnCameraDist;
			FName InRespawnCameraBehavior;
			FText HUDReasonText;
		} Params{RespawnLoc, RespawnRot, RespawnCameraDist, InRespawnCameraBehavior, HUDReasonText};
		this->ProcessEvent(Func, &Params);
	}
	AFortVolume *GetCurrentVolume()
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "GetCurrentVolume").Func;
		if (!Func)
			return nullptr;
		AFortVolume *ret;
		this->ProcessEvent(Func, &ret);
		return ret;
	}

	void ServerSetTeam(uint8 InTeam)
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "ServerSetTeam").Func;
		if (!Func)
			return;

		struct FortPlayerControllerAthena_ServerSetTeam final
		{
		public:
			uint8 InTeam; // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		} Params{InTeam};

		this->ProcessEvent(Func, &Params);
	}

	void OnRep_CreativePlotLinkedVolume()
	{
		static class UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "OnRep_CreativePlotLinkedVolume");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return

				this->ProcessEvent(Func, nullptr);
	}

	void OnRep_UpdatedObjectiveStats()
	{
		static class UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerController", "OnRep_UpdatedObjectiveStats");
		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return;

		this->ProcessEvent(Func, nullptr);
	}

	void FLUXBroadcastMatchResults(FAthenaRewardResult &Result);

	void ClientSendMatchStatsForPlayer(const FAthenaMatchStats &Result)
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "ClientSendEndBattleRoyaleMatchForPlayer").Func;
		if (!Func)
			return;
		struct
		{
			FAthenaMatchStats TeamStats;
		} Params{Result};
		this->ProcessEvent(Func, &Params);
	}

	void ClientReportTournamentPlacementPointsScored(int32 Placement, int32 PointsEarned)
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "ClientReportTournamentPlacementPointsScored").Func;
		if (!Func)
			return;
		struct
		{
			int32 Placement;
			int32 PointsEarned;
		} Params{Placement, PointsEarned};
		this->ProcessEvent(Func, &Params);
	}

	void ClientSendTeamStatsForPlayer(const FAthenaMatchTeamStats &Result)
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "ClientSendTeamStatsForPlayer").Func;
		if (!Func)
			return;
		struct
		{
			FAthenaMatchTeamStats TeamStats;
		} Params{Result};
		this->ProcessEvent(Func, &Params);
	}

	bool IsInAircraft()
	{
		static SDK::UFunction *Func = nullptr;
		if (!Func)
			Func = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthena", "IsInAircraft").Func;
		if (!Func)
			return false;

		struct FortPlayerControllerAthena_IsInAircraft final
		{
		public:
			bool                                          ReturnValue;                                       // 0x0000(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		} Params{};
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}
public:
	DECLARE_STATIC_CLASS(AFortPlayerControllerAthena)
	DECLARE_DEFAULT_OBJECT(AFortPlayerControllerAthena)
};