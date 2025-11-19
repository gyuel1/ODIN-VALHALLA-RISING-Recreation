// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_MonsterEngageType.generated.h"

enum class EMonsterEngageType : uint8;
/**
 * 
 */
UCLASS()
class OD_API UBTD_MonsterEngageType : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Condition")
	EMonsterEngageType EngageType;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
	
};
