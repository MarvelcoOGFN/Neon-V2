#include "pch.h"
#include <functional>

#include "Engine/Plugins/Kismet/Public/KismetMemLibrary.h"

#include "Engine/Plugins/Neon/Public/Neon.h"
#include "Engine/Runtime/Engine/Classes/NetDriver.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"

std::map<std::wstring, std::function<void*()>> UKismetMemLibrary::FunctionMap;
std::map<std::wstring, std::function<uintptr_t()>> UKismetMemLibrary::OffsetFunctionMap; 
std::map<std::wstring, void*> UKismetMemLibrary::CachedValues;
std::map<std::wstring, uintptr_t> UKismetMemLibrary::CachedOffsets; 

void UKismetMemLibrary::Add(const TCHAR* Name, FunctionType Function)
{
    FunctionMap[Name] = Function;
}

void UKismetMemLibrary::Add(const TCHAR* Name, OffsetFunctionType Function)
{
    OffsetFunctionMap[Name] = Function;
}

void UKismetMemLibrary::LoadCache()
{
    for (const auto& Entry : FunctionMap)
    {
        UE_LOG(LogTemp, Log, TEXT("%s: 0x%x"),
            Entry.first.c_str(),
            Get<uint32>(Entry.first.c_str()) - IMAGEBASE);
    }

    for (const auto& Entry : OffsetFunctionMap)
    {
        uint32 Offset = Get<uint32>(Entry.first.c_str());
        UE_LOG(LogTemp, Log, TEXT("%s: 0x%x"), Entry.first.c_str(), Offset - IMAGEBASE);
    }

    GNeon->ChangeState(TEXT("Preparing"));
}

DEFINE_OFFSET(GIsServer)
{
    if (Engine_Version >= FEngineVersion(4, 25, 0))
    {
        auto Addr = Memcury::Scanner::FindStringRef(L"AllowCommandletRendering");

        std::vector<std::vector<uint8_t>> BytesToCheck = {
            {0x88, 0x05},
            {0xC6, 0x05},
            {0x88, 0x1D},
            {0x44, 0x88}
        };
        int Picked = 0;
        int Skip = 2;

        for (int i = 0; i < 50; i++)
        {
            auto Curr = (uint8_t*)(Addr.Get() - i);
            for (auto& Bytes : BytesToCheck)
            {
                if (*Curr == Bytes[0]) {
                    bool Found = true;
                    for (int j = 1; j < Bytes.size(); j++)
                    {
                        if (*(Curr + j) != Bytes[j])
                        {
                            Found = false;
                            break;
                        }
                    }
                    if (Found) {
                        auto Relative = Bytes[0] == 0x44 ? 3 : 2;

                        if (Bytes[0] == 0x44 && *(Curr + 2) == 0x74) continue;

                        if (!Picked) Picked = Bytes[0];
                        else if (Picked != Bytes[0]) continue;

                        if (Skip > 0) {
                            Skip--;
                            continue;
                        }

                        auto Scanner = Memcury::Scanner(Curr);
                        return Bytes[0] == 0xC6 ? Scanner.RelativeOffset(Relative, 1).Get() + 1: Scanner.RelativeOffset(Relative).Get() + 1;
                    }
                }
            }
        }
    }

    return reinterpret_cast<uintptr_t>(
        Memcury::Scanner::FindStringRef(L"STAT_UpdateLevelStreaming", 0, false)
            .ScanFor({ 0x80, 0x3D }, false, 1)
            .RelativeOffset(2)
            .GetAs<uint8*>() + 1
    );
}

DEFINE_OFFSET(StartAircraftPhase)
{
    if (Engine_Version < 427) 
    {
        auto strRef = Memcury::Scanner::FindStringRef(L"STARTAIRCRAFT").Get();

        if (!strRef)
            return 0;

        int NumCalls = 0;

        for (int i = 0; i < 150; i++)
        {
            if (*(uint8_t*)(strRef + i) == 0xE8)
            {
                NumCalls++;

                if (NumCalls == 2) 
                {
                    return Memcury::Scanner(strRef + i).RelativeOffset(1).Get();
                }
            }
        }
    }
    else
    {
        auto StatAddress = Memcury::Scanner::FindStringRef(L"STAT_StartAircraftPhase").Get();

        for (int i = 0; i < 1000; i++)
        {
            if (*(uint8_t*)(uint8_t*)(StatAddress - i) == 0x48 && *(uint8_t*)(uint8_t*)(StatAddress - i + 1) == 0x8B && *(uint8_t*)(uint8_t*)(StatAddress - i + 2) == 0xC4)
            {
                return StatAddress - i;
            }
        }
    }

    return 0;
}

DEFINE_OFFSET(PickTeam)
{
    if (Engine_Version == 4.26)
    {
        auto testAddr = Memcury::Scanner::FindPattern("88 54 24 10 53 56 41 54 41 55 41 56 48 83 EC 60 4C 8B A1", false).Get();

        if (!testAddr)
            testAddr = Memcury::Scanner::FindPattern("88 54 24 10 53 55 56 41 55 41 ? 48 83 EC 70 48", false).Get();

        if (testAddr)
            return testAddr;
    }

    else if (Engine_Version == 5.00)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 88 54 24 10 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 70 45 33 ED 4D").Get(); 

    else if (Engine_Version >= 4.27) 
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 88 54 24 10 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 70 4C 8B A1").Get();

    if (Fortnite_Version == 7.20 || Fortnite_Version == 7.30)
        return Memcury::Scanner::FindPattern("89 54 24 10 53 56 41 54 41 55 41 56 48 81 EC").Get();

    auto Addr = Memcury::Scanner::FindStringRef(L"PickTeam for [%s] used beacon value [%d]", false, 0, Engine_Version >= 427);

    if (!Addr.Get())
        Addr = Memcury::Scanner::FindStringRef(L"PickTeam for [%s] used beacon value [%s]");

    return FindBytes(Addr, Fortnite_Version <= 4.1 ? std::vector<uint8_t>{ 0x48, 0x89, 0x6C } : std::vector<uint8_t>{ 0x40, 0x55 }, 1000, 0, true);
}

DEFINE_OFFSET(GIsClient)
{
    if (Engine_Version >= FEngineVersion(4, 25, 0))
    {
        auto Addr = Memcury::Scanner::FindStringRef(L"AllowCommandletRendering");

        std::vector<std::vector<uint8_t>> BytesToCheck = {
            {0x88, 0x05},
            {0xC6, 0x05},
            {0x88, 0x1D},
            {0x44, 0x88}
        };
        int Picked = 0;
        int Skip = 2;

        for (int i = 0; i < 50; i++)
        {
            auto Curr = (uint8_t*)(Addr.Get() - i);
            for (auto& Bytes : BytesToCheck)
            {
                if (*Curr == Bytes[0]) {
                    bool Found = true;
                    for (int j = 1; j < Bytes.size(); j++)
                    {
                        if (*(Curr + j) != Bytes[j])
                        {
                            Found = false;
                            break;
                        }
                    }
                    if (Found) {
                        auto Relative = Bytes[0] == 0x44 ? 3 : 2;

                        if (Bytes[0] == 0x44 && *(Curr + 2) == 0x74) continue;

                        if (!Picked) Picked = Bytes[0];
                        else if (Picked != Bytes[0]) continue;

                        if (Skip > 0) {
                            Skip--;
                            continue;
                        }

                        auto Scanner = Memcury::Scanner(Curr);
                        return Bytes[0] == 0xC6 ? Scanner.RelativeOffset(Relative, 1).Get() : Scanner.RelativeOffset(Relative).Get();
                    }
                }
            }
        }
    }

    return reinterpret_cast<uintptr_t>(
        Memcury::Scanner::FindStringRef(L"STAT_UpdateLevelStreaming", 0, false)
            .ScanFor({ 0x80, 0x3D }, false, 1)
            .RelativeOffset(2)
            .GetAs<uint8*>()
    );
}

DEFINE_OFFSET(SetState)
{
    if (Fortnite_Version <= 9.10) return Memcury::Scanner::FindPattern("4C 8B DC 55 53 56 57 41 56 49 8D 6B ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 33 F6 0F B6 FA").Get();
    return Memcury::Scanner::FindPattern("40 55 53 56 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 33 F6").Get();
}

DEFINE_OFFSET(SpawnInitialSafeZone)
{
    return Memcury::Scanner::FindPattern("48 8B C4 48 89 48 ? 57 48 81 EC ? ? ? ? 48 89 58 ? 48 8B F9").Get();
}

DEFINE_OFFSET(LoadPlayset)
{
    if (Fortnite_Version.Season() <= 7) return 0;

    if (Fortnite_Version.Season() <= 8 && Fortnite_Version >= 7) return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B B9").Get();
    
    return Memcury::Scanner::FindStringRef(L"UPlaysetLevelStreamComponent::LoadPlayset Error: no owner for %s")
    .ScanFor({ 0x48, 0x89, 0x5C }, false).Get();
}

DEFINE_OFFSET(StaticFindObject)
{
    if (Engine_Version >= 5.00)
    {
        auto addr = Memcury::Scanner::FindPattern("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 33 F6 4C 8B E1 45 0F B6 E9 49 8B F8 41 8B C6", false).Get();

        if (!addr)
            addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 4C 89 64 24 ? 55 41 55 41 57 48 8B EC 48 83 EC 60 45 8A E1 4C 8B E9 48 83 FA").Get(); // 20.00

        return addr;
    }

    if (Engine_Version >= 4.27 && Fortnite_Version.Season() >= 16) 
    {
        if (Fortnite_Version.Season() < 18)
        {
            if (Fortnite_Version == 16.50)
                return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60 45 33 ED 45 8A F9 44 38 2D ? ? ? ? 49 8B F8 48 8B F2 4C 8B E1").Get();
		
            return Memcury::Scanner::FindPattern("40 55 53 57 41 54 41 55 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85").Get();
        }
        else
            return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60 45 33 ED 45 8A F9 44 38 2D ? ? ? ? 49 8B F8 48 8B").Get();
    }
    
    return reinterpret_cast<uintptr_t>(
        Memcury::Scanner::FindStringRef(L"Illegal call to StaticFindObject() while serializing object data!", 0, false)
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<uint8*>()
    );
}

DEFINE_OFFSET(GetGameSessionClass)
{
    if (Fortnite_Version.Season() >= 17) return 0;
    return *Memcury::Scanner::FindPatterns({ "41 FF 90 ? ? ? ? 33 FF 48 8B D8", "41 FF 90 ? ? ? ? 48 8B F8 48 83 38 00", "? FF 90 ? ? ? ? 48 8B F8 48 39 18" })
        .AbsoluteOffset(3)
        .GetAs<uint32_t*>() / 8;
}

DEFINE_OFFSET(ToDeathCause)
{
    if (Engine_Version == 4.19)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 41 0F B6 F8 48 8B DA 48 8B F1 E8 ? ? ? ? 33 ED").Get();
    if (Engine_Version == 4.20)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 0F B6 FA 48 8B D9 E8 ? ? ? ? 33 F6 48 89 74 24").Get();
    if (Engine_Version == 4.21) // 5.1
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 0F B6 FA 48 8B D9 E8 ? ? ? ? 33").Get();
}

// THANK YOU NOVA
DEFINE_OFFSET(GetMaxTickRate)
{
    auto StringRef = Memcury::Scanner::FindStringRef(L"GETMAXTICKRATE");
    if (StringRef.IsValid())
    {
        auto VFI = *StringRef
            .ScanFor({ 0x48, 0x8B })
            .ScanFor({ 0xE8 })
            .RelativeOffset(1)
            .ScanFor({ 0xFF, 0x90 })
            .AbsoluteOffset(2)
            .GetAs<int*>() / 8;

        if (VFI >= 60 && VFI <= 100)
            return VFI;
        else
        {
            VFI = *StringRef
                .ScanFor({ 0xFF, 0x90 })
                .AbsoluteOffset(2)
                .GetAs<int*>() / 8;

            return VFI;
        }
    }

    return -1;
}

DEFINE_OFFSET(StaticLoadObject)
{
    auto sRef = Memcury::Scanner::FindStringRef(L"STAT_LoadObject").Get();
    uint64_t StaticLoadObject = 0;
    if (!sRef) {
        sRef = Memcury::Scanner::FindStringRef(L"Calling StaticLoadObject during PostLoad may result in hitches during streaming.").Get();
        for (int i = 0; i < 1000; i++) {
            auto Ptr = (uint8_t*)(sRef - i);
            if (*Ptr == 0x40 && *(Ptr + 1) == 0x55) {
                StaticLoadObject = uint64_t(Ptr);
                break;
            }
        }
    }
    else {
        for (int i = 0; i < 1000; i++) {
            auto Ptr = (uint8_t*)(sRef - i);

            if (*Ptr == 0x4c && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x4c) {
                StaticLoadObject = uint64_t(Ptr);
                break;
            }
            else if (*Ptr == 0x48 && *(Ptr + 1) == 0x8b && *(Ptr + 2) == 0xc4) {
                StaticLoadObject = uint64_t(Ptr);
                break;
            }
        }
    }

    return StaticLoadObject;    
}

DEFINE_OFFSET(GetNetMode)
{
    auto Addr = Memcury::Scanner::FindStringRef(L"PREPHYSBONES");
    auto BeginningFunction = FindBytes(Addr, { 0x40, 0x55 }, 1000, 0, true);

    for (int i = 0; i < 400; i++)
    {
        uint8_t* cur = (uint8_t*)(BeginningFunction + i);
        if (*cur == 0xE8 && *(cur - 1) != 0x8B && *(cur - 1) != 0xE7)
        {
            return Memcury::Scanner(BeginningFunction + i).RelativeOffset(1).Get();
        }
    }

    return Memcury::Scanner::FindPattern("48 83 EC ? 48 83 79 ? ? 74 ? B8").Get();
}

DEFINE_OFFSET(KickPlayer)
{
      if (Fortnite_Version >= 23.00)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F0 48 8B DA 48 85 D2 74 ? 48 83 BA").Get();
    if (Engine_Version == 4.16) // <1.8
        return Memcury::Scanner::FindPattern("40 53 56 48 81 EC ? ? ? ? 48 8B DA 48 8B F1 E8 ? ? ? ? 48 8B 06 48 8B CE").Get(); // postlogin
    if (Engine_Version == 4.19 || Engine_Version == 4.20) // 1.11 & 2.4.2 & 4.1 & 4.5
        return Memcury::Scanner::FindPattern("40 53 56 41 56 48 81 EC ? ? ? ? 45 33 F6 48 8B DA 44 89 B4 24 ? ? ? ? 48 8B F1 E8 ? ? ? ? 48 8B 06 48 8B CE FF 90 ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9 74").Get();
    if (Fortnite_Version == 18)
        return Memcury::Scanner::FindPattern("48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 60 20 55 41 56 41 57 48 8B EC 48 83 EC 60 48 83 65 ? ? 4C 8B F2 83 65 E8 00 4C 8B E1 83 65 EC").Get();
    if (Fortnite_Version == 19)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 60 48 8B FA 48 8B F1 E8").Get();
	if (Fortnite_Version >= 7 && Fortnite_Version < 16) // && instead of || ??
        return Memcury::Scanner::FindPattern("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 49 8B F0 48 8B DA 48 85 D2").Get();

    uint64 Ret = 0;

    auto Addr = Memcury::Scanner::FindStringRef(L"Validation Failure: %s. kicking %s", false, 0, Fortnite_Version >= 19);

    if (Addr.Get())
    {
        Ret = Addr.Get() ? FindBytes(Addr, { 0x40, 0x55 }, 1000, 0, true) : Ret;

        if (!Ret)
            Ret = Addr.Get() ? FindBytes(Addr, { 0x40, 0x53 }, 2000, 0, true) : Ret;
    }

    if (Ret)
        return Ret;

    auto Addr2 = Memcury::Scanner::FindStringRef(L"Failed to kick player"); // L"KickPlayer %s Reason %s"
    auto Addrr = Addr2.Get();

    for (int i = 0; i < 3000; i++)
    {
        if (*(uint8_t*)(uint8_t*)(Addrr - i) == 0x48 && *(uint8_t*)(uint8_t*)(Addrr - i + 1) == 0x89 && *(uint8_t*)(uint8_t*)(Addrr - i + 2) == 0x5C)
        {
            return Addrr - i;
        }

        if (Fortnite_Version >= 17)
        {
            if (*(uint8_t*)(uint8_t*)(Addrr - i) == 0x48 && *(uint8_t*)(uint8_t*)(Addrr - i + 1) == 0x8B && *(uint8_t*)(uint8_t*)(Addrr - i + 2) == 0xC4)
            {
                return Addrr - i;
            }
        }
    }

    return Memcury::Scanner::FindPattern("40 53 41 56 48 81 EC ? ? ? ? 48 8B 01 48 8B DA 4C 8B F1 FF 90").Get();
}

DEFINE_OFFSET(TickFlush)
{
    return reinterpret_cast<uintptr_t>(
        Memcury::Scanner::FindStringRef(L"STAT_NetTickFlush")
        .ScanFor({ 0x4C, 0x8B, 0xDC }, false)
        .GetAs<uint8*>()
     );
}

DEFINE_OFFSET(StartNewSafeZonePhase)
{
    if (Fortnite_Version.Season() <= 9 && Fortnite_Version >= 8.30)
        return Memcury::Scanner::FindPattern("89 54 24 ? 48 89 4C 24 ? 53 56 57 41 56 41 57 48 81 EC ? ? ? ? 4C 8B B9").Get();
    if (Engine_Version == 4.19)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 70 48 8B B9 ? ? ? ? 33 DB 0F 29 74 24 ? 48 8B F1 48 85 FF 74 2C E8").Get(); // 1.11
    if (Engine_Version == 4.20)
        return Memcury::Scanner::FindPattern("E8 ? ? ? ? EB 31 80 B9 ? ? ? ? ?").RelativeOffset(1).Get(); // 3.5
    if (Fortnite_Version.Season() >= 7 && Fortnite_Version.Season() <= 8) // intentional, 8.00 has the same pattern.
        return Memcury::Scanner::FindPattern("E9 ? ? ? ? 48 8B C1 40 38 B9").RelativeOffset(1).Get(); // 7.40
    if (Engine_Version == 4.23)
        return Memcury::Scanner::FindPattern("E8 ? ? ? ? EB 42 80 BA").RelativeOffset(1).Get(); // doesnt work
    
    auto Addr = Memcury::Scanner::FindStringRef(
        L"FortGameModeAthena: No MegaStorm on SafeZone[%d].  GridCellThickness is less than 1.0.",
        true, 0, Engine_Version >= 427
    ).Get();

    if (!Addr) return 0;

    for (int i = 0; i < 100000; i++) {
        uint8_t* p = (uint8_t*)(Addr - i);

        if ((p[0] == 0x40 && (p[1] == 0x53 || p[1] == 0x55)) ||
            (Fortnite_Version < 8 && p[0] == 0x48 && p[1] == 0x89 && p[2] == 0x5C) ||
            (p[0] == 0x48 && p[1] == 0x8B && p[2] == 0xC4))
            return Addr - i;
    }
}

DEFINE_OFFSET(HTTPRequestNetMode)
{
    if (Engine_Version >= 4.23)
        return -1;

    if (Engine_Version >= 4.22)
        return 0x28;

    return (uint32)*Memcury::Scanner::FindStringRef(L"MCP-Profile: Dispatching request to %s")
        .ScanFor({ 0x8B, 0x4F })
        .AbsoluteOffset(2)
        .GetAs<uint8*>();
}

DEFINE_OFFSET(PickupInit) // from reboot im lazy
{
    if (Engine_Version == 4.19)
        return Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 41 0F B6 E9").Get(); // 1.11
    if (Engine_Version == 4.20)
    {
        if (Fortnite_Version <= 3.3)
            return Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 41 0F B6 E9 49 8B F8 48 8B F1 0F 85 ? ? ? ? 48 83 7A").Get(); // 3.3

        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 20 80 B9 ? ? ? ? ? 45 0F B6 F1 49 8B E8").Get(); // 4.1
    }
    if (Engine_Version == 4.21)
    {
        auto addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 55 57 41 57 48 83 EC 30 80 B9 ? ? ? ? ? 41 0F B6", false).Get(); // 6.21

        if (!addr)
            addr = Memcury::Scanner::FindPattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 41 0F B6 E9").Get(); // 5.41

        return addr;
    }
    if (Engine_Version == 4.22)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 41 56 41 57 48 83 EC 30 80 B9 ? ? ? ? ? 45 0F B6 F1").Get(); // 7.30
    if (Engine_Version == 4.23)
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 41 56 41 57 48 83 EC 30 80 B9 ? ? ? ? ? 45 0F B6 F1 4D").Get(); // 8.51 & 10.40
}

DEFINE_OFFSET(SpawnActor)
{
    return Memcury::Scanner::FindPattern("40 53 56 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 0F 28 1D ? ? ? ? 0F 57 D2 48 8B B4 24").Get();
}

DEFINE_OFFSET(SetEditingPlayer)
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 48 8B FA 48 8B D9 75 ? 48 83 B9").Get();
}

DEFINE_OFFSET(RegisterComponentWithWorld)
{
    auto Addr = Memcury::Scanner::FindPattern("48 83 EC ? 44 0F B6 05 ? ? ? ? 48 8D 05 ? ? ? ? 48 89 5C 24 ? 48 8B D9 48 89 6C 24", false).Get();;
    if (Addr)
        return Addr;
    
    return Memcury::Scanner::FindPattern("48 83 EC ? 44 0F B6 0D ? ? ? ? 48 8D 05 ? ? ? ? 48 89 5C 24", false).Get();
}

DEFINE_OFFSET(CreateAndConfigureNavigationSystem)
{
    if (Fortnite_Version.Season() >= 14) return Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? F6 41 ? ? 48 8B DA 48 8B F1 75").Get();
    if (Fortnite_Version.Season() <= 11) return Memcury::Scanner::FindPattern("40 55 53 57 48 8B EC 48 83 EC ? F6 41").Get();
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 56 48 83 EC ? F6 41 ? ? 48 8B DA", false).Get();
}

DEFINE_OFFSET(BotManagerSetup)
{
    if (Fortnite_Version == 13.40) return IMAGEBASE + 0x1fb6070;
    
    if (Fortnite_Version >= 12.50 && Fortnite_Version <= 13.30)
    {
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 45 33 E4").Get();
    }
    
    if (Fortnite_Version <= 12.41 && Fortnite_Version >= 12.00)
    {
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 55 41 57 48 8B EC 48 83 EC ? 45 33 E4").Get();
    }

    auto StringRef = Memcury::Scanner::FindStringRef("Can't configure player bots if no GameMode set");

    return StringRef.Get() ? FindBytes(StringRef, { 0x40, 0x55 }, 1000, 0, true) : 0;
}

DEFINE_OFFSET(SpawnBot)
{
    if (Fortnite_Version < 12.00 || Fortnite_Version.Season() >= 14)
        return 0;
    
    auto String = Memcury::Scanner::FindStringRef(L"UFortServerBotManagerAthena::SpawnBot invalid spawn location", true, 0, false).Get();

    for (int i = 0; i < 1000; i++)
    {
        if (*(uint8_t*)(String - i) == 0x48 && *(uint8_t*)(String - i + 1) == 0x8b && *(uint8_t*)(String - i + 2) == 0xc4)
        {
            return String - i;
        }
    }
}

DEFINE_OFFSET(SpawnBotRet)
{
    if (Fortnite_Version == 12.61)
    {
        return IMAGEBASE + 0x1aaa8df;
    }

    if (Fortnite_Version == 13.40) return IMAGEBASE + 0x20263FF;
    
    auto strRef = Memcury::Scanner::FindStringRef(L"Unable to create UFortAthenaAIBotCustomizationData object. BotClass = %s", true, 0, Fortnite_Version >= 19.00).Get();

    for (int i = 0; i < 1000; i++)
    {
        if ((uint8_t)(strRef + i) == 0x48 && (uint8_t)(strRef + i + 1) == 0x8b && (uint8_t)(strRef + i + 2) == 0xd8)
        {
            return strRef + i;
        }
    }
}

DEFINE_OFFSET(CompletePickupAnimation)
{
 static uint64 CachedResult = 0;
    if (CachedResult != 0)
        return CachedResult;
    
    if (Fortnite_Version >= 1.7 && Fortnite_Version < 2.5)
        return CachedResult = Memcury::Scanner::FindPattern("4C 8B DC 53 55 56 48 83 EC 60 48 8B F1 48 8B 89 ? ? ? ? 48 85 C9").Get();
    
    if (Fortnite_Version >= 2.5 && Fortnite_Version < 5.0) {
        auto addy = Memcury::Scanner::FindPattern("4C 8B DC 53 55 56 48 83 EC 60 48 8B F1 48 8B 89", false).Get();
        if (!addy)
            addy = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 85 C9 74 20 48 8D 44 24").Get();
        return CachedResult = addy;
    }
    
    if (Fortnite_Version >= 5.0 && Fortnite_Version < 7.0) {
        auto adda = Memcury::Scanner::FindPattern("40 53 56 48 83 EC 38 4C 89 6C 24 ? 48 8B F1 4C 8B A9", false).Get();
        if (!adda)
            adda = Memcury::Scanner::FindPattern("40 53 56 57 48 83 EC 30 4C 89 6C 24 ? 48 8B F1 4C 8B A9 ? ? ? ? 4D 85 ED 0F 84").Get();
        return CachedResult = adda;
    }
    
    if (Fortnite_Version >= 7.0 && Fortnite_Version < 8.0)
        return CachedResult = Memcury::Scanner::FindPattern("40 53 56 57 48 83 EC 30 4C 89 6C 24 ? 48 8B F1 4C 8B A9 ? ? ? ? 4D 85 ED 0F 84").Get();
    
    if (Fortnite_Version >= 8.0 && Fortnite_Version < 17.0)
    {
        CachedResult = Memcury::Scanner::FindPattern("40 53 56 48 83 EC 38 4C 89 6C 24 ? 48 8B F1 4C 8B A9 ? ? ? ? 4D 85 ED").Get();
        if (CachedResult == 0)
        {
            return CachedResult == Memcury::Scanner::FindPattern("40 53 56 48 83 EC ? 4C 89 6C 24 ? 48 8B F1 4C 8B A9").Get();    
        }
        else {
            return CachedResult;
        }
    }
    
    if (Fortnite_Version >= 17.0 && Fortnite_Version < 19.0) {
        auto sig = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 20 48 8B B1 ? ? ? ? 48 8B D9 48 85 F6", false).Get();
        
        if (!sig)
            sig = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B B9 ? ? ? ? 48 8B D9 48 85 FF 74 16 48 89", false).Get();
        if (!sig)
            sig = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 10 48 89 68 18 57 48 83 EC 20 48 8B D9 48 8B 89 ? ? ? ? 48 85").Get();
        return CachedResult = sig;
    }
    
    if (Fortnite_Version >= 19.0) {
        auto addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B B9", false).Get();
        if (!addr)
            addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 48 8D AC 24 ? ? ? ? 48 81 EC A0 01 00 00", false).Get();
        if (!addr)
            addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B B9 ? ? ? ? 45 33 E4 48 8B D9 48 85 FF 74 0F").Get();
        
        return CachedResult = addr;
    }    
}

DEFINE_OFFSET(RemoveFromAlivePlayers)
{
    static uint64 CachedResult = 0;
    if (CachedResult) return CachedResult;

    auto Addrr = Memcury::Scanner::FindStringRef(
        L"FortGameModeAthena: Player [%s] removed from alive players list (Team [%d]).  Player count is now [%d].  Team count is now [%d].", false).Get();

    if (!Addrr)
        Addrr = Memcury::Scanner::FindStringRef(
            L"FortGameModeAthena: Player [%s] removed from alive players list (Team [%d]).  Player count is now [%d]. PlayerBots count is now [%d]. Team count is now [%d].", false).Get();

    if (!Addrr)
        Addrr = Memcury::Scanner::FindStringRef(
            L"FortGameModeAthena::RemoveFromAlivePlayers: Player [%s] PC [%s] removed from alive players list (Team [%d]).  Player count is now [%d]. PlayerBots count is now [%d]. Team count is now [%d].",
            true, 0, Fortnite_Version.Season() >= 16 && Fortnite_Version.Season() < 24).Get();

    auto match = [](uint8_t* p, std::initializer_list<uint8_t> bytes) {
        for (size_t i = 0; i < bytes.size(); i++)
            if (*(p + i) != *(bytes.begin() + i)) return false;
        return true;
    };

    for (int i = 0; i < 2000; i++) {
        uint8_t* p = (uint8_t*)(Addrr - i);

        if (match(p, {0x4C, 0x89, 0x4C}))                return CachedResult = (uint64)p;
        if (match(p, {0x48, 0x89, 0x54})) {
            for (int z = 3; z < 50; z++) {
                if (match(p - z, {0x4C, 0x89, 0x4C}))    return CachedResult = (uint64)(p - z);
            }
            return CachedResult = (uint64)p;
        }
        if (match(p, {0x48, 0x8B, 0xC4}))                return CachedResult = (uint64)p;
    }

    return CachedResult = 0;
}

DEFINE_OFFSET(CanActivateAbility)
{
    if (Fortnite_Version <= 4.5 && Fortnite_Version >= 3.0)
    {
        return Memcury::Scanner::FindPattern("4C 89 4C 24 ? 89 54 24 ? 55 53 57").Get();
    }
    
    auto Addrr = Memcury::Scanner::FindStringRef(L"CanActivateAbility %s failed, blueprint refused", true, 0, Engine_Version >= 500).Get();

    if (Addrr)
    {
        for (int i = 0; i < 2000; i++)
        {
            if (*(uint8_t*)(uint8_t*)(Addrr - i) == 0x48 && *(uint8_t*)(uint8_t*)(Addrr - i + 1) == 0x89 && *(uint8_t*)(uint8_t*)(Addrr - i + 2) == 0x5C)
            {
                return Addrr - i;
            }

            if (*(uint8_t*)(uint8_t*)(Addrr - i) == 0x48 && *(uint8_t*)(uint8_t*)(Addrr - i + 1) == 0x8B && *(uint8_t*)(uint8_t*)(Addrr - i + 2) == 0xC4)
            {
                return Addrr - i;
            }
        }
    }
}

DEFINE_OFFSET(ApplyCharacterCustomization)
{
    if (Fortnite_Version.Season() == 10) {
        auto Addr = Memcury::Scanner::FindPattern("48 89 54 24 ? 55 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 80 B9 ? ? ? ? ? 48").Get();
        if (Addr) return Addr;
    }

    auto Addrr = Memcury::Scanner::FindStringRef(L"AFortPlayerState::ApplyCharacterCustomization - Failed initialization, using default parts. Player Controller: %s PlayerState: %s, HeroId: %s", false, 0, Fortnite_Version >= 20, true).Get();
    if (!Addrr) return 0;

    for (int i = 0; i < 7000; i++) {
        if (*(uint8_t*)(Addrr - i) == 0x40 && *(uint8_t*)(Addrr - i + 1) == 0x53) return Addrr - i;
        if (Fortnite_Version >= 15 && *(uint8_t*)(Addrr - i) == 0x48 && *(uint8_t*)(Addrr - i + 1) == 0x89) return Addrr - i;
        if (*(uint8_t*)(Addrr - i) == 0x48 && *(uint8_t*)(Addrr - i + 1) == 0x8B && *(uint8_t*)(Addrr - i + 2) == 0xC4) return Addrr - i;
    }

    uint64 Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 50 10 55 57 48 8D 68 A1 48 81 EC ? ? ? ? 80 B9").Get();
    return Addr;

}

DEFINE_OFFSET(ApplyCost)
{
    return Memcury::Scanner::FindPattern("40 55 56 57 41 55 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 41 0F 10 01").Get();
}

DEFINE_OFFSET(SendComplexCustomStatEvent)
{
    return Memcury::Scanner::FindPattern("4C 8B DC 49 89 5B ? 57 48 83 EC ? 33 C0 49 8D 53").Get();
}

DEFINE_OFFSET(SendStatEventWithTags)
{
    if (auto Sig = Memcury::Scanner::FindPattern(
            "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 49 8B F9 48 8B 49").Get();
        Sig && Fortnite_Version >= 11.00)
        return Sig;

    auto TryFind = [](auto String, std::initializer_list<std::initializer_list<uint8_t>> Patterns) -> uint64_t {
        if (String.IsValid()) {
            for (auto& Pat : Patterns) {
                if (auto Addr = FindBytes(String, Pat, 1000, 0, true); Addr && Addr != -1)
                    return Addr;
            }
        }
        return 0;
    };

    if (auto Addr = TryFind(
            Memcury::Scanner::FindStringRef(
                L"SendStatEventWithTags: Cannot be called, %s is no longer registered!", true, 0, false),
            { {0x4C,0x89,0x44,0x24}, {0x57,0x41,0x55,0x48,0x83,0xEC,0x78}, {0x48,0x89,0x5C}, {0x48,0x8B,0xC4} }))
        return Addr;

    if (auto Addr = TryFind(
            Memcury::Scanner::FindStringRef(
                L"SendStatEventWithTags: Cannot be called on the client!", true, 0, false),
            { {0x48,0x89,0x5C} }))
        return Addr;

    return 0;
}

DEFINE_OFFSET(ClientOnPawnDied)
{
    auto Addr = Memcury::FindFunction(L"ClientOnPawnDied", std::vector<uint8_t>{ 0x48, 0x89, 0x5C });
    return Addr;
}

DEFINE_OFFSET(UseSpareAmmo)
{
    return Memcury::Scanner::FindStringRef(L"Weapon [%s] - Failed to find an Inventory Owner, we won't be able to delete the ammo out of the inventory!")
        .ScanFor({ 0x89, 0x54 }, false)
        .Get();    
}

DEFINE_OFFSET(HandleZiplineStateChanged)
{
    auto StrRef = Memcury::Scanner::FindStringRef(L"ZIPLINES!! Role(%s)  AFortPlayerPawn::ServerEndZiplining_Implementation bFromJump=%d", 0, false);

    if (!StrRef.IsValid())
        StrRef = Memcury::Scanner::FindStringRef(L"ZIPLINES!! GetLocalRole()(%s)  AFortPlayerPawn::ServerEndZiplining_Implementation bFromJump=%d", 0, false);

    if (!StrRef.IsValid())
        return NULL;

    return StrRef
        .ScanFor({ 0x74, 0x05 })
        .ScanFor({ 0xE8 }, true, 1)
        .RelativeOffset(1)
        .Get();
}

DEFINE_OFFSET(SpawnLoot)
{
    return Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? ? 48 8B D9 0F 82 ? ? ? ? 48 89 74 24 ? 48 8D 54 24 ? 45 33 C0").Get();   
}

DEFINE_OFFSET(FortReplicationGraph__ServerReplicateActors)
{
    int Season = static_cast<int>(std::floor(std::stod(Fortnite_Version.ToString())));
    const auto& FV = Fortnite_Version; 
    const auto& EV = Engine_Version;
        
    return EV == 4.20 ? 0x53 :
    (EV == 4.21) ? (Season == 5 ? 0x54 : 0x56) :
    (EV >= 4.22 && EV <= 4.24) ?
        (FV >= 7.40 && FV < 8.40) ? 0x57 :
        (EV == 4.24) ?
            (FV >= 11.00 && FV <= 11.10) ? 0x57 :
            ((FV == 11.30 || FV == 11.31) ? 0x59 : 0x5A)
        : 0x56
    : (Season == 12 || Season == 13) ? 0x5D :
      (Season == 14 || FV <= 15.2) ? 0x5E :
      (FV >= 15.3 && EV < 5.00) ? 0x5F :
      (Season >= 19 && Season <= 20) ? 0x66 :
      (Season >= 21) ? 0x67 : 0;
}

DEFINE_OFFSET(GetPlayerViewPoint)
{
    if (Engine_Version == 4.20)
    {
        return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 41 56 41 57 48 8B EC 48 83 EC ? 48 8B F2 48 C7 45 ? ? ? ? ? 48 8B 55").Get();
    }

    if (Fortnite_Version.Season() >= 6 && Fortnite_Version.Season() <= 9)
    {
        return Memcury::Scanner::FindPattern("48 89 74 24 ? 55 57 41 56 48 8B EC 48 83 EC ? 48 8B 81").Get();
    }
    
    auto strRef = Memcury::Scanner::FindStringRef(
      L"%s failed to spawn a pawn", 
      true, 
      0, 
      Fortnite_Version >= 19 && Fortnite_Version < 24
  ).Get();
    if (!strRef) return 0;

    uint64 addr = 0;
    for (int i = 0; i < 1000; i++) {
        auto cur = strRef - i;
        if ((*(uint8_t*)cur == 0x40 && *(uint8_t*)(cur + 1) == 0x53) ||
            (*(uint8_t*)cur == 0x48 && *(uint8_t*)(cur + 1) == 0x89 && *(uint8_t*)(cur + 2) == 0x5C)) {
            addr = cur;
            break;
            }
    }
    if (!addr) return 0;

    auto controllerDefault = AFortPlayerControllerAthena::StaticClass();
    void** vft = controllerDefault->GetVTable();

    int idx = 0;
    for (int i = 0; i < 500; i++) {
        if (vft[i] == (void*)addr) { idx = i; break; }
    }
    if (!idx) return 0;

    return (int64_t)vft[idx - 1];
}

DEFINE_OFFSET(ReplaceBuildingActor)
{
    auto StringRef = Memcury::Scanner::FindPattern("4C 8B DC 55 56 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B 85 ? ? ? ? 33 F6", false);
    if (StringRef.IsValid())
        return StringRef.Get();

    StringRef = Memcury::Scanner::FindStringRef(L"STAT_Fort_BuildingSMActorReplaceBuildingActor", 0, false);
    if (StringRef.IsValid())
    {
        return StringRef
            .ScanFor({ 0x55, 0x57 }, false)
            .ScanFor({ 0x8B }, false)
            .Get() - 1;
    }

    StringRef = Memcury::Scanner::FindPattern("4C 89 44 24 ? 55 56 57 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 45");
    if (StringRef.IsValid())
    {
        return StringRef
            .Get();
    }

    return NULL;
}

DEFINE_OFFSET(CantBuild)
{
    uint64 Result = 0;

    Result = Memcury::Scanner::FindPattern("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC ? 49 8B E9 4D 8B F0", false).Get();
    if (!Result)
        Result = Memcury::Scanner::FindPattern("48 89 54 24 ? 55 56 41 56 48 83 EC 50", false).Get(); // 4.20
    if (!Result)
        Result = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 4D 8B F1 4D 8B F8", false).Get(); // 4.26.1
    if (!Result)
        Result = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 60 49 8B E9 4D 8B F8 48 8B DA 48 8B F9 BE ? ? ? ? 48", false).Get(); // 5.00
    if (!Result)
        Result = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 70 49 8B E9 4D 8B F8 48 8B DA 48 8B F9").Get(); // 20.00

    return Result;
}

DEFINE_OFFSET(OnDamageServer)
{
    auto Addr = Memcury::FindFunction(L"OnDamageServer", 
    Fortnite_Version <= 1.9 ? std::vector<uint8_t>{ 0x4C, 0x89, 0x4C } : 
    Fortnite_Version <= 2.4 || Fortnite_Version >= 19.00 ? std::vector<uint8_t>{ 0x48, 0x8B, 0xC4 } : std::vector<uint8_t>{ 0x40, 0x55 }
);
    
    return Addr;
}

DEFINE_OFFSET(GiveAbility)
{
    Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("E8 ? ? ? ? 8B D3 49 8B CE E8 ? ? ? ? 48 8B 4D");
    if (!Scanner.Get())
        Scanner = Memcury::Scanner::FindPattern("E8 ? ? ? ? 8B 5D ? 48 8B CE", true);
    
    return Scanner.RelativeOffset(1).Get();
}

DEFINE_OFFSET(SendRequestNow)
{
    auto Scanner = Memcury::Scanner::FindStringRef(
        L"MCP-Profile: Dispatching request to %s",
        false,
        0,
        Fortnite_Version.Season() >= 19
    );

    if (!Scanner.IsValid())
    {
        Scanner = Memcury::Scanner::FindStringRef(
            L"MCP-Profile: Dispatching request to %s - ContextCredentials: %s",
            false,
            0,
            Fortnite_Version.Season() >= 19
        );
    }

    return Scanner.ScanFor({ 0x48, 0x89, 0x5C }, false).Get();
}

DEFINE_OFFSET(InternalTryActivateAbility)
{
    return Memcury::Scanner::FindStringRef(L"InternalTryActivateAbility called with invalid Handle! ASC: %s. AvatarActor: %s")
        .ScanFor({ 0x4C, 0x89, 0x4C }, false)
        .Get();
}

DEFINE_OFFSET(FindNamedNetDriver)
{
    if (Engine_Version >= 4.25)
    {
        return Memcury::Scanner::FindStringRef(L"RecordReplay: failed to create demo net driver!")
            .ScanFor({ 0x48, 0x8B, 0x0D })
            .ScanFor({ 0xE8 })
            .RelativeOffset(1)
        .Get();
    }

    return Memcury::Scanner::FindStringRef(L"RecordReplay: failed to create demo net driver!")
        .ScanFor({ 0x4C })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .Get();
}

DEFINE_OFFSET(Step)
{
    return Memcury::Scanner::FindPattern("48 8B 41 20 4C 8B D2 48 8B D1 44 0F B6 08 48 FF").Get();
}

DEFINE_OFFSET(StepExplicitProperty)
{
    return Memcury::Scanner::FindPattern("41 8B 40 ? 4D 8B C8").Get();
}

DEFINE_OFFSET(Listen)
{
    auto ListenInstruction = Memcury::Scanner::FindPattern("E8 ? ? ? ? 84 C0 75 ? 80 3D ? ? ? ? ? 72 ? 45 33 C0 48 8D 55").Get();
    return ListenInstruction ? ListenInstruction : Memcury::Scanner::FindPattern("E8 ? ? ? ? 84 C0 75 ? 80 3D ? ? ? ? ? 72 ? 45 33 C0 48 8D 95").Get();
}

DEFINE_OFFSET(ChangeGameSessionID)
{
    if (Fortnite_Version >= 23) return 0;
    return Memcury::Scanner::FindStringRef(L"Changing GameSessionId from '%s' to '%s'")
        .ScanFor({ 0x40, 0x55 }, false, 0, 1, 2000)
        .Get();
}

DEFINE_OFFSET(GetWorldContextFromObject)
{
    Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 85 C0 74 ? 4C 63 B0");
    if (Scanner.Get())
        return Scanner.RelativeOffset(1).Get();
}

DEFINE_OFFSET(CreateNetDriver)
{
    if (Fortnite_Version >= 16.00) return 0;
    auto CreateNetDriver = Memcury::Scanner::FindPattern("49 8B D8 48 8B F9 E8 ?? ?? ?? ?? 48 8B D0 4C 8B C3 48 8B CF 48 8B 5C 24 ?? 48 83 C4 ?? 5F E9 ?? ?? ?? ??").Get();
    if (!CreateNetDriver) {
        CreateNetDriver = Memcury::Scanner::FindPattern("48 8B D9 E8 ?? ?? ?? ?? 4C 8B 44 24 ?? 48 8B D0 48 8B CB E8 ?? ?? ?? ?? 48 83 C4 ?? 5B C3").Get();
        if (!CreateNetDriver) CreateNetDriver = Memcury::Scanner::FindPattern("33 D2 E8 ?? ?? ?? ?? 48 8B D0 4C 8B C3 48 8B CF E8 ?? ?? ?? ?? 48 8B 5C 24 ?? 48 83 C4 ?? 5F C3").Get();
    }
    for (int i = 0; i < 50; i++) {
        auto Ptr = (uint8_t*)(CreateNetDriver - i);

        if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x5c) {
            CreateNetDriver = uint64_t(Ptr);
            return CreateNetDriver;
        }
        else if (*Ptr == 0x4C && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x44) {
            CreateNetDriver = uint64_t(Ptr);
            return CreateNetDriver;
        }
    }
    
    return 0;
}

DEFINE_OFFSET(CreateNamedNetDriver)
{
    return Memcury::Scanner::FindStringRef(L"RecordReplay: failed to create demo net driver!")
        .ScanFor({ 0x84, 0xC0 }, false)
        .ScanFor({ 0xE8 }, false)
        .RelativeOffset(1)
        .Get();
}

DEFINE_OFFSET(InitListen)
{
    if (Fortnite_Version >= 19.00)
        return Memcury::Scanner::FindPattern(
            "4C 8B DC 49 89 5B 10 49 89 73 18 57 48 83 EC 50 48 8B BC 24 ?? ?? ?? ?? 49 8B F0 48 8B"
        ).Get();

    if (Fortnite_Version >= 16.40)
        return Memcury::Scanner::FindPattern(
            "4C 8B DC 49 89 5B 08 49 89 73 10 57 48 83 EC 50 48 8B BC 24 ? ? ? ? 49 8B F0 48 8B 01 48 8B"
        ).Get();

    if (Fortnite_Version >= 13.40 && Fortnite_Version <= 15.50)
        return Memcury::Scanner::FindPattern(
            "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B BC 24 ? ? ? ? 49 8B F0"
        ).Get();

    auto Addr = Memcury::Scanner::FindStringRef(L"%s IpNetDriver listening on port %i");
    return FindBytes(
        Addr,
        Fortnite_Version < 18.40 ? std::vector<uint8_t>{0x48, 0x89, 0x5C} : std::vector<uint8_t>{0x4C, 0x8B, 0xDC},
        2000, 0, true, 1
    );
}

DEFINE_OFFSET(AActor_GetNetMode)
{
    std::vector<std::string> Patterns = { "E8 ? ? ? ? 8D 48 FE", "E8 ? ? ? ? 44 8B F8 41 83 FF 03", "E8 ? ? ? ? 83 F8 ? 7C ? FF C7" };
    for (const auto& Pattern : Patterns)
    {
        auto Result = Memcury::Scanner::FindPattern(Pattern.c_str()).RelativeOffset(1).Get();
        if (Result)
            return Result;
    }
    return 0;
}

DEFINE_OFFSET(SetWorld)
{
    int Season = (int)std::floor(std::stod(Fortnite_Version.ToString()));

    if (Fortnite_Version <= 13.00)
        return Memcury::Scanner::FindStringRef(L"AOnlineBeaconHost::InitHost failed")
            .ScanFor({ 0x48, 0x8B, 0xD0, 0xE8 }, false).RelativeOffset(4).Get();

    if (Season == 14)
        return Memcury::Scanner::FindPattern("40 55 56 41 56 48 8B EC 48 83 EC ? 48 89 5C 24").Get();

    if (Fortnite_Version >= 23.00)
        return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8D 8B ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 85 FF 0F 84 ? ? ? ? 83 64 24").RelativeOffset(1).Get();

    int VTIndex = 0;
    if (Season == 13) VTIndex = 0x70;
    else if (Season == 14 || Fortnite_Version <= 15.2) VTIndex = 0x71;
    else if (Fortnite_Version >= 15.3 && Season < 18) VTIndex = 0x72;
    else if (Season == 18) VTIndex = 0x73;
    else if (Season >= 19 && Season < 21) VTIndex = 0x7A;
    else if (Season == 20) VTIndex = 0x7B;
    else if (Season >= 21) VTIndex = 0x7C;

    if (VTIndex)
        return uint64(UNetDriver::GetDefaultObj()->GetVTable()[VTIndex]);

    if (Engine_Version >= 4.26)
        return Memcury::Scanner::FindPattern("40 55 56 41 56 48 8B EC 48 83 EC ? 48 89 5C 24", true).Get();

    return Memcury::Scanner::FindStringRef(L"AOnlineBeaconHost::InitHost failed", true)
        .ScanFor({ 0xE8 }, false, 1).RelativeOffset(1).Get();
}