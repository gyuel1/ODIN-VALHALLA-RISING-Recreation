// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Init/OD_GA_Init_FillVitals.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Recovery/OD_GE_Instant_Recovery_Vitals.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;
using namespace TriggerTags;

UOD_GA_Init_FillVitals::UOD_GA_Init_FillVitals()
	:Super()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Init.FillVitals")));

	Effect = UOD_GE_Instant_Recovery_Vitals::StaticClass();

	FAbilityTriggerData Trig;
	Trig.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	Trig.TriggerTag = FGameplayTag(OD_Init_FillVitals);
	AbilityTriggers.Add(Trig);
}

void UOD_GA_Init_FillVitals::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

if (!Effect)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	return;
}

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(ActorInfo->AvatarActor.Get());
		
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect, 1, EffectContext);
		FGameplayEffectSpec& Spec = *EffectSpecHandle.Data.Get();

		float MaxHealth = ASC->GetNumericAttribute(UODAttributeSet::GetMaxHealthAttribute());
		float MaxMana = ASC->GetNumericAttribute(UODAttributeSet::GetMaxManaAttribute());
				
		MaxHealth = FMath::Clamp<float>(MaxHealth, 0.0f, MaxHealth - ASC->GetNumericAttribute(UODAttributeSet::GetHealthAttribute()));
		MaxMana = FMath::Clamp<float>(MaxMana, 0.0f, MaxMana - ASC->GetNumericAttribute(UODAttributeSet::GetManaAttribute()));

		Spec.DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Effect.Init.FillVitals")));
		Spec.SetSetByCallerMagnitude(OD_Attribute_BaseStats_Health, MaxHealth);
		Spec.SetSetByCallerMagnitude(OD_Attribute_BaseStats_Mana, MaxMana);

		ASC->ApplyGameplayEffectSpecToSelf(Spec);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Init_FillVitals::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(OD_Init_FillVitals);
	}

	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
