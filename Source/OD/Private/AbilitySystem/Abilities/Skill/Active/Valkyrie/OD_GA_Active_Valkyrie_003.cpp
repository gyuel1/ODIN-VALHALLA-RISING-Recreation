// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/Valkyrie/OD_GA_Active_Valkyrie_003.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODCombatCharacter.h"
#include "Helper/ODStaticFunction.h"


UOD_GA_Active_Valkyrie_003::UOD_GA_Active_Valkyrie_003()
	:Super()
{
	AbilityTags.AddTag(FGameplayTag(OD_Active_Valkyrie_003));
	CheckAttack = 0;
}

bool UOD_GA_Active_Valkyrie_003::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	 if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	 	return false;

	return true;
}

void UOD_GA_Active_Valkyrie_003::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	MontageTask->OnCompleted.AddDynamic(this,&UOD_GA_Active_Valkyrie_003::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this,&UOD_GA_Active_Valkyrie_003::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this,&UOD_GA_Active_Valkyrie_003::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this,&UOD_GA_Active_Valkyrie_003::OnMontageBlendOut);
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
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_003::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();

		//Spawn Event / OD_Notify_Combat_Spawn_1
		const FGameplayTag NotifySpawnTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Combat.Spawn.1"));
		auto* WaitSpawnTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifySpawnTag,
			nullptr,
			true,
			true);
		WaitSpawnTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_003::OnSpawnNotifyEvent);
		WaitSpawnTask->ReadyForActivation();
	}
	
	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_003::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();
	if (K2_HasAuthority())
		CommitAbility(Handle,ActorInfo,ActivationInfo);
}

void UOD_GA_Active_Valkyrie_003::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_Valkyrie_003::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_003::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_003::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_003::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_003::OnSpawnSuccess(AActor* SpawnedActor)
{
}

void UOD_GA_Active_Valkyrie_003::OnSpawnFail(AActor* SpawnedActor)
{
}


//공격에 맞은적
void UOD_GA_Active_Valkyrie_003::OnHitNotifyEvent(const FGameplayEventData InData)
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
	if (!ASC)	return;
		
	
	//Enemy 가져오기
	const FVector MakeOffset = Owner->GetActorForwardVector() * AttackOffsetRange;
	TArray<AODCombatCharacter*> HitEnemyArray = ODStaticFunc::DetectSphereInRange<AODCombatCharacter>(
		Owner, AttackRange, ECC_GameTraceChannel3, MakeOffset);

	if (HitEnemyArray.IsEmpty())
		return;
	
	//Effect 스펙 만들기
	const FActiveSkillData& SkillData = DA->GetActiveSkillData();
	const FGameplayEffectSpecHandle& SpecHandle = CreateHitEffect(UOD_GE_Combat_OnDamage::StaticClass(),
																  SkillData.DamagePercent,
																  SkillData.DamageCount);
	const FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();

	//순회하며 로직 실행
	for (AODCombatCharacter* HitEnemy : HitEnemyArray)
	{
		if (!IsValid(HitEnemy))
			continue;
		
		UAbilitySystemComponent* TargetASC = HitEnemy->GetAbilitySystemComponent();
		if (!TargetASC)
			continue;

		if (TargetASC->HasMatchingGameplayTag(FGameplayTag(OD_State_Dead)))
		{
			continue;
		}

		//OnDamage
		ASC->ApplyGameplayEffectSpecToTarget(Spec, TargetASC);

		//처음맞았으면 딜레이어택 달아주기 + 배열에 추가
		if (!DelayAttackArray.Contains(FDelayAttackLink(HitEnemy)))
		{
			AActor* SpawnedActor;
			FGameplayAbilityTargetDataHandle TDH = FGameplayAbilityTargetDataHandle();
			auto* Task = UAbilityTask_SpawnActor::SpawnActor(this, TDH, DelayActorClass);

			Task->Success.AddDynamic(this, &UOD_GA_Active_Valkyrie_003::OnSpawnSuccess);
			Task->DidNotSpawn.AddDynamic(this, &UOD_GA_Active_Valkyrie_003::OnSpawnFail);

			if (Task->BeginSpawningActor(this, TDH, DelayActorClass, SpawnedActor))
			{
				if (AODDelayAttackActor* DelayAttackActor = Cast<AODDelayAttackActor>(SpawnedActor))
				{
					//SkillParameters
					const FODSkillDamageParms& DamageParms = FODSkillDamageParms(
						SkillData.DamagePercent, SkillData.DamageCount);
					const FODSkillParameters& SkillParameters = FODSkillParameters(DamageParms, DelayAttackParams);
					DelayAttackActor->InitAbilityInfo(ASC, HitEnemy, SkillParameters);

					//배열에 추가
					DelayAttackArray.Add(FDelayAttackLink(HitEnemy, DelayAttackActor));


					SpawnedActor->AttachToActor(HitEnemy,FAttachmentTransformRules::KeepRelativeTransform);
				}
			}
			Task->FinishSpawningActor(this, TDH, SpawnedActor);
			if (!IsValid(SpawnedActor))
				Task->EndTask();
			
			Task->ReadyForActivation();
		}
	}

	//딜레이 어택 카운트 시작
	CheckAttack++;
	if (CheckAttack >= SkillData.DamageCount)
	{
		for (FDelayAttackLink& DelayAttack : DelayAttackArray)
		{
			if (IsValid(DelayAttack.DelayAttackActor))
				DelayAttack.DelayAttackActor->OnTickTrigger();
		}
	}
		
	
}

//데칼만 표시할건데 이게 필요함?
void UOD_GA_Active_Valkyrie_003::OnSpawnNotifyEvent(const FGameplayEventData InData)
{

	// 데칼을 꼭 스폰해야하는가? 좀고민해봄
	//데칼 스폰 해야함 spawn trigger
}

void UOD_GA_Active_Valkyrie_003::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	//데칼 지우고 (고민중)
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
	
}
