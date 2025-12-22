#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"

class UBrainComponent : public UActorComponent {
public:
    void StartLogic() {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BrainComponent", "StartLogic");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void RestartLogic() {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BrainComponent", "RestartLogic");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
public:
    DECLARE_STATIC_CLASS(UBrainComponent);
    DECLARE_DEFAULT_OBJECT(UBrainComponent);
};
