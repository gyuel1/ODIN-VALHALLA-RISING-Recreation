// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "OD_ANS_Tag.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UOD_ANS_Tag : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag",
		meta = (AllowPrivateAccess = true, Categories="OD.Notify"))
	FGameplayTag Tag;
};
