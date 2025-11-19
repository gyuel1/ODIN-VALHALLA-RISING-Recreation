// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_ReturnToStartIfFar.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UBTS_ReturnToStartIfFar : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_ReturnToStartIfFar();
	
	UPROPERTY(EditAnywhere, Category = "Key")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Key")
	FBlackboardKeySelector bReturnSpawnLocationKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
