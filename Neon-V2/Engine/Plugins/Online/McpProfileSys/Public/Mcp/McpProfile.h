#pragma once
#include "pch.h"
/* credits to nova (ender) */
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"

template< class ObjectType>
class TSharedPtr
{
public:
    ObjectType* Object;

    int32 SharedReferenceCount;
    int32 WeakReferenceCount;

    FORCEINLINE ObjectType* Get()
    {
        return Object;
    }
    FORCEINLINE ObjectType* Get() const
    {
        return Object;
    }
    FORCEINLINE ObjectType& operator*()
    {
        return *Object;
    }
    FORCEINLINE const ObjectType& operator*() const
    {
        return *Object;
    }
    FORCEINLINE ObjectType* operator->()
    {
        return Object;
    }
    FORCEINLINE ObjectType* operator->() const
    {
        return Object;
    }
};

class FMcpItem
{
public:
    FString ItemType;
    FString InstanceId;
    FString TemplateId;
    int Quantity;
private:
    unsigned char Pad[0x18];
public:
    TMap<FString, void*> Attributes;
    UObject* Instance;
    int64 LastUpdate;
    TWeakObjectPtr<class UMcpProfile> OwningProfile;
};

class FMcpItemsContainer
{
public:
    TMap<FString, TSharedPtr<FMcpItem>> Items;
    TMap<FString, FString> ItemsByType;
};

class FMcpProfileState
{
    void* Pad;
public:
    FMcpItemsContainer ItemsContainer;
    TArray<void*> PendingForceQueryDelegates;
    TArray<void*> PendingUpdateQueue;
    int64 MtxBalanceCacheVersion;
    int32 MtxBalanceCache;
};

class UMcpProfile : public UObject
{
public:
    inline FMcpProfileState* InternalGetState()
    {
        static auto StateOffset = UKismetMemLibrary::GetOffset(this, "CommandRevision") + 0x8;
        return *(FMcpProfileState**)(__int64(this) + StateOffset);
    }

    __declspec(property(get = InternalGetState))
    FMcpProfileState* State;
public:
    DECLARE_STATIC_CLASS(UMcpProfile)
    DECLARE_DEFAULT_OBJECT(UMcpProfile)
};