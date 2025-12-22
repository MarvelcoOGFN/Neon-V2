#include "pch.h"

#include "Engine/Plugins/Neon/Public/Neon.h"

void Main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    AllocConsole();
    FILE* f; (void)freopen_s(&f, "CONOUT$", "w", stdout); (void)freopen_s(&f, "CONOUT$", "w", stderr);
    
    UNeon* Neon = GNeon = new UNeon();
    Neon->ChangeState(TEXT("Loading"));
    Neon->Initialize();
}

bool DllMain(HMODULE hModule, int rsn, void*)
{
    if (rsn == DLL_PROCESS_ATTACH)
    {
       CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
    }
    
    return true;
}
