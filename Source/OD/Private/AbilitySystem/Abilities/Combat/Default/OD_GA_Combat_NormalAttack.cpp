// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/Default/OD_GA_Combat_NormalAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Debug/ODLogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helper/ODDebugHelper.h"
#include "Player/ODPlayerState.h"
#include "Tags/ODAbilityTags.h"

using namespace GameplayEffectTags;


UOD_GA_Combat_NormalAttack::UOD_GA_Combat_NormalAttack()
	: Super()
{
	MontageRate = 1.0f;
	SKillDamagePercent = 100.f;
	SkillAttackCount = 1.f;

	AbilityTags.AddTag(FGameplayTag(GameplayAbilityTags::OD_Ability_Combat_NormalAttack));
	ActivationBlockedTags.AddTag(FGameplayTag(GameplayAbilityTags::OD_Ability_Block));

	Effect = UOD_GE_Combat_OnDamage::StaticClass();
}

bool UOD_GA_Combat_NormalAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayTagContainer* SourceTags,
                                                    const FGameplayTagContainer* TargetTags,
                                                    FGameplayTagContainer* OptionalRelevantTags) const
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


void UOD_GA_Combat_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo,
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

	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Combat_NormalAttack::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Combat_NormalAttack::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Combat_NormalAttack::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Combat_NormalAttack::OnMontageBlendOut);
	MontageTask->ReadyForActivation();


	//UAbilityTask_WaitGameplayEvent::WaitGameplayEvent / notify Event
	if (K2_HasAuthority())
	{
		//Hit Event
		const FGameplayTag NotifyHitTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Combat.Hit.1"));
		auto* WaitHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyHitTag,
			nullptr,
			true,
			true);
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Combat_NormalAttack::OnHitNotifyEvent);
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
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Combat_NormalAttack::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();
}

void UOD_GA_Combat_NormalAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
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
void UOD_GA_Combat_NormalAttack::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_NormalAttack::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_NormalAttack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_NormalAttack::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//notify Event
void UOD_GA_Combat_NormalAttack::OnHitNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority())
		return;

	//Tag OD.Notify.Combat.Hit.1
	if (InData.EventTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Combat.Hit.1"))))
	{
		
		//Effect
		if (AODCombatCharacter* ActiveCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo()))
		{
			if (!Effect || !ActiveCharacter->GetTarget())
				return;


			if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(ActiveCharacter->GetTarget()))
			{
				if (UAbilitySystemComponent* TargetAsc = AscInterface->GetAbilitySystemComponent())
				{
					//Make EffectSpec
					FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->
						MakeEffectContext();
					EffectContextHandle.AddSourceObject(ActiveCharacter);
					
					FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->
						MakeOutgoingSpec(
							Effect, 1, EffectContextHandle);
					

					FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
					Spec.AddDynamicAssetTag(OD_Effect_Combat_OnDamage);

					Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_DamagePercent, SKillDamagePercent);
					Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_AttackCount, SkillAttackCount);
					
					GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
						*SpecHandle.Data.Get(), TargetAsc);
				}
			}
		}
	}
}

void UOD_GA_Combat_NormalAttack::OnRemoveNotifyEvent(const FGameplayEventData InData)
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

