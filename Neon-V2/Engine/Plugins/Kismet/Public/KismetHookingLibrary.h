#pragma once

#include "pch.h"

enum EHook
{
    Exec,
    EveryVFT,
    VFT,
    StaticFindVft,
    StaticVFT,
    EveryStaticVFT,
    Address,
    Modify,
    Null,
    Op,
    RTrue,
    RFalse,
    FTrue,
    Byte,
};

class UHook
{
public:
    uintptr_t Address;
    uintptr_t Swap;
    std::string Path;
    uint32 Byte;
    void** Original = nullptr;
    void* Detour;
    UClass* Class;
};

class UKismetHookingLibrary
{
public:
static inline __int64 GetVTableIndex(UFunction* Function, bool bBreakWhenHitRet = false)
{
    if (!Function)
        return -1;

    auto NativeAddr = (__int64)Function->GetNativeFunc();
    auto FuncName = Function->GetFName().ToString().ToString();
    
    std::wstring ValidateWStr = std::wstring(FuncName.begin(), FuncName.end()) + L"_Validate";
    const wchar_t* ValidateWCStr = ValidateWStr.c_str();
    bool bHasValidateFunc = Memcury::Scanner::FindStringRef(ValidateWCStr, false).Get();
    bool bFoundValidate = !bHasValidateFunc;
    
    __int64 RetAddr = 0;

    auto h = [](__int64 callAddr) -> __int64 {
        std::string wtf = "";
        bool bFoundFirstNumber = false;

        for (__int64 z = (callAddr + 5); z != (callAddr + 1); z -= 1)
        {
            auto anafa = (int)(*(uint8_t*)z);
            auto asfk = anafa < 10 ? "0" + std::format("{:x}", anafa) : std::format("{:x}", anafa);

            if (*(uint8_t*)z == 0 ? bFoundFirstNumber : true)
            {
                wtf += asfk;
                bFoundFirstNumber = true;
            }
        }

        std::transform(wtf.begin(), wtf.end(), wtf.begin(), ::toupper);
        
        __int64 result = 0;
        __int64 base = 1;
        for (int i = wtf.length() - 1; i >= 0; i--)
        {
            char c = wtf[i];
            if (c >= '0' && c <= '9')
                result += (c - '0') * base;
            else if (c >= 'A' && c <= 'F')
                result += (c - 'A' + 10) * base;
            base *= 16;
        }
        
        return result;
    };

    for (int i = 0; i < 2000; i++)
    {
        uint8_t opcode1 = *(uint8_t*)(NativeAddr + i);
        uint8_t opcode2 = *(uint8_t*)(NativeAddr + i + 1);

        if (Fortnite_Version >= 19.00)
        {
            if (opcode1 == 0x41 && opcode2 == 0xFF)
            {
                bFoundValidate = true;
                continue;
            }
        }

        if ((opcode1 == 0xFF && opcode2 == 0x90) || (opcode1 == 0xFF && opcode2 == 0x93))
        {
            if (bFoundValidate)
            {
                return h(NativeAddr + i);
            }
            else
            {
                bFoundValidate = true;
                continue;
            }
        }

        if (opcode1 == 0x48 && opcode2 == 0xFF && *(uint8_t*)(NativeAddr + i + 2) == 0xA0)
        {
            if (bFoundValidate)
            {
                std::string wtf = "";
                bool bFoundFirstNumber = false;

                for (__int64 z = (NativeAddr + i + 6); z != (NativeAddr + i + 2); z -= 1)
                {
                    auto anafa = (int)(*(uint8_t*)z);
                    auto asfk = anafa < 10 ? "0" + std::format("{:x}", anafa) : std::format("{:x}", anafa);

                    if (*(uint8_t*)z == 0 ? bFoundFirstNumber : true)
                    {
                        wtf += asfk;
                        bFoundFirstNumber = true;
                    }
                }

                std::transform(wtf.begin(), wtf.end(), wtf.begin(), ::toupper);

                __int64 result = 0;
                __int64 base = 1;
                for (int j = wtf.length() - 1; j >= 0; j--)
                {
                    char c = wtf[j];
                    if (c >= '0' && c <= '9')
                        result += (c - '0') * base;
                    else if (c >= 'A' && c <= 'F')
                        result += (c - 'A' + 10) * base;
                    base *= 16;
                }
                
                return result;
            }
        }

        if (!RetAddr && opcode1 == 0xC3)
        {
            RetAddr = NativeAddr + i;
            if (bBreakWhenHitRet)
                break;
        }
    }

    __int64 functionAddy = 0;
    if (RetAddr)
    {
        int i = 0;
        for (__int64 CurrentAddy = RetAddr; CurrentAddy != NativeAddr && i < 2000; CurrentAddy -= 1, i++)
        {
            if (*(uint8_t*)CurrentAddy == 0xE8)
            {
                functionAddy = (CurrentAddy + 1 + 4) + *(int*)(CurrentAddy + 1);
                break;
            }
        }
    }

    return !functionAddy ? -1 : functionAddy;
}
    
    static void Hook(UHook* Hook, EHook Type = EHook::Address);
};