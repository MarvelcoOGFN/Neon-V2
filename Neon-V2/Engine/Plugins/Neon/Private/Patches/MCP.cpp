#include "pch.h"
#include "Engine/Plugins/Neon/Public/Patches/MCP.h"

#include "Engine/Plugins/Kismet/Public/KismetMemLibrary.h"

void MCP::SendRequestNow(long long a1, long long* a2, int a3 /* a3 is net mode i believe? */)
{
    if (Engine_Version >= 4.23) return SendRequestNowOG(a1, a2, 3);
    
    static uint8 Offset = UKismetMemLibrary::Get<uint8>(L"HTTPRequestNetMode");
    *(DWORD*)(__int64(a2) + Offset) = (DWORD)3;
    
    return SendRequestNowOG(a1, a2, 3);
}
