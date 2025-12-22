#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"

enum class EVolumeState : uint8
{
    Uninitialized = 0,
    ReadOnly = 1,
    Initializing = 2,
    Ready = 3,
    EVolumeState_MAX = 4,
};

class AFortVolume : public AActor
{
public:
    DEFINE_BOOL(AFortVolume, bShowPublishWatermark)
    DEFINE_BOOL(AFortVolume, bNeverAllowSaving)
    DEFINE_MEMBER(EVolumeState, AFortVolume, VolumeState)
    DECLARE_STATIC_CLASS(AFortVolume)
    DECLARE_DEFAULT_OBJECT(AFortVolume)

    void OnRep_VolumeState()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortVolume", "OnRep_VolumeState");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
};