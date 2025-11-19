// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UBTTask_MoveToAttackRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	float AttackRange;

	UPROPERTY()
	float FallbackRange = 50.f;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	UPROPERTY()
	UBlackboardKeyType* TargetKey;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
