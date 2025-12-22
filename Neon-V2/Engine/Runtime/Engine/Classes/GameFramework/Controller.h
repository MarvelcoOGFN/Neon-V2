#pragma once
#include "pch.h"

#include "Actor.h"
#include "Pawn.h"
#include "Engine/Runtime/Engine/Core/Public/Math/Rotator.h"
#include "Engine/Runtime/Engine/Core/Public/Math/Vector.h"

class AController : public AActor
{
public:
    DEFINE_MEMBER(FName, AController, StateName)
    DEFINE_PTR(APlayerState, AController, PlayerState);
    DEFINE_PTR(APawn, AController, Pawn);
public:
    void OnRep_Pawn() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("Controller", "OnRep_Pawn").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }

    void UnPossess() {
        static SDK::UFunction* Func = nullptr;
        if (!Func) Func = SDK::PropLibrary->GetFunctionByName("Controller", "UnPossess").Func;
        if (Func) this->ProcessEvent(Func, nullptr);
    }
    
    FRotator GetControlRotation()
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("Controller", "GetControlRotation").Func;
        if (!Func) return FRotator();
        
        struct { FRotator ReturnValue; } Params{};
        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }
    
    AActor* GetViewTarget()
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("Controller", "GetViewTarget").Func;
        if (!Func) return nullptr;
        
        struct { AActor* ReturnValue; } Params{};
        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }
public:
    void SetControlRotation(const FRotator& NewRotation)
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("Controller", "SetControlRotation").Func;
        if (!Func) return;
        
        struct { FRotator NewRotation; } Params{ NewRotation };
        this->ProcessEvent(Func, &Params);
    }

    void ClientSetRotation(const FRotator& NewRotation, bool bResetCamera)
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("Controller", "ClientSetRotation").Func;
        if (!Func) return;
        
        struct { FRotator NewRotation; bool bResetCamera; } Params{ NewRotation, bResetCamera };
        this->ProcessEvent(Func, &Params);
    }

    void StopMovement()
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("Controller", "StopMovement").Func;
        if (!Func) return;
        
        this->ProcessEvent(Func, nullptr);
    }

    bool LineOfSightTo(const AActor* Other, const FVector& ViewPoint, bool bAlternateChecks) 
    {
        static auto Func = SDK::PropLibrary->GetFunctionByName("Controller", "LineOfSightTo").Func;
        if (!Func) return false;
        
        struct { const AActor* Other; FVector ViewPoint; bool bAlternateChecks; bool ReturnValue; uint8 Pad[2]; } 
        Params{ Other, ViewPoint, bAlternateChecks };
        
        this->ProcessEvent(Func, &Params);
        return Params.ReturnValue;
    }
};

class APlayerController : public AController
{
public:
    DEFINE_MEMBER(FRotator, APlayerController, LastSpectatorSyncRotation);
    DEFINE_MEMBER(FVector, APlayerController, LastSpectatorSyncLocation);
    DEFINE_BOOL(APlayerController, bPlayerIsWaiting)
    DEFINE_MEMBER(APawn*, APlayerController, AcknowledgedPawn);
public:
    DECLARE_STATIC_CLASS(APlayerController);
    DECLARE_DEFAULT_OBJECT(APlayerController);
};