// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/Sniper/OD_GA_Active_Sniper_001.h"

#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/SpawnActor/Projectiles/ODProjectileActor.h"
#include "Character/ODCombatCharacter.h"

//??
class AODProjectileActor;

UOD_GA_Active_Sniper_001::UOD_GA_Active_Sniper_001()
	:Super()
{
	AbilityTags.AddTag(OD_Active_Sniper_001);
	SkillProjectileParms = FODSkillProjectileParms(3000.f);
}

bool UOD_GA_Active_Sniper_001::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOD_GA_Active_Sniper_001::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	MontageTask->OnCompleted.AddDynamic(this,&UOD_GA_Active_Sniper_001::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this,&UOD_GA_Active_Sniper_001::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this,&UOD_GA_Active_Sniper_001::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this,&UOD_GA_Active_Sniper_001::OnMontageBlendOut);
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
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Sniper_001::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();
	}

	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Sniper_001::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();

	if (K2_HasAuthority())
		CommitAbility(Handle,ActorInfo,ActivationInfo);

	
}

void UOD_GA_Active_Sniper_001::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_Sniper_001::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Sniper_001::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Sniper_001::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Sniper_001::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Sniper_001::OnSpawnSuccess(AActor* SpawnedActor)
{
}

void UOD_GA_Active_Sniper_001::OnSpawnFail(AActor* SpawnedActor)
{
}

void UOD_GA_Active_Sniper_001::OnHitNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority() || !AbilitySpawnActor)
		return;

	if (!InData.EventTag.MatchesTagExact(FGameplayTag(NotifyTags::OD_Notify_Combat_Hit_1)))
		return;

	UODActiveSkillDA* DA = GetActiveSkillDA(CurrentActorInfo);
	if (!DA || !AbilitySpawnActor)
		return;

	AActor* SpawnedActor;
	FGameplayAbilityTargetDataHandle TDH = FGameplayAbilityTargetDataHandle();
	auto* Task = UAbilityTask_SpawnActor::SpawnActor(this, TDH, AbilitySpawnActor);

	Task->Success.AddDynamic(this, &UOD_GA_Active_Sniper_001::OnSpawnSuccess);
	Task->DidNotSpawn.AddDynamic(this, &UOD_GA_Active_Sniper_001::OnSpawnFail);

	if (Task->BeginSpawningActor(this, TDH, AbilitySpawnActor, SpawnedActor))
	{
		if (AODProjectileActor* Projectile = Cast<AODProjectileActor>(SpawnedActor))
		{
			AActor* GetOwner = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor();
			if (AODCombatCharacter* Owner = Cast<AODCombatCharacter>(GetOwner))
			{
				AODCombatCharacter* Target = Cast<AODCombatCharacter>(Owner->GetTarget());

				//Params
				const FActiveSkillData& SkillData = DA->GetActiveSkillData();
				const FODSkillDamageParms DamageParms = FODSkillDamageParms(
					SkillData.DamagePercent, SkillData.DamageCount);


				const FODSkillParameters SkillParameters = FODSkillParameters(DamageParms, SkillProjectileParms);
				Projectile->InitAbilityInfo(
					GetAbilitySystemComponentFromActorInfo(),
					Target,
					SkillParameters);


				//Spawn Transform
				FVector ForwardLoc = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 10.f;
				FRotator Quat = Owner->GetActorRotation();

				FTransform SpawnTransform(Quat, ForwardLoc);
				Projectile->SetActorTransform(SpawnTransform);
			}
		}
	}

	Task->FinishSpawningActor(this, TDH, SpawnedActor);
	Task->ReadyForActivation();
}
	
	



void UOD_GA_Active_Sniper_001::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}
