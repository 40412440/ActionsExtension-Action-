// Copyright 2015-2017 Piperift. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptInterface.h"
#include "TaskOwnerInterface.h"

#include "Task.generated.h"

/**
 * Result of a node execution
 */
UENUM()
enum class ETaskState : uint8
{
    RUNNING  UMETA(DisplayName = "Running"),
    SUCCESS  UMETA(DisplayName = "Success"),
    FAILURE  UMETA(DisplayName = "Failure"),
    ERROR    UMETA(DisplayName = "Error"),
    CANCELED UMETA(DisplayName = "Canceled"),
    NOT_RUN  UMETA(DisplayName = "Not Run")
};

DECLARE_LOG_CATEGORY_EXTERN(TaskLog, Log, All);

class UTaskManagerComponent;

/**
 * 
 */
UCLASS(Blueprintable, meta = (ExposedAsyncProxy))
class AIEXTENSION_API UTask : public UObject, public FTickableGameObject, public ITaskOwnerInterface
{
    GENERATED_UCLASS_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Task)
    void Activate();

    virtual const bool AddChildren(UTask* NewChildren) override;
    virtual const bool RemoveChildren(UTask* Children) override;


    /** Event when tick is received for this tickable object . */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
    void ReceiveTick(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = Task)
    void Finish(bool bSuccess, bool bError);
    UFUNCTION(BlueprintCallable, Category = Task)
    void Cancel();

    /** Event when play begins for this actor. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Activate"))
    void ReceiveActivate();

    /** Event when finishing this task. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Finished"))
    void ReceiveFinished(bool bSucceded);

    /** Event when this task is canceled. */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Canceled"))
    void ReceiveCanceled();


    void Destroy();

    virtual void OnActivation() {}

    virtual UTaskManagerComponent* GetTaskOwnerComponent() override;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task, meta = (DisplayName = "GetTaskOwnerComponent"))
    UTaskManagerComponent* ExposedGetTaskOwnerComponent();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    AActor* GetTaskOwnerActor();


    //~ Begin Ticking
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Object)
    uint8 bWantsToTick:1;

    //Tick length in seconds. 0 is default tick rate
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Object)
    float TickRate;

private:
    float TickTimeElapsed;
    //~ End Ticking

protected:
    UPROPERTY()
    ETaskState State;

    UPROPERTY()
    TSet<UTask*> ChildrenTasks;

    //~ Begin Tickable Object Interface
    virtual void Tick(float DeltaTime) override;
    virtual void TaskTick(float DeltaTime) {}

    virtual bool IsTickable() const override {
        return bWantsToTick && IsActivated()
            && !IsPendingKill()
            && IsInitialized() && !GetParent()->IsPendingKill();
    }

    virtual TStatId GetStatId() const override {
        RETURN_QUICK_DECLARE_CYCLE_STAT(UTask, STATGROUP_Tickables);
    }
    //~ End Tickable Object Interface

public:
    //Inlines
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    const bool IsInitialized() const {
        UObject const * const Outer = GetOuter();
        return Outer && Outer->GetClass()->ImplementsInterface(UTaskOwnerInterface::StaticClass());
    }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    FORCEINLINE bool IsActivated() const { return IsInitialized() && State == ETaskState::RUNNING; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    FORCEINLINE bool Succeeded() const { return IsInitialized() && State == ETaskState::SUCCESS; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    FORCEINLINE bool Failed() const { return IsInitialized() && State == ETaskState::FAILURE; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    FORCEINLINE bool IsCanceled() const { return State == ETaskState::CANCELED; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    FORCEINLINE ETaskState GetState() const { return State; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = Task)
    FORCEINLINE UObject* const GetParent() const {
        return IsInitialized() ? GetOuter() : nullptr;
    }

    FORCEINLINE ITaskOwnerInterface* GetParentInterface() const {
        return Cast<ITaskOwnerInterface>(GetOuter());
    }


    virtual UWorld* GetWorld() const override {
        const UObject* const InParent = GetParent();

        return InParent ? InParent->GetWorld() : nullptr;
    }
};
