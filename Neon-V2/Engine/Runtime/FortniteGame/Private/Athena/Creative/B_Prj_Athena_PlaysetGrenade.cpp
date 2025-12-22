#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Creative/B_Prj_Athena_PlaysetGrenade.h"

#include "Engine/Plugins/Kismet/Public/KismetSystemLibrary.h"
#include "Engine/Runtime/Engine/Classes/World.h"
#include "Engine/Runtime/Engine/Classes/Components/SceneComponent.h"

void AB_Prj_Athena_PlaysetGrenade_C::OnLoaded_3645F4484F4ECED813C69D92F55C7A1F(UObject* Loaded)
{
    auto Playset = GetCachedPlayset();
    if (Playset->GetClass())
    {
        std::string PathName = UKismetSystemLibrary::GetDefaultObj()->CallFunc<FString>(
            "KismetSystemLibrary", "GetPathName", Playset).ToString().c_str();

        size_t LastDot = PathName.find_last_of('.');
        if (LastDot != std::string::npos)
        {
            std::string AssetName = PathName.substr(LastDot + 1);

            if (AssetName.rfind("PID_", 0) == 0)
            {
                size_t pgPos = AssetName.find("PG_");
                size_t cpPos = AssetName.find("CP_");
                if (pgPos != std::string::npos) 
                    AssetName = AssetName.substr(pgPos);
                else if (cpPos != std::string::npos) 
                    AssetName = AssetName.substr(cpPos);
            }

            if (AssetName.rfind("CP_", 0) == 0 || AssetName.rfind("PG_", 0) == 0)
            {
                std::vector<std::string> parts;
                size_t start = 0, pos = 0;
                while ((pos = AssetName.find('_', start)) != std::string::npos)
                {
                    if (pos > start) parts.push_back(AssetName.substr(start, pos - start));
                    start = pos + 1;
                }
                if (start < AssetName.length()) 
                    parts.push_back(AssetName.substr(start));

                if (parts.size() >= 3)
                {
                    std::string BasePath;
                    if (parts[1] == "Devices")
                        BasePath = "/Game/Creative/Maps/Devices/";
                    else if (AssetName.rfind("CP_", 0) == 0)
                        BasePath = "/Game/Creative/Maps/Prefabs/";
                    else
                        BasePath = "/Game/Playgrounds/Maps/Playsets/";

                    std::string MappedAssetName = AssetName;
                    std::vector<std::pair<std::string, std::string>> Replacements = {
                        {"CP_JungleTemple_Gallery", "CP_Jungle_Temple_Gallery"},
                        {"CP_JungleTemple_", "CP_Jungle_Temple_"},
                        {"CP_Obstacle_Course_G", "CP_Obstacle_Course_Gallery"},
                        {"PG_Obstacle_Gallery", "PG_Obstacle_Course_Gallery"},
                        {"PG_WVillage", "PG_Winter_Village"}
                    };
                    for (const auto& [find, replace] : Replacements) {
                        if (size_t pos = MappedAssetName.find(find); pos != std::string::npos)
                            MappedAssetName.replace(pos, find.length(), replace);
                    }

                    std::vector<std::pair<std::string, std::string>> Mappings = {
                        {"Winter", "WinterVillage"}, {"Tilted", "TiltedTower"}, {"RetailRows", "RetailRow"},
                        {"Elemental", "Props"}, {"Ring", "Props"}, {"Arctic", "ArcticBase"},
                        {"Jungle", "JungleTemple"}, {"Creative", "Devices"}, {"Music", "Devices"}, {"Vehicle", "Devices"}
                    };
                    for (const auto& [find, replace] : Mappings) {
                        if (parts[1] == find) { parts[1] = replace; break; }
                    }
                    if (parts[1] == "Obstacle") parts[1] += "Course";

                    std::string AssetFullName = MappedAssetName + "." + MappedAssetName;
                    UWorld* World = nullptr;

                    std::vector<std::string> paths = {
                        MappedAssetName.find("Gallery") != std::string::npos ? BasePath + parts[1] + "/Gallery/" + AssetFullName : "",
                        BasePath + parts[1] + "/" + AssetFullName,
                        BasePath + AssetFullName,
                        parts.size() >= 4 ? BasePath + parts[1] + "/" + parts[2] + "/" + AssetFullName : ""
                    };

                    for (const auto& path : paths) {
                        if (!path.empty() && !World) World = StaticLoadObject<UWorld>(path);
                    }

                    if (!World && parts[1] == "Devices" && parts.size() >= 3) {
                        for (const auto& cat : {"Traps", "Weapons", "Consumables", "Objectives", "Galleries", "Spawners"}) {
                            if (World = StaticLoadObject<UWorld>(BasePath + parts[1] + "/" + cat + "/" + AssetFullName)) break;
                        }
                    }

                    if (!World) {
                        std::string size = parts[2] == "S" ? "Small/" : parts[2] == "M" ? "Medium/" : 
                                          parts[2] == "L" ? "Large/" : parts[2] == "XL" ? "XLarge/" : "";
                        if (!size.empty()) World = StaticLoadObject<UWorld>(BasePath + parts[1] + "/" + size + AssetFullName);
                    }

                    if (World)
                    {
                        FRotator ProjectileRotation = K2_GetActorRotation();
                        FVector ProjectileLocation = Playset->CallFunc<FVector>("FortPlaysetItemDefinition", "AdjustToFinalLocation", GetWorld(), Playset, K2_GetActorLocation(), ProjectileRotation);
                                
                        if (ULevel* Level = World->GetPersistentLevel())
                        {
                            auto& Actors = *reinterpret_cast<TArray<class AActor*>*>(
                                reinterpret_cast<uintptr_t>(Level) + 0x98);
                            for (auto& Actor : Actors)
                            {
                                if (Actor && Actor->GetRootComponent())
                                {
                                    FVector RelativeLocation = Actor->GetRootComponent()->GetRelativeLocation();
                                    FRotator RelativeRotation = Actor->GetRootComponent()->GetRelativeRotation();

                                    bool bMirrored = false;
                                    static UClass* BuildingSMActorClass = ABuildingSMActor::StaticClass();
                                    if (Actor->IsA(BuildingSMActorClass))
                                    {
                                        static int32 bMirroredOffset = UKismetMemLibrary::GetOffset(Actor, "bMirrored");
                                        if (bMirroredOffset != 0)
                                        {
                                            bMirrored = *reinterpret_cast<bool*>(__int64(Actor) + bMirroredOffset);
                                        }
                                    }
                                    
                                    float yawRad = (ProjectileRotation.Yaw + 180.0f) * PI / 180.0f;
                                    float cosYaw = std::cos(yawRad);
                                    float sinYaw = std::sin(yawRad);

                                    FVector RotatedLocation = FVector(
                                        RelativeLocation.X * cosYaw - RelativeLocation.Y * sinYaw,
                                        RelativeLocation.X * sinYaw + RelativeLocation.Y * cosYaw,
                                        RelativeLocation.Z
                                    );

                                    FVector SpawnLocation = ProjectileLocation + RotatedLocation;

                                    FRotator FinalRotation = FRotator(
                                        RelativeRotation.Pitch,
                                        RelativeRotation.Yaw + ProjectileRotation.Yaw + 180.0f + (bMirrored ? 180.0f : 0.0f),
                                        RelativeRotation.Roll
                                    );

                                    auto SpawnedActor = GetWorld()->SpawnActor<AActor>(
                                        Actor->GetClass(),
                                        SpawnLocation,
                                        FinalRotation
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }
    }  
}