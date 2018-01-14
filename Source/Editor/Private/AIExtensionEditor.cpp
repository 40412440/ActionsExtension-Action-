// Copyright 2015-2018 Piperift. All Rights Reserved.

#include "AIExtensionEditorPrivatePCH.h"

#include "Kismet2/KismetEditorUtilities.h"

#include "FactionCustomization.h"

#include "Action.h"
#include "Task_BehaviourTree.h"

DEFINE_LOG_CATEGORY(LogAIExtensionEditor)
 
#define LOCTEXT_NAMESPACE "AIExtensionEditor"
 
void FAIExtensionEditorModule::StartupModule()
{
    UE_LOG(LogAIExtensionEditor, Warning, TEXT("AIExtensionEditor: Log Started"));

    RegisterPropertyTypeCustomizations();
    PrepareAutoGeneratedDefaultEvents();
}
 
void FAIExtensionEditorModule::ShutdownModule()
{
    UE_LOG(LogAIExtensionEditor, Warning, TEXT("AIExtensionEditor: Log Ended"));

    CreatedAssetTypeActions.Empty();

    // Cleanup all information for auto generated default event nodes by this module
    FKismetEditorUtilities::UnregisterAutoBlueprintNodeCreation(this);
}


void FAIExtensionEditorModule::RegisterPropertyTypeCustomizations()
{
    RegisterCustomPropertyTypeLayout("Faction", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FFactionCustomization::MakeInstance));
}

void FAIExtensionEditorModule::PrepareAutoGeneratedDefaultEvents()
{
    //Tickable Object events
    RegisterDefaultEvent(UTickableObject, ReceiveBeginPlay);
    RegisterDefaultEvent(UTickableObject, ReceiveTick);

    //Task events
    RegisterDefaultEvent(UAction, ReceiveActivate);
    RegisterDefaultEvent(UAction, ReceiveTick);
    RegisterDefaultEvent(UAction, ReceiveFinished);

    //Behaviour Tree Task events
    RegisterDefaultEvent(UTask_BehaviourTree, ReceiveActivate);
    RegisterDefaultEvent(UTask_BehaviourTree, Root);
    RegisterDefaultEvent(UTask_BehaviourTree, ReceiveFinished);

}


void FAIExtensionEditorModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
    check(PropertyTypeName != NAME_None);

    static FName PropertyEditor("PropertyEditor");
    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
    PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_GAME_MODULE(FAIExtensionEditorModule, AIExtensionEditor);