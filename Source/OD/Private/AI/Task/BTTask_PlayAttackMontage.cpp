// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_PlayAttackMontage.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "AI/ODAIController.h"
#include "Character/ODMonsterBase.h"
#include "Components/ODCombatComponent.h"
#include "Tags/ODAbilityTags.h"

using namespace GameplayAbilityTags;

UBTTask_PlayAttackMontage::UBTTask_PlayAttackMontage()
{
	bCreateNodeInstance = true;
}

void UBTTask_PlayAttackMontage::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
	Super::OnInstanceCreated(OwnerComp);

	//Seed값 생성 및 초기화
	const AActor* Owner = OwnerComp.GetAIOwner();
	const FString Key = FString::Printf(TEXT("%s|%s|%d"), *OwnerComp.GetWorld()->GetMapName(), Owner ? *Owner->GetName() : TEXT(""), 0);
	const uint32 Seed = FCrc::StrCrc32(*Key);

	Random.Initialize((int32)Seed);
}

EBTNodeResult::Type UBTTask_PlayAttackMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
#pragma region Precheck_Cache
	CachedBehaviorComp = &OwnerComp;
	
	CachedAIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (CachedAIController.IsValid())
	{
		 AODMonsterBase* Monster = Cast<AODMonsterBase>(CachedAIController->GetPawn());
		 if (!Monster) return EBTNodeResult::Failed;

		//어빌리티 사용
		CachedASC = Monster->GetAbilitySystemComponent();
		if (!CachedASC.IsValid()) return EBTNodeResult::Failed;
#pragma endregion
		
#pragma region Weights_Build_CDF
		//가중치를 활용한 Tag선택
		TArray<FAttackTypeWeight> TagWeights = {
			{FGameplayTag::RequestGameplayTag("OD.Ability.Combat.NormalAttack"), 0.7f},
			{FGameplayTag::RequestGameplayTag("OD.Ability.AttackType.Skill"), 0.3f}
		};

		FWeightCDF WeightCDF;
		for (const FAttackTypeWeight& W : TagWeights)
		{
			if (W.Tag.MatchesTagExact(OD_Ability_Combat_NormalAttack) || HasCandidatesForTag(W.Tag))
			{
				WeightCDF.AttackType.Add(W);
			}
		}

		if (WeightCDF.AttackType.Num() == 0)
		{
			WeightCDF.AttackType.Add({ OD_Ability_Combat_NormalAttack, 1.f }); // 안전망
		}
		
		WeightCDF.Rebuild();
#pragma endregion

#pragma region Tag_Select
		const int32 PickedIndex = WeightCDF.Pick(Random);
		if (PickedIndex == INDEX_NONE)
		{
			return EBTNodeResult::Failed;
		}

		const FGameplayTag SelectedTag = WeightCDF.AttackType[PickedIndex].Tag;
#pragma endregion

#pragma region Candidates_Filter
		TArray<FGameplayAbilitySpec*> Matching;
		for (FGameplayAbilitySpec& Spec : CachedASC->GetActivatableAbilities())
		{
			if (!Spec.Ability)
			{
				continue;
			}

			const FGameplayTagContainer& AbilityTags = Spec.Ability->AbilityTags;

			//const bool bMatch = AbilityTags.HasAny(SelectedTag) || Spec.DynamicAbilityTags.HasAny(SelectedTag);
			const bool bMatch = AbilityTags.HasTag(SelectedTag) || Spec.DynamicAbilityTags.HasTag(SelectedTag);
			if (!bMatch) continue;

			if (Spec.Ability->DoesAbilitySatisfyTagRequirements(*CachedASC, nullptr, nullptr, nullptr))
			{
				Matching.Add(&Spec);
			}
		}
#pragma endregion

#pragma region GA_TryActivate
		FGameplayAbilitySpec* ChosenSpec;
		if (Matching.Num() > 0)
		{
			const int32 Index = FMath:: RandRange(0, Matching.Num() - 1);
			ChosenSpec = Matching[Index];
		}
		else
		{
			return EBTNodeResult::Failed;
		}
		
		if (!ChosenSpec || ChosenSpec->IsActive())
		{
			//UE_LOG(LogTemp, Error, TEXT("!ChosenSpec && ChosenSpec->IsActive()"));
			return EBTNodeResult::Failed;
		}
		
		if (CachedASC->TryActivateAbility(ChosenSpec->Handle))
		{
			if (UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance())
			{
				CachedAnimInstance = AnimInstance;
				if (!CachedAnimInstance.IsValid())
					return EBTNodeResult::Failed;
				
				UAnimMontage* AttackMontage = AnimInstance->GetCurrentActiveMontage();
				if (AttackMontage)
				{
					FOnMontageBlendingOutStarted BlendingOutDelegate;
					BlendingOutDelegate.BindUObject(this, &UBTTask_PlayAttackMontage::EndPlayMontage);

					AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, AttackMontage);
					
					return EBTNodeResult::InProgress;				
				}			
			}				
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("TryActive false"));
		}
	}
#pragma endregion
	
	return EBTNodeResult::Failed;
}

void UBTTask_PlayAttackMontage::EndPlayMontage(UAnimMontage* Montage, bool bInterrupted)
{
	if (UAnimInstance* AnimInstance = CachedAnimInstance.Get())
	{
		AnimInstance->Montage_Stop(0.f, Montage);
	}
	
	UBehaviorTreeComponent* BT = CachedBehaviorComp.Get();
	if (BT)
	{
		FinishLatentTask(*BT,bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
	}
}

//Tag기반 GA 검사.
bool UBTTask_PlayAttackMontage::HasCandidatesForTag(const FGameplayTag& Tag)
{
	if (!CachedASC.Get())
	{
		return false;
	}

	for (FGameplayAbilitySpec& Spec : CachedASC->GetActivatableAbilities())
	{
		if (!Spec.Ability)
		{
			continue;
		}

		const FGameplayTagContainer& AbilityTags = Spec.Ability->AbilityTags;
		const bool bMatch = AbilityTags.HasTag(Tag) || Spec.DynamicAbilityTags.HasTag(Tag);

		if (!bMatch)
		{
			continue;
		}

		if (Spec.Ability->DoesAbilitySatisfyTagRequirements(*CachedASC, nullptr, nullptr, nullptr))
		{
			return true;
		}
	}

	return false;
}
