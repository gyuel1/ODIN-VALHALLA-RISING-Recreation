// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/Valkyrie/OD_GA_Active_Valkyrie_004.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODPlayer.h"
#include "Helper/ODStaticFunction.h"
#include "Notify/OD_ANS_Tag.h"


UOD_GA_Active_Valkyrie_004::UOD_GA_Active_Valkyrie_004()
	: Super()
{
	AbilityTags.AddTag(OD_Active_Valkyrie_004);
	AttackRadius = 0.f;
	JumpRate = 0.f;
}

bool UOD_GA_Active_Valkyrie_004::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayTagContainer* SourceTags,
                                                    const FGameplayTagContainer* TargetTags,
                                                    FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOD_GA_Active_Valkyrie_004::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	for (const FAnimNotifyEvent& Ev : AbilityMontage->Notifies)
	{
		if (Ev.NotifyStateClass && Ev.NotifyStateClass->IsA(UOD_ANS_Tag::StaticClass()))
		{
			JumpRate = Ev.GetDuration();
			break;
		}
	}

	//Montage
	auto* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage,
		MontageRate);
	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnMontageBlendOut);
	MontageTask->ReadyForActivation();

	//UAbilityTask_WaitGameplayEvent::WaitGameplayEvent / notify Event
	if (K2_HasAuthority())
	{
		//Hit Event / OD_Notify_Combat_Hit_1
		auto* WaitHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyTags::OD_Notify_Combat_Hit_1,
			nullptr,
			true,
			true);
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();
	}

		//Hit Event / OD_Notify_Trigger_2
		auto* WaitJumpTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyTags::OD_Notify_Trigger_2,
			nullptr,
			false,
			true);
		WaitJumpTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnJumpNotifyEvent);
		WaitJumpTask->ReadyForActivation();

		


	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_004::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();
	
	if (K2_HasAuthority())
		CommitAbility(Handle, ActorInfo, ActivationInfo);
}

void UOD_GA_Active_Valkyrie_004::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_Valkyrie_004::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_004::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_004::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_004::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_004::OnHitNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority())
		return;

	if (!InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Combat_Hit_1))
		return;

	UODActiveSkillDA* DA = GetActiveSkillDA(CurrentActorInfo);
	if (!DA) return;

	AODCombatCharacter* Owner = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo());
	if (!Owner) return;

	UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
	if (!ASC) return;

	//Enemy 가져오기
	TArray<AODCombatCharacter*> HitEnemyArray = ODStaticFunc::DetectSphereInRange<AODCombatCharacter>(
		Owner, AttackRadius, ECC_GameTraceChannel3);

	const FActiveSkillData& SkillData = DA->GetActiveSkillData();
	const FGameplayEffectSpecHandle& SpecHandle = CreateHitEffect(UOD_GE_Combat_OnDamage::StaticClass(),
	                                                              SkillData.DamagePercent,
	                                                              SkillData.DamageCount);
	const FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();

	for (AODCombatCharacter* HitEnemy : HitEnemyArray)
	{
		UAbilitySystemComponent* TargetASC = HitEnemy->GetAbilitySystemComponent();
		if (!TargetASC)
			continue;

		OnHitNiagaraSystem(HitEnemy);
		ASC->ApplyGameplayEffectSpecToTarget(Spec, TargetASC);
	}
}

void UOD_GA_Active_Valkyrie_004::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}

//Trigger2
void UOD_GA_Active_Valkyrie_004::OnJumpNotifyEvent(const FGameplayEventData InData)
{

	AODPlayer* Player = Cast<AODPlayer>(CurrentActorInfo->AvatarActor);
	if (!Player) return;

	AODCombatCharacter* Target = Cast<AODCombatCharacter>(Player->GetTarget());
	if (!Target) return;
	
	
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_2))
	{
		UAbilityTask_ApplyRootMotionMoveToForce* JumpTask =
			UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(
				this,
				NAME_None,
				Target->GetActorLocation(),
				JumpRate / MontageRate,
				false,
				MOVE_Walking,
				false,
				nullptr,
				ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
		FVector::ZeroVector,
		0.0f);

		JumpTask->ReadyForActivation();
	}
}
