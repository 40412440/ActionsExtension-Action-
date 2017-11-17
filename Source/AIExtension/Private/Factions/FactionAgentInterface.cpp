// Copyright 2015-2016 Piperift. All Rights Reserved.

#include "AIExtensionPrivatePCH.h"
#include "FactionAgentInterface.h"



//----------------------------------------------------------------------//
// UGenericTeamAgentInterface
//----------------------------------------------------------------------//
UFactionAgentInterface::UFactionAgentInterface(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{}

FFaction IFactionAgentInterface::GetFaction() const
{
    FFaction Faction;
    EventGetFaction(Faction);
    return Faction;
}

void IFactionAgentInterface::SetFaction(const FFaction & Faction)
{
    EventSetFaction(Faction);
}

const ETeamAttitude::Type IFactionAgentInterface::GetAttitudeTowards(const AActor& Other) const
{
    const FFaction OtherFaction = IFactionAgentInterface::Execute_GetFaction(&Other);
    return GetFaction().GetAttitudeTowards(OtherFaction);
}

const FFaction IFactionAgentInterface::Execute_GetFaction(const AActor* Other)
{
    if (Other && Other->Implements<UFactionAgentInterface>())
    {
        const IFactionAgentInterface* OtherFactionAgent = Cast<IFactionAgentInterface>(Other);
        if (OtherFactionAgent)
        {
            //C++ Interface
            return OtherFactionAgent->GetFaction();
        }
        else
        {
            //BP Interface
            FFaction OtherFaction;
            IFactionAgentInterface::Execute_EventGetFaction(Other, OtherFaction);
            return OtherFaction;
        }
    }
    return FFaction::NoFaction;
}