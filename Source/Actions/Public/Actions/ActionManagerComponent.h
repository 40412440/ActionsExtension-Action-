// Copyright 2015-2018 Piperift. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "ActionOwnerInterface.h"
#include "ActionManagerComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Tasks), meta = (BlueprintSpawnableComponent))
class ACTIONS_API UActionManagerComponent : public UActorComponent, public IActionOwnerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Begin ITaskOwnerInterface interface
	virtual const bool AddChildren(UAction* NewChildren) override;
	virtual const bool RemoveChildren(UAction* Children) override;
	virtual UActionManagerComponent* GetActionOwnerComponent() override;
	// End ITaskOwnerInterface interface

protected:

	UPROPERTY()
	TArray<UAction*> ChildrenTasks;
};