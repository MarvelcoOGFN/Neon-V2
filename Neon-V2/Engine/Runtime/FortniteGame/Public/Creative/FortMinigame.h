#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/FortVolume.h"
#include "Engine/Runtime/Engine/Core/Public/Internationalization/Text.h"
#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"
//#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"
#include "Engine/Runtime/Engine/Classes/World.h"


enum class EFortMinigameState : uint8
{
	PreGame                                  = 0,
	Setup                                    = 1,
	Transitioning                            = 2,
	WaitingForCameras                        = 3,
	Warmup                                   = 4,
	InProgress                               = 5,
	PostRoundEnd                             = 6,
	PostGameEnd                              = 7,
	PostGameAbandon                          = 8,
	PostGameReset                            = 9,
	EFortMinigameState_MAX                   = 10,
};

	struct FMinigamePlayers final : public FFastArraySerializer
	{
	public:
		TArray<struct FMinigamePlayer>                Players;                                           // 0x0108(0x0010)(Edit, ZeroConstructor, DisableEditOnTemplate, Transient, EditConst, NativeAccessSpecifierPublic)
		class AFortMinigame*                          Owner;                                             // 0x0118(0x0008)(ZeroConstructor, Transient, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};
	
	struct FMinigamePlayer final : public FFastArraySerializerItem
{
public:
	uint8                                         Pad_15FB[0x4];                                     // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	struct FUniqueNetIdRepl                       UniquePlayerId;                                    // 0x0010(0x0028)(Edit, BlueprintVisible, BlueprintReadOnly, DisableEditOnTemplate, EditConst, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                          bHasValidResetData;                                // 0x0038(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         TeamBeforeMinigameStarted;                         // 0x0039(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_15FC[0x2];                                     // 0x003A(0x0002)(Fixing Size After Last Property [ Dumper-7 ])
	struct FVector                                LocationBeforeMinigameStarted;                     // 0x003C(0x000C)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FRotator                               RotationBeforeMinigameStarted;                     // 0x0048(0x000C)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	bool                                          bIsLocationBeforeGameStartedBlocked;               // 0x0054(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                          bWasSkydivingBeforeMinigameStarted;                // 0x0055(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                          bWasFlyingBeforeMinigameStarted;                   // 0x0056(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                          bIsTeleportingOrRespawningForGameplay;             // 0x0057(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                          bIsLocalPlayer;                                    // 0x0058(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_15FD[0x7];                                     // 0x0059(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	class AFortPlayerStartCreative*               ClaimedPlayerStart;                                // 0x0060(0x0008)(Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_15FE[0x8];                                     // 0x0068(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FFortMinigameStat final
{
public:
	TSubclassOf<class UObject>    Filter;                                            // 0x0000(0x0008)(Edit, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         Count;                                             // 0x0008(0x0004)(Edit, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_150F[0x4];                                     // 0x000C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FFortMinigameGroupStats
{
public:
	TArray<struct FFortMinigameStat>              Stats;                                             // 0x0000(0x0010)(Edit, ZeroConstructor, EditConst, NativeAccessSpecifierPublic)
};

struct FFortMinigamePlayerStats final : public FFortMinigameGroupStats
{
public:
	struct FUniqueNetIdRepl                       Player;                                            // 0x0010(0x0028)(Edit, EditConst, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

struct FFortMinigamePlayerBucketStats final : public FFortMinigameGroupStats
{
public:
	int32                                         BucketIndex;                                       // 0x0010(0x0004)(Edit, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_17EE[0x4];                                     // 0x0014(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FFortMinigameStatCollection final
{
public:
	struct FFortMinigameGroupStats                GroupStats;                                        // 0x0000(0x0010)(Edit, DisableEditOnTemplate, EditConst, NativeAccessSpecifierPrivate)
	TArray<struct FFortMinigamePlayerStats>       PlayerStats;                                       // 0x0010(0x0010)(Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, NativeAccessSpecifierPrivate)
	TArray<struct FFortMinigamePlayerBucketStats> PlayerBucketStats;                                 // 0x0020(0x0010)(Edit, ZeroConstructor, DisableEditOnTemplate, EditConst, NativeAccessSpecifierPrivate)
};

class AFortMinigame : public AActor
{
public:
	DefineUOriginal(void, ChangeMinigameState, AFortMinigame*, EFortMinigameState State);
public:
	void GetParticipatingPlayers(TArray<class AFortPlayerState*>& OutPlayers);
	bool DetermineSpawnLocation(AFortPlayerStateAthena* PlayerState, FVector* OutLocation, FRotator* OutRotation, bool* bOutRespawningOnGround);

	FText GetClientMinigameStartedText()
	{
		return UKismetStringLibrary::Conv_StringToText(L"Starting Game ");
	}
public:
	DEFINE_MEMBER(FFortMinigameStatCollection, AFortMinigame, RoundStats)
	DEFINE_MEMBER(FFortMinigameStatCollection, AFortMinigame, GameStats)
	DEFINE_MEMBER(FMinigamePlayers, AFortMinigame, SpectatingPlayerArray)
	DEFINE_MEMBER(FMinigamePlayers, AFortMinigame, PlayerArray)
	DEFINE_MEMBER(EFortMinigameState, AFortMinigame, CurrentState)
	DEFINE_MEMBER(int32, AFortMinigame, CurrentRound)
	DEFINE_MEMBER(int32, AFortMinigame, TotalRounds)
	DEFINE_MEMBER(uint8, AFortMinigame, WinningTeamIndex)
	DEFINE_MEMBER(float, AFortMinigame, RoundWinnerDisplayTime)
	DEFINE_MEMBER(float, AFortMinigame, RoundScoreDisplayTime)
	DEFINE_BOOL(AFortMinigame, bTeamMinigame)
	DEFINE_MEMBER(int32, AFortMinigame, NumTeams)
	DEFINE_MEMBER(FName, AFortMinigame, MinigameStartCameraBehavior)
	DEFINE_MEMBER(FName, AFortMinigame, MinigameEndCameraBehavior)
	DEFINE_MEMBER(TArray<int32>, AFortMinigame, RoundWinHistory)
	DEFINE_MEMBER(FText, AFortMinigame, ClientMinigameEndedText)
public:

	DECLARE_STATIC_CLASS(AFortMinigame)
	DECLARE_DEFAULT_OBJECT(AFortMinigame)
};