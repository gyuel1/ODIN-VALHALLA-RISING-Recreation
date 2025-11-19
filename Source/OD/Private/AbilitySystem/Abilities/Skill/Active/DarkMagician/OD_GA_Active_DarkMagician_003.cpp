// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/DarkMagician/OD_GA_Active_DarkMagician_003.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "AbilitySystem/SpawnActor/Hazard/ODHazardActor.h"
#include "Character/ODPlayer.h"


UOD_GA_Active_DarkMagician_003::UOD_GA_Active_DarkMagician_003()
	: Super()
{
	AbilityTags.AddTag(FGameplayTag(OD_Active_DarkMagician_003));
}

bool UOD_GA_Active_DarkMagician_003::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                        const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayTagContainer* SourceTags,
                                                        const FGameplayTagContainer* TargetTags,
                                                        FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	return true;
}

void UOD_GA_Active_DarkMagician_003::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsActive())
		return;

	UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo);
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
	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnMontageBlendOut);
	MontageTask->ReadyForActivation();

	//UAbilityTask_WaitGameplayEvent::WaitGameplayEvent / notify Event
	if (K2_HasAuthority())
	{
		//Hit Event
		const FGameplayTag NotifySpawnTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Combat.Hit.1"));
		auto* WaitHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifySpawnTag,
			nullptr,
			true,
			true);
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnSpawnNotifyEvent);
		WaitHitTask->ReadyForActivation();
	}

	//Remove Block Tag
	const FGameplayTag NotifyRemoveTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Trigger.1"));
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyRemoveTag,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();

	if (K2_HasAuthority())
		CommitAbility(Handle, ActorInfo, ActivationInfo);
}

void UOD_GA_Active_DarkMagician_003::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_DarkMagician_003::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_003::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UOD_GA_Active_DarkMagician_003::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_003::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_003::OnSpawnSuccess(AActor* SpawnedActor)
{
}

void UOD_GA_Active_DarkMagician_003::OnSpawnFail(AActor* SpawnedActor)
{
}

void UOD_GA_Active_DarkMagician_003::OnSpawnNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority())
		return;

	if (!InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Combat_Hit_1))
		return;

	UODActiveSkillDA* DA = GetActiveSkillDA(CurrentActorInfo);
	if (!DA || !AbilitySpawnActor)
		return;

	//Tag OD.Notify.Combat.Hit.1
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Combat_Hit_1))
	{
		AActor* SpawnedActor;
		FGameplayAbilityTargetDataHandle TDH = FGameplayAbilityTargetDataHandle();
		auto* Task = UAbilityTask_SpawnActor::SpawnActor(this, TDH, AbilitySpawnActor);

		Task->Success.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnSpawnSuccess);
		Task->DidNotSpawn.AddDynamic(this, &UOD_GA_Active_DarkMagician_003::OnSpawnFail);

		if (Task->BeginSpawningActor(this, TDH, AbilitySpawnActor, SpawnedActor))
		{
			
			if (AODHazardActor* MakeSpawnActor = Cast<AODHazardActor>(SpawnedActor))
			{
				AActor* GetOwner = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor();
				if (AODCombatCharacter* Owner = Cast<AODCombatCharacter>(GetOwner))
				{
					AODCombatCharacter* Target = Cast<AODCombatCharacter>(Owner->GetTarget());
					if (!Target)
						return;

					//Params (AODHazardActor)
					const  FActiveSkillData SkillData = DA->GetActiveSkillData();
					const FODSkillDamageParms DamageParms = FODSkillDamageParms(SkillData.DamagePercent, SkillData.DamageCount);
					
					const FODSkillParameters SkillParameters = FODSkillParameters(DamageParms, HazardPrams);
					
					MakeSpawnActor->InitAbilityInfo(
						GetAbilitySystemComponentFromActorInfo(),
						Target,
						SkillParameters);
					
					//SpawnPoint
					FVector ForwardLoc = Target->GetActorLocation();
					FRotator Rot = Owner->GetActorRotation();

					FTransform SpawnTransform(Rot, ForwardLoc);
					MakeSpawnActor->SetActorTransform(SpawnTransform);
				}
			}
		}
		Task->FinishSpawningActor(this,TDH,SpawnedActor);
		Task->ReadyForActivation();
	}
}

void UOD_GA_Active_DarkMagician_003::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}
