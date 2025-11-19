// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/DarkMagician/OD_GA_Active_DarkMagician_004.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "AbilitySystem/SpawnActor/DelayAttack/ODDelayAttackActor.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODPlayer.h"


UOD_GA_Active_DarkMagician_004::UOD_GA_Active_DarkMagician_004()
	:Super()
{
	AbilityTags.AddTag(OD_Active_DarkMagician_004);
}

bool UOD_GA_Active_DarkMagician_004::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOD_GA_Active_DarkMagician_004::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	MontageTask->OnCompleted.AddDynamic(this,&UOD_GA_Active_DarkMagician_004::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this,&UOD_GA_Active_DarkMagician_004::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this,&UOD_GA_Active_DarkMagician_004::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this,&UOD_GA_Active_DarkMagician_004::OnMontageBlendOut);
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
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_004::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();

		//Hit Event / OD_Notify_Combat_Hit_1
		auto* WaitNiagaraTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyTags::OD_Notify_Trigger_2,
			nullptr,
			false,
			true);
		WaitNiagaraTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_004::OnNiagaraNotifyEvent);
		WaitNiagaraTask->ReadyForActivation();
	}

	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_004::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();

	if (K2_HasAuthority())
		CommitAbility(Handle,ActorInfo,ActivationInfo);

	
}

void UOD_GA_Active_DarkMagician_004::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_DarkMagician_004::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_004::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_004::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_004::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_004::OnSpawnSuccess(AActor* SpawnedActor)
{
}

void UOD_GA_Active_DarkMagician_004::OnSpawnFail(AActor* SpawnedActor)
{
}

void UOD_GA_Active_DarkMagician_004::OnHitNotifyEvent(const FGameplayEventData InData)
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

void UOD_GA_Active_DarkMagician_004::OnNiagaraNotifyEvent(const FGameplayEventData InData)
{
	
	if (!InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_2))
		return;

	if (AODPlayer* ActiveCharacter = Cast<AODPlayer>(GetAvatarActorFromActorInfo()))
	{
		if (AActor* Target = ActiveCharacter->GetTarget())
		{
			OnHitNiagaraSystem(Target);
		}
	}
}

// void UOD_GA_Active_DarkMagician_004::OnHitNotifyEvent(const FGameplayEventData InData)
// {
// 	if (!K2_HasAuthority() || !AbilitySpawnActor)
// 		return;
// 	
// 	if (!InData.EventTag.MatchesTagExact(FGameplayTag(NotifyTags::OD_Notify_Combat_Hit_1)))
// 		return;
//
// 	UODActiveSkillDA* DA = GetActiveSkillDA(CurrentActorInfo);
// 	if (!DA || !AbilitySpawnActor)
// 		return;
//
// 	AActor* SpawnedActor;
// 	FGameplayAbilityTargetDataHandle TDH = FGameplayAbilityTargetDataHandle();
// 	auto* Task = UAbilityTask_SpawnActor::SpawnActor(this, TDH, AbilitySpawnActor);
//
// 	Task->Success.AddDynamic(this, &UOD_GA_Active_DarkMagician_004::OnSpawnSuccess);
// 	Task->DidNotSpawn.AddDynamic(this, &UOD_GA_Active_DarkMagician_004::OnSpawnFail);
//
// 	if (Task->BeginSpawningActor(this, TDH, AbilitySpawnActor, SpawnedActor))
// 	{
// 		if (AODDelayAttackActor* DelayAttackActor = Cast<AODDelayAttackActor>(SpawnedActor))
// 		{
// 			AActor* GetOwner = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor();
// 			if (AODCombatCharacter* Owner = Cast<AODCombatCharacter>(GetOwner))
// 			{
// 				if (AODCombatCharacter* Target = Cast<AODCombatCharacter>(Owner->GetTarget()))
// 				{
// 					//Params
// 					const FActiveSkillData& SkillData = DA->GetActiveSkillData();
// 					const FODSkillDamageParms DamageParms = FODSkillDamageParms(
// 						SkillData.DamagePercent, SkillData.DamageCount);
//
//
// 					const FODSkillParameters SkillParameters = FODSkillParameters(DamageParms, DelayAttackParams);
// 					DelayAttackActor->InitAbilityInfo(
// 						GetAbilitySystemComponentFromActorInfo(),
// 						Target,
// 						SkillParameters);
//
//
// 					//Spawn Transform
// 					DelayAttackActor->AttachToActor(Target, FAttachmentTransformRules::KeepRelativeTransform);
// 					DelayAttackActor->OnTickTrigger();
// 				}
// 			}
// 		}
// 	}
//
// 	Task->FinishSpawningActor(this, TDH, SpawnedActor);
// 	Task->ReadyForActivation();
//
// 	
// }

void UOD_GA_Active_DarkMagician_004::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}
