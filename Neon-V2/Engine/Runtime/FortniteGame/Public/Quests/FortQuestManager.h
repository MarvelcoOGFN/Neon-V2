#pragma once
#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortAccoladeItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/FortQuestItemDefinition.h"

struct FFluxBroadcastQuestProgress
{
	std::string BackendName;
	int32 Count;
};

enum class EFortQuestObjectiveStatEvent : uint8
{
	Kill                                     = 0,
	TeamKill                                 = 1,
	KillContribution                         = 2,
	Damage                                   = 3,
	Damage_OLD                               = 4,
	Visit                                    = 5,
	Land                                     = 6,
	Emote                                    = 7,
	Spray                                    = 8,
	Toy                                      = 9,
	Build                                    = 10,
	BuildingEdit                             = 11,
	BuildingRepair                           = 12,
	BuildingUpgrade                          = 13,
	PlaceTrap                                = 14,
	Complete                                 = 15,
	Craft                                    = 16,
	Collect                                  = 17,
	Win                                      = 18,
	Interact                                 = 19,
	TeamInteract                             = 20,
	Destroy                                  = 21,
	Ability                                  = 22,
	WaveComplete                             = 23,
	Custom                                   = 24,
	ComplexCustom                            = 25,
	Client                                   = 26,
	AthenaRank                               = 27,
	AthenaOutlive                            = 28,
	RevivePlayer                             = 29,
	Heal                                     = 30,
	EarnVehicleTrickPoints                   = 31,
	VehicleAirTime                           = 32,
	TimeElapsed                              = 33,
	NumGameplayEvents                        = 34,
	Acquire                                  = 35,
	Consume                                  = 36,
	OpenCardPack                             = 37,
	PurchaseCardPack                         = 38,
	Convert                                  = 39,
	Upgrade                                  = 40,
	UpgradeRarity                            = 41,
	QuestComplete                            = 42,
	AssignWorker                             = 43,
	CollectExpedition                        = 44,
	CollectSuccessfulExpedition              = 45,
	LevelUpCollectionBook                    = 46,
	LevelUpAthenaSeason                      = 47,
	LevelUpBattlePass                        = 48,
	GainAthenaSeasonXp                       = 49,
	HasItem                                  = 50,
	HasAccumulatedItem                       = 51,
	SlotInCollection                         = 52,
	AlterationRespec                         = 53,
	AlterationUpgrade                        = 54,
	HasCompletedQuest                        = 55,
	HasAssignedWorker                        = 56,
	HasUpgraded                              = 57,
	HasConverted                             = 58,
	HasUpgradedRarity                        = 59,
	HasLeveledUpCollectionBook               = 60,
	SlotHeroInLoadout                        = 61,
	HasLeveledUpAthenaSeason                 = 62,
	HasLeveledUpBattlePass                   = 63,
	HasGainedAthenaSeasonXp                  = 64,
	MinigameDynamicEvent                     = 65,
	MinigameComplete                         = 66,
	MinigameDeath                            = 67,
	MinigameAssist                           = 68,
	Max_None                                 = 69,
	EFortQuestObjectiveStatEvent_MAX         = 70,
};

struct FNexaBroadcastQuestProgress
{
	std::string BackendName;
	int32 Count;
};

enum class EInlineObjectiveStatTagCheckEntryType : uint8
{
	Target = 0,
	Source = 1,
	Context = 2,
	EInlineObjectiveStatTagCheckEntryType_MAX = 3,
};

struct alignas(0x08) FTableRowBase
{
public:
	uint8 Pad_0[0x8];
};

struct FFortQuestObjectiveStatXPTableRow final : public FTableRowBase
{
	DEFINE_MEMBER(EFortQuestObjectiveStatEvent, FFortQuestObjectiveStatXPTableRow, Type)
	DEFINE_MEMBER(FGameplayTagContainer, FFortQuestObjectiveStatXPTableRow, SourceTags)
	DEFINE_MEMBER(FGameplayTagContainer, FFortQuestObjectiveStatXPTableRow, TargetTags)
	DEFINE_MEMBER(FGameplayTagContainer, FFortQuestObjectiveStatXPTableRow, ContextTags)
};

struct FInlineObjectiveStatTagCheckEntry final
{
	DEFINE_MEMBER(EInlineObjectiveStatTagCheckEntryType, FInlineObjectiveStatTagCheckEntry, Type)
	DEFINE_BOOL(FInlineObjectiveStatTagCheckEntry, Require)
	DEFINE_MEMBER(FGameplayTag, FInlineObjectiveStatTagCheckEntry, Tag)
};

struct FFortQuestObjectiveStatTableRow final : public FTableRowBase
{
public:
	EFortQuestObjectiveStatEvent                  Type;                                              // 0x0008(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_182E[0x7];                                     // 0x0009(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	class FString                                 TargetTags;                                        // 0x0010(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString                                 SourceTags;                                        // 0x0020(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString                                 ContextTags;                                       // 0x0030(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString                                 Condition;                                         // 0x0040(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FString                                 TemplateId;                                        // 0x0050(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<class FString>                         AlternateTemplateIds;                              // 0x0060(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  TargetTagContainer;                                // 0x0070(0x0020)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  SourceTagContainer;                                // 0x0090(0x0020)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  ContextTagContainer;                               // 0x00B0(0x0020)(BlueprintVisible, BlueprintReadOnly, Transient, NativeAccessSpecifierPublic)
	bool                                          bIsCached;                                         // 0x00D0(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_182F[0x17];                                    // 0x00D1(0x0017)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

enum class EFortSimulatedXPSize : uint8
{
	None = 0,
	VerySmall = 1,
	Small = 2,
	Medium = 3,
	Large = 4,
	VeryLarge = 5,
	EFortSimulatedXPSize_MAX = 6,
};

struct FFortQuestObjectiveStat
{
	DEFINE_MEMBER(TArray<struct FInlineObjectiveStatTagCheckEntry>, FFortQuestObjectiveStat, TagConditions)
	DEFINE_MEMBER(EFortQuestObjectiveStatEvent, FFortQuestObjectiveStat, Type)
	DEFINE_BOOL(FFortQuestObjectiveStat, bHasInclusiveTargetTags)
	DEFINE_BOOL(FFortQuestObjectiveStat, bHasInclusiveSourceTags)
	DEFINE_BOOL(FFortQuestObjectiveStat, bHasInclusiveContextTags)
};

struct FFortQuestRewardTableRow final : public FTableRowBase
{
public:
	DEFINE_MEMBER(int32, FFortQuestRewardTableRow, Quantity)
	FString QuestTemplateId;
	FName TemplateId;
	bool Hidden;
	bool Feature;
	bool Selectable;
};

class UFortQuestObjectiveInfo : public UObject
{
public:
	DEFINE_MEMBER(EFortQuestObjectiveStatEvent, UFortQuestObjectiveInfo, StatEvent)
public:
	DECLARE_STATIC_CLASS(UFortQuestObjectiveInfo)
	DECLARE_DEFAULT_OBJECT(UFortQuestObjectiveInfo)
};

class UFortQuestItem : public UFortItem
{
public:
	inline bool HasCompletedObjective(const FDataTableRowHandle &ObjectiveStatHandle)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestItem", "HasCompletedObjective");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return false;
		struct
		{
			FDataTableRowHandle ObjectiveStatHandle;
			bool ReturnValue;
		} Params{};
		Params.ObjectiveStatHandle = ObjectiveStatHandle;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline bool HasCompletedObjectiveWithName(FName BackendName)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestItem", "HasCompletedObjectiveWithName");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return false;
		struct
		{
			FName BackendName;
			bool ReturnValue;
		} Params{};
		Params.BackendName = BackendName;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline bool HasCompletedQuest()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestItem", "HasCompletedQuest");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return false;
		struct
		{
			bool ReturnValue;
		} Params{};
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline UFortQuestObjectiveInfo *GetObjectiveInfo(FDataTableRowHandle &ObjectiveStatHandle)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestItem", "GetObjectiveInfo");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return nullptr;
		struct FortQuestItem_GetObjectiveInfo final
		{
		public:
			struct FDataTableRowHandle ObjectiveStatHandle; // 0x0000(0x0010)(Parm, NoDestructor, NativeAccessSpecifierPublic)
			class UFortQuestObjectiveInfo *ReturnValue;		// 0x0010(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		} Params{};

		Params.ObjectiveStatHandle = ObjectiveStatHandle;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline UFortQuestItemDefinition *GetQuestDefinitionBP()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestItem", "GetQuestDefinitionBP");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return nullptr;
		struct
		{
			UFortQuestItemDefinition *ReturnValue;
		} Params{};
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

public:
	DECLARE_STATIC_CLASS(UFortQuestItem)
	DECLARE_DEFAULT_OBJECT(UFortQuestItem)
};

class UFortMinigameManager : public UObject
{
public:
	DEFINE_PTR(AFortPlayerControllerAthena, UFortMinigameManager, PlayerController);
	DefineUOriginal(void, SendStatEventWithTags,
				UFortMinigameManager *,
				EFortQuestObjectiveStatEvent Type,
				UObject *TargetObject,
				FGameplayTagContainer &TargetTags,
				FGameplayTagContainer &SourceTags,
				FGameplayTagContainer &ContextTags,
				int Count)
};

class UFortQuestManager final : public UObject
{
public:
	inline AFortPlayerController *GetPlayerControllerBP()
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "GetPlayerControllerBP");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return nullptr;
		struct
		{
			AFortPlayerController *ReturnValue;
		} Params{};
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline int32 GetObjectiveCompletionCount(const UFortQuestItemDefinition *Definition, FName BackendName)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "GetObjectiveCompletionCount");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return 0;
		struct
		{
			const UFortQuestItemDefinition *Definition;
			FName BackendName;
			int32 ReturnValue;
		} Params{};
		Params.Definition = Definition;
		Params.BackendName = BackendName;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline bool HasCompletedObjective(const UFortQuestItemDefinition *Definition, const FDataTableRowHandle &ObjectiveStatHandle)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "HasCompletedObjective");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return false;
		struct
		{
			const UFortQuestItemDefinition *Definition;
			FDataTableRowHandle ObjectiveStatHandle;
			bool ReturnValue;
		} Params{};
		Params.Definition = Definition;
		Params.ObjectiveStatHandle = ObjectiveStatHandle;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	void InitializeQuestAbilities(APawn *Pawn)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "InitializeQuestAbilities");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return;
		struct
		{
			APawn *PlayerPawn;
		} Params{Pawn};
		this->ProcessEvent(Func, &Params);
	}

	inline bool HasCompletedQuest(const UFortQuestItemDefinition *Definition)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "HasCompletedQuest");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return false;
		struct
		{
			const UFortQuestItemDefinition *Definition;
			bool ReturnValue;
		} Params{};
		Params.Definition = Definition;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline bool HasCompletedObjectiveWithName(const UFortQuestItemDefinition *Definition, FName BackendName)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "HasCompletedObjectiveWithName");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return false;
		struct
		{
			const UFortQuestItemDefinition *Definition;
			FName BackendName;
			bool ReturnValue;
		} Params{};
		Params.Definition = Definition;
		Params.BackendName = BackendName;
		this->ProcessEvent(Func, &Params);
		return Params.ReturnValue;
	}

	inline void GetSourceAndContextTags(FGameplayTagContainer *OutSourceTags, FGameplayTagContainer *OutContextTags)
	{
		static SDK::UFunction *Func = nullptr;
		static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortQuestManager", "GetSourceAndContextTags");
		if (!Func)
			Func = Info.Func;
		if (!Func)
			return;
		struct
		{
			FGameplayTagContainer OutSourceTags;
			FGameplayTagContainer OutContextTags;
		} Params{};
		this->ProcessEvent(Func, &Params);
		*OutContextTags = Params.OutContextTags;
		*OutSourceTags = Params.OutSourceTags;
	}

	static std::vector<FFluxBroadcastQuestProgress> GetFluxBroadcastQuestProgress(AFortPlayerControllerAthena *PlayerController);
	void GiveAccolade(AFortPlayerControllerAthena *PlayerController, UFortAccoladeItemDefinition *Accolade);

	void SendStatEvent(FGameplayTagContainer &SourceTags, FGameplayTagContainer &TargetTags, bool *QuestActive, bool *QuestCompleted, int32 Count, EFortQuestObjectiveStatEvent StatEvent, FGameplayTagContainer &ContextTags);
DefineUOriginal(void, SendComplexCustomStatEvent, UFortQuestManager *, UObject *TargetObj, FGameplayTagContainer &SourceTags, FGameplayTagContainer &TargetTags, bool *QuestActive, bool *QuestCompleted, int32 Count)
		DefineUOriginal(void, SendCustomStatEvent, UFortQuestManager *, struct FDataTableRowHandle &ObjectiveStat, int32 Count, bool bForceFlush) public : DEFINE_MEMBER(TArray<UFortQuestItem *>, UFortQuestManager, CurrentQuests)
																																							   DECLARE_STATIC_CLASS(UFortQuestManager)
																																								   DECLARE_DEFAULT_OBJECT(UFortQuestManager)
};