#pragma once
#include "Engine/Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"

class UPrimitiveComponent : public USceneComponent
{
public:
    DEFINE_MEMBER(float, UPrimitiveComponent, BoundsScale)
    DECLARE_STATIC_CLASS(UPrimitiveComponent)
    DECLARE_DEFAULT_OBJECT(UPrimitiveComponent)
};