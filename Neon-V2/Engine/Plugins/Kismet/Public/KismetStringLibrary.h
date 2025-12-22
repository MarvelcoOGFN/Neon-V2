#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Internationalization/Text.h"

class UKismetStringLibrary : public UObject
{
public:
    static FName Conv_StringToName(const FString& InString);
    static FString Conv_NameToString(FName InName);
    static FText Conv_StringToText(FString InName);
public:
    DECLARE_STATIC_CLASS(UKismetStringLibrary)
    DECLARE_DEFAULT_OBJECT(UKismetStringLibrary)
};
