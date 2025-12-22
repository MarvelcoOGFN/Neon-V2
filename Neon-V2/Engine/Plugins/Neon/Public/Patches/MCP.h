#pragma once
#include "pch.h"

namespace MCP
{
    inline void (*SendRequestNowOG)(__int64 a1, __int64* a2, int a3);
    void SendRequestNow(__int64 a1, __int64* a2, int a3);
}