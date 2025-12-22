#pragma once

#include "pch.h"

class UNeon
{
public:
    bool bHasInitialized = false;
    bool bFreeBuilds = false;
    bool bOutpost = false;
    bool bProd = false;
    bool bDedicatedServer = false;
    bool bCreative = true;
    bool bLategame = false; /* dont set this unless u need to */
    int32 Port;
public:
    void Initialize();
    void ChangeState(const wchar_t *State);
    void PostHook();
    void UpdatePort(int32 P);
};

inline UNeon *GNeon = NULL; 