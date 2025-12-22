#pragma once

#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerStateZone.h"

class AFortPlayerStateOutpost : public AFortPlayerState
{
public:
	DEFINE_BOOL(AFortPlayerStateOutpost, bIsWorldDataOwner);
	
public:
	DECLARE_DEFAULT_OBJECT(AFortPlayerStateOutpost);
	DECLARE_STATIC_CLASS(AFortPlayerStateOutpost);
};