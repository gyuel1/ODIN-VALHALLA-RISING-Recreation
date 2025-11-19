// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Boss/OD_GA_Boss_Enrage.h"

#include "BrainComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "AI/ODAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/ODMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags/ODAbilityTags.h"

// OD.Ability.Boss.Enrage 

bool UOD_GA_Boss_Enrage::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                            const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	
	if (!ASC) return false;

	FGameplayTagContainer Blocked;
	// block할 태그
	Blocked.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block")));
	Blocked.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Effect.Enrage.Status")));
	Blocked.AddTag(GameplayAbilityTags::OD_Ability_Combat_State_Death);
	if (ASC->HasAnyMatchingGameplayTags(Blocked))
	{
		return false;
	}

	//현재 실행중인 AM이 있으면 실패.
	if (UAnimMontage* PlayCurrentMontage = GetCurrentMontage())
	{
		return false;
	}
	
	return true;
}

void UOD_GA_Boss_Enrage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_HasAuthority()) return;
	
	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const int32 Index = PickIndexDeterministic(ActivationInfo, MontageArray.Num());
	if (!MontageArray[Index])
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	UClass* GEClass = EnrageStatusEffect.Get();
	if (!GEClass->IsChildOf(UGameplayEffect::StaticClass()))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GEClass, 1, Ctx);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	
	if (ActorInfo->AvatarActor.IsValid())
	{
		if (AODMonsterBase* Monster = Cast<AODMonsterBase>(ActorInfo->AvatarActor))
		{
			if (AODAIController* Controller = Cast<AODAIController>(Monster->GetController()))
			{
				if (UBehaviorTreeComponent* BTC = Cast<UBehaviorTreeComponent>(Controller->GetBrainComponent()))
				{
					BTC->StopTree(EBTStopMode::Forced);
				}
			}
		}
	}

	//이동 막기
	if (AODCombatCharacter* MakeCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
		{
			MakeMove->MaxWalkSpeed = 0.f;
		}
	}
	
	//Montage
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontageArray[Index],
		MontageRate);

	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Boss_Enrage::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Boss_Enrage::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Boss_Enrage::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Boss_Enrage::OnMontageBlendOut);
	MontageTask->ReadyForActivation();
}

void UOD_GA_Boss_Enrage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (ActorInfo->AvatarActor.IsValid())
	{
		if (AODMonsterBase* Monster = Cast<AODMonsterBase>(ActorInfo->AvatarActor))
		{
			if (AODAIController* Controller = Cast<AODAIController>(Monster->GetController()))
			{
				if (UBehaviorTreeComponent* BTC = Cast<UBehaviorTreeComponent>(Controller->GetBrainComponent()))
				{
					BTC->RestartTree();
				}
			}
		}
	}
}

#pragma region MontageDele
void UOD_GA_Boss_Enrage::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Boss_Enrage::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Boss_Enrage::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Boss_Enrage::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
#pragma endregion

int32 UOD_GA_Boss_Enrage::PickIndexDeterministic(const FGameplayAbilityActivationInfo& Info, int32 Count)
{
	const FPredictionKey Key = Info.GetActivationPredictionKey();
	if (Key.IsValidKey())
	{
		uint32 Seed = 0x9E3779B9u;
		Seed ^= (uint32)Key.Current;
		Seed ^= ((uint32)Key.Base * 2654435761u);
		Seed ^= FCrc::StrCrc32(*GetClass()->GetPathName());

		FRandomStream S(Seed);
		(void)S.GetUnsignedInt();
		return S.RandHelper(Count);
	}
	return FMath::RandHelper(Count);
}

