// Copyright 2015-2017 Piperift. All Rights Reserved.

#include "AIExtensionPrivatePCH.h"
#include "TaskFunctionLibrary.h"

UTask* UTaskFunctionLibrary::CreateTask(TScriptInterface<ITaskOwnerInterface> InOwner, TSubclassOf<class UTask> TaskType)
{
    ITaskOwnerInterface* Owner = &(*InOwner);
    if (!Owner)
        return nullptr;

    if (!TaskType->IsValidLowLevel() || TaskType == UTask::StaticClass())
        return nullptr;

    UTask* NewTask = NewObject<UTask>(Cast<UObject>(Owner), TaskType);
    NewTask->Initialize(Owner);
    return NewTask;
}

void UTaskFunctionLibrary::ActivateTask(UTask* Task)
{
    if (Task) {
        Task->Activate();
    }
}


