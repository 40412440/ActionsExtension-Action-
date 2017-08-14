// Copyright 2015-2016 Piperift. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Faction.h"
#include "FactionAgentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class AIEXTENSION_API UFactionAgentInterface : public UGenericTeamAgentInterface
{
    GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class AIEXTENSION_API IFactionAgentInterface : public IGenericTeamAgentInterface
{
    GENERATED_IINTERFACE_BODY()


public:

    /** Retrieve faction identifier in form of Faction */
    UFUNCTION(BlueprintImplementableEvent, Category = Faction, meta = (DisplayName = "GetFaction"))
    void EventGetFaction(FFaction& OutFaction) const;

    /** Assigns faction */
    UFUNCTION(BlueprintImplementableEvent, Category = Faction, meta = (DisplayName = "SetFaction"))
    void EventSetFaction(const FFaction& Faction);

private:


    /** Retrieve faction identifier in form of Faction */
    virtual FFaction GetFaction() const;

    /** Assigns faction */
    virtual void SetFaction(const FFaction& Faction);

    /** Retrieved owner attitude toward given Other object */
    virtual const ETeamAttitude::Type GetAttitudeTowards(const AActor& Other) const
    {
        // OtherActor has Faction Agent Interface
        if (Other.Implements<UFactionAgentInterface>())
        {
            const IFactionAgentInterface* OtherFactionAgent = Cast<IFactionAgentInterface>(&Other);
            
            FFaction OtherFaction;
                
            if (OtherFactionAgent)
            {
                //C++ Interface
                OtherFaction = OtherFactionAgent->GetFaction();
            }
            else
            {
                //Blueprint Interface
                IFactionAgentInterface::Execute_EventGetFaction(&Other, OtherFaction);
            }

            return GetFaction().GetAttitudeTowards(OtherFaction);
        }
        return ETeamAttitude::Neutral;
    }

    /** Begin GenericTeamAgent interface */

    /** Assigns Team Agent to given TeamID */
    virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override {
        SetFaction(FFaction(TeamID));
    }

    /** Retrieve team identifier in form of FGenericTeamId */
    virtual FGenericTeamId GetGenericTeamId() const override {
        return GetFaction().GetTeam();
    }

    /** Retrieved owner attitude toward given Other object */
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override
    {
        return GetAttitudeTowards(Other);
    }

    /** End GenericTeamAgent interface */
};
