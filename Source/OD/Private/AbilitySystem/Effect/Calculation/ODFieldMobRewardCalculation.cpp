// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Calculation/ODFieldMobRewardCalculation.h"

#include <cmath>

#include "AbilitySystem/ODPlayerAttributeSet.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;
//

UODFieldMobRewardCalculation::UODFieldMobRewardCalculation()
{
}

void UODFieldMobRewardCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters Eval;
	Eval.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	Eval.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Gold = Spec.GetSetByCallerMagnitude(OD_Attribute_Transaction_Gold,false,0.f);
	float Exp = Spec.GetSetByCallerMagnitude(OD_Attribute_Transaction_Exp,false,0.f);
	float Weight = Spec.GetSetByCallerMagnitude(OD_Magnitude_Reward_Weight,false,0.f);

	//========================================Calculation========================================//
	
	if (Weight <= 0.f)
		return;

	float GoldResult = Gold > 0.f ? Weight * Gold : 0.f;
	if (GoldResult > 0.f)
	{
		const float Rand = FMath::RandRange(0.9f,1.1f);
		GoldResult = std::trunc(GoldResult * Rand);
		
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UODPlayerAttributeSet::GetGoldAttribute(),
				EGameplayModOp::Additive,
				GoldResult));
	}

	float ExpResult = Exp > 0.f ? Weight * Exp : 0.f;
	if (ExpResult > 0.f)
	{
		ExpResult = std::trunc(ExpResult);
		
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UODPlayerAttributeSet::GetCurrentExpAttribute(),
				EGameplayModOp::Additive,
				ExpResult));
	}
}
