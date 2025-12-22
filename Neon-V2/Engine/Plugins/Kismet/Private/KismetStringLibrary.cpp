#include "pch.h"
#include "Engine/Plugins/Kismet/Public/KismetStringLibrary.h"

FName UKismetStringLibrary::Conv_StringToName(const FString &InString)
{
    static UFunction *Func = PropLibrary->GetFunctionByName("KismetStringLibrary", "Conv_StringToName").Func;
    if (!Func)
        return FName();

    struct
    {
        FString InString;
        FName ReturnValue;
    } Params{InString};

    static UObject *DefaultObject = GetDefaultObj();
    DefaultObject->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}

FText UKismetStringLibrary::Conv_StringToText(FString InName)
{
    static SDK::UFunction *Func = nullptr;
    static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("KismetTextLibrary", "Conv_StringToText");

    if (Func == nullptr)
        Func = Info.Func;
    if (!Func)
        return FText();

    struct KismetStringLibrary_Conv_StringToText final
    {
    public:
        class FString inString;  // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        class FText ReturnValue; // 0x0008(0x0010)(Parm, OutParm, ZeroConstructor, ReturnParm, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    } KismetStringLibrary_Conv_NameToString_Params{InName};

    static UObject *Object = nullptr;
    if (!Object)
        Object = SDK::StaticClassImpl("KismetTextLibrary")->GetClassDefaultObject();

    Object->ProcessEvent(Func, &KismetStringLibrary_Conv_NameToString_Params);

    return KismetStringLibrary_Conv_NameToString_Params.ReturnValue;
}

FString UKismetStringLibrary::Conv_NameToString(FName InName)
{
    static UFunction *Func = PropLibrary->GetFunctionByName("KismetStringLibrary", "Conv_NameToString").Func;
    if (!Func)
        return FString();

    struct
    {
        FName InName;
        FString ReturnValue;
    } Params{InName};

    static UObject *DefaultObject = GetDefaultObj();
    DefaultObject->ProcessEvent(Func, &Params);

    return Params.ReturnValue;
}
