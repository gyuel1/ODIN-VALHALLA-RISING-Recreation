// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/Recovery/OD_GE_Instant_Recovery_Vitals.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;

UOD_GE_Instant_Recovery_Vitals::UOD_GE_Instant_Recovery_Vitals()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

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
