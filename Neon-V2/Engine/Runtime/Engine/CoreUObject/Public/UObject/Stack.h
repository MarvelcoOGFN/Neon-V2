#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/KismetMemLibrary.h"

class FOutputDevice
{
public:
	bool bSuppressEventTag;
	bool bAutoEmitLineTerminator;
	uint8_t _Padding1[0x6];
};

struct FFrame : public FOutputDevice
{
public:
    void** VFT;
    UFunction* Node;
    UObject* Object;
    uint8* Code;
    uint8* Locals;
    void* MostRecentProperty;
    uint8_t* MostRecentPropertyAddress;

private:
    static void* GetNext(void* Property) {
        if (!Property) return nullptr;
        
        if (Fortnite_Version >= 11.00f) {
			return ((FField*)Property)->Next;
        } else {
            return *(void**)((uint8_t*)Property + 0x28);
        }
    }

public:
    void*& GetPropertyChainForCompiledIn()
    {
        static auto PropertyChainForCompiledInOffset = 0x80;
        return *(void**)(__int64(this) + PropertyChainForCompiledInOffset);
    }

    uint8_t*& GetMostRecentPropertyAddress()
    {
        return MostRecentPropertyAddress;
    }

    __forceinline void StepExplicitProperty(void* const Result, void* Property)
    {
        static void (*StepExplicitPropertyOriginal)(__int64 frame, void* const Result, void* Property) = decltype(StepExplicitPropertyOriginal)(UKismetMemLibrary::GetAddress<uintptr_t>(TEXT("StepExplicitProperty")));
        StepExplicitPropertyOriginal(__int64(this), Result, Property);
    }

    __forceinline void Step(UObject* Context, RESULT_DECL)
    {
        static void (*StepOriginal)(__int64 frame, UObject* Context, RESULT_DECL) = decltype(StepOriginal)(UKismetMemLibrary::GetAddress<uintptr_t>(TEXT("Step")));
        StepOriginal(__int64(this), Context, RESULT_PARAM);
    }

    __forceinline void StepCompiledIn(void* const Result, bool ForceExplicitProp = false)
    {
        if (Code && !ForceExplicitProp)
        {
            Step(Object, Result);
        }
        else
        {
            void* Property = GetPropertyChainForCompiledIn();
            if (Property) {
                GetPropertyChainForCompiledIn() = GetNext(Property);
                StepExplicitProperty(Result, Property);
            }
        }
    }

    template<typename TNativeType>
    __forceinline TNativeType& StepCompiledInRef(void* const TemporaryBuffer)
    {
        GetMostRecentPropertyAddress() = nullptr;

        if (Code)
        {
            Step(Object, TemporaryBuffer);
        }
        else
        {
            void* Property = GetPropertyChainForCompiledIn();
            if (Property) {
                GetPropertyChainForCompiledIn() = GetNext(Property);
                StepExplicitProperty(TemporaryBuffer, Property);
            }
        }

        return (GetMostRecentPropertyAddress() != nullptr) ? *(TNativeType*)(GetMostRecentPropertyAddress()) : *(TNativeType*)TemporaryBuffer;
    }
    
    template <typename T>
    T& StepCompiledInRef() {
        T TempVal{};
        GetMostRecentPropertyAddress() = nullptr;

        if (Code)
        {
            Step(this, Object, &TempVal);
        }
        else
        {
            void* Property = GetPropertyChainForCompiledIn();
            if (Property) {
                GetPropertyChainForCompiledIn() = GetNext(Property);
                StepExplicitProperty(this, &TempVal, Property);
            }
        }

        return GetMostRecentPropertyAddress() ? *(T*)GetMostRecentPropertyAddress() : TempVal;
    }

    void IncrementCode() {
        Code = (uint8_t*)(__int64(Code) + (bool)Code);
    }
};
