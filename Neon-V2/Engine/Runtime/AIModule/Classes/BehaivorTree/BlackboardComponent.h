#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"

class UBlackboardComponent final : public UActorComponent {
private:
    struct BlackboardComponent_SetValueAsEnum final
    {
    public:
        FName KeyName;
        uint8 EnumValue;
    };

    struct BlackboardComponent_SetValueAsBool final
    {
    public:
        FName KeyName;
        bool BoolValue;
    };
public:
    void SetValueAsEnum(const class FName& KeyName, uint8 EnumValue) {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BlackboardComponent", "SetValueAsEnum");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

		BlackboardComponent_SetValueAsEnum Params;
		Params.KeyName = KeyName;
		Params.EnumValue = EnumValue;

        this->ProcessEvent(Func, &Params);
    }

    void SetValueAsBool(const class FName& KeyName, bool BoolValue) {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BlackboardComponent", "SetValueAsBool");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        BlackboardComponent_SetValueAsBool Params;
        Params.KeyName = KeyName;
        Params.BoolValue = BoolValue;

        this->ProcessEvent(Func, &Params);
    }

    void SetValueAsVector(const class FName& KeyName, const struct FVector& VectorValue) {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BlackboardComponent", "SetValueAsVector");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct BlackboardComponent_SetValueAsVector final
        {
        public:
            class FName                                   KeyName;
            struct FVector                                VectorValue;
        };

        BlackboardComponent_SetValueAsVector Params;
        Params.KeyName = KeyName;
        Params.VectorValue = std::move(VectorValue);

        this->ProcessEvent(Func, &Params);
    }

    bool GetValueAsBool(const class FName& KeyName) {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BlackboardComponent", "GetValueAsBool");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        struct BlackboardComponent_GetValueAsBool final
        {
        public:
            class FName KeyName;
            bool ReturnValue;
        };

        BlackboardComponent_GetValueAsBool Params;
        Params.KeyName = KeyName;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    uint8 GetValueAsEnum(const class FName& KeyName) {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BlackboardComponent", "GetValueAsEnum");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return false;

        struct BlackboardComponent_GetValueAsEnum final
        {
        public:
            class FName                                   KeyName;                                           
            uint8                                         ReturnValue;                                      
        };

        BlackboardComponent_GetValueAsEnum Params;
        Params.KeyName = KeyName;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }

    FVector GetValueAsVector(const class FName& KeyName) {
        static SDK::UFunction* Func = nullptr;
        SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("BlackboardComponent", "GetValueAsVector");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return FVector();

        struct BlackboardComponent_GetValueAsVector final
        {
        public:
            class FName                                   KeyName;
            FVector                                ReturnValue;                                       
        };

        BlackboardComponent_GetValueAsVector Params;
        Params.KeyName = KeyName;

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }
public:
	DECLARE_STATIC_CLASS(UBlackboardComponent)
	DECLARE_DEFAULT_OBJECT(UBlackboardComponent)
};
