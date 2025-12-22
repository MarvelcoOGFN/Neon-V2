#pragma once
#include "pch.h"

#include "PlayerState.h"

class APawn : public AActor
{
public:
    DEFINE_PTR(APlayerState, APawn, PlayerState)
    DEFINE_PTR(class AController, APawn, Controller);
public:
    DECLARE_STATIC_CLASS(APawn)
    DECLARE_DEFAULT_OBJECT(APawn)
};
