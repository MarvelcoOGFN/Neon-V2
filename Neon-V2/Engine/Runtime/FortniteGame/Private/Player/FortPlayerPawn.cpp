#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Player/FortPlayerPawn.h"

#include "Engine/Plugins/Kismet/Public/FortKismetLibrary.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/AI/FortAthenaAIBotController.h"
#include "Engine/Runtime/FortniteGame/Public/Athena/Player/FortPlayerControllerAthena.h"
#include "Engine/Runtime/FortniteGame/Public/Building/BuildingSMActor.h"
#include "Engine/Runtime/FortniteGame/Public/Quests/FortQuestManager.h"

void AFortPlayerPawn::OnCapsuleBeginOverlap(FFrame& Stack)
{
    UPrimitiveComponent* OverlappedComp;
    AActor* OtherActor;
    UPrimitiveComponent* OtherComp;
    int32 OtherBodyIndex;
    bool bFromSweep;
    FHitResult SweepResult;
    Stack.StepCompiledIn(&OverlappedComp);
    Stack.StepCompiledIn(&OtherActor);
    Stack.StepCompiledIn(&OtherComp);
    Stack.StepCompiledIn(&OtherBodyIndex);
    Stack.StepCompiledIn(&bFromSweep);
    Stack.StepCompiledIn(&SweepResult);
    Stack.IncrementCode();

    if (!this->GetController())
        return;
    
    if (OtherActor->GetClass()->GetFName().ToString() == TEXT("B_HidingProp_WilliePete_C"))
    {
        static std::unordered_set<AActor*> Pawns;

        if (Pawns.find(OtherActor) == Pawns.end())
        {
            Pawns.insert(OtherActor);
        }
        else
        {
            std::thread([this]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                CallFunc<void>(
                    "FortPawn",
                    "LaunchCharacterJump",
                    FVector{150, 150, 7500},
                    false,
                    false,
                    true,
                    true
                );
                std::this_thread::sleep_for(std::chrono::milliseconds(750));
                BeginSkydiving(false);
            }).detach();
            Pawns.erase(OtherActor);
        }
        return;
    }
    
    auto Pickup = Cast<AFortPickup>(OtherActor);
    if (!Pickup)
        return;

    if (Pickup->GetPawnWhoDroppedPickup() == this)
        return;

    auto* ItemDef = Pickup->GetPrimaryPickupItemEntry().GetItemDefinition();
    if (!ItemDef)
        return;

    if (GetQuickbar(ItemDef) != EFortQuickBars::Primary)
    {
        CallFunc<void>("FortPlayerPawn", "ServerHandlePickup", Pickup, 0.4f, FVector(), true);
    }
}

void AFortPlayerPawn::ServerSendZiplineState(FFrame& Stack)
{
    FZiplinePawnState State;
    Stack.StepCompiledIn(&State);
    Stack.IncrementCode();
    Set("FortPlayerPawn", "ZiplineState", State);
    UKismetMemLibrary::Get<void (*)(AFortPlayerPawn*)>(L"HandleZiplineStateChanged")(this);

    if (State.bJumped)
    {
        {
            auto Velocity = Get<UObject*>("Character", "CharacterMovement")->Get<FVector>("MovementComponent", "Velocity");
            auto VelocityX = Velocity.X * -0.5f;
            auto VelocityY = Velocity.Y * -0.5f;
            CallFunc<void>("FortPawn", "LaunchCharacterJump", FVector{ VelocityX >= -750 ? fminf(VelocityX, 750) : -750, VelocityY >= -750 ? fminf(VelocityY, 750) : -750, 1200 }, false, false, true, true);
        }
    }
}

void AFortPlayerPawn::ServerHandlePickupInfo(FFrame& Stack)
{
    AFortPickup* Pickup;
    FFortPickupRequestInfo Params;
    Stack.StepCompiledIn(&Pickup);
    Stack.StepCompiledIn(&Params);
    Stack.IncrementCode();

    if (!Pickup || Pickup->GetbPickedUp())
        return;

    auto PlayerController = (AFortPlayerControllerAthena*)GetController();
    
    if (!PlayerController || !Pickup->GetPrimaryPickupItemEntry().GetItemDefinition())
        return;
    
    GetIncomingPickups().Add(Pickup);
    
    Pickup->GetPickupLocationData().bPlayPickupSound = Params.bPlayPickupSound;
    Pickup->GetPickupLocationData().FlyTime = 0.40f;
    Pickup->GetPickupLocationData().ItemOwner = this;
    Pickup->GetPickupLocationData().PickupGuid = Pickup->GetPrimaryPickupItemEntry().GetItemGuid();
    Pickup->GetPickupLocationData().PickupTarget = this;
    Pickup->GetPickupLocationData().StartDirection = (FVector_NetQuantizeNormal)Params.Direction;
    Pickup->OnRep_PickupLocationData();

    Pickup->SetbPickedUp(true);
    Pickup->OnRep_bPickedUp();
}

void AFortPlayerPawn::CompletePickupAnimation(AFortPickup* Pickup)
{
    auto Pawn = Cast<AFortPlayerPawn>(Pickup->GetPickupLocationData().PickupTarget);
    if (!Pawn) return CompletePickupAnimationOG(Pickup);

    auto PC = Cast<AFortPlayerControllerAthena>(Pawn->GetController());
    if (!PC) return CompletePickupAnimationOG(Pickup);

    AFortAthenaAIBotController* AI = Fortnite_Version >= 11.00 ? Cast<AFortAthenaAIBotController>(PC) : nullptr;
    AFortInventory* Inv = PC->GetWorldInventory();
    if (!Inv) return;

    auto MyPawn = AI ? Cast<AFortPlayerPawn>(AI->GetPawn()) : PC->GetMyFortPawn();
    if (!MyPawn) return;

    auto& PickupEntry = Pickup->GetPrimaryPickupItemEntry();
    auto Def = PickupEntry.GetItemDefinition();
    auto MaxStack = Def->GetMaxStackSize();
    auto& Items = Inv->GetInventory().GetItemInstances();

    int Ammo = 0;
    if (auto WDef = Cast<UFortWeaponItemDefinition>(Def))
        if (auto Stats = Inv->GetStats(WDef))
            Ammo = Stats->GetClipSize();

    int ItemCount = 0;
    for (auto* I : Items)
        if (GetQuickbar(I->GetItemEntry().GetItemDefinition()) == EFortQuickBars::Primary &&
            I->GetItemEntry().GetItemDefinition()->GetbInventorySizeLimited())
            ItemCount++;

    bool bShouldSwap = (ItemCount - 6) >= 5;
    bool bIsStackable = (Fortnite_Version.Season() <= 8)
        ? (MaxStack > 1)
        : Def->IsStackable();

    if (bIsStackable)
    {
        UFortWorldItem* ExistingItem = nullptr;
        for (auto* I : Items)
            if (I && I->GetItemEntry().GetItemDefinition() == Def &&
                I->GetItemEntry().GetCount() <= MaxStack)
            { ExistingItem = I; break; }

        if (ExistingItem)
        {
            auto& Entry = ExistingItem->GetItemEntry();

            FFortItemEntryStateValue* State = nullptr;
            for (auto& Value : Entry.GetStateValues())
                if (Value.StateType == EFortItemEntryState::ShouldShowItemToast)
                { State = &Value; break; }

            if (!State) {
                FFortItemEntryStateValue Value{};
                Value.StateType = EFortItemEntryState::ShouldShowItemToast;
                Value.IntValue = true;
                Entry.GetStateValues().Add(Value);
            }
            else State->IntValue = true;

            int NewCount = Entry.GetCount() + PickupEntry.GetCount();
            if (NewCount > MaxStack)
            {
                Entry.SetCount(MaxStack);
                int Overflow = NewCount - MaxStack;

                if (Def->GetbAllowMultipleStacks() && ItemCount < 5)
                    Inv->GiveItem(Def, Overflow, Ammo, PickupEntry.GetLevel());
                else
                    Inv->SpawnPickup(PC->GetViewTarget()->K2_GetActorLocation(), Def, Overflow,
                                     PickupEntry.GetLoadedAmmo(), EFortPickupSourceTypeFlag::Player,
                                     EFortPickupSpawnSource::Unset, MyPawn, true);
            }
            else
                Entry.SetCount(NewCount);

            Inv->ReplaceEntry(Entry);
        }
        else
        {
            if (PickupEntry.GetCount() > MaxStack)
            {
                int Overflow = PickupEntry.GetCount() - MaxStack;

                if (ItemCount == 5 && GetQuickbar(Def) == EFortQuickBars::Primary)
                {
                    auto CW = MyPawn->GetCurrentWeapon();
                    if (CW && GetQuickbar(CW->GetWeaponData()) == EFortQuickBars::Primary)
                    {
                        FGuid CWGuid = CW->GetItemEntryGuid();
                        for (auto* I : Items)
                            if (I->GetItemEntry().GetItemGuid() == CWGuid)
                            {
                                auto& CWEntry = I->GetItemEntry();
                                Inv->SpawnPickup(MyPawn->K2_GetActorLocation() + MyPawn->GetActorForwardVector() * 70.f + FVector(0,0,50),
                                                 CWEntry.GetItemDefinition(), CWEntry.GetCount(), CWEntry.GetLoadedAmmo(),
                                                 EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, MyPawn, true);
                                Inv->Remove(CWGuid);
                                break;
                            }
                    }
                    else
                    {
                        Inv->SpawnPickup(PC->GetViewTarget()->K2_GetActorLocation(), Def, MaxStack,
                                         PickupEntry.GetLoadedAmmo(), EFortPickupSourceTypeFlag::Player,
                                         EFortPickupSpawnSource::Unset, MyPawn, true);
                        CompletePickupAnimationOG(Pickup);
                        return;
                    }

                    Inv->GiveItem(Def, MaxStack, Ammo, PickupEntry.GetLevel());
                }
                else
                    Inv->GiveItem(Def, MaxStack, Ammo, PickupEntry.GetLevel());

                if (Def->GetbAllowMultipleStacks() && ItemCount < 5)
                    Inv->GiveItem(Def, Overflow, Ammo, PickupEntry.GetLevel());
                else
                    Inv->SpawnPickup(PC->GetViewTarget()->K2_GetActorLocation(), Def, Overflow,
                                     PickupEntry.GetLoadedAmmo(), EFortPickupSourceTypeFlag::Player,
                                     EFortPickupSpawnSource::Unset, MyPawn, true);
            }
            else
            {
                if (bShouldSwap && GetQuickbar(Def) == EFortQuickBars::Primary)
                {
                    auto CW = MyPawn->GetCurrentWeapon();
                    if (CW && GetQuickbar(CW->GetWeaponData()) == EFortQuickBars::Primary)
                    {
                        FGuid CWGuid = CW->GetItemEntryGuid();
                        for (auto* I : Items)
                            if (I->GetItemEntry().GetItemGuid() == CWGuid)
                            {
                                auto& CWEntry = I->GetItemEntry();
                                Inv->SpawnPickup(MyPawn->K2_GetActorLocation() + MyPawn->GetActorForwardVector() * 70.f + FVector(0,0,50),
                                                 CWEntry.GetItemDefinition(), CWEntry.GetCount(), CWEntry.GetLoadedAmmo(),
                                                 EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, MyPawn, true);
                                Inv->Remove(CWGuid);
                                break;
                            }
                        Inv->GiveItem(Def, PickupEntry.GetCount(), Ammo, PickupEntry.GetLevel());
                    }
                    else
                        Inv->SpawnPickup(PC->GetViewTarget()->K2_GetActorLocation(), Def, PickupEntry.GetCount(),
                                         PickupEntry.GetLoadedAmmo(), EFortPickupSourceTypeFlag::Player,
                                         EFortPickupSpawnSource::Unset, MyPawn, true);
                }
                else
                    Inv->GiveItem(Def, PickupEntry.GetCount(), Ammo, PickupEntry.GetLevel());
            }
        }
    }
    else
    {
        if (ItemCount == 5 && GetQuickbar(Def) == EFortQuickBars::Primary)
        {
            auto CW = MyPawn->GetCurrentWeapon();
            if (CW && GetQuickbar(CW->GetWeaponData()) == EFortQuickBars::Primary)
            {
                FGuid CWGuid = CW->GetItemEntryGuid();
                for (auto* I : Items)
                    if (I->GetItemEntry().GetItemGuid() == CWGuid)
                    {
                        auto& CWEntry = I->GetItemEntry();
                        Inv->SpawnPickup(MyPawn->K2_GetActorLocation() + MyPawn->GetActorForwardVector() * 70.f + FVector(0,0,50),
                                         CWEntry.GetItemDefinition(), CWEntry.GetCount(), CWEntry.GetLoadedAmmo(),
                                         EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, MyPawn, true);
                        Inv->Remove(CWGuid);
                        break;
                    }
                Inv->GiveItem(Def, PickupEntry.GetCount(), Ammo, PickupEntry.GetLevel());
            }
            else
                Inv->SpawnPickup(PC->GetViewTarget()->K2_GetActorLocation(), Def, PickupEntry.GetCount(),
                                 PickupEntry.GetLoadedAmmo(), EFortPickupSourceTypeFlag::Player,
                                 EFortPickupSpawnSource::Unset, MyPawn, true);
        }
        else
            Inv->GiveItem(Def, PickupEntry.GetCount(), Ammo, PickupEntry.GetLevel());
    }

    CompletePickupAnimationOG(Pickup);
}

void AFortPlayerPawn::ServerHandlePickup(FFrame& Stack)
{
    AFortPickup* Pickup;
    float InFlyTime;
    FVector InStartDirection;
    bool bPlayPickupSound;
    Stack.StepCompiledIn(&Pickup);
    Stack.StepCompiledIn(&InFlyTime);
    Stack.StepCompiledIn(&InStartDirection);
    Stack.StepCompiledIn(&bPlayPickupSound);
    Stack.IncrementCode();
    
    if (!Pickup || Pickup->GetbPickedUp())
        return;

    auto PlayerController = (AFortPlayerControllerAthena*)GetController();
    
    if (!PlayerController || !Pickup->GetPrimaryPickupItemEntry().GetItemDefinition())
        return;
    
    GetIncomingPickups().Add(Pickup);
    
    Pickup->GetPickupLocationData().bPlayPickupSound = bPlayPickupSound;
    Pickup->GetPickupLocationData().FlyTime = 0.40f;
    Pickup->GetPickupLocationData().ItemOwner = this;
    Pickup->GetPickupLocationData().PickupGuid = Pickup->GetPrimaryPickupItemEntry().GetItemGuid();
    Pickup->GetPickupLocationData().PickupTarget = this;
    Pickup->GetPickupLocationData().StartDirection = (FVector_NetQuantizeNormal)InStartDirection;
    Pickup->OnRep_PickupLocationData();

    Pickup->SetbPickedUp(true);
    Pickup->OnRep_bPickedUp();
}
