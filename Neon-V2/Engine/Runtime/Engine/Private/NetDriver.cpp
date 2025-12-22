#include "pch.h"
#include "Engine/Runtime/Engine/Classes/NetDriver.h"
#include "Engine/Plugins/Kismet/Public/KismetMemLibrary.h"
#include "Engine/Plugins/Neon/Public/Events.h"
#include "Engine/Plugins/Neon/Public/Neon.h"
#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/UObjectGlobals.h"

void UNetDriver::TickFlush(UNetDriver* a1, float DeltaSeconds)
{
    TickFlushOG(a1, DeltaSeconds);

    static bool bStartedBus = false;
    static bool bPlayerJoined = false;
    static bool bTerminating = false;
    if (!bPlayerJoined)
    {
        if (a1->GetClientConnections().Num() > 0)
        {
            bPlayerJoined = true;
        }
    }
    if (bPlayerJoined && !bTerminating)
    {
        if (a1->GetClientConnections().Num() == 0)
        {
            bTerminating = true;
            std::thread([](){ 
                Sleep(7000);  
                TerminateProcess(GetCurrentProcess(), 0);
            }).detach();
        }
    }
    
    if (GetAsyncKeyState(VK_F5) & 0x1 && !bStartedBus) {
        bStartedBus = true;
        ExecuteConsoleCommand((UObject*)a1->GetWorld(), L"startaircraft", nullptr);
    }

    if (GetAsyncKeyState(VK_F6) & 1) Events::StartEvent();
    if (GetAsyncKeyState(VK_F12) & 0x1) GNeon->bFreeBuilds = !GNeon->bFreeBuilds;

    if (GetAsyncKeyState(VK_F7) & 0x1)
    {
        auto WorldManagerState = a1->GetWorld()->GetAuthorityGameMode()->GetGameState()->GetWorldManager()->GetWorldManagerState();
        auto WorldManagerRole = a1->GetWorld()->GetAuthorityGameMode()->GetGameState()->GetWorldManager()->GetRole();
        UE_LOG(LogTemp, Log, "%d %d", (int)WorldManagerState, (int)WorldManagerRole);
    }
    
    auto* ReplicationDriver = a1->GetReplicationDriver();
    if (!ReplicationDriver) return;
    static uint64 CachedResult = UKismetMemLibrary::Get<uint64>(L"FortReplicationGraph__ServerReplicateActors");

    static auto Func = reinterpret_cast<void(*)(UReplicationDriver*)>(
        ReplicationDriver->GetVTable()[CachedResult]
    );
    Func(ReplicationDriver);
}
