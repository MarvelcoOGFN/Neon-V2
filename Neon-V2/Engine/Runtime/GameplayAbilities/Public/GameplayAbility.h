#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"

class UGameplayAbility : public UObject
{
public:
    DECLARE_STATIC_CLASS(UGameplayAbility)
    DECLARE_DEFAULT_OBJECT(UGameplayAbility)
};

struct FGameplayEffectApplicationInfo final
{
public:
    DEFINE_PTR(UClass, FGameplayEffectApplicationInfo, GameplayEffect);
    float Level;       // 0x0020(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    uint8 Pad_24[0x4]; // 0x0024(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

class UFortAbilitySet : public UObject
{
public:
    DEFINE_MEMBER(TArray<TSubclassOf<UClass>>, UFortAbilitySet, GameplayAbilities);
    DEFINE_MEMBER(TArray<FGameplayEffectApplicationInfo>, UFortAbilitySet, PassiveGameplayEffects);
};

class UFortGameplayAbility : public UGameplayAbility
{
public:
    inline class UAbilitySystemComponent *GetActivatingAbilityComponent()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameplayAbility", "GetActivatingAbilityComponent");
        if (!Func)
            Func = Info.Func;
        if (!Func)
            return nullptr;

        struct
        {
            class UAbilitySystemComponent *ReturnValue;
        } Params{};

        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }

DefineUOriginal(__int64, ApplyCost, UFortGameplayAbility *, void *a2, void *a3, void *a4) public : DECLARE_STATIC_CLASS(UFortGameplayAbility)
                                                                                                       DECLARE_DEFAULT_OBJECT(UFortGameplayAbility)
};