#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Classes/NetSerialization.h"
#include "Engine/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Runtime/Engine/Core/Public/Math/Vector.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Runtime/FortniteGame/Public/FortGameStateZone.h"

class UFortPlaylistAthena;

struct FPlaylistPropertyArray : public FFastArraySerializer
{
public:
    DEFINE_PTR(UFortPlaylistAthena, FPlaylistPropertyArray, BasePlaylist)
    DEFINE_PTR(UFortPlaylistAthena, FPlaylistPropertyArray, OverridePlaylist)
    DEFINE_MEMBER(int32, FPlaylistPropertyArray, PlaylistReplicationKey)
public:
    DECLARE_STATIC_CLASS(FPlaylistPropertyArray)
};

struct FAircraftFlightInfo final
{
public:
    struct FVector_NetQuantize100 FlightStartLocation; // 0x0000(0x000C)(Edit, BlueprintVisible, BlueprintReadOnly, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    struct FRotator FlightStartRotation;               // 0x000C(0x000C)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
    float FlightSpeed;                                 // 0x0018(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float TimeTillFlightEnd;                           // 0x001C(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float TimeTillDropStart;                           // 0x0020(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
    float TimeTillDropEnd;                             // 0x0024(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

class AFortAthenaAircraft : public AActor
{
public:
    DEFINE_MEMBER(FRotator, AFortAthenaAircraft, CameraInitialRotation)
    
    DEFINE_MEMBER(FAircraftFlightInfo, AFortAthenaAircraft, FlightInfo)
        DEFINE_MEMBER(float, AFortAthenaAircraft, DropEndTime)
        DEFINE_MEMBER(float, AFortAthenaAircraft, DropStartTime)
        
    DEFINE_MEMBER(float, AFortAthenaAircraft, FlightStartTime)
    DEFINE_MEMBER(float, AFortAthenaAircraft, FlightEndTime)
public:
    DECLARE_STATIC_CLASS(AFortAthenaAircraft)
    DECLARE_DEFAULT_OBJECT(AFortAthenaAircraft)
};

class AFortAthenaMapInfo : public AActor
{
public:
    DEFINE_MEMBER(FScalableFloat, AFortAthenaMapInfo, AircraftDesiredDoorOpenTime);
    DEFINE_MEMBER(__int64, AFortAthenaMapInfo, SafeZoneDefinition);
    DEFINE_MEMBER(TArray<struct FAircraftFlightInfo>, AFortAthenaMapInfo, FlightInfos);

public:
    DECLARE_STATIC_CLASS(AFortAthenaMapInfo);
    DECLARE_DEFAULT_OBJECT(AFortAthenaMapInfo)
};

struct FAdditionalLevelStreamed final
{
public:
    class FName LevelName;
    bool bIsServerOnly;
    uint8 Pad_1B33[0x3];
};

enum class EAthenaGamePhase : uint8
{
    None = 0,
    Setup = 1,
    Warmup = 2,
    Aircraft = 3,
    SafeZones = 4,
    EndGame = 5,
    Count = 6,
    EAthenaGamePhase_MAX = 7,
};

enum class EAirCraftBehavior : uint8
{
    Default = 0,
    OpposingAirCraftForEachTeam = 1,
    FlyTowardFirstCircleCenter = 2,
    NoAircraft = 3,
    EAirCraftBehavior_MAX = 4,
};

enum class ESafeZoneStartUp : uint8
{
    UseDefaultGameBehavior = 0,
    StartsWithWarmUp = 1,
    StartsWithAirCraft = 2,
    ESafeZoneStartUp_MAX = 3,
};

enum class EAthenaGamePhaseStep : uint8
{
    None = 0,
    Setup = 1,
    Warmup = 2,
    GetReady = 3,
    BusLocked = 4,
    BusFlying = 5,
    StormForming = 6,
    StormHolding = 7,
    StormShrinking = 8,
    Countdown = 9,
    FinalCountdown = 10,
    EndGame = 11,
    Count = 12,
    EAthenaGamePhaseStep_MAX = 13,
};

class AFortGameStateAthena : public AFortGameStateZone
{
public:
    void OnRep_CurrentPlaylistId();
    void OnRep_CurrentPlaylistInfo();
    void OnRep_AdditionalPlaylistLevelsStreamed();
    void OnFinishedStreamingAdditionalPlaylistLevel();
    void OnRep_CurrentPlaylistData();
    void SetPlaylist(UFortPlaylistAthena *Playlist);
    void OnRep_WinningPlayerState()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameStateAthena", "OnRep_WinningPlayerState");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }

    void OnRep_WinningTeam()
    {
        static SDK::UFunction *Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameStateAthena", "OnRep_WinningTeam");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, nullptr);
    }
    void OnRep_GamePhase(EAthenaGamePhase OldGamePhase)
    {
        static SDK::UFunction* Func = nullptr;
        static SDK::FFunctionInfo Info = SDK::PropLibrary->GetFunctionByName("FortGameStateAthena", "OnRep_GamePhase");

        if (Func == nullptr)
            Func = Info.Func;
        if (!Func)
            return;

        this->ProcessEvent(Func, &OldGamePhase);
    }

public:
    using Map = TMap<TSubclassOf<UObject>, int32>;
    DEFINE_MEMBER(float, AFortGameStateAthena, DefaultParachuteDeployTraceForGroundDistance)
    DEFINE_MEMBER(uint8, AFortGameStateAthena, WinningTeam)
    DEFINE_PTR(AFortPlayerStateAthena, AFortGameStateAthena, WinningPlayerState)
        DEFINE_MEMBER(Map, AFortGameStateAthena, AllPlayerBuildableClassesIndexLookup);
    DEFINE_MEMBER(ESafeZoneStartUp, AFortGameStateAthena, CachedSafeZoneStartUp);
    DEFINE_MEMBER(EAirCraftBehavior, AFortGameStateAthena, AirCraftBehavior);
    DEFINE_PTR(AFortAthenaMapInfo, AFortGameStateAthena, MapInfo)
    DEFINE_MEMBER(int, AFortGameStateAthena, PlayersLeft)
    DEFINE_MEMBER(int32, AFortGameStateAthena, CurrentPlaylistId)
    DEFINE_MEMBER(FPlaylistPropertyArray, AFortGameStateAthena, CurrentPlaylistInfo)
    DEFINE_BOOL(AFortGameStateAthena, bGameModeWillSkipAircraft)
      DEFINE_MEMBER(float, AFortGameStateAthena, SafeZonesStartTime)
        DEFINE_MEMBER(EAthenaGamePhaseStep, AFortGameStateAthena, GamePhaseStep)
      DEFINE_BOOL(AFortGameStateAthena, bAircraftIsLocked)
    DEFINE_MEMBER(EAthenaGamePhase, AFortGameStateAthena, GamePhase)
    DEFINE_MEMBER(int, AFortGameModeAthena, SafeZonePhase)
    DEFINE_MEMBER(TArray<AFortAthenaAircraft *>, AFortGameStateAthena, Aircrafts)
    DEFINE_MEMBER(TArray<struct FAdditionalLevelStreamed>, AFortGameState, AdditionalPlaylistLevelsStreamed);
    DEFINE_PTR(UFortPlaylistAthena, AFortGameStateAthena, CurrentPlaylistData)
public:
    DECLARE_STATIC_CLASS(AFortGameStateAthena)
    DECLARE_DEFAULT_OBJECT(AFortGameStateAthena)
};
