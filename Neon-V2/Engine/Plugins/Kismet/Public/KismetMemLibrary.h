#pragma once

#include "pch.h"
#include <map>
#include <string>
#include <functional>

class UKismetMemLibrary
{
public:
    using FunctionType = void*(*)();     
    using OffsetFunctionType = uintptr_t(*)();
    
    static void Add(const TCHAR* Name, FunctionType Function);
    static void Add(const TCHAR* Name, OffsetFunctionType Function);
    
    static int GetOffset(UObject* Obj, const std::string& PropName) {
        int Offset = -1;
        for (UStruct* Struct = Obj->GetClass(); Struct;
            Struct = Struct->GetSuperStruct()) {
            auto FuncInfo = SDK::PropLibrary->GetPropertyByName(
                Struct->GetFName().ToString().ToString(), PropName);

            Offset = FuncInfo.Offset;

            if (Offset != -1)
                break;
            }
        return Offset;
    }
    
    static int GetOffsetStruct(const std::string& StructName, const std::string& PropName) {
        int Offset = -1;
        for (UStruct* Struct = (UStruct*)GUObjectArray.FindObject(StructName); Struct;
            Struct = Struct->GetSuperStruct()) {
            auto FuncInfo = SDK::PropLibrary->GetPropertyByName(
                Struct->GetFName().ToString().ToString(), PropName);

            Offset = FuncInfo.Offset;

            if (Offset != -1)
                break;
            }
        return Offset;
    }
    
    template <typename T>
    static T Get(const TCHAR* Name)
    {
        auto offsetIt = CachedOffsets.find(Name);
        if (offsetIt != CachedOffsets.end())
        {
            if constexpr (std::is_pointer_v<T>)
            {
                return reinterpret_cast<T>(offsetIt->second);
            }
            else
            {
                return static_cast<T>(offsetIt->second);
            }
        }

        auto valueIt = CachedValues.find(Name);
        if (valueIt != CachedValues.end())
        {
            return reinterpret_cast<T>(valueIt->second);
        }

        auto offsetFuncIt = OffsetFunctionMap.find(Name);
        if (offsetFuncIt != OffsetFunctionMap.end())
        {
            uintptr_t result = offsetFuncIt->second();
            CachedOffsets[Name] = result;
            
            if constexpr (std::is_pointer_v<T>)
            {
                return reinterpret_cast<T>(result);
            }
            else
            {
                return static_cast<T>(result);
            }
        }

        auto funcIt = FunctionMap.find(Name);
        if (funcIt != FunctionMap.end())
        {
            void* result = funcIt->second();
            CachedValues[Name] = result;
            return reinterpret_cast<T>(result);
        }

        if constexpr (std::is_pointer_v<T>)
        {
            return nullptr;
        }
        else if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(-1);
        }
        else
        {
            return T{};
        }
    }
    
    template <typename T>
    static T* GetAddress(const TCHAR* Name)
    {
        uintptr_t address = Get<uintptr_t>(Name); 
        if (address == static_cast<uintptr_t>(-1))
            return nullptr;
        return reinterpret_cast<T*>(address); 
    }

    static void LoadCache();

private:
    static std::map<std::wstring, std::function<void*()>> FunctionMap;
    static std::map<std::wstring, std::function<uintptr_t()>> OffsetFunctionMap; 
    static std::map<std::wstring, void*> CachedValues;
    static std::map<std::wstring, uintptr_t> CachedOffsets; 
};

#define DEFINE_OFFSET(Name) \
uintptr_t Name##_Function(); \
static auto Name##Initializer = []() { \
UKismetMemLibrary::Add(TEXT(#Name), Name##_Function); \
return 0; \
}(); \
uintptr_t Name##_Function()