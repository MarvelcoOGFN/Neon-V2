#pragma once
#include "pch.h"

#include "GameplayAbility.h"
#include "Engine/Runtime/Engine/Classes/NetSerialization.h"

struct FGameplayAbilitySpecHandle
{
    int Handle;
};

struct FGameplayEffectContextHandle final
{
    uint8                                         Pad_0[0x18];                                      
};

enum class EGameplayAbilityActivationMode : uint8
{
    Authority                                = 0,
    NonAuthority                             = 1,
    Predicting                               = 2,
    Confirmed                                = 3,
    Rejected                                 = 4,
    EGameplayAbilityActivationMode_MAX       = 5,
};

struct FPredictionKey final
{
public:
    uint8_t Pad[0x10]; 
};

struct FGameplayAbilityActivationInfo final
{
public:
    EGameplayAbilityActivationMode                ActivationMode;                                    
    uint8                                         bCanBeEndedByOtherInstance : 1;               
    uint8                                         Pad_2[0x6];                                   
    struct FPredictionKey                         PredictionKeyWhenActivated;                        
};

struct FGameplayAbilitySpec final : public FFastArraySerializerItem
{
    DEFINE_MEMBER(FGameplayAbilitySpecHandle, FGameplayAbilitySpec, Handle);
    DEFINE_BOOL(FGameplayAbilitySpec, InputPressed);
    DEFINE_PTR(UGameplayAbility, FGameplayAbilitySpec, Ability);
    DEFINE_PTR(UObject, FGameplayAbilitySpec, SourceObject);
    DEFINE_MEMBER(int32, FGameplayAbilitySpec, Level);
    DEFINE_MEMBER(int32, FGameplayAbilitySpec, InputID);
    DEFINE_MEMBER(FGameplayAbilityActivationInfo, FGameplayAbilitySpec, ActivationInfo);
    DEFINE_BOOL(FGameplayAbilitySpec, RemoveAfterActivation)
};

struct FGameplayAbilitySpecContainer : public FFastArraySerializer
{
    DEFINE_MEMBER(TArray<FGameplayAbilitySpec>, FGameplayAbilitySpecContainer, Items);
};