// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ODAnimNotify_Tag.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODAnimNotify_Tag : public UAnimNotify
{
	GENERATED_BODY()

public:
	UODAnimNotify_Tag();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tag", meta=(AllowPrivateAccess=true, Categories="OD.Notify"))
	FGameplayTag Tag;
};
