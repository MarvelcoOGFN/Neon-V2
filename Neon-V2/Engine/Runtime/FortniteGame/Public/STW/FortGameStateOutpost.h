#pragma once
#include "pch.h"

#include "Engine/Runtime/FortniteGame/Public/FortGameStateZone.h"

class UFortZoneTheme : public UObject
{
public:
	DECLARE_DEFAULT_OBJECT(UFortZoneTheme)
	DECLARE_STATIC_CLASS(UFortZoneTheme)
	
};

class AFortGameStateOutpost : public AFortGameStateZone
{
public:
	DEFINE_PTR(UFortZoneTheme, AFortGameStateZone, ZoneTheme)

public:
	DECLARE_DEFAULT_OBJECT(AFortGameStateOutpost)
	DECLARE_STATIC_CLASS(AFortGameStateOutpost)
};
