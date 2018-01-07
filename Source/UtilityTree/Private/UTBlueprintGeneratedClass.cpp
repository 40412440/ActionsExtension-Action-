// Copyright 2015-2017 Piperift. All Rights Reserved.

/*=============================================================================
	UTBlueprintGeneratedClass.cpp: The object generated by compiling an UtilityTreeBlueprint
=============================================================================*/ 

#include "UTBlueprintGeneratedClass.h"
#include "UtilityTreeModule.h"

#include "Serialization/ObjectWriter.h"
#include "Serialization/ObjectReader.h"
#include "BonePose.h"
//#include "UtilityTree/UTNodeBase.h"
#include "UtilityTree.h"


/////////////////////////////////////////////////////
// FUTBlueprintDebugData

#if WITH_EDITORONLY_DATA

void FUTBlueprintDebugData::TakeSnapshot(UUtilityTree* Instance)
{
	checkSlow(&(CastChecked<UUTBlueprintGeneratedClass>((UObject*)(Instance->GetClass()))->GetAnimBlueprintDebugData()) == this);

	if (SnapshotBuffer == NULL)
	{
		SnapshotBuffer = new TSimpleRingBuffer<FUTFrameSnapshot>(30*30);
	}

	SnapshotBuffer->WriteNewElementInitialized().InitializeFromInstance(Instance);
}

void FUTBlueprintDebugData::ResetSnapshotBuffer()
{
	if (SnapshotBuffer != NULL)
	{
		delete SnapshotBuffer;
		SnapshotBuffer = NULL;
	}
}

float FUTBlueprintDebugData::GetSnapshotLengthInSeconds()
{
	if (SnapshotBuffer != NULL)
	{
		//@TODO: Shouldn't use hardcoded snapshot length; ideally we actually use timestamps, etc...
		return SnapshotBuffer->Num() * 1.0f / 30.0f;
	}

	return 0.0f;
}

int32 FUTBlueprintDebugData::GetSnapshotLengthInFrames()
{
	if (SnapshotBuffer != NULL)
	{
		//@TODO: Shouldn't use hardcoded snapshot length; ideally we actually use timestamps, etc...
		return SnapshotBuffer->Num();
	}

	return 0;
}

void FUTBlueprintDebugData::SetSnapshotIndex(UUtilityTree* Instance, int32 NewIndex)
{
	if (SnapshotBuffer != NULL)
	{
		int32 SavedIndex = SnapshotIndex;
		int32 ClampedIndex = FMath::Clamp<int32>(NewIndex, 0, SnapshotBuffer->Num() - 1);
		SnapshotIndex = (NewIndex == INDEX_NONE) ? INDEX_NONE : ClampedIndex;

		// Apply the desired snapshot (or the most recent one if free-running was selected)
		if ((SnapshotIndex != SavedIndex) && (SnapshotBuffer->Num() > 0))
		{
			(*SnapshotBuffer)(ClampedIndex).CopyToInstance(Instance);
		}
	}
}

void FUTBlueprintDebugData::SetSnapshotIndexByTime(UUtilityTree* Instance, double TargetTime)
{
	const int32 SavedIndex = SnapshotIndex;

	int32 NewIndex = INDEX_NONE;

	if ((SnapshotBuffer != NULL) && (SnapshotBuffer->Num() > 0))
	{
		if ((TargetTime < (*SnapshotBuffer)(0).TimeStamp) && (TargetTime >= (*SnapshotBuffer)(SnapshotBuffer->Num()-1).TimeStamp))
		{
			for (NewIndex = SnapshotBuffer->Num() - 1; NewIndex > 0; --NewIndex)
			{
				if (TargetTime < (*SnapshotBuffer)(NewIndex-1).TimeStamp)
				{
					break;
				}
			}
		}
	}

	// Determine which snapshot to use
	SetSnapshotIndex(Instance, NewIndex);
}

void FUTBlueprintDebugData::ResetNodeVisitSites()
{
	UpdatedNodesThisFrame.Empty(UpdatedNodesThisFrame.Num());
}

void FUTBlueprintDebugData::RecordNodeVisit(int32 TargetNodeIndex, int32 SourceNodeIndex, float BlendWeight)
{
	new (UpdatedNodesThisFrame) FNodeVisit(SourceNodeIndex, TargetNodeIndex, BlendWeight);
}

void FUTBlueprintDebugData::RecordNodeVisitArray(const TArray<FNodeVisit>& Nodes)
{
	UpdatedNodesThisFrame.Append(Nodes);
}

void FUTBlueprintDebugData::AddPoseWatch(int32 NodeID, FColor Color)
{
	for (FUTNodePoseWatch& PoseWatch : UTNodePoseWatch)
	{
		if (PoseWatch.NodeID == NodeID)
		{
			PoseWatch.PoseDrawColour = Color;
			return;
		}
	}

	//Not found so make new one
	UTNodePoseWatch.Add(FUTNodePoseWatch());
	FUTNodePoseWatch& NewUTNodePoseWatch = UTNodePoseWatch.Last();
	NewUTNodePoseWatch.NodeID = NodeID;
	NewUTNodePoseWatch.PoseDrawColour = Color;
	NewUTNodePoseWatch.PoseInfo = MakeShareable(new FCompactHeapPose());
}

void FUTBlueprintDebugData::RemovePoseWatch(int32 NodeID)
{
	for (int32 PoseWatchIdx = 0; PoseWatchIdx < UTNodePoseWatch.Num(); ++PoseWatchIdx)
	{
		if (UTNodePoseWatch[PoseWatchIdx].NodeID == NodeID)
		{
			UTNodePoseWatch.RemoveAtSwap(PoseWatchIdx);
			return;
		}
	}
}

void FUTBlueprintDebugData::UpdatePoseWatchColour(int32 NodeID, FColor Color)
{
	for (FUTNodePoseWatch& PoseWatch : UTNodePoseWatch)
	{
		if (PoseWatch.NodeID == NodeID)
		{
			PoseWatch.PoseDrawColour = Color;
			return;
		}
	}
}
/////////////////////////////////////////////////////
// FBinaryObjectWriter

class FBinaryObjectWriter : public FObjectWriter
{
public:
	FBinaryObjectWriter(UObject* Obj, TArray<uint8>& InBytes)
		: FObjectWriter(InBytes)
	{
		ArWantBinaryPropertySerialization = true;
		Obj->Serialize(*this);
	}
};

/////////////////////////////////////////////////////
// FBinaryObjectReader

class FBinaryObjectReader : public FObjectReader
{
public:
	FBinaryObjectReader(UObject* Obj, TArray<uint8>& InBytes)
		: FObjectReader(InBytes)
	{
		ArWantBinaryPropertySerialization = true;
		Obj->Serialize(*this);
	}
};

/////////////////////////////////////////////////////
// FAnimationFrameSnapshot

void FUTFrameSnapshot::InitializeFromInstance(UUtilityTree* Instance)
{
	FBinaryObjectWriter Writer(Instance, SerializedData);
	//TimeStamp = Instance->LifeTimer;
}

void FUTFrameSnapshot::CopyToInstance(UUtilityTree* Instance)
{
	FBinaryObjectReader Reader(Instance, SerializedData);
}

#endif

/////////////////////////////////////////////////////
// UUTBlueprintGeneratedClass

UUTBlueprintGeneratedClass::UUTBlueprintGeneratedClass(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RootUTNodeIndex = INDEX_NONE;
}

void UUTBlueprintGeneratedClass::Link(FArchive& Ar, bool bRelinkExistingProperties)
{
	Super::Link(Ar, bRelinkExistingProperties);

	// @TODO: Shouldn't be necessary to clear these, but currently the class gets linked twice during compilation
	RootUTNodeProperty = NULL;

	// Initialize derived members
	for (TFieldIterator<UProperty> It(this); It; ++It)
	{
		if (UStructProperty* StructProp = Cast<UStructProperty>(*It))
		{
			/*if (StructProp->Struct->IsChildOf(FUTNode_Base::StaticStruct()))
			{
				UTNodeProperties.Add(StructProp);
			}*/
		}
	}

	// Pull info down from root utility tree class
	UUTBlueprintGeneratedClass* RootClass = this;
	while(UUTBlueprintGeneratedClass* NextClass = Cast<UUTBlueprintGeneratedClass>(RootClass->GetSuperClass()))
	{
		RootClass = NextClass;
	}

	if(RootClass != this)
	{
		// Copy root, state notifies and baked machines from the root class
		RootUTNodeIndex = RootClass->RootUTNodeIndex;
	}

	if (UTNodeProperties.Num() > 0)
	{
		const bool bValidRootIndex = (RootUTNodeIndex >= 0) && (RootUTNodeIndex < UTNodeProperties.Num());
		if (bValidRootIndex)
		{
			RootUTNodeProperty = UTNodeProperties[UTNodeProperties.Num() - 1 - RootUTNodeIndex];
		}
		else
		{
			UE_LOG(LogUtility, Warning, TEXT("Invalid utility tree root node index %d on '%s' (only %d nodes)"), RootUTNodeIndex, *GetPathName(), UTNodeProperties.Num());
			UTNodeProperties.Empty();

			// After the log instead of in the if() to make sure the log statement is emitted
			// @fixBG : this ensure is blocking copy up so commenting it out. 
			// ensure(bValidRootIndex);
		}
	}

	if(RootClass != this)
	{
		if(OrderedSavedPoseIndices.Num() != RootClass->OrderedSavedPoseIndices.Num() || OrderedSavedPoseIndices != RootClass->OrderedSavedPoseIndices)
		{
			// Derived and our parent has a new ordered pose order, copy over.
			OrderedSavedPoseIndices = RootClass->OrderedSavedPoseIndices;
		}
	}
}

void UUTBlueprintGeneratedClass::PurgeClass(bool bRecompilingOnLoad)
{
	Super::PurgeClass(bRecompilingOnLoad);

#if WITH_EDITORONLY_DATA
	UTBlueprintDebugData = FUTBlueprintDebugData();
#endif
}

uint8* UUTBlueprintGeneratedClass::GetPersistentUberGraphFrame(UObject* Obj, UFunction* FuncToCheck) const
{
	if(!IsInGameThread())
	{
		// we cant use the persistent frame if we are executing in parallel (as we could potentially thunk to BP)
		return nullptr;
	}

	return Super::GetPersistentUberGraphFrame(Obj, FuncToCheck);
}

#if WITH_EDITORONLY_DATA

const int32* UUTBlueprintGeneratedClass::GetNodePropertyIndexFromGuid(FGuid Guid, EAIPropertySearchMode::Type SearchMode /*= EUTPropertySearchMode::OnlyThis*/)
{
	if (SearchMode == EAIPropertySearchMode::OnlyThis)
	{
		return UTBlueprintDebugData.NodeGuidToIndexMap.Find(Guid);
	}
	else
	{
		TArray<const UBlueprintGeneratedClass*> BlueprintHierarchy;
		GetGeneratedClassesHierarchy(this, BlueprintHierarchy);

		for (const UBlueprintGeneratedClass* Blueprint : BlueprintHierarchy)
		{
			if (const UUTBlueprintGeneratedClass* UTBlueprintClass = Cast<UUTBlueprintGeneratedClass>(Blueprint))
			{
				const int32* NodeIndex = UTBlueprintClass->UTBlueprintDebugData.NodeGuidToIndexMap.Find(Guid);

				if (NodeIndex)
				{
					return NodeIndex;
				}
			}

		}
	}

	return NULL;
}

#endif