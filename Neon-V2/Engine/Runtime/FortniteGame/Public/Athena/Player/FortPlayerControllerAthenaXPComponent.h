#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Internationalization/Text.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/WeaponItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerState.h"

struct FPrimaryAssetType final
{
    FName Name;
};
struct FPrimaryAssetId final
{
    FPrimaryAssetType PrimaryAssetType;
    FName PrimaryAssetName;
};

struct FXPEventEntry : public FFastArraySerializerItem
{
    uint8 Pad_4665[0x4];
    FText SimulatedXpEvent;
    UObject *QuestDef;
    FPrimaryAssetId Accolade;
    float Time;
    int32 EventXpValue;
    int32 TotalXpEarnedInMatch;
    uint8 Pad_4666[0x4];
};

enum class EXPEventPriorityType : uint8
{
    NearReticle = 0,
    XPBarOnly = 1,
    TopCenter = 2,
    Feed = 3,
    EXPEventPriorityType_MAX = 4
};

struct FXPEventInfo final
{
    FName EventName;
    FText SimulatedText;
    UObject *QuestDef;
    EXPEventPriorityType Priority;
    uint8 Pad_29[0x3];
    int32 EventXpValue;
    int32 TotalXpEarnedInMatch;
    FPrimaryAssetId Accolade;
    int32 RestedValuePortion;
    int32 SeasonBoostValuePortion;
    int32 RestedXPRemaining;
};

struct FPlaylistData final
{
    FName PlaylistName;
    std::string TournamentId;
    std::string EventWindowId;
    FString RegionId;
};

struct FAthenaAccolades final
{
    UObject *AccoladeDef;
    FString TemplateId;
    int32 Count;
    uint8 Pad_1C[0x4];
};

class UFortPlayerControllerAthenaXPComponent : public UObject
{
public:
    void OnXPEvent(const FXPEventInfo &Param_HighPrioXPEvent)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthenaXPComponent", "OnXPEvent");
        if (!Func)
            Func = Info.Func;
        if (!Func)
            return;
        struct
        {
            FXPEventInfo XPEvent;
        } Params{};
        Params.XPEvent = Param_HighPrioXPEvent;
        this->ProcessEvent(Func, &Params);
    }

    void HighPrioXPEvent(const FXPEventEntry &Param_HighPrioXPEvent)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthenaXPComponent", "HighPrioXPEvent");
        if (!Func)
            Func = Info.Func;
        if (!Func)
            return;
        struct
        {
            FXPEventEntry HighPrioXPEvent;
        } params{Param_HighPrioXPEvent};
        this->ProcessEvent(Func, &params);
    }

    void OnRep_bRegisteredWithQuestManager()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerControllerAthenaXPComponent", "OnRep_bRegisteredWithQuestManager");
        if (!Func)
            Func = Info.Func;
        if (!Func)
            return;
        this->ProcessEvent(Func, nullptr);
    }

    DEFINE_MEMBER(TArray<FAthenaAccolades>, UFortPlayerControllerAthenaXPComponent, PlayerAccolades);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, CombatXp);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, SurvivalXp);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, MedalBonusXP);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, ChallengeXp);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, MatchXp);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, TotalXpEarned);
    DEFINE_MEMBER(int64, UFortPlayerControllerAthenaXPComponent, InMatchProfileVer);
    DEFINE_MEMBER(int32, UFortPlayerControllerAthenaXPComponent, RestXP);
    DEFINE_MEMBER(TArray<FXPEventEntry>, UFortPlayerControllerAthenaXPComponent, WaitingQuestXp);

    DECLARE_STATIC_CLASS(UFortPlayerControllerAthenaXPComponent)
    DECLARE_DEFAULT_OBJECT(UFortPlayerControllerAthenaXPComponent)
};
