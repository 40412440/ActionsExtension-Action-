// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BrainComponent.h"
#include "GameplayTaskOwnerInterface.h"
#include "BPBehaviourTreeComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AIEXTENSION_API UBPBehaviourTreeComponent : public UBrainComponent, public IGameplayTaskOwnerInterface
{
    GENERATED_BODY()

public:
    UBPBehaviourTreeComponent(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintImplementableEvent, Category = BehaviourTree)
    void Root();
};