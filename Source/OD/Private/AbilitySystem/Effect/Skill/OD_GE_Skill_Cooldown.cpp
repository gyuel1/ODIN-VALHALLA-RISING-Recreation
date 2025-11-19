// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Skill/OD_GE_Skill_Cooldown.h"

#include "Tags/ODAbilityTags.h"


UOD_GE_Skill_Cooldown::UOD_GE_Skill_Cooldown()
{

	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat SBC;
	SBC.DataTag = FGameplayTag(AttributeDataTags::OD_Magnitude_Skill_Cooldown);
	
	DurationMagnitude = FGameplayEffectModifierMagnitude(SBC);

	Period = FScalableFloat(0.f);   
	bExecutePeriodicEffectOnApplication = false;
	
	StackingType = EGameplayEffectStackingType::None;
	
	
}
