// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/DarkMagician/OD_GA_Active_DarkMagician_002.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODCombatCharacter.h"
#include "Helper/ODStaticFunction.h"


UOD_GA_Active_DarkMagician_002::UOD_GA_Active_DarkMagician_002()
	:Super()
{
	AbilityTags.AddTag(OD_Active_DarkMagician_002);
}

bool UOD_GA_Active_DarkMagician_002::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOD_GA_Active_DarkMagician_002::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	MontageTask->OnCompleted.AddDynamic(this,&UOD_GA_Active_DarkMagician_002::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this,&UOD_GA_Active_DarkMagician_002::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this,&UOD_GA_Active_DarkMagician_002::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this,&UOD_GA_Active_DarkMagician_002::OnMontageBlendOut);
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
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_002::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();
	}

	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_DarkMagician_002::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();

	if (K2_HasAuthority())
		CommitAbility(Handle,ActorInfo,ActivationInfo);
}

void UOD_GA_Active_DarkMagician_002::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_DarkMagician_002::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_002::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_002::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_002::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_DarkMagician_002::OnHitNotifyEvent(const FGameplayEventData InData)
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

	if (!Owner->GetTarget())
		return;
	AODCombatCharacter* Target = Cast<AODCombatCharacter>(Owner->GetTarget());
	if (!Target)
		return;

	const FVector OwnerLocation = Owner->GetActorLocation();
	const FVector OwnerForwardVector = Owner->GetActorForwardVector();

	const FVector StartLocation = FVector(OwnerLocation.X  + OwnerForwardVector.X * 500.f,
											OwnerLocation.Y   + OwnerForwardVector.Y * 500.f,
											OwnerLocation.Z);
	
	//Enemy 가져오기
	TArray<AODCombatCharacter*> HitEnemyArray = ODStaticFunc::DetectBoxInRange<AODCombatCharacter>(
		Owner, StartLocation,StartLocation,FVector(650.f,250.f,250.f) );

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

void UOD_GA_Active_DarkMagician_002::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}
