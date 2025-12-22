#pragma once
#include "pch.h"

class UReplicationDriver : public UObject
{
public:
    DECLARE_STATIC_CLASS(UReplicationDriver)
    DECLARE_DEFAULT_OBJECT(UReplicationDriver)
};

class UNetConnection : public UObject
{
public:
    DEFINE_BOOL(UNetConnection, InternalAck);
    
    DECLARE_STATIC_CLASS(UNetConnection)
    DECLARE_DEFAULT_OBJECT(UNetConnection, UObject);
};

class UNetDriver : public UObject
{
public:
    DEFINE_MEMBER(int32, UNetDriver, MaxClientRate);
    DEFINE_MEMBER(int32, UNetDriver, MaxInternetClientRate);
    DEFINE_MEMBER(FName, UNetDriver, NetDriverName);
    DEFINE_PTR(UReplicationDriver, UNetDriver, ReplicationDriver);
    DEFINE_MEMBER(TArray<UNetConnection*>, UNetDriver, ClientConnections);
    DEFINE_PTR(class UWorld, UNetDriver, World);
public:
    static inline void (*TickFlushOG)(UNetDriver*, float);
    static void TickFlush(UNetDriver*, float DeltaSeconds);
public:
    DECLARE_STATIC_CLASS(UNetDriver);
    DECLARE_DEFAULT_OBJECT(UNetDriver);
};
