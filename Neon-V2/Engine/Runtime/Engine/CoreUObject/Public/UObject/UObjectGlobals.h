#pragma once
#include "pch.h"

#include "Engine/Plugins/Kismet/Public/KismetMemLibrary.h"

template <typename T>
inline T* StaticFindObject(const std::string& Path, UClass* Class = UObject::StaticClass()) {
    static auto fn = reinterpret_cast<
        SDK::UObject* (*)(SDK::UClass*, SDK::UObject*, const wchar_t*, bool)
    >(
        UKismetMemLibrary::Get<uintptr_t>(TEXT("StaticFindObject"))
    );
    if (!fn) UE_LOG(LogTemp, Fatal, "Failed to find StaticFindObject");
    
    SDK::UObject* obj = fn ? fn(Class, nullptr, std::wstring(Path.begin(), Path.end()).c_str(), false) : nullptr;
    return reinterpret_cast<T*>(obj);
}

template<typename T>
T* StaticLoadObject(std::string name) {
    static auto fn = (SDK::UObject* (*)(SDK::UClass*, SDK::UObject*, const TCHAR*, const TCHAR*, uint32_t, SDK::UObject*, bool)) (uint64_t(UKismetMemLibrary::Get<uintptr_t>(TEXT("StaticLoadObject"))));
    T* Object = StaticFindObject<T>(name);
    if (!Object) {
        auto Name = std::wstring(name.begin(), name.end()).c_str();
        UObject* BaseObject = fn(T::StaticClass(), nullptr, Name, nullptr, 0, nullptr, false);
        Object = static_cast<T*>(BaseObject);
    }
    return Object;
}

template<typename T>
T* StaticLoadObject(std::string name, UClass* ClassToLoad) {
    static auto fn = (SDK::UObject* (*)(SDK::UClass*, SDK::UObject*, const TCHAR*, const TCHAR*, uint32_t, SDK::UObject*, bool)) (uint64_t(UKismetMemLibrary::Get<uintptr_t>(TEXT("StaticLoadObject"))));
    T* Object = StaticFindObject<T>(name);
    if (!Object) {
        auto Name = std::wstring(name.begin(), name.end()).c_str();
        UObject* BaseObject = fn(ClassToLoad, nullptr, Name, nullptr, 0, nullptr, false);
        Object = static_cast<T*>(BaseObject);
    }
    return Object;
}

#define callExecOG(_Tr, _Pt, _Th, _ParamsStruct) ([&](){ \
auto _Fn = StaticFindObject<UFunction>(_Pt "." # _Th); \
if (_Fn && _Th##OG) { \
auto originalWrapper = [](UObject* Context, void* TheStack, void* const Z_Param__Result) { \
if (auto* TypedContext = Cast<std::remove_reference_t<decltype(*_Tr)>>(Context)) { \
FFrame& Stack = *static_cast<FFrame*>(TheStack); \
_Th##OG(TypedContext, Stack); \
} \
}; \
_Fn->SetNativeFunc(originalWrapper); \
_Tr->ProcessEvent(_Fn, &_ParamsStruct); \
\
auto currentWrapper = [](UObject* Context, void* TheStack, void* const Z_Param__Result) { \
if (auto* TypedContext = Cast<std::remove_reference_t<decltype(*_Tr)>>(Context)) { \
FFrame& Stack = *static_cast<FFrame*>(TheStack); \
TypedContext->_Th(Stack); \
} \
}; \
_Fn->SetNativeFunc(currentWrapper); \
} \
})()