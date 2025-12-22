#pragma once
#include "pch.h"

enum class EFastArraySerializerDeltaFlags : uint8
{
    None = 0,
    HasBeenSerialized = 1,
    HasDeltaBeenRequested = 2,
    IsUsingDeltaSerialization = 4,
    EFastArraySerializerDeltaFlags_MAX = 5,
};

struct FFastArraySerializerItem
{
    int                                                ReplicationID;                                            // 0x0000(0x0004) (ZeroConstructor, IsPlainOldData, RepSkip, RepNotify, Interp, NonTransactional, EditorOnly, NoDestructor, AutoWeak, ContainsInstancedReference, AssetRegistrySearchable, SimpleDisplay, AdvancedDisplay, Protected, BlueprintCallable, BlueprintAuthorityOnly, TextExportTransient, NonPIEDuplicateTransient, ExposeOnSpawn, PersistentInstance, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic, NativeAccessSpecifierProtected, NativeAccessSpecifierPrivate)
    int                                                ReplicationKey;                                           // 0x0004(0x0004) (ZeroConstructor, IsPlainOldData, RepSkip, RepNotify, Interp, NonTransactional, EditorOnly, NoDestructor, AutoWeak, ContainsInstancedReference, AssetRegistrySearchable, SimpleDisplay, AdvancedDisplay, Protected, BlueprintCallable, BlueprintAuthorityOnly, TextExportTransient, NonPIEDuplicateTransient, ExposeOnSpawn, PersistentInstance, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic, NativeAccessSpecifierProtected, NativeAccessSpecifierPrivate)
    int                                                MostRecentArrayReplicationKey;                            // 0x0008(0x0004) (ZeroConstructor, IsPlainOldData, RepSkip, RepNotify, Interp, NonTransactional, EditorOnly, NoDestructor, AutoWeak, ContainsInstancedReference, AssetRegistrySearchable, SimpleDisplay, AdvancedDisplay, Protected, BlueprintCallable, BlueprintAuthorityOnly, TextExportTransient, NonPIEDuplicateTransient, ExposeOnSpawn, PersistentInstance, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic, NativeAccessSpecifierProtected, NativeAccessSpecifierPrivate)

    FFastArraySerializerItem()
    {
        ReplicationID = -1;
        ReplicationKey = -1;
        MostRecentArrayReplicationKey = -1;
    }
};

struct FFastArraySerializer
{
    static inline bool bNewSerializer;

    int& GetArrayReplicationKey()
    {
        static int ArrayReplicationKeyOffset = 0x50 + 0x4;

        return *(int*)(__int64(this) + ArrayReplicationKeyOffset);
    }

    int& GetIDCounter()
    {
        static int IDCounterOffset = 0x50;

        return *(int*)(__int64(this) + IDCounterOffset);
    }

    int& GetCachedNumItems()
    {
        static int CachedNumItemsOffset = 0x50 + 0x8 + 0x50 + (bNewSerializer ? 0x50 : 0x0);
        
        return *(int*)(__int64(this) + CachedNumItemsOffset);
    }

    int& GetCachedNumItemsToConsiderForWriting()
    {
        static int CachedNumItemsToConsiderForWritingOffset = 0x50 + 0x8 + 0x50 + 0x4 + (bNewSerializer ? 0x50 : 0x0);

        return *(int*)(__int64(this) + CachedNumItemsToConsiderForWritingOffset);
    }

    void MarkItemDirty(FFastArraySerializerItem* Item)
    {
        if (Item->ReplicationID == -1)
        {
            Item->ReplicationID = ++GetIDCounter();

            if (GetIDCounter() == -1)
                GetIDCounter()++;
        }

        Item->ReplicationKey++;
        MarkArrayDirty();
    }

    void MarkArrayDirty()
    {
        GetArrayReplicationKey()++;

        if (GetArrayReplicationKey() == -1)
            GetArrayReplicationKey()++;

        GetCachedNumItems() = -1;
        GetCachedNumItemsToConsiderForWriting() = -1;
    }
};
