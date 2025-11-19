// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/Valkyrie/OD_GA_Active_Valkyrie_001.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODPlayer.h"
#include "Debug/ODLogChannels.h"


UOD_GA_Active_Valkyrie_001::UOD_GA_Active_Valkyrie_001()
	:Super()
{
	
}

//Can
bool UOD_GA_Active_Valkyrie_001::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                    const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	
	return true;
}

//Active
void UOD_GA_Active_Valkyrie_001::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!IsActive())
		return;

	UODActiveSkillDA* DA =  GetActiveSkillDA(ActorInfo);
	if (!DA)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	const FActiveSkillData& SkillData = DA->GetActiveSkillData();
	
	if (!IsValid(AbilityMontage))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	//Montage
	auto* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage,
		MontageRate);
	MontageTask->OnCompleted.AddDynamic(this,&UOD_GA_Active_Valkyrie_001::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this,&UOD_GA_Active_Valkyrie_001::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this,&UOD_GA_Active_Valkyrie_001::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this,&UOD_GA_Active_Valkyrie_001::OnMontageBlendOut);
	MontageTask->ReadyForActivation();

	//UAbilityTask_WaitGameplayEvent::WaitGameplayEvent / notify Event
	if (K2_HasAuthority())
	{
		//Hit Event / OD_Notify_Combat_Hit_1
		auto* WaitHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyTags::OD_Notify_Combat_Hit_1,
			nullptr,
			false,
			true);
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_001::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();
	}


	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_001::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();


	if (K2_HasAuthority())
		CommitAbility(Handle,ActorInfo,ActivationInfo);
}

//End
void UOD_GA_Active_Valkyrie_001::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_Valkyrie_001::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_001::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_001::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_001::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UOD_GA_Active_Valkyrie_001::OnHitNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority())
		return;
	
	UODActiveSkillDA* DA = GetActiveSkillDA(CurrentActorInfo);

	//Tag OD.Notify.Combat.Hit.1
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Combat_Hit_1))
	{
		UE_LOG(LogTemp, Warning, TEXT("001 Notify Hit"));

		//Effect
		if (AODPlayer* ActiveCharacter = Cast<AODPlayer>(GetAvatarActorFromActorInfo()))
		{
			if (AActor* Target = ActiveCharacter->GetTarget())
			{
				if (UAbilitySystemComponent* TargetAsc =
					UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
				{
					if (DA)
					{
						const FActiveSkillData& SkillData = DA->GetActiveSkillData();
					
						const FGameplayEffectSpecHandle& EffectSpecHandle = CreateHitEffect(
							UOD_GE_Combat_OnDamage::StaticClass(),
							SkillData.DamagePercent,
							SkillData.DamageCount);

						
						OnHitNiagaraSystem(Target);	
						GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
							*EffectSpecHandle.Data.Get(), TargetAsc);
					}
				}
			}
		}
	}
}

void UOD_GA_Active_Valkyrie_001::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}


