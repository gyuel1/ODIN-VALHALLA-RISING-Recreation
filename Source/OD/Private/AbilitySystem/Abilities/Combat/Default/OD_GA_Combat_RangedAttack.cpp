// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/Default/OD_GA_Combat_RangedAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_SpawnActor.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "AbilitySystem/SpawnActor/Projectiles/ODProjectileActor.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/TransformCalculus3D.h"
#include "Tags/ODAbilityTags.h"


UOD_GA_Combat_RangedAttack::UOD_GA_Combat_RangedAttack()
{
	MontageRate = 1.0f;
	SKillDamagePercent = 100.f;
	SkillAttackCount = 1.f;
	ProjectileSpeed = 200.f;

	AbilityTags.AddTag(FGameplayTag(GameplayAbilityTags::OD_Ability_Combat_NormalAttack));
	ActivationBlockedTags.AddTag(FGameplayTag(GameplayAbilityTags::OD_Ability_Block));
	
	Effect = UOD_GE_Combat_OnDamage::StaticClass();
}

bool UOD_GA_Combat_RangedAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		if (AODCombatCharacter* CombatCharacter = Cast<AODCombatCharacter>(ActorInfo->AvatarActor))
		{
			if (AActor* Target = CombatCharacter->GetTarget())
			{
				//Dead Check
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target))
				{
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
					{
						const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(TEXT("OD.State.Dead"));
						if (ASC->HasMatchingGameplayTag(DeadTag))
							return false;
					}
				}

				//player
				if (CombatCharacter->IsA(AODPlayer::StaticClass()))
				{
					const FVector SourceLocation = CombatCharacter->GetActorLocation();
					const FVector TargetLocation = Target->GetActorLocation();
					const float Distance = FVector::Dist(SourceLocation, TargetLocation);

					if (Distance <= AbilityRange)
						return true;
				}
			}
			//monster
			if (CombatCharacter->IsA(AODMonsterBase::StaticClass()))
			{
				return true;
			}
		}
	}
	return false;
}

void UOD_GA_Combat_RangedAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (MontageArray.IsEmpty())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	const int32 Index = PickIndexDeterministic(ActivationInfo, MontageArray.Num());
	if (!MontageArray[Index])
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	//이동막기  및 방향 동기화
	if (AODCombatCharacter* MakeCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
		{
			if (MakeCharacter->IsA(AODPlayer::StaticClass()) && MakeCharacter->GetTarget())
			{
				//MakeMove->RotationRate = FRotator::ZeroRotator;
				FVector MyLoc = MakeCharacter->GetActorLocation();
				FVector TargetLoc = MakeCharacter->GetTarget()->GetActorLocation();
				FRotator LookAtRot = (TargetLoc - MyLoc).Rotation();

				MakeCharacter->SetActorRotation(FRotator(MakeCharacter->GetActorRotation().Roll, LookAtRot.Yaw,
				                                         MakeCharacter->GetActorRotation().Pitch));
				MakeMove->bOrientRotationToMovement = false;
				MakeMove->MaxWalkSpeed = 0.f;
			}
		}
	}

	//Block Tag
	const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AddLooseGameplayTag(BlockTag);
	}


	//Montage
	if (IsValid(MontageTask) && MontageTask->GetState() != EGameplayTaskState::Finished)
		return;

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontageArray[Index],
		MontageRate);

	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnMontageBlendOut);
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
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnSpawnNotifyEvent);
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
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();
}

void UOD_GA_Combat_RangedAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AODCombatCharacter* MakeCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (MakeCharacter->IsA(AODPlayer::StaticClass()))
		{
			if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
			{
				MakeMove->bOrientRotationToMovement = true;
				MakeMove->MaxWalkSpeed = 600.f;
			}
		}
	}

	//Remove Block Tag
	const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(BlockTag);
	}

	//EndTask
	if (MontageTask && MontageTask->GetState() != EGameplayTaskState::Finished)
	{
		MontageTask->EndTask();
	}
	MontageTask = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
//call back
void UOD_GA_Combat_RangedAttack::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_RangedAttack::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_RangedAttack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_RangedAttack::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_RangedAttack::OnSpawnSuccess(AActor* SpawnedActor)
{
	// if (!K2_HasAuthority())
	// 	return;
	
}

void UOD_GA_Combat_RangedAttack::OnSpawnFail(AActor* SpawnedActor)
{
	// if (!K2_HasAuthority())
	// 	return;
	//
	// if (!SpawnedActor)
	// 	SpawnedActor->Destroy();
}


void UOD_GA_Combat_RangedAttack::OnSpawnNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority() || !ProjectileActor)
		return;
	
	if (InData.EventTag.MatchesTagExact(FGameplayTag(NotifyTags::OD_Notify_Combat_Hit_1)))
	{
		AActor* SpawnedActor;
		FGameplayAbilityTargetDataHandle TDH = FGameplayAbilityTargetDataHandle();
		auto* Task = UAbilityTask_SpawnActor::SpawnActor(this, TDH, ProjectileActor);
		
		Task->Success.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnSpawnSuccess);
		Task->DidNotSpawn.AddDynamic(this, &UOD_GA_Combat_RangedAttack::OnSpawnFail);

		if (Task->BeginSpawningActor(this, TDH, ProjectileActor, SpawnedActor))
		{
			if (AODProjectileActor* Projectile = Cast<AODProjectileActor>(SpawnedActor))
			{
				AActor* GetOwner = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor();
				if (AODCombatCharacter* Owner = Cast<AODCombatCharacter>(GetOwner))
				{
					AODCombatCharacter* Target = Cast<AODCombatCharacter>(Owner->GetTarget());
					if (!IsValid(Target))
						return;
					
					//Params
					const FODSkillDamageParms DamageParms = FODSkillDamageParms(SKillDamagePercent,SkillAttackCount);
					const FODSkillProjectileParms ProjectileParms = (ProjectileSpeed);
					
					const FODSkillParameters SkillParameters = FODSkillParameters(DamageParms,ProjectileParms);
					Projectile->InitAbilityInfo(
						GetAbilitySystemComponentFromActorInfo(),
						Target,
						SkillParameters);
					
					FVector ForwardLoc =  Owner->GetActorLocation() + Owner->GetActorForwardVector() * 10.f; 
					FRotator  Quat = Owner->GetActorRotation();
					
					FTransform SpawnTransform(Quat,ForwardLoc);
					Projectile->SetActorTransform(SpawnTransform);
				}
			}
		}
		
		Task->FinishSpawningActor(this,TDH,SpawnedActor);
		Task->ReadyForActivation();

		
	}
}

void UOD_GA_Combat_RangedAttack::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	//Tag OD.Notify.Trigger.1
	if (InData.EventTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Trigger.1"))))
	{
		//Block Tag
		const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(BlockTag);
		}
	}
}
