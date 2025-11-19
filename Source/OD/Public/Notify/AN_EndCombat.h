// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EndCombat.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UAN_EndCombat : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual FString GetNotifyName_Implementation() const override {return TEXT("AN_EndCombat");}
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	
	
};
