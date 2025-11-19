// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect//Init/OD_GE_Init_Regeneration.h"

#include "AbilitySystem/ODAttributeSet.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;

UOD_GE_Init_Regeneration::UOD_GE_Init_Regeneration()
{
	
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 5.0f;

	{
		FGameplayModifierInfo Health;
		Health.Attribute = UODAttributeSet::GetHealthAttribute();
		Health.ModifierOp = EGameplayModOp::Additive;

		FSetByCallerFloat SetByCallerHealth;
		SetByCallerHealth.DataTag = OD_Attribute_BaseStats_Health;
		Health.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerHealth);

		Modifiers.Add(Health);
	}

	{
		FGameplayModifierInfo Mana;
		Mana.Attribute = UODAttributeSet::GetManaAttribute();
		Mana.ModifierOp = EGameplayModOp::Additive;

		FSetByCallerFloat SetByCallerMana;
		SetByCallerMana.DataTag = OD_Attribute_BaseStats_Mana;
		Mana.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerMana);

		Modifiers.Add(Mana);
	}

	
}
