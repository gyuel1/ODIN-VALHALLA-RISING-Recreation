// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_SetMovementSpeed.generated.h"

class AODMonsterBase;
class AODAIController;
/**
 * 
 */
UCLASS()
class OD_API UBTS_SetMovementSpeed : public UBTService
{
	GENERATED_BODY()

	UBTS_SetMovementSpeed();

public:	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
