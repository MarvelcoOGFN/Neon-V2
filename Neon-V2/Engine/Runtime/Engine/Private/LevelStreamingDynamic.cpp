#include "pch.h"
#include "Engine/Runtime/Engine/Classes/LevelStreamingDynamic.h"

ULevelStreamingDynamic* ULevelStreamingDynamic::LoadLevelInstance(
    UObject* WorldContextObject,
    const FString& LevelName,
    const FVector& Location,
    const FRotator& Rotation,
    bool* bOutSuccess,
    const FString& OptionalLevelNameOverride
)
{
    static SDK::UFunction* Func = nullptr;
    if (!Func)
    {
        const SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("LevelStreamingDynamic", "LoadLevelInstance");
        Func = Info.Func;
        if (!Func)
        {
            if (bOutSuccess) *bOutSuccess = false;
            return nullptr;
        }
    }

    struct 
    {
        UObject*                WorldContextObject;
        FString                 LevelName;
        FVector                 Location;
        FRotator                Rotation;
        bool                    bOutSuccess;
        uint8                   Pad[0x7]; 
        FString                 OptionalLevelNameOverride;
        ULevelStreamingDynamic* ReturnValue;
    } Params{};
    
    Params.WorldContextObject = WorldContextObject;
    Params.LevelName = LevelName;
    Params.Location = Location;
    Params.Rotation = Rotation;
    Params.OptionalLevelNameOverride = OptionalLevelNameOverride;

    SDK::StaticClassImpl("LevelStreamingDynamic")
        ->GetClassDefaultObject()
        ->ProcessEvent(Func, &Params);

    if (bOutSuccess)
        *bOutSuccess = Params.bOutSuccess;

    return Params.ReturnValue;
}
