#pragma once
#include "pch.h"

#ifndef PLATFORM_CPU_ARM_FAMILY
#if (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64))
#define PLATFORM_CPU_ARM_FAMILY 1
#else
#define PLATFORM_CPU_ARM_FAMILY 0
#endif
#endif

#define PLATFORM_WEAKLY_CONSISTENT_MEMORY PLATFORM_CPU_ARM_FAMILY
#define FORCE_THREADSAFE_SHAREDPTRS PLATFORM_WEAKLY_CONSISTENT_MEMORY

enum class ESPMode { NotThreadSafe = 0, Fast = FORCE_THREADSAFE_SHAREDPTRS ? 1 : 0, ThreadSafe = 1 };

class FReferenceControllerBase {
public:
    FORCEINLINE explicit FReferenceControllerBase() : SharedReferenceCount(1), WeakReferenceCount(1) {}
    int32 SharedReferenceCount;
    int32 WeakReferenceCount;
};

template<ESPMode Mode> class FSharedReferencer { public: FReferenceControllerBase* ReferenceController; };
template<class ObjectType, ESPMode Mode> class TSharedRef { public: ObjectType* Object; FSharedReferencer<Mode> SharedReferenceCount; };

struct ITextData {};

class FText {
public:
    TSharedRef<ITextData, ESPMode::ThreadSafe> TextData;
    uint32 Flags;
};