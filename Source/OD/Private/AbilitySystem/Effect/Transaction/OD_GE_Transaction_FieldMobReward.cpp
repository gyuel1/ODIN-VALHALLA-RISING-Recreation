// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Transaction/OD_GE_Transaction_FieldMobReward.h"
#include "AbilitySystem/Effect/Calculation/ODFieldMobRewardCalculation.h"


UOD_GE_Transaction_FieldMobReward::UOD_GE_Transaction_FieldMobReward()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayEffectExecutionDefinition& ExecDef = Executions.AddDefaulted_GetRef();
	ExecDef.CalculationClass = UODFieldMobRewardCalculation::StaticClass();
}
