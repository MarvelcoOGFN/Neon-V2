#include "pch.h"
#include "Engine/Runtime/FortniteGame/Public/Creative/FortCreativePortalManager.h"
#include <Engine/Runtime/Engine/Classes/World.h>

AFortAthenaCreativePortal* AFortCreativePortalManager::ClaimPortal()
{
    AFortAthenaCreativePortal* Portal = nullptr;
    if (Fortnite_Version >= 12.00)
    {
        for (int i = 0; i < this->GetAllPortals().Num(); i++)
        {
            auto CurrentPortal = this->GetAllPortals()[i];
            if (!CurrentPortal->GetLinkedVolume() || CurrentPortal->GetLinkedVolume()->GetVolumeState() == EVolumeState::Ready)
                continue;
            Portal = CurrentPortal;
        }
    }
    else
    {
        if (this->GetAvailablePortals().Num() > 0)
        {
            auto CurrentPortal = this->GetAvailablePortals()[0];
            Portal = CurrentPortal;
            this->GetAvailablePortals().Remove(0);
            this->GetUsedPortals().Add(Portal);
        }
           
        
    }
    return Portal;
}
