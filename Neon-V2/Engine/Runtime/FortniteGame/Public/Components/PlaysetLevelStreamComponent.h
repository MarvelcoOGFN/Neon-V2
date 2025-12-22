#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"

class UPlaysetLevelStreamComponent : public UActorComponent
{
public:
    void SetPlayset(class UFortPlaysetItemDefinition *NewPlayset)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("PlaysetLevelStreamComponent", "SetPlayset");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, &NewPlayset);
    }

    DECLARE_STATIC_CLASS(UPlaysetLevelStreamComponent)
    DECLARE_DEFAULT_OBJECT(UPlaysetLevelStreamComponent)
};
