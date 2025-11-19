// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdateTargetFromPerception.generated.h"

class AODPlayer;
class AODAIController;
struct FAIStimulus;
/**
 * 
 */
UCLASS()
class OD_API UBTS_UpdateTargetFromPerception : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY()
	TObjectPtr<AODAIController> AIController;
};
