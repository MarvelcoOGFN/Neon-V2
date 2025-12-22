#pragma once
#include "pch.h"

#include "Engine/Runtime/Engine/Core/Public/Misc/Guid.h"
#include "Engine/Runtime/FortniteGame/Public/FortGameMode.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

struct FFortMissionPlacementActorItem final
{
public:
	struct FGameplayTagContainer                  ItemIdentifyingTags;                               // 0x0000(0x0020)(Edit, DisableEditOnInstance, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  TagsToAddToChosenPlacementActor;                   // 0x0020(0x0020)(Edit, DisableEditOnInstance, NativeAccessSpecifierPublic)
	class UEnvQuery*                              PlacementQuery;                                    // 0x0040(0x0008)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	TSoftClassPtr<class UClass>                   ActorToPlace;                                      // 0x0048(0x0028)(Edit, DisableEditOnInstance, Protected, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	int32                                         NumLocationsToFind;                                // 0x0070(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	bool                                          bSpawnActorAutomatically;                          // 0x0074(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	bool                                          bShouldReserveLocations;                           // 0x0075(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	uint8                                         Pad_76[0x1];                                       // 0x0076(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
	bool                                          bSnapToGrid;                                       // 0x0077(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	bool                                          bAdjustPlacementForFloors;                         // 0x0078(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	bool                                          bDontCreateSpawnRiftsNearby;                       // 0x0079(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, AdvancedDisplay, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	uint8                                         Pad_7A[0x6];                                       // 0x007A(0x0006)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FFortMissionPlacementItems final
{
public:
   struct FGameplayTagContainer                  TagsToAddToChosenPlacementActors;                  // 0x0000(0x0020)(Edit, DisableEditOnInstance, NativeAccessSpecifierPublic)
   TArray<struct FFortMissionPlacementFoundationItem> AdditionalWorldFoundations;                        // 0x0020(0x0010)(Edit, ZeroConstructor, DisableEditOnInstance, NativeAccessSpecifierPublic)
   TArray<struct FFortMissionPlacementActorItem> ActorsAndLocations;                                // 0x0030(0x0010)(Edit, ZeroConstructor, DisableEditOnInstance, NativeAccessSpecifierPublic)
};

class UFortMissionInfo : public UObject
{
public:
   DECLARE_DEFAULT_OBJECT(UFortMissionInfo)
   DECLARE_STATIC_CLASS(UFortMissionInfo)
   DEFINE_BOOL(UFortMissionInfo, bStartPlayingOnLoad)
   DEFINE_MEMBER(FFortMissionPlacementItems, UFortMissionInfo, MissionPlacementInfo)
};


struct FGameplayTagQuery final
{
public:
   int32                                         TokenStreamVersion;                                // 0x0000(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
   uint8                                         Pad_4[0x4];                                        // 0x0004(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
   TArray<struct FGameplayTag>                   TagDictionary;                                     // 0x0008(0x0010)(ZeroConstructor, NativeAccessSpecifierPrivate)
   TArray<uint8>                                 QueryTokenStream;                                  // 0x0018(0x0010)(ZeroConstructor, NativeAccessSpecifierPrivate)
   class FString                                 UserDescription;                                   // 0x0028(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
   class FString                                 AutoDescription;                                   // 0x0038(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
};

class AFortMission : public AActor
{
public:
   DEFINE_PTR(UFortMissionGenerator, AFortMission, MissionGenerator)
   DEFINE_MEMBER(FGuid, AFortMission, MissionGuid)
   DEFINE_PTR(UFortMissionInfo, AFortMission, MissionInfo)

   static inline char (*InitializeActorsForMissionOG)(AFortMission*, AFortMission*,int,  __int64);
   static char InitializeActorsForMission(AFortMission*, AFortMission* Mission, int , __int64);
public:
   DECLARE_DEFAULT_OBJECT(AFortMission)
   DECLARE_STATIC_CLASS(AFortMission)
};

class UFortMissionLibrary : public UObject
{
public:
   DECLARE_DEFAULT_OBJECT(UFortMissionLibrary)
   DECLARE_STATIC_CLASS(UFortMissionLibrary)

   static void LoadMission(UObject* WorldContextObject, UFortMissionInfo* MissionInfo)
   {
      static auto Func = SDK::PropLibrary->GetFunctionByName("FortMissionLibrary", "LoadMission").Func;
      if (!Func) return;
        
      struct
      {
      public:
         class UObject*                                WorldContextObject;                                // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         class UFortMissionInfo*                       MissionInfo;                                       // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
      } Params{WorldContextObject, MissionInfo};
      GetDefaultObj()->ProcessEvent(Func, &Params);
   }

   static TArray<AActor*> SpawnAtPlacementActorsHelper(UObject* WorldContextObject, UClass* Entries, bool* bSucceeded)
   {
      static auto Func = SDK::PropLibrary->GetFunctionByName("FortMissionLibrary", "SpawnAtPlacementActorsHelper").Func;
      if (!Func) return TArray<AActor*>();
        
      struct
      {
      public:
         class UObject*                                WorldContextObject;                                // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         class UClass*                                 Entries;                                           // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         bool                                          bSucceeded;                                        // 0x0010(0x0001)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         uint8                                         Pad_11[0x7];                                       // 0x0011(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
         TArray<class AActor*>                         ReturnValue;                                       // 0x0018(0x0010)(Parm, OutParm, ZeroConstructor, ReturnParm, NativeAccessSpecifierPublic)
      } Params{WorldContextObject, Entries};
      
      GetDefaultObj()->ProcessEvent(Func, &Params);

      *bSucceeded = Params.bSucceeded;

      return Params.ReturnValue;
   }

   static void SetWorldSavingEnabled(UObject* WorldContextObject, bool bEnable)
   {
      static auto Func = SDK::PropLibrary->GetFunctionByName("FortMissionLibrary", "SetWorldSavingEnabled").Func;
      if (!Func) return;
        
      struct
      {
      public:
         class UObject*                                WorldContextObject;                                // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         bool                                          bEnabled;                                          // 0x0008(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         uint8                                         Pad_9[0x7];                                        // 0x0009(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
      } Params{WorldContextObject, bEnable};
      
      GetDefaultObj()->ProcessEvent(Func, &Params);
   }

   static TArray<AFortMission*> GetAllLoadedMissions(UObject* WorldContextObject)
   {
      static auto Func = SDK::PropLibrary->GetFunctionByName("FortMissionLibrary", "GetAllLoadedMissions").Func;
      if (!Func) return TArray<AFortMission*>();
        
      struct 
      {
      public:
         class UObject*                                WorldContextObject;                                // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
         TArray<class AFortMission*>                   ReturnValue;                                       // 0x0008(0x0010)(Parm, OutParm, ZeroConstructor, ReturnParm, NativeAccessSpecifierPublic)
      } Params{WorldContextObject};
      
      GetDefaultObj()->ProcessEvent(Func, &Params);

      return Params.ReturnValue;
   }
};

struct FFortOutpostCoreInfo
{
   DEFINE_MEMBER(TArray<FString>, FFortOutpostCoreInfo, AccountsWithEditPermission)
};

class AFortGameModeOutpost : public AFortGameMode
{
public:
   DEFINE_MEMBER(FFortOutpostCoreInfo, AFortGameModeOutpost, CurrentCoreInfo)
   
public:
   DECLARE_DEFAULT_OBJECT(AFortGameModeOutpost)
   DECLARE_STATIC_CLASS(AFortGameModeOutpost)
};
