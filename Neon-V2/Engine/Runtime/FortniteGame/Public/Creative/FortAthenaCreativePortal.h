#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/FortVolume.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"

struct FCreativeLoadedLinkData
{
    DEFINE_MEMBER(FString, FCreativeLoadedLinkData, CreatorName)
    DEFINE_MEMBER(FString, FCreativeLoadedLinkData, SupportCode)
    DEFINE_MEMBER(FString, FCreativeLoadedLinkData, Mnemonic)
    DEFINE_MEMBER(FString, FCreativeLoadedLinkData, AccountId)
    DEFINE_MEMBER(int32, FCreativeLoadedLinkData, Version)
};

class AFortAthenaCreativePortal : public ABuildingActor
{
public:
    AFortVolume *GetLinkedVolume()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaCreativePortal", "GetLinkedVolume");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        // 0x0008 (0x0008 - 0x0000)
        struct FortAthenaCreativePortal_GetLinkedVolume final
        {
        public:
            class AFortVolume *ReturnValue; // 0x0000(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{};

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    void OnRep_OwningPlayer()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaCreativePortal", "OnRep_OwningPlayer");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_PlayersReady()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaCreativePortal", "OnRep_PlayersReady");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_PortalOpen()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaCreativePortal", "OnRep_PortalOpen");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_IslandInfo()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortAthenaCreativePortal", "OnRep_IslandInfo");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
    void TeleportPlayerToLinkedVolume(FFrame &Stack);
    DEFINE_MEMBER(TArray<struct FUniqueNetIdRepl>, AFortAthenaCreativePortal, PlayersReady)
    DEFINE_MEMBER(FUniqueNetIdRepl, AFortAthenaCreativePortal, OwningPlayer)
    DEFINE_BOOL(AFortAthenaCreativePortal, bPortalOpen)
    DEFINE_BOOL(AFortAthenaCreativePortal, bUserInitiatedLoad)
    DEFINE_BOOL(AFortAthenaCreativePortal, bInErrorState)
    DEFINE_MEMBER(FCreativeLoadedLinkData, AFortAthenaCreativePortal, IslandInfo)

    DECLARE_STATIC_CLASS(AFortAthenaCreativePortal)
    DECLARE_DEFAULT_OBJECT(AFortAthenaCreativePortal)
};
