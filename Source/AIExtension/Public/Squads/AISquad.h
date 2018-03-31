// Copyright 2015-2018 Piperift. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SquadOrder.h"
#include "AIGeneric.h"
#include "AISquad.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class AIEXTENSION_API AAISquad : public AInfo
{
    GENERATED_BODY()

protected:

    UPROPERTY(EditAnywhere, Category = Generic)
    ECombatState State;

    //Used for editor only
    UPROPERTY(EditInstanceOnly, meta = (DisplayName = "Members"))
    TArray<APawn*> EditorMembers;

    //Used for editor only
    UPROPERTY(EditInstanceOnly, meta = (DisplayName = "Leader"))
    APawn* EditorLeader;

public:

    UPROPERTY(BlueprintReadWrite)
    TArray<AAIGeneric*> Members;

    UPROPERTY(BlueprintReadWrite)
    AAIGeneric* Leader;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector PositionIncrement;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TSubclassOf<USquadOrder> CurrentOrder;


    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool HasMember(const AAIGeneric* member) const {
        return Members.Contains(member);
    }

    UFUNCTION(BlueprintCallable)
    void AddMember(AAIGeneric* Member);

    UFUNCTION(BlueprintCallable)
    void RemoveMember(AAIGeneric* Member);

    UFUNCTION(BlueprintCallable)
    virtual void SetLeader(AAIGeneric* NewLeader);

    UFUNCTION(BlueprintCallable)
    inline AAIGeneric* GetLeader() const
    {
        return Leader;
    }

    UFUNCTION(BlueprintCallable)
    void SendOrder(USquadOrder* Order);

#if WITH_EDITORONLY_DATA
    virtual bool CanEditChange(const UProperty* InProperty) const override;
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    /***************************************
    * INLINES                              *
    ***************************************/

    /** Get the current State of this AI */
    FORCEINLINE const ECombatState GetState() {
        return State;
    }
};
