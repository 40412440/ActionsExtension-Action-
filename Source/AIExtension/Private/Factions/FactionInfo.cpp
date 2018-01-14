// Copyright 2015-2018 Piperift. All Rights Reserved.

#include "AIExtensionPrivatePCH.h"
#include "FactionInfo.h"

#include "AIExtensionSettings.h"


const FFaction FFactionInfo::GetFaction() 
{
    const auto* Self = this;
    //Find this faction by pointer
    const int32 Index = GetDefault<UAIExtensionSettings>()->Factions.IndexOfByPredicate([Self](const auto& FactionInfo) {
        return Self == &FactionInfo;
    });

    const FFaction Faction(Index);
    checkf(Faction.IsNone(), TEXT("Faction Info can never be None"))
    return Faction;
}

void FFactionInfo::SetRelation(const FFaction& OtherFaction, const ETeamAttitude::Type Attitude)
{
    if (OtherFaction.IsNone())
        return;

    FFactionRelation InRelation(this->GetFaction(), OtherFaction, Attitude);

    TArray<FFactionRelation>& Relations = GetMutableDefault<UAIExtensionSettings>()->Relations;

    //Remove possible similar relation
    FFactionRelation* const FoundRelationPtr = Relations.FindByKey(InRelation);
    if (FoundRelationPtr == NULL) 
    {
        Relations.AddUnique(InRelation);
    }
    else
    {
        FoundRelationPtr->Attitude = Attitude;
    }
}
