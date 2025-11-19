// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Init/OD_GA_Init_MaxExp.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Init/OD_GE_Init_MaxExp.h"
#include "Tags/ODAbilityTags.h"
using namespace GameplayAbilityTags;
using namespace TriggerTags;


UOD_GA_Init_MaxExp::UOD_GA_Init_MaxExp()
	:Super()
{
	AbilityTags.AddTag(OD_Ability_Init_MaxExp);

	FAbilityTriggerData Trig;
	Trig.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	Trig.TriggerTag = FGameplayTag(OD_Init_MaxExp);
	AbilityTriggers.Add(Trig);

	Effect = UOD_GE_Init_MaxExp::StaticClass();
}

bool UOD_GA_Init_MaxExp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	return true;
}

void UOD_GA_Init_MaxExp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!K2_HasAuthority() || !Effect || !ASC)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
		return;
	}
	
	FGameplayEffectContextHandle ContextHandle =  ASC->MakeEffectContext();
	
	const float Level = ASC->GetNumericAttribute(UODAttributeSet::GetLevelAttribute());
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect,Level,ContextHandle);

	FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
	ASC->ApplyGameplayEffectSpecToSelf(Spec);

	EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
}

void UOD_GA_Init_MaxExp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(OD_Init_MaxExp);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
