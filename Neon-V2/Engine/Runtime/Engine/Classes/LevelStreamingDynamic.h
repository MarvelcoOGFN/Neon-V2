#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Math/Rotator.h"
#include "Engine/Runtime/Engine/Core/Public/Math/Vector.h"

class ULevelStreamingDynamic : public UObject
{
public:
    static ULevelStreamingDynamic* LoadLevelInstance(class UObject* WorldContextObject, const class FString& LevelName, const struct FVector& Location, const struct FRotator& Rotation, bool* bOutSuccess, const class FString& OptionalLevelNameOverride);
public:
    DECLARE_STATIC_CLASS(ULevelStreamingDynamic)
    DECLARE_DEFAULT_OBJECT(ULevelStreamingDynamic)
};
