// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"

#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Calculation/ODOnDamageCalculation.h"
#include "Tags/ODAbilityTags.h"


UOD_GE_Combat_OnDamage::UOD_GE_Combat_OnDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayEffectExecutionDefinition& ExecDef = Executions.AddDefaulted_GetRef();
	ExecDef.CalculationClass = UODOnDamageCalculation::StaticClass();
	
}
