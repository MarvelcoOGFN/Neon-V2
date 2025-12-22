#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"

class UActorComponent : public UObject
{
public:
    struct ActorComponent_Activate final
    {
    public:
        bool bReset;
    };

    struct ActorComponent_SetActive final
    {
    public:
        bool bNewActive;
        bool bReset;
    };

public:
    AActor *GetOwner()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("ActorComponent", "GetOwner");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return nullptr;
        struct ActorComponent_GetOwner final
        {
        public:
            class AActor *ReturnValue; // 0x0000(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } GetOwnerParams{};

        this->ProcessEvent(Func, &GetOwnerParams);

        return GetOwnerParams.ReturnValue;
    }

    void Activate(bool bReset)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("ActorComponent", "Activate");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        ActorComponent_Activate Params;
        Params.bReset = bReset;

        this->ProcessEvent(Func, &Params);
    }

    void SetActive(bool bNewActive, bool bReset)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("ActorComponent", "SetActive");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        ActorComponent_SetActive Params;
        Params.bNewActive = bNewActive;
        Params.bReset = bReset;

        this->ProcessEvent(Func, &Params);
    }

    void OnRep_IsActive()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("ActorComponent", "OnRep_IsActive");
        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
};
