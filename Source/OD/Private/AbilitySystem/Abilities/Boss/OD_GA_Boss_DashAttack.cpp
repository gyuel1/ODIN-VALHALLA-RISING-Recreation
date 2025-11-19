// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Boss/OD_GA_Boss_DashAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/ODMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToActorForce.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/ODAbilityTags.h"


bool UOD_GA_Boss_DashAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return true;
}

void UOD_GA_Boss_DashAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!K2_HasAuthority()) return;
	
	bIsAbilityEnding = false;
	
	if (TriggerEventData == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!TriggerEventData->EventTag.MatchesTagExact(NotifyTags::OD_Notify_ANS_1))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	CachedMonster = Cast<AODMonsterBase>(ActorInfo->AvatarActor.Get());
	if (CachedMonster == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (UAnimInstance* Anim = CachedMonster->GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Stop(0.1f);
	}

	if (UAbilitySystemComponent* ASC = CachedMonster->FindComponentByClass<UAbilitySystemComponent>())
	{
		const FGameplayTag Block = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
		ASC->AddLooseGameplayTag(Block);
		
		FGameplayTagContainer ToCancel;
		ToCancel.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.AttackType.Skill.AOE.SingleHit")));
		
		ASC->CancelAbilities(&ToCancel, nullptr);
	}

	const FVector StartLocation = CachedMonster->GetActorLocation();
	const FVector ForwardXY = CachedMonster->GetActorForwardVector().GetSafeNormal2D();

	FVector EndLocation = StartLocation + ForwardXY * DashDistance;

	if (bLockFacingDuringDash)
	{
		if (auto* Move = CachedMonster->GetCharacterMovement())
		{
			Move->bOrientRotationToMovement = false;
			CachedMonster->bUseControllerRotationYaw = true;
			CachedMonster->SetActorRotation(ForwardXY.Rotation());
			UE_LOG(LogTemp, Error, TEXT("Rotation Setting"));
		}
	}

	float Duration = (DashSpeed > 0.f) ? DashDistance / DashSpeed : 0.1f;
	
	auto* MoveTask = UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(
			this,
		NAME_None,
		EndLocation,
		Duration,
		false,
		MOVE_Walking, 
		false,
		nullptr,
		ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
		FVector::ZeroVector,
		0.0f
	);

	if (MoveTask)
	{
		MoveTask->OnTimedOut.AddDynamic(this, &UOD_GA_Boss_DashAttack::OnMoveFinished);
		MoveTask->OnTimedOutAndDestinationReached.AddDynamic(this, &UOD_GA_Boss_DashAttack::OnMoveFinished);
		MoveTask->ReadyForActivation();
	}
}

void UOD_GA_Boss_DashAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (CachedMonster == nullptr) return;

	UE_LOG(LogTemp, Error, TEXT("EndAbility"));

	//Remove Block Tag
	const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(BlockTag);
	}
	
	if (bLockFacingDuringDash)
	{
		if (auto* Move = CachedMonster->GetCharacterMovement())
		{
			Move->bOrientRotationToMovement = true;
			CachedMonster->bUseControllerRotationYaw = false;
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Boss_DashAttack::OnMoveFinished()
{
	if (AODMonsterBase* Monster = Cast<AODMonsterBase>(GetAvatarActorFromActorInfo()))
	{
		if (UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance())
		{
			if (DashAttackEndMontage)
			{
				float PlayRate = 1.0f;

				UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DashAttackEndMontage, PlayRate);
				MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Boss_DashAttack::OnMontageCompleted);
				MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Boss_DashAttack::OnMontageCompleted);
				MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Boss_DashAttack::OnMontageCompleted);
				MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Boss_DashAttack::OnMontageCompleted);

				MontageTask->ReadyForActivation();
			}
			else
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
		}
	}
}

void UOD_GA_Boss_DashAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}