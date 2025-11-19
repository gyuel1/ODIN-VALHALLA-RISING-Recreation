// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_MonsterStatus.generated.h"

enum class EMonsterStatus : uint8;
/**
 * 
 */
UCLASS()
class OD_API UBTD_MonsterStatus : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Condition")
	EMonsterStatus DesignatedMonsterStatus;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
	
};