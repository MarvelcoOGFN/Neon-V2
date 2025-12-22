#include "pch.h"
#include "../Public/Events.h"

#include "Engine/Runtime/Engine/Classes/GameplayStatics.h"
#include "Engine/Runtime/Engine/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

void Events::StartEvent()
{
	bool bStartedEvent = false;
	
	for (auto& Event : EventsArray)
	{
		if (Event.Version == Fortnite_Version)
		{
			UClass* LoaderClass = StaticFindObject<UClass>(Event.Loader);
			UObject* Loader = nullptr;
			
			if (LoaderClass)
				Loader = UGameplayStatics::GetAllActorsOfClass(GetWorld(), LoaderClass)[0];
			
			if (Event.LoaderFunc != "" && Loader)
			{
				if (UFunction* LoaderFunction = StaticFindObject<UFunction>(Event.LoaderFunc))
				{
					int Param = 1;
					Loader->ProcessEvent(LoaderFunction, &Param);
					UE_LOG(LogTemp, Warning, "StartEvent: Initialized Event");
				}
				else
				{
					UE_LOG(LogTemp, Warning, "StartEvent: Failed to initialize Event");
				}
			}

			UClass* ScriptingClass = StaticFindObject<UClass>(Event.Scripting);
			UObject* Scripting = nullptr;

			if (ScriptingClass)
				Scripting = UGameplayStatics::GetAllActorsOfClass(GetWorld(), ScriptingClass)[0];

			for (auto& FunctionPair : Event.Functions)
			{
				UFunction* Function = StaticFindObject<UFunction>(FunctionPair.second);

				if (!Function)
				{
					UE_LOG(LogTemp, Warning, "StartEvent: Failed to find: %s", FunctionPair.second.c_str());
					continue;
				}

				if (FunctionPair.first)
				{
					if (FunctionPair.second.contains("OnReady"))
					{
						static int CurrentPlaylistInfoOffset = UKismetMemLibrary::GetOffset(GetWorld()->GetGameState(), "CurrentPlaylistInfo");
						FPlaylistPropertyArray& CurrentPlaylistInfoPtr = *reinterpret_cast<FPlaylistPropertyArray*>(__int64(GetWorld()->GetGameState()) + CurrentPlaylistInfoOffset);
						
						auto Playlist = Fortnite_Version >= 6.10 ? CurrentPlaylistInfoPtr.GetBasePlaylist() : GetWorld()->GetGameState()->GetCurrentPlaylistData();
						
						struct { UObject* GameState; UFortPlaylistAthena* Playlist; FGameplayTagContainer PlaylistContextTags; } OnReadyParams{GetWorld()->GetGameState(), Playlist, FGameplayTagContainer()};

						Loader->ProcessEvent(Function, &OnReadyParams);
					}
					else
					{
						Loader->ProcessEvent(Function, nullptr);
					}
				}
				else
				{
					if (FunctionPair.second.contains("OnReady"))
					{
						static int CurrentPlaylistInfoOffset = UKismetMemLibrary::GetOffset(GetWorld()->GetGameState(), "CurrentPlaylistInfo");
						FPlaylistPropertyArray& CurrentPlaylistInfoPtr = *reinterpret_cast<FPlaylistPropertyArray*>(__int64(GetWorld()->GetGameState()) + CurrentPlaylistInfoOffset);
						
						auto Playlist = Fortnite_Version >= 6.10 ? CurrentPlaylistInfoPtr.GetBasePlaylist() : GetWorld()->GetGameState()->GetCurrentPlaylistData();
						
						struct { UObject* GameState; UFortPlaylistAthena* Playlist; FGameplayTagContainer PlaylistContextTags; } OnReadyParams{GetWorld()->GetGameState(), Playlist, FGameplayTagContainer()};

						Scripting->ProcessEvent(Function, &OnReadyParams);
					}
					else
					{
						Scripting->ProcessEvent(Function, nullptr);
					}

					if (Fortnite_Version == FFortniteVersion(8, 51, 0) && Scripting)
					{
						std::thread([Scripting]
						{
							std::this_thread::sleep_for(std::chrono::minutes(3));
							static auto SetUnvaultItemNameFn = StaticFindObject<UFunction>("/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C.SetUnvaultItemName");
					
							FName Name = UKismetStringLibrary::Conv_StringToName(L"DrumGun");
							Scripting->ProcessEvent(SetUnvaultItemNameFn, &Name);

							static auto PillarsConcludedFn = StaticFindObject<UFunction>("/Game/Athena/Prototype/Blueprints/White/BP_SnowScripting.BP_SnowScripting_C.PillarsConcluded");
							Scripting->ProcessEvent(PillarsConcludedFn, &Name);
						}).detach();
					}
				}
			}

			UE_LOG(LogTemp, Log, "StartEvent: Started Event.");
			bStartedEvent = true;
		}
	}

	if (!bStartedEvent)
		UE_LOG(LogTemp, Warning, "StartEvent: Failed to start Event.");
}
