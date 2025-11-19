// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ODFieldMobRewardCalculation.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODFieldMobRewardCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UODFieldMobRewardCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	
	
	
};
