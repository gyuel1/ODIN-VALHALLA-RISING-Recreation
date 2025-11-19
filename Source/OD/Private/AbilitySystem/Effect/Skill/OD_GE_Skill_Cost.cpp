// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Skill/OD_GE_Skill_Cost.h"

#include "AbilitySystem/ODAttributeSet.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;


UOD_GE_Skill_Cost::UOD_GE_Skill_Cost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	{
		FGameplayModifierInfo Mana;
		Mana.Attribute = UODAttributeSet::GetManaAttribute();
		Mana.ModifierOp = EGameplayModOp::Additive;

		FSetByCallerFloat Cost;
		Cost.DataTag = FGameplayTag(OD_Magnitude_Skill_Cost);
		Mana.ModifierMagnitude = FGameplayEffectModifierMagnitude(Cost);

		Modifiers.Add(Mana);
	}
}
