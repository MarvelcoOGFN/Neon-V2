#pragma once
#include "pch.h"

#include "GameFramework/Controller.h"

class ULocalPlayer : public UObject
{
public:
    DEFINE_PTR(APlayerController, ULocalPlayer, PlayerController);
public:
    DECLARE_STATIC_CLASS(ULocalPlayer);
    DECLARE_DEFAULT_OBJECT(ULocalPlayer);
};  