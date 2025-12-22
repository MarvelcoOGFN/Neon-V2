#include "pch.h"
#include "Engine/Runtime/GameplayAbilities/Public/AbilitySystemComponent.h"

#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Stack.h"

// ServerTryActivateAbility, ServerTryActivateAbilityWithEventData not in use currently, but might be useful later

void UAbilitySystemComponent::ServerTryActivateAbility(FFrame& Stack)
{
    FGameplayAbilitySpecHandle Handle;
    bool InputPressed;
    FPredictionKey PredictionKey;
    Stack.StepCompiledIn(&Handle);
    Stack.StepCompiledIn(&InputPressed);
    Stack.StepCompiledIn(&PredictionKey);
    Stack.IncrementCode();
    
    FGameplayAbilitySpec* Spec = nullptr;
    auto& ActivatableAbilities = this->GetActivatableAbilities();
    auto& Items = ActivatableAbilities.GetItems();
    int32 GameplayAbilitySpecSize = StaticClassImpl("GameplayAbilitySpec")->GetSize();
    for (int i = 0; i < Items.Num(); i++) {
        auto CurrentSpec = (FGameplayAbilitySpec*) ((uint8*) Items.GetData() + (i * GameplayAbilitySpecSize));
        
        if (CurrentSpec->GetHandle().Handle == Handle.Handle)
        {
            Spec = CurrentSpec;
            break; 
        }
    }

    if (!Spec)
    {
        return;
    }

    Spec->SetInputPressed(true);

    UGameplayAbility* InstancedAbility = nullptr;
    
    if (!UKismetMemLibrary::Get<bool (*)(
        UAbilitySystemComponent*, 
        FGameplayAbilitySpecHandle, 
        FPredictionKey, 
        UGameplayAbility**, 
        void*, 
        const FGameplayEventData*
    )>(L"InternalTryActivateAbility")(this, Handle, PredictionKey, &InstancedAbility, nullptr, nullptr))
    {
        Spec->SetInputPressed(false);
    }
    
    this->GetActivatableAbilities().MarkItemDirty(Spec); 
}

void UAbilitySystemComponent::ServerTryActivateAbilityWithEventData(FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey PredictionKey, const FGameplayEventData& TriggerEventData)
{
    FGameplayAbilitySpec* Spec = nullptr;
    auto& ActivatableAbilities = this->GetActivatableAbilities();
    auto& Items = ActivatableAbilities.GetItems();
    int32 GameplayAbilitySpecSize = StaticClassImpl("GameplayAbilitySpec")->GetSize();
    for (int i = 0; i < Items.Num(); i++) {
        auto CurrentSpec = (FGameplayAbilitySpec*) ((uint8*) Items.GetData() + (i * GameplayAbilitySpecSize));
        
        if (CurrentSpec->GetHandle().Handle == Handle.Handle)
        {
            Spec = CurrentSpec;
            break; 
        }
    }

    if (!Spec)
    {
        return;
    }

    Spec->SetInputPressed(InputPressed);

    UGameplayAbility* InstancedAbility = nullptr;
    
    if (!UKismetMemLibrary::Get<bool (*)(
        UAbilitySystemComponent*, 
        FGameplayAbilitySpecHandle, 
        FPredictionKey, 
        UGameplayAbility**, 
        void*, 
        const FGameplayEventData*
    )>(L"InternalTryActivateAbility")(this, Handle, PredictionKey, &InstancedAbility, nullptr, &TriggerEventData))
    {
        Spec->SetInputPressed(false);
    }
    
    this->GetActivatableAbilities().MarkItemDirty(Spec);
}

void UAbilitySystemComponent::InternalServerTryActivateAbility(
    FGameplayAbilitySpecHandle Handle, 
    bool InputPressed, 
    FPredictionKey* PredictionKey, 
    FGameplayEventData* TriggerEventData)
{
    static int32 PredictionKeySize = StaticClassImpl("PredictionKey")->GetSize();
    FGameplayAbilitySpec* Spec = nullptr;
    auto& ActivatableAbilities = this->GetActivatableAbilities();
    auto& Items = ActivatableAbilities.GetItems();
    static int32 GameplayAbilitySpecSize = StaticClassImpl("GameplayAbilitySpec")->GetSize();
    for (int i = 0; i < Items.Num(); i++) {
        auto CurrentSpec = (FGameplayAbilitySpec*) ((uint8*) Items.GetData() + (i * GameplayAbilitySpecSize));
        
        if (CurrentSpec->GetHandle().Handle == Handle.Handle)
        {
            Spec = CurrentSpec;
            break; 
        }
    }

    if (!Spec) return;

    Spec->SetInputPressed(true);

    UGameplayAbility* InstancedAbility = nullptr;

    bool Result = false;
    auto InternalTryActivateAbility = UKismetMemLibrary::Get<bool(*)(
        UAbilitySystemComponent*, 
        FGameplayAbilitySpecHandle, 
        FPredictionKey, 
        UGameplayAbility**, 
        void*, 
        const FGameplayEventData*
    )>(L"InternalTryActivateAbility");

    if (PredictionKeySize == 0x10) {
        Result = InternalTryActivateAbility(this, Handle, *reinterpret_cast<FPredictionKey*>(&*(PadHex10*)PredictionKey), &InstancedAbility, nullptr, TriggerEventData);
    } else {
        Result = InternalTryActivateAbility(this, Handle, *reinterpret_cast<FPredictionKey*>(&*(PadHex18*)PredictionKey), &InstancedAbility, nullptr, TriggerEventData);
    }
    
    if (!Result) Spec->SetInputPressed(false);
    
    this->GetActivatableAbilities().MarkItemDirty(Spec);
}

void UAbilitySystemComponent::GiveAbility(UClass* Ability)
{
    if (!Ability) return;

    static std::unordered_map<UClass*, UGameplayAbility*> AbilityCache;

    UGameplayAbility* AbilityCDO;
    auto It = AbilityCache.find(Ability);
    if (It != AbilityCache.end())
    {
        AbilityCDO = It->second;
    }
    else
    {
        AbilityCDO = Cast<UGameplayAbility>(Ability->GetClassDefaultObject());
        if (!AbilityCDO) return;
        AbilityCache[Ability] = AbilityCDO;
    }

    static int32 GameplayAbilitySpecSize = StaticClassImpl("GameplayAbilitySpec")->GetSize();
    FGameplayAbilitySpec* Spec = (FGameplayAbilitySpec*)VirtualAlloc(
            nullptr,
            GameplayAbilitySpecSize,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE
        );
    
    new(Spec) FGameplayAbilitySpec();

    Spec->MostRecentArrayReplicationKey = -1;
    Spec->ReplicationID = -1;
    Spec->ReplicationKey = -1;
    Spec->GetHandle().Handle = rand();
    Spec->SetAbility(AbilityCDO);
    Spec->SetSourceObject(nullptr);
    Spec->SetInputID(-1);
    Spec->SetLevel(1);

    FGameplayAbilitySpecHandle Handle = Spec->GetHandle();

    UKismetMemLibrary::Get<FGameplayAbilitySpecHandle(__fastcall*)(
        UAbilitySystemComponent*,
        FGameplayAbilitySpecHandle*,
        const FGameplayAbilitySpec&
    )>(L"GiveAbility")(this, &Handle, *Spec);

    Spec->~FGameplayAbilitySpec();
    VirtualFree(Spec, 0, MEM_RELEASE);
}

void UAbilitySystemComponent::GiveAbilitySet(UFortAbilitySet* Set)
{
    if (Set)
    {
        for (int i = 0; i < Set->GetGameplayAbilities().Num(); i++) {
            if (Set->GetGameplayAbilities().IsValidIndex(i))
            {
                UClass* Class = Set->GetGameplayAbilities()[i];
                if (!Class) {
                    continue;
                }
                GiveAbility(Class);
            }
        }

        for (int i = 0; i < Set->GetPassiveGameplayEffects().Num(); i++) {
            if (Set->GetPassiveGameplayEffects().IsValidIndex(i))
            {
                FGameplayEffectApplicationInfo& EffectInfo = Set->GetPassiveGameplayEffects()[i];
                if (!EffectInfo.GetGameplayEffect()) continue;
                FGameplayEffectContextHandle EffectContext{};
                BP_ApplyGameplayEffectToSelf(EffectInfo.GetGameplayEffect(), EffectInfo.Level, EffectContext);
            }
        }
    }
}