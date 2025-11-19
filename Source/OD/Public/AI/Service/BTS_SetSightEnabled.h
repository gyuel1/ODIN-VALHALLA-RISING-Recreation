// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetSightEnabled.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UBTS_SetSightEnabled : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_SetSightEnabled();
	
	UPROPERTY(EditAnywhere, Category="Percetion")
	bool bDisableOnEnter = true;

	UPROPERTY(EditAnywhere, Category="Percetion")
	bool bEnableOnExit = true;
	
protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	
};
