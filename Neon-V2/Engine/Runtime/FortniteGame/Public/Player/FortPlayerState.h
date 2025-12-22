#pragma once
#include "pch.h"

#include "FortPlayerStateZone.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UFortHeroType : public UObject
{
public:
    DEFINE_MEMBER(TArray<TSoftObjectPtr<class UFortHeroSpecialization>>, UFortHeroType, Specializations);
public:
    DECLARE_STATIC_CLASS(UFortHeroType)
    DECLARE_DEFAULT_OBJECT(UFortHeroType)
};

enum class EFortCustomPartType : uint8
{
    Head                                     = 0,
    Body                                     = 1,
    Hat                                      = 2,
    Backpack                                 = 3,
    Charm                                    = 4,
    Face                                     = 5,
    NumTypes                                 = 6,
    EFortCustomPartType_MAX                  = 7,
};

class UCustomCharacterPart : public UObject
{
public:
    DECLARE_STATIC_CLASS(UCustomCharacterPart)
    DECLARE_DEFAULT_OBJECT(UCustomCharacterPart)
public:
    DEFINE_MEMBER(EFortCustomPartType, UCustomCharacterPart, CharacterPartType)
};

class UFortHeroSpecialization : public UObject
{
public:
    DEFINE_MEMBER(TArray<TSoftObjectPtr<UCustomCharacterPart>>, UFortHeroSpecialization, CharacterParts);
public:
    DECLARE_STATIC_CLASS(UFortHeroSpecialization)
    DECLARE_DEFAULT_OBJECT(UFortHeroSpecialization)
};

struct FDeathInfo final
{
public:
	DEFINE_PTR(AActor, FDeathInfo, FinisherOrDowner); // 0x0000(0x0008)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	DEFINE_MEMBER(bool, FDeathInfo, bDBNO); // 0x0010(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	DEFINE_MEMBER(uint8, FDeathInfo, DeathCause); // 0x0011(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	DEFINE_MEMBER(FGameplayTagContainer, FDeathInfo, DeathTags); // 0x0012(0x0020)(BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic)
	uint8                                         DeathClassSlot;                                    // 0x0012(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1B19[0x1];                                     // 0x0013(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
	DEFINE_MEMBER(float, FDeathInfo, Distance);
	float                                         Distance;                                          // 0x0014(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	DEFINE_MEMBER(FVector, FDeathInfo, DeathLocation); // 0x0018(0x000C)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	DEFINE_BOOL(FDeathInfo, bInitialized)
	uint8                                         Pad_1B1A[0x3];                                     // 0x0025(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
	uint8                                         Pad_1B1B[0x8];                                     // 0x0048(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FFortRespawnData final
{
public:
    bool                                          bRespawnDataAvailable;                             // 0x0000(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    bool                                          bClientIsReady;                                    // 0x0001(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    bool                                          bServerIsReady;                                    // 0x0002(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8                                         Pad_1820[0x1];                                     // 0x0003(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
    struct FVector                                RespawnLocation;                                   // 0x0004(0x000C)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    struct FRotator                               RespawnRotation;                                   // 0x0010(0x000C)(ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
    float                                         RespawnCameraDistance;                             // 0x001C(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

enum class ETeamMemberState : uint8
{
    None = 0,
    FIRST_CHAT_MESSAGE = 1,
    NeedAmmoHeavy = 1,
    NeedAmmoLight = 2,
    NeedAmmoMedium = 3,
    NeedAmmoShells = 4,
    NeedAmmoRocket = 5,
    ChatBubble = 6,
    EnemySpotted = 7,
    NeedBandages = 8,
    NeedMaterials = 9,
    NeedShields = 10,
    NeedWeapon = 11,
    LAST_CHAT_MESSAGE = 11,
    MAX = 12,
};


class AFortPlayerStateAthena : public AFortPlayerStateZone
{
public:
    void OnRep_SquadId() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "OnRep_SquadId").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }

    void OnRep_TeamKillScore() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "OnRep_TeamKillScore").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }

    void OnRep_KillScore() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "OnRep_KillScore").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }

    void OnRep_Place() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "OnRep_Place").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }

    void ClientReportKill(const AFortPlayerStateAthena* Player) {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "ClientReportKill").Func;
        if (!Func) return;
        struct { const AFortPlayerStateAthena* Player; } Params{Player};
        this->ProcessEvent(Func, &Params);
    }

    inline uint8 ToDeathCause(const FGameplayTagContainer& InTags, bool bWasDBNO) {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "ToDeathCause").Func;
        if (!Func)
        {
            static auto ToDeathCause = UKismetMemLibrary::Get<uint8(*)(const FGameplayTagContainer& DeathTags, bool bWasDBNO)>(L"ToDeathCause");

            return ToDeathCause(InTags, bWasDBNO);
        }
        struct { FGameplayTagContainer InTags; bool bWasDBNO; uint8 ReturnValue; uint8 Pad[6]; } Params{InTags, bWasDBNO};
        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }

    void OnRep_DeathInfo() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "OnRep_DeathInfo").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }

    void ClientReportTeamKill(int32 TeamKills) {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "ClientReportTeamKill").Func;
        if (!Func) return;
        struct { int32 TeamKills; } Params{TeamKills};
        this->ProcessEvent(Func, &Params);
    }

    void OnRep_ReplicatedTeamMemberState() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("FortPlayerStateAthena", "OnRep_ReplicatedTeamMemberState").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }
public:
    DefineUOriginal(bool, Server_SetCanEditCreativeIsland, AFortPlayerStateAthena*, bool bCanEdit, TArray<class FString> WhiteList);
public:
    DEFINE_MEMBER(FFortRespawnData, AFortPlayerStateAthena, RespawnData);
    DEFINE_MEMBER(FDeathInfo, AFortPlayerStateAthena, DeathInfo);
    DEFINE_MEMBER(int32, AFortPlayerStateAthena, RebootCounter);
    DEFINE_MEMBER(uint8, AFortPlayerStateAthena, TeamIndex);
    DEFINE_MEMBER(uint8, AFortPlayerStateAthena, SquadId);
    DEFINE_MEMBER(int32, AFortPlayerStateAthena, Place);
    DEFINE_MEMBER(int32, AFortPlayerStateAthena, KillScore);
    DEFINE_MEMBER(ETeamMemberState, AFortPlayerStateAthena, ReplicatedTeamMemberState);
    DEFINE_MEMBER(ETeamMemberState, AFortPlayerStateAthena, TeamMemberState);
    DEFINE_MEMBER(int32, AFortPlayerStateAthena, TeamKillScore);
    DEFINE_PTR(UFortHeroType, AFortPlayerStateAthena, HeroType);
public:
    DECLARE_STATIC_CLASS(AFortPlayerStateAthena)
    DECLARE_DEFAULT_OBJECT(AFortPlayerStateAthena)
};