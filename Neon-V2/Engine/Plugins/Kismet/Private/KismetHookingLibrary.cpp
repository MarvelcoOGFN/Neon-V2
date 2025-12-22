#include "pch.h"
#include "Engine/Plugins/Kismet/Public/KismetHookingLibrary.h"

#include "Minhook.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/Misc.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/UObjectGlobals.h"

void UKismetHookingLibrary::Hook(UHook* Hook, EHook Type)
{
    switch (Type)
    {
    case EHook::StaticVFT:
        {
            UObject* DefaultObj = Hook->Class->GetClassDefaultObject();
            auto VTable = DefaultObj->GetVTable();
    
            UFunction* Function = DefaultObj->GetFunction(Hook->Path);
            if (!Function)
            {
                UE_LOG(LogKismet, Log, "UKismetHookingLibrary::Hook | EHook::StaticVFT: Failed to find function %s", Hook->Path.c_str());
                return;
            }
            
            int idx = GetVTableIndex(Function) / 8; 

            if (Hook->Original)
            {
                *Hook->Original = VTable[idx];
            }
            
            DWORD vpog;
            VirtualProtect(VTable + idx, 8, PAGE_EXECUTE_READWRITE, &vpog);
            VTable[idx] = Hook->Detour;
            VirtualProtect(VTable + idx, 8, vpog, &vpog);
            break;
        }
    case EHook::EveryStaticVFT:
        {
            auto Objects = GetObjectsOfClass(Hook->Class);
            int32 Index = 0;
            
            for (auto& Obj : Objects)
            {
                void** VTable = Obj->GetVTable();
                if (!VTable)
                {
                    UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: Failed to get vtable for class!");
                    continue;
                }

                UFunction* Function = Obj->GetFunction(Hook->Path);
                if (!Function)
                {
                    UE_LOG(LogKismet, Log, "UKismetHookingLibrary::Hook | EHook::StaticVFT: Failed to find function %s", Hook->Path.c_str());
                    continue;
                }
            
                int idx = GetVTableIndex(Function) / 8;
                Index = idx;

                if (Hook->Original)
                {
                    *Hook->Original = VTable[idx];
                }

                DWORD oldProtect;
                if (!VirtualProtect(&VTable[idx], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
                {
                    UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: VirtualProtect failed!");
                    continue;
                }

                VTable[idx] = Hook->Detour;

                DWORD temp;
                VirtualProtect(&VTable[idx], sizeof(void*), oldProtect, &temp);
            }

            UE_LOG(LogKismet, Log, "UKismetHookingLibrary::Hook | EHook::EveryStaticVFT: Hooked %s, Idx: 0x%x", Hook->Path.c_str(), Index);
        }
        break;
    case EHook::Exec:
        {
            UFunction* Func = StaticLoadObject<UFunction>(Hook->Path);
            if (Func)
            {
                if (Hook->Original)
                {
                    *Hook->Original = Func->GetNativeFunc();
                }

                Func->SetNativeFunc(reinterpret_cast<FNativeFuncPtr>(Hook->Detour));

                UE_LOG(LogKismet, Log, "UKismetHookingLibrary::Hook | EHook::Exec: %s", Hook->Path.c_str());
            } else
            {
                UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::Exec: Failed to find function %s", Hook->Path.c_str());
            }
            break;
        }
    case EHook::Modify:
        {
            uint8_t *DetourAddr = (uint8_t *)Hook->Swap;
            uint8_t *instrAddr = (uint8_t *)Hook->Address;
            int64_t delta = (int64_t)(DetourAddr - (instrAddr + 5));
            auto addr = reinterpret_cast<int32_t *>(instrAddr + 1);
            DWORD dwProtection;
            VirtualProtect(addr, 4, PAGE_EXECUTE_READWRITE, &dwProtection);

            *addr = static_cast<int32_t>(delta);

            DWORD dwTemp;
            VirtualProtect(addr, 4, dwProtection, &dwTemp);
             
            MH_CreateHook((LPVOID)Hook->Swap, Hook->Detour, Hook->Original);
            MH_EnableHook((LPVOID)Hook->Swap);
            break;
        }
    case EHook::VFT:
        {
            UObject* DefaultObj = Hook->Class->GetClassDefaultObject();
            if (!DefaultObj)
            {
                UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: Invalid DefaultObj for class!");
                return;
            }

            void** vft = DefaultObj->GetVTable();
            if (!vft)
            {
                UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: Failed to get vtable for class!");
                return;
            }

            if (Hook->Original)
            {
                *Hook->Original = vft[Hook->Address];
            }

            DWORD oldProtect;
            if (!VirtualProtect(&vft[Hook->Address], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
            {
                UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: VirtualProtect failed!");
                return;
            }

            vft[Hook->Address] = Hook->Detour;

            DWORD temp;
            VirtualProtect(&vft[Hook->Address], sizeof(void*), oldProtect, &temp);

            break;
        }
    case EHook::EveryVFT:
        {
            for (auto& Obj : GetObjectsOfClass(Hook->Class))
            {
                void** vft = Obj->GetVTable();
                if (!vft)
                {
                    UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: Failed to get vtable for class!");
                    return;
                }

                if (Hook->Original)
                {
                    *Hook->Original = vft[Hook->Address];
                }

                DWORD oldProtect;
                if (!VirtualProtect(&vft[Hook->Address], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
                {
                    UE_LOG(LogKismet, Fatal, "UKismetHookingLibrary::Hook | EHook::VFT: VirtualProtect failed!");
                    return;
                }

                vft[Hook->Address] = Hook->Detour;

                DWORD temp;
                VirtualProtect(&vft[Hook->Address], sizeof(void*), oldProtect, &temp);
            }
            
            break;
        }
    case EHook::FTrue:
        {
            DWORD og;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), PAGE_EXECUTE_READWRITE, &og);
            *(uint8_t*)Hook->Address = 0xE8;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), og, &og);    
            break;
        }
    case EHook::Null:
        {
            if (Hook->Address == 0x0) return;
            DWORD og;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), PAGE_EXECUTE_READWRITE, &og);
            *(uint8_t*)Hook->Address = 0xC3;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), og, &og);    
            break;
        }
    case EHook::Op:
        {
            DWORD og;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), PAGE_EXECUTE_READWRITE, &og);
            *(uint8_t*)Hook->Address = 0x90;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), og, &og);    
            break;
        }
    case EHook::Byte:
        {
            DWORD og;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), PAGE_EXECUTE_READWRITE, &og);
            *(uint8_t*)Hook->Address = Hook->Byte;
            VirtualProtect(LPVOID(Hook->Address), sizeof(byte), og, &og);    
            break;
        }
    case EHook::RTrue:
        {
            MH_CreateHook(LPVOID(Hook->Address), RetTrue, nullptr);
            MH_EnableHook(LPVOID(Hook->Address));
            break;
        }
    case EHook::RFalse:
        {
            MH_CreateHook(LPVOID(Hook->Address), RetFalse, nullptr);
            MH_EnableHook(LPVOID(Hook->Address));
            break;
        }
    case EHook::Address:
        if (Hook->Address == 0x0) return;
        MH_CreateHook(LPVOID(Hook->Address), Hook->Detour, Hook->Original);
        MH_EnableHook(LPVOID(Hook->Address));
        break;
    default:
        break;
    }

    Hook->Address = 0x0;
    Hook->Class = nullptr;
    Hook->Path = "";
    Hook->Original = nullptr;
    Hook->Detour = nullptr;
    Hook->Swap = 0x0;
}