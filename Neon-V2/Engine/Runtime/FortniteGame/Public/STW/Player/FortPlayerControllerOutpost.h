#pragma once

#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerController.h"

class AFortPlayerControllerOutpost : public AFortPlayerController
{
public:
	DEFINE_BOOL(AFortPlayerStateOutpost, bIsOutpostOwnerInPIE);
public:
	DECLARE_DEFAULT_OBJECT(AFortPlayerControllerOutpost);
	DECLARE_STATIC_CLASS(AFortPlayerControllerOutpost);
};
