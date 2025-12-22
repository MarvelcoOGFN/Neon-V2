#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingFoundation.h"

#include "Engine/Runtime/Engine/Classes/LevelStreamingDynamic.h"
#include "Engine/Runtime/Engine/Classes/World.h"

void ABuildingFoundation::SetDynamicFoundationEnabled(FFrame& Stack)
{
    int32 bEnabledInt = 0;
    Stack.StepCompiledIn(&bEnabledInt);
    Stack.IncrementCode();
    bool bEnabled = bEnabledInt != 0;
    
    SetFoundationEnabledState(bEnabled ? EDynamicFoundationEnabledState::Enabled : EDynamicFoundationEnabledState::Disabled);
    GetDynamicFoundationRepData().EnabledState = (EDynamicFoundationEnabledState::Enabled);
    OnRep_DynamicFoundationRepData();

    struct {bool bEnabled;} Params{true};
    return callExecOG(this, "/Script/FortniteGame.BuildingFoundation.SetDynamicFoundationEnabled", SetDynamicFoundationEnabled, Params);
}

void ABuildingFoundation::SetDynamicFoundationTransform(FFrame& Stack)
{
    FTransform Transform;
    Stack.StepCompiledIn(&Transform);

    UE_LOG(LogTemp, Log, "Foundation: %s", GetFName().ToString().ToString().c_str());

    Set("BuildingFoundation", "DynamicFoundationTransform", Transform);
    GetStreamingData().SetFoundationLocation(Transform.Translation);
    GetDynamicFoundationRepData().Rotation = Transform.Rotation.ToRotator();
    GetDynamicFoundationRepData().Translation = Transform.Translation;
    OnRep_DynamicFoundationRepData();
    if (GetFName().ToString().ToString() == "Fortilla_Foundation_MANG")
    {
        for (auto &World : GetAdditionalWorlds())
        {
            bool Success = false;
            ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), UKismetStringLibrary::Conv_NameToString(World.SoftObjectPtr.ObjectID.AssetPathName), Transform.Translation, Transform.Rotation.ToRotator(), &Success, FString());
        }
    }
    
    return SetDynamicFoundationTransformOG(this, Stack);
}