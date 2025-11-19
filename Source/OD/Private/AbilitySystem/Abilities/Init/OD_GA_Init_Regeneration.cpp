// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Init/OD_GA_Init_Regeneration.h"

#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Init/OD_GE_Init_Regeneration.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;
using namespace	TriggerTags;
using namespace GameplayAbilityTags;
using namespace GameplayEffectTags;

UOD_GA_Init_Regeneration::UOD_GA_Init_Regeneration()
	:Super()
{
	AbilityTags.AddTag(FGameplayTag(OD_Ability_Init_Regeneration));
	
	Effect = UOD_GE_Init_Regeneration::StaticClass();

	FAbilityTriggerData Trig;
	Trig.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	Trig.TriggerTag = FGameplayTag(OD_Init_Regeneration);
	AbilityTriggers.Add(Trig);

	
}

void UOD_GA_Init_Regeneration::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
		if (K2_HasAuthority())
		{

			ASC->RemoveLooseGameplayTag(OD_Init_Regeneration);
			
			FGameplayTagContainer Tags; 
			Tags.AddTag(FGameplayTag(OD_Effect_Init_Regeneration));
			ASC->RemoveActiveEffectsWithGrantedTags(Tags);
			
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(ActorInfo->AvatarActor.Get());

			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect, 1, EffectContext);
			FGameplayEffectSpec& Spec = *EffectSpecHandle.Data.Get();

			float HealthRegeneration = ASC->GetNumericAttribute(UODAttributeSet::GetHealthRegenerationAttribute());
			float ManaRegeneration = ASC->GetNumericAttribute(UODAttributeSet::GetManaRegenerationAttribute());

			Spec.DynamicGrantedTags.AddTag(FGameplayTag(OD_Effect_Init_Regeneration));
			
			Spec.SetSetByCallerMagnitude(OD_Attribute_BaseStats_Health, HealthRegeneration);
			Spec.SetSetByCallerMagnitude(OD_Attribute_BaseStats_Mana, ManaRegeneration);

			ASC->ApplyGameplayEffectSpecToSelf(Spec);
		}
	}


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Init_Regeneration::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(OD_Init_Regeneration);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
