#pragma once
#include "pch.h"

#include "FortPlayerPawn.h"
#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Inventory/FortInventory.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/AthenaCharacterItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/ItemDefinitions/AthenaPickaxeItemDefinition.h"
#include "Engine/Runtime/FortniteGame/Public/Items/FortQuickBars.h"

#include <Engine/Plugins/Neon/Public/Neon.h>

#include "Engine/Plugins/Online/McpProfileSys/Public/Mcp/McpProfile.h"

struct FFortAthenaLoadout final
{
public:
    DEFINE_PTR(UAthenaPickaxeItemDefinition, FFortAthenaLoadout, Pickaxe);
    DEFINE_PTR(UAthenaCharacterItemDefinition, FFortAthenaLoadout, Character);
};

enum class EFortTeam : uint8
{
    Spectator = 0,
    HumanCampaign = 1,
    Monster = 2,
    HumanPvP_Team1 = 3,
    HumanPvP_Team2 = 4,
    MAX = 255,
};

struct FBuildingClassData
{
    UClass *BuildingClass;
    int PreviousBuildingLevel;
    int UpgradeLevel;
};

enum class ESubGame : uint8
{
    Campaign = 0,
    Athena = 1,
    Invalid = 2,
    Count = 2,
    ESubGame_MAX = 3,
};

class AFortPlayerController : public APlayerController
{
public:
    DEFINE_PTR(UMcpProfile, AFortPlayerController, CreativeModeProfile);
    DefineUOriginal(void, ServerAttemptInteract, AFortPlayerController *PlayerController, FFrame &Stack);
    DefineUOriginal(void, GetPlayerViewPoint, AFortPlayerController *PlayerController, FVector &OutViewLocation, FRotator &OutViewRotation);
    void ServerLoadingScreenDropped(FFrame &Stack);
    void ServerExecuteInventoryItem(FGuid ItemGuid);
    DefineUOriginal(void, ServerEmote, AFortPlayerController *PlayerController, FFrame &Stack);
    void ServerAttemptAircraftJump(FRotator Rotation);
    class UFortQuestManager *GetQuestManager(ESubGame SubGame)
    {
        static class UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortPlayerController", "GetQuestManager");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        struct FortPlayerController_GetQuestManager final
        {
        public:
            ESubGame SubGame;                     // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_1[0x7];                     // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
            class UFortQuestManager *ReturnValue; // 0x0008(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};

        Params.SubGame = SubGame;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    void ClientReportDamagedResourceBuilding(class ABuildingSMActor *BuildingSMActor, enum EFortResourceType PotentialResourceType, int32 PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot)
    {
        static UFunction *Func = SDK::PropLibrary->GetFunctionByName("FortPlayerController", "ClientReportDamagedResourceBuilding").Func;
        if (!Func)
        {
            return;
        }

        struct
        {
            ABuildingSMActor *B;
            EFortResourceType T;
            int32 C;
            bool D, W;
        } Params{BuildingSMActor, PotentialResourceType, PotentialResourceCount, bDestroyed, bJustHitWeakspot};

        this->ProcessEvent(Func, &Params);
    }

    void OnRep_QuickBar()
    {
        static SDK::UFunction *Func = nullptr;
        if (!Func)
            Func = SDK::PropLibrary->GetFunctionByName("FortPlayerController", "OnRep_QuickBar").Func;
        if (Func)
            this->ProcessEvent(Func, nullptr);
    }

    void HandleWorldInventoryLocalUpdate()
    {
        static UFunction *Func = SDK::PropLibrary->GetFunctionByName("FortPlayerController", "HandleWorldInventoryLocalUpdate").Func;
        if (!Func)
        {
            return;
        }

        this->ProcessEvent(Func, nullptr);
    }

    void ClientForceUpdateQuickbar(EFortQuickBars QuickbarToRefresh)
    {
        static UFunction *Func = SDK::PropLibrary->GetFunctionByName("FortPlayerController", "ClientForceUpdateQuickbar").Func;
        if (!Func)
        {
            return;
        }

        struct FortPlayerController_ClientForceUpdateQuickbar final
        {
        public:
            EFortQuickBars QuickbarToRefresh; // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        FortPlayerController_ClientForceUpdateQuickbar Params{QuickbarToRefresh};

        this->ProcessEvent(Func, &Params);
    }
    bool CanBuildFree();
    

    void ServerRepairBuildingActor(FFrame& Stack);
    void ServerCreateBuildingActor(FFrame& Stack);
    void ServerBeginEditingBuildingActor(FFrame& Stack);
    void ServerEditBuildingActor(FFrame& Stack);
    void ServerEndEditingBuildingActor(FFrame& Stack);
    void ServerPlayEmoteItem(FFrame& Stack);
    void ServerAttemptInventoryDrop(FFrame& Stack);
    std::vector<struct FItemAndCount> GetLategameLoadout();
public:
    DEFINE_PTR(AFortPlayerPawn, AFortPlayerController, MyFortPawn);
    DEFINE_PTR(AFortInventory, AFortPlayerController, WorldInventory);
    DEFINE_MEMBER(FFortAthenaLoadout, AFortPlayerController, CosmeticLoadoutPC);
    DEFINE_MEMBER(FFortAthenaLoadout, AFortPlayerControllerAthena, CustomizationLoadout);
    DEFINE_PTR(AFortQuickBars, AFortPlayerController, QuickBars);

public:
    DECLARE_STATIC_CLASS(AFortPlayerController)
    DECLARE_DEFAULT_OBJECT(AFortPlayerController)
};