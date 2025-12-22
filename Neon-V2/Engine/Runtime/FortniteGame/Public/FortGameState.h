#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Plugins/Kismet/Public/DataTableFunctionLibrary.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/Engine/Core/Public/Misc/Guid.h"

class AFortMissionManager : public AActor
{
public:
public:
	DECLARE_DEFAULT_OBJECT(AFortMissionManager)
	DECLARE_STATIC_CLASS(AFortMissionManager)
};

enum class EFortWorldManagerState : uint8
{
	WMS_Created = 0,
	WMS_QueryingWorld = 1,
	WMS_WorldQueryComplete = 2,
	WMS_CreatingNewWorld = 3,
	WMS_LoadingExistingWorld = 4,
	WMS_Running = 5,
	WMS_Failed = 6,
	WMS_MAX = 7,
};

struct FFortGeneratedDifficultyOptions final
{
public:
	float GameDifficultyAtGeneration;											// 0x0000(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 DifficultyOptionPointsAvailableAtGeneration;							// 0x0004(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 MaxEncounterSpawnPointsAtGeneration;									// 0x0008(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 MinDifficultyOptionPointsToUse;										// 0x000C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<struct FFortGeneratedEncounterSequence> GeneratedEncounterSequences; // 0x0010(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
	TArray<struct FFortGeneratedMissionOption> MissionOptions;					// 0x0020(0x0010)(ZeroConstructor, Protected, NativeAccessSpecifierProtected)
	TArray<struct FFortGeneratedEncounterProfile> GeneratedEncounterProfiles;	// 0x0030(0x0010)(ZeroConstructor, Protected, NativeAccessSpecifierProtected)
};

enum class EMissionGenerationCategory : uint8
{
	Primary = 0,
	Secondary = 1,
	Tertiary = 2,
	Survivor = 3,
	Max_None = 4,
	EMissionGenerationCategory_MAX = 5,
};

class UFortMissionGenerator : public UObject
{
public:
	DECLARE_DEFAULT_OBJECT(UFortMissionGenerator);
	DECLARE_STATIC_CLASS(UFortMissionGenerator)
};

struct FFortMissionEntry final
{
public:
	float Weight;															// 0x0000(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 WorldMinLevel;													// 0x0004(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 WorldMaxLevel;													// 0x0008(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_C[0x4];														// 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	struct FDataTableRowHandle MinDifficultyInfoRow;						// 0x0010(0x0010)(Edit, NoDestructor, NativeAccessSpecifierPublic)
	class UFortMissionGenerator *MissionGenerator;							// 0x0020(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UFortMissionInfo *MissionInfo;									// 0x0028(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	EMissionGenerationCategory GenerationCategory;							// 0x0030(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_31[0x7];														// 0x0031(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	struct FFortGeneratedDifficultyOptions GeneratedDifficultyOptions;		// 0x0038(0x0040)(NativeAccessSpecifierPublic)
	TArray<struct FFortMissionPlacementItemLookupData> BlueprintLookupData; // 0x0078(0x0010)(ZeroConstructor, Transient, NativeAccessSpecifierPrivate)
};

enum class EFortMissionStatus : uint8
{
	Created = 0,
	InProgress = 1,
	Succeeded = 2,
	Failed = 3,
	NeutralCompletion = 4,
	Quit = 5,
	Max_None = 6,
	EFortMissionStatus_MAX = 7,
};

struct FFortMissionRecord final
{
public:
	FFortMissionEntry MissionEntry;							 // 0x0000(0x0088)(NativeAccessSpecifierPublic)
	UFortMissionGenerator *MissionGenerator;				 // 0x0088(0x0008)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FFortGeneratedDifficultyOptions GeneratedMissionOptions; // 0x0090(0x0040)(Transient, NativeAccessSpecifierPublic)
	int32 DayGenerated;										 // 0x00D0(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 UIIndex;											 // 0x00D4(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FUniqueNetIdRepl QuestOwnerAccount;						 // 0x00D8(0x0028)(HasGetValueTypeHash, NativeAccessSpecifierPublic)
	EFortMissionStatus MissionStatus;						 // 0x0100(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_101[0x7];										 // 0x0101(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	TArray<struct FFortObjectiveRecord> ObjectiveRecords;	 // 0x0108(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
	TArray<uint8> MissionData;								 // 0x0118(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
	FGuid MissionGuid;										 // 0x0128(0x0010)(IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
};

struct FFortMissionManagerRecord final
{
public:
	TSubclassOf<class AFortMissionManager> MissionManagerClass; // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<struct FFortMissionRecord> MissionRecords;			// 0x0008(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
	int32 NumRequiredMissionsOfType[0x4];						// 0x0018(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<uint8> MissionManagerData;							// 0x0028(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
};

struct FMcpLootResult final
{
public:
	class FString TierGroupName;		// 0x0000(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<struct FMcpLootEntry> Items; // 0x0010(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
};

struct FFortZoneInstanceInfo
{
public:
	class FString WorldId;						// 0x0000(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString TheaterId;					// 0x0010(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString TheaterMissionId;				// 0x0020(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString TheaterMissionAlertId;		// 0x0030(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TSoftClassPtr<class UClass> ZoneThemeClass; // 0x0040(0x0028)(UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

struct FFortZoneInstanceDetails final : public FFortZoneInstanceInfo
{
public:
	TSoftClassPtr<class UClass> MissionGenerator;					// 0x0068(0x0028)(UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FDataTableRowHandle ZoneDifficultyInfo;					// 0x0090(0x0010)(NoDestructor, NativeAccessSpecifierPublic)
	struct FMcpLootResult MissionRewards;							// 0x00A0(0x0020)(NativeAccessSpecifierPublic)
	TArray<struct FMcpLootResult> DifficultyIncreaseMissionRewards; // 0x00C0(0x0010)(ZeroConstructor, NativeAccessSpecifierPublic)
	struct FMcpLootResult MissionAlertRewards;						// 0x00D0(0x0020)(NativeAccessSpecifierPublic)
	struct FMcpLootResult ZoneModifiers;							// 0x00F0(0x0020)(NativeAccessSpecifierPublic)
	class FString MissionAlertCategoryName;							// 0x0110(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 TileIndex;												// 0x0120(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 Pad_124[0x4];												// 0x0124(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UFortWorldRecord : public UObject
{
public:
	DEFINE_MEMBER(FFortZoneInstanceDetails, UFortWorldRecord, ZoneInstanceInfo)

public:
	DECLARE_DEFAULT_OBJECT(UFortWorldRecord);
	DECLARE_STATIC_CLASS(UFortWorldRecord)
};

class AFortWorldManager : public AActor
{
public:
	DEFINE_MEMBER(TArray<FFortMissionEntry>, AFortWorldManager, ReactiveMissionsToAdd);
	DEFINE_MEMBER(FFortMissionManagerRecord, AFortWorldManager, MissionManagerRecord);
	DEFINE_MEMBER(EFortWorldManagerState, AFortWorldManager, WorldManagerState);
	DEFINE_PTR(UFortWorldRecord, AFortWorldManager, CurrentWorldRecord);

	DefineUOriginal(void *, GetCurrentTheaterMapData, AFortWorldManager *);

public:
	DECLARE_DEFAULT_OBJECT(AFortWorldManager)
	DECLARE_STATIC_CLASS(AFortWorldManager)
};

class AFortPoiManager : public AActor
{
public:
	DECLARE_STATIC_CLASS(AFortPoiManager)
	DECLARE_DEFAULT_OBJECT(AFortPoiManager)
};

class AFortGameState : public AActor
{
public:
	DECLARE_DEFAULT_OBJECT(AFortGameState)
	DECLARE_STATIC_CLASS(AFortGameState)
public:
	DEFINE_PTR(AFortPoiManager, AFortGameState, PoiManager);
	DEFINE_BOOL(AFortGameState, bDBNOEnabledForGameMode)
	DEFINE_MEMBER(int32, AFortGameState, WorldLevel);
	DEFINE_PTR(class AFortMissionManager, AFortGameState, MissionManager);
	DEFINE_PTR(AFortWorldManager, AFortGameState, WorldManager);

public:
	void OnRep_AdditionalPlaylistLevelsStreamed()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameState", "OnRep_AdditionalPlaylistLevelsStreamed");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return;

		this->ProcessEvent(Func, nullptr);
	}

	void OnRep_MissionManager()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameState", "OnRep_MissionManager");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return;

		this->ProcessEvent(Func, nullptr);
	}

	void OnFinishedStreamingAdditionalPlaylistLevel()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameState", "OnFinishedStreamingAdditionalPlaylistLevel");

		if (Func == nullptr)
			Func = Info.Func;
		if (!Func)
			return;

		this->ProcessEvent(Func, nullptr);
	}
};