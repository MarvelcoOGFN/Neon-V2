#pragma once
#include "pch.h"

#include "LocalPlayer.h"

class UGameInstance : public UObject
{
public:
    DEFINE_MEMBER(TArray<ULocalPlayer*>, UGameInstance, LocalPlayers);
public:
    DECLARE_STATIC_CLASS(UGameInstance);
    DECLARE_DEFAULT_OBJECT(UGameInstance);
};
