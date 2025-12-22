#pragma once
#include "pch.h"

#include "GameplayAbilitySpec.h"
#include "Engine/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerState.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTag.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

struct alignas(0x08) FGameplayAbilityTargetDataHandle final
{
    public:
    uint8                                         Pad_0[0x28];                                       // 0x0000(0x0028)(Fixing Struct Size After Last Property [ Dumper-8 ])
};

struct FGameplayEventData final
{
public:
	struct FGameplayTag                           EventTag;                                          // 0x0000(0x0008)(Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AActor*                                 Instigator;                                        // 0x0008(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AActor*                                 Target;                                            // 0x0010(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UObject*                                OptionalObject;                                    // 0x0018(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UObject*                                OptionalObject2;                                   // 0x0020(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FGameplayEffectContextHandle           ContextHandle;                                     // 0x0028(0x0018)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  InstigatorTags;                                    // 0x0040(0x0020)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  TargetTags;                                        // 0x0060(0x0020)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
	float                                         EventMagnitude;                                    // 0x0080(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_84[0x4];                                       // 0x0084(0x0004)(Fixing Size After Last Property [ Dumper-8 ])
	struct FGameplayAbilityTargetDataHandle       TargetData;                                        // 0x0088(0x0028)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
};

class IInterface : public UObject
{
public:
    DECLARE_STATIC_CLASS(IInterface)
    DECLARE_DEFAULT_OBJECT(IInterface)
};

class IAbilitySystemInterface : public IInterface
{
public:
    DECLARE_STATIC_CLASS(IAbilitySystemInterface)
    DECLARE_DEFAULT_OBJECT(IAbilitySystemInterface)
};

class UAbilitySystemComponent : public UActorComponent
{
public:
    void BP_ApplyGameplayEffectToSelf(TSubclassOf<class UGameplayEffect> GameplayEffectClass, float Level, const struct FGameplayEffectContextHandle& EffectContext)
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("AbilitySystemComponent", "BP_ApplyGameplayEffectToSelf");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct AbilitySystemComponent_BP_ApplyGameplayEffectToSelf final
        {
        public:
            TSubclassOf<class UGameplayEffect>            GameplayEffectClass;                               // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            float                                         Level;                                             // 0x0008(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8                                         Pad_3CA2[0x4];                                     // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
            struct FGameplayEffectContextHandle           EffectContext;                                     // 0x0010(0x0018)(Parm, NativeAccessSpecifierPublic)
            void*            ReturnValue;                                       // 0x0028(0x0008)(Parm, OutParm, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        
        AbilitySystemComponent_BP_ApplyGameplayEffectToSelf Params{};

        Params.GameplayEffectClass = GameplayEffectClass;
        Params.Level = Level;
        Params.EffectContext = EffectContext;
    
        this->ProcessEvent(Func, &Params);
    }

    FGameplayEffectContextHandle MakeEffectContext()
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("AbilitySystemComponent", "MakeEffectContext");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return FGameplayEffectContextHandle();


        struct AbilitySystemComponent_MakeEffectContext final
        {
        public:
            struct FGameplayEffectContextHandle           ReturnValue;                                       // 0x0000(0x0018)(Parm, OutParm, ReturnParm, NativeAccessSpecifierPublic)
        } Params{};

        this->ProcessEvent(Func, &Params);

        return Params.ReturnValue;
    }
    
    void ServerTryActivateAbility(const struct FGameplayAbilitySpecHandle& AbilityToActivate, bool InputPressed, void* PredictionKey)
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("AbilitySystemComponent", "ServerTryActivateAbility");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        struct AbilitySystemComponent_ServerTryActivateAbility final
        {
        public:
            struct FGameplayAbilitySpecHandle             AbilityToActivate;                                 // 0x0000(0x0004)(Parm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            bool                                          InputPressed;                                      // 0x0004(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
            uint8                                         Pad_5[0x3];                                        // 0x0005(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
            void*                         PredictionKey;                                     // 0x0008(0x0018)(Parm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
        };
        
        AbilitySystemComponent_ServerTryActivateAbility Params{};

        Params.AbilityToActivate = std::move(AbilityToActivate);
        Params.InputPressed = InputPressed;
        Params.PredictionKey = PredictionKey;
    
        this->ProcessEvent(Func, &Params);
    }
public:
    DEFINE_MEMBER(FGameplayAbilitySpecContainer, UAbilitySystemComponent, ActivatableAbilities)
public:
    void InternalServerTryActivateAbility(FGameplayAbilitySpecHandle, bool, FPredictionKey*, FGameplayEventData*);
    void ServerTryActivateAbility(FFrame& Stack);
    void ServerTryActivateAbilityWithEventData(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey PredictionKey, const FGameplayEventData& TriggerEventData);
    void GiveAbility(UClass* Ability);
    void GiveAbilitySet(UFortAbilitySet* Set);
public:
    DECLARE_STATIC_CLASS(UAbilitySystemComponent)
    DECLARE_DEFAULT_OBJECT(UAbilitySystemComponent)
};