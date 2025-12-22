#pragma once
#include "pch.h"

class AFortWeap_BuildingTool : public AFortWeapon
{
public:
    DEFINE_PTR(UObject, AFortWeap_BuildingTool, DefaultMetadata);
    void OnRep_DefaultMetadata(UObject *OldMetadata)
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortWeap_BuildingTool", "OnRep_DefaultMetadata");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct FortWeap_BuildingTool_OnRep_DefaultMetadata final
        {
        public:
            class UObject *OldMetadata; // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        } Params{OldMetadata};

        this->ProcessEvent(Func, &Params);
    }

public:
    DECLARE_STATIC_CLASS(AFortWeap_BuildingTool)
    DECLARE_DEFAULT_OBJECT(AFortWeap_BuildingTool)
};
