// Copyright 2015-2017 Piperift. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "IUtilityTreeEditor.h"
#include "Misc/NotifyHook.h"
#include "EditorUndoClient.h"
#include "BlueprintEditor.h"

class IDetailsView;
class SDockableTab;
class SGraphEditor;
class SQuestPalette;
class UEdGraphNode;
class UUtilityTree;
struct FPropertyChangedEvent;
struct Rect;

/*-----------------------------------------------------------------------------
   FUtilityTreeEditor
-----------------------------------------------------------------------------*/

class FUtilityTreeEditor : public FBlueprintEditor, public IUtilityTreeEditor
{
public:
	/** Edits the specified Quest object */
	void InitUtilityTreeEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode);

private:
	/**
	* Updates existing utility tree blueprints to make sure that they are up to date
	*
	* @param	Blueprint	The blueprint to be updated
	*/
	void EnsureBlueprintIsUpToDate(UBlueprint* Blueprint);

public:
	FUtilityTreeEditor();

	virtual ~FUtilityTreeEditor();


	// IToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// End of IToolkit interface


	/** Returns a pointer to the Blueprint object we are currently editing, as long as we are editing exactly one */
	virtual UBlueprint* GetBlueprintObj() const override;
};