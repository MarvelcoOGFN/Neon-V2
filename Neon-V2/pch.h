// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cstring>
#include <string>
#include <thread>
#include <shellapi.h>
#include <random>
#include <Engine/Plugins/JSON/nlohmann.hpp>
using namespace nlohmann;
#include "FortSDK/FortSDK/include/SDK.h"
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
using namespace SDK;
using namespace std;
DEFINE_LOG_CATEGORY(LogPlayerController);
DEFINE_LOG_CATEGORY(LogBuildingActor);
DEFINE_LOG_CATEGORY(LogNeon);
DEFINE_LOG_CATEGORY(LogOther);
DEFINE_LOG_CATEGORY(LogTemp);
DEFINE_LOG_CATEGORY(LogGameMode);
DEFINE_LOG_CATEGORY(LogKismet);
DEFINE_LOG_CATEGORY(LogGameState);

struct FUniqueNetIdWrapper
{
public:
    uint8                                         Pad_18[0x1];                                       // 0x0000(0x0001)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FUniqueNetIdRepl final : public FUniqueNetIdWrapper
{
public:
    uint8                                         Pad_B8[0x17];                                      // 0x0001(0x0017)(Fixing Size After Last Property [ Dumper-7 ])
    TArray<uint8>                                 ReplicationBytes;                                  // 0x0018(0x0010)(ZeroConstructor, Transient, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
};

inline bool IsValidPointer(void* ptr) {
    return ptr != nullptr && 
           ptr != (void*)0xFFFFFFFFULL && 
           ptr != (void*)0xCCCCCCCCULL && 
           ptr != (void*)0xDDDDDDDDULL &&
           (uintptr_t)ptr > 0x10000;
}

#define IMAGEBASE uint64_t(GetModuleHandle(0))

static int RetTrue() { return 1; }
static int RetFalse() { return 0; }

static void ExecuteConsoleCommand(SDK::UObject* WorldContextObject, const SDK::FString& Command, SDK::UObject* Controller)
{
    static SDK::UFunction* Func = SDK::PropLibrary->GetFunctionByName("KismetSystemLibrary", "ExecuteConsoleCommand").Func;
    static SDK::UObject* Object = SDK::StaticClassImpl("KismetSystemLibrary")->GetClassDefaultObject();
    
    if (!Func) return;
    
    struct { SDK::UObject* WorldContextObject; SDK::FString Command; SDK::UObject* PlayerController; } 
    Params{ WorldContextObject, std::move(Command), Controller };
    
    Object->ProcessEvent(Func, &Params);
}

#define DefineUOriginal(_Rt, _Name, _ThisType, ...) \
static inline _Rt (*_Name##OG)(_ThisType, ##__VA_ARGS__); \
_Rt _Name(__VA_ARGS__);

#define DefineOriginal(_Rt, _Name, ...) _Rt (*_Name##OG)(##__VA_ARGS__); _Rt _Name(##__VA_ARGS__);
#define CREATE_HOOK(_FuncName, _HookVar, _LambdaParams, _CallParams) \
_HookVar->Detour = reinterpret_cast<void*>(+[]_LambdaParams { \
return a1->_FuncName _CallParams; \
});

struct PadHex10 { char Pad[0x10]; };
struct PadHex18 { char Pad[0x18]; };
struct PadHexA8 { char Pad[0xA8]; };
struct PadHexB0 { char Pad[0xB0]; };

static std::vector<std::wstring> args = []() {
    std::vector<std::wstring> result;
    LPWSTR* argv;
    int argc;

    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv) {
        for (int i = 0; i < argc; ++i) {
            result.emplace_back(argv[i]);
        }
        LocalFree(argv);
    }
    return result;
    }();

static void PostRequest(const std::wstring& host, const std::wstring& path, const std::string& body = "{}")
{
    HINTERNET s = WinHttpOpen(L"Flux/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!s) return;

    HINTERNET c = WinHttpConnect(s, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!c) { WinHttpCloseHandle(s); return; }

    HINTERNET r = WinHttpOpenRequest(c, L"POST", path.c_str(), NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (r)
    {
        WinHttpSendRequest(r, L"Content-Type: application/json\r\n", 0,
            (LPVOID)body.data(), (DWORD)body.size(), (DWORD)body.size(), 0);
        WinHttpReceiveResponse(r, NULL);
        WinHttpCloseHandle(r);
    }
    WinHttpCloseHandle(c);
    WinHttpCloseHandle(s);
}

static std::string GetRequest(const std::wstring& host, const std::wstring& path)
{
    std::string response;
    
    HINTERNET s = WinHttpOpen(L"Flux/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!s) return response;

    HINTERNET c = WinHttpConnect(s, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!c) { WinHttpCloseHandle(s); return response; }

    HINTERNET r = WinHttpOpenRequest(c, L"GET", path.c_str(), NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (r)
    {
        if (WinHttpSendRequest(r, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
            WinHttpReceiveResponse(r, NULL))
        {
            DWORD bytesAvailable = 0;
            while (WinHttpQueryDataAvailable(r, &bytesAvailable) && bytesAvailable > 0)
            {
                std::vector<char> buffer(bytesAvailable + 1, 0);
                DWORD bytesRead = 0;
                if (WinHttpReadData(r, buffer.data(), bytesAvailable, &bytesRead))
                {
                    response.append(buffer.data(), bytesRead);
                }
                else break;
            }
        }
        WinHttpCloseHandle(r);
    }
    WinHttpCloseHandle(c);
    WinHttpCloseHandle(s);
    
    return response;
}

#endif //PCH_H
