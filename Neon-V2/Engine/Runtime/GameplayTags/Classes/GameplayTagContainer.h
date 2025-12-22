#pragma once
#include "pch.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTag.h"

struct FGameplayTagContainer final
{
public:
    TArray<struct FGameplayTag>                   GameplayTags;                                      // 0x0000(0x0010)(Edit, BlueprintVisible, ZeroConstructor, EditConst, SaveGame, Protected, NativeAccessSpecifierProtected)
    TArray<struct FGameplayTag>                   ParentTags;                                        // 0x0010(0x0010)(ZeroConstructor, Transient, Protected, NativeAccessSpecifierProtected)
public:
    bool HasTag(const FGameplayTag& TagToCheck) const
    {
        for (auto& Tag : GameplayTags) {
            if (Tag.TagName == TagToCheck.TagName) {
                return true;
            }
        }
        for (auto& Tag : ParentTags) {
            if (Tag.TagName == TagToCheck.TagName) {
                return true;
            }
        }
        return false;
    }
public:
    bool HasAll(const FGameplayTagContainer& ContainerToCheck) const
    {
        for (auto& Tag : ContainerToCheck.GameplayTags) {
            bool Found2 = false;
            for (auto& Tag2 : GameplayTags) {
                if (Tag2.TagName == Tag.TagName) {
                    Found2 = true;
                    break;
                }
            }
            if (!Found2) for (auto& Tag2 : ParentTags) {
                if (Tag2.TagName == Tag.TagName) {
                    Found2 = true;
                    break;
                }
            }
            if (!Found2) return false;
        }
        return true;
    }

    void AppendTags( FGameplayTagContainer& Other)
    {
        static int GameplayTagSize = StaticClassImpl("GameplayTag")->GetSize();
        for (const auto& GameplayTag : Other.GameplayTags)
        {
            if (!HasTag(GameplayTag))
                GameplayTags.Add(GameplayTag, GameplayTagSize);
        }
        for (const auto& ParentTag : Other.ParentTags)
        {
            if (!HasTag(ParentTag))
                ParentTags.Add(ParentTag, GameplayTagSize);
        }
    }
};