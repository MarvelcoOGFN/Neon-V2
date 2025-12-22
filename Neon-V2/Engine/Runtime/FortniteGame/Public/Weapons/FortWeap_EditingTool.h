#pragma once
#include "pch.h"

class AFortWeap_EditingTool : public UObject
{
public:
    void OnRep_EditActor()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortWeap_EditingTool", "OnRep_EditActor");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
    DEFINE_BOOL(AFortWeap_EditingTool, bEditConfirmed)
    DEFINE_PTR(class ABuildingSMActor, AFortWeap_EditingTool, EditActor);

public:
    DECLARE_STATIC_CLASS(AFortWeap_EditingTool)
    DECLARE_DEFAULT_OBJECT(AFortWeap_EditingTool)
};
