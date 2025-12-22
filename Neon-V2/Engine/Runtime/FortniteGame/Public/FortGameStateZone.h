#pragma once
#include "pch.h"

#include "FortGameState.h"
#include "Creative/FortCreativePortalManager.h"

class AFortGameStateZone : public AFortGameState
{
public:
    bool IsRespawningAllowed(class AFortPlayerStateAthena *PlayerState)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameStateZone", "IsRespawningAllowed");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        struct FortGameStateZone_IsRespawningAllowed final
        {
        public:
            class AFortPlayerStateAthena *PlayerState; // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            bool ReturnValue;                          // 0x0008(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8 Pad_9[0x7];                          // 0x0009(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
        } Params{};

        Params.PlayerState = PlayerState;

        this->ProcessEvent(Func, &Params);



        return Params.ReturnValue;
    }
    DEFINE_PTR(AFortCreativePortalManager, AFortGameStateZone, CreativePortalManager);
    DEFINE_BOOL(AFortGameStateZone, bDBNODeathEnabled)
};
