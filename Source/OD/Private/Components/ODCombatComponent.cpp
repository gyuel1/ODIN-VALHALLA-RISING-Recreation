// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Abilities/Skill/Active/OD_GA_Active_Base.h"
#include "Character/ODCombatCharacter.h"
#include "DataAssets/Ability/ODActiveSkillDA.h"
#include "DataAssets/Ability/ODPassiveSkillDA.h"
#include "DSP/AudioDebuggingUtilities.h"
#include "Net/UnrealNetwork.h"
#include "PrimaryDataAsset/DT/ODBaseStatsPDA.h"
#include "PrimaryDataAsset/GenericGA/OD_PDA_GenericGA.h"
#include "Structs/StatsDataStruct.h"
#include "Subsystem/ODGameDataSubsystem.h"
#include "Systems/ODAssetManager.h"
#include "Tags/ODAbilityTags.h"

using namespace AttributeDataTags;

// Sets default values for this component's properties
UODCombatComponent::UODCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UODCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODCombatComponent,ActiveAbilityArray);
	
}

void UODCombatComponent::ODCombatInitialize(AODCombatCharacter* InOwner)
{
	if (!GetOwner()->HasAuthority())
		return;
	
	if (UAbilitySystemComponent* ASC = InOwner->GetAbilitySystemComponent())
	{
		AActor* Avatar = ASC->GetAvatarActor();
		// 공용 어빌리티 찾기
		TArray<UOD_PDA_GenericGA*> Array = UODAssetManager::Get().LoadSyncPrimaryDataAsset<UOD_PDA_GenericGA>(FName("GenericGA"));
		UOD_PDA_GenericGA* GenericGA = nullptr;
		
		for (UOD_PDA_GenericGA* LoopGenericGA : Array)
		{
			if (LoopGenericGA->GetODCharacterType() == InOwner->GetODCharacterTyp())
			{
				GenericGA = LoopGenericGA;
				break;
			}
		}
		if (!GenericGA)
			return;

		//부여 및 사용
		for (TSubclassOf GiveAndTryGA : GenericGA->GetGenericGiveAndTryGA())
		{
			if (GiveAndTryGA)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GiveAndTryGA);
				FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);

				if (AbilitySpec.Ability->AbilityTags.IsValid())
				{
					ASC->TryActivateAbilitiesByTag(AbilitySpec.Ability->AbilityTags);
				}
			}
		}
		//단순 부여
		for (TSubclassOf GiveGA : GenericGA->GetGenericGiveGA())
		{
			if (GiveGA) 
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GiveGA);
				FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
			
			}
		}
		

		//액터별 고유 어빌리티 부여
		for (UODAbilityDA* ActiveAbilityDA : ActiveAbilityArray)
		{
			if (ActiveAbilityDA->GetAbility())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ActiveAbilityDA->GetAbility());
				FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
				ActiveAbilityHandle.Add(ActiveAbilityDA->GetTag(),Handle);
			}
		}
	}
}

FGameplayAbilitySpecHandle UODCombatComponent::FindAbilitySpecHandleByTag(const FGameplayTag& InTag)
{
	//기본 어빌리티 컨테이너에서 찾기
	if (ActiveAbilityHandle.Contains(InTag))
	{
 		return *ActiveAbilityHandle.Find(InTag);
	}
	
	return FGameplayAbilitySpecHandle();
}

FGameplayAbilitySpec* UODCombatComponent::FindAbilitySpecByTag(const FGameplayTag& InTag,
                                                               UAbilitySystemComponent* InASC)
{
	if (InASC)
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(InTag);

		TArray<FGameplayAbilitySpec*> Specs;
		InASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, Specs);

		if (!Specs.IsEmpty())
			return Specs[0];
	}

	return nullptr;
}


void UODCombatComponent::InitStats()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
		return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			ASC->SetNumericAttributeBase(UODAttributeSet::GetLevelAttribute(), Stats.Level);
			
			ASC->SetNumericAttributeBase(UODAttributeSet::GetHealthAttribute(), Stats.MaxHealth);
			ASC->SetNumericAttributeBase(UODAttributeSet::GetMaxHealthAttribute(), Stats.MaxHealth );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetHealthRegenerationAttribute(), Stats.HealthRegeneration );

			ASC->SetNumericAttributeBase(UODAttributeSet::GetManaAttribute(), Stats.MaxMana );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetMaxManaAttribute(), Stats.MaxMana );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetManaRegenerationAttribute(), Stats.ManaRegeneration );

			ASC->SetNumericAttributeBase(UODAttributeSet::GetMovementSpeedAttribute(), Stats.MovementSpeed );

			ASC->SetNumericAttributeBase(UODAttributeSet::GetAttackPowerAttribute(), Stats.AttackPower );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetAttackSpeedAttribute(), Stats.AttackSpeed );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetAccuracyAttribute(), Stats.Accuracy );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetCriticalChanceAttribute(), Stats.CriticalChance );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetCriticalDamageAttribute(), Stats.CriticalDamage );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetDamageIncreaseAttribute(), Stats.DamageIncrease );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetCrowdControlAccuracyAttribute(), Stats.CrowdControlAccuracy );

			ASC->SetNumericAttributeBase(UODAttributeSet::GetDefenseAttribute(), Stats.Defense );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetEvasionAttribute(), Stats.Evasion );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetDamageReductionAttribute(), Stats.DamageReduction );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetCrowdControlResistanceAttribute(), Stats.CrowdControlResistance );
			ASC->SetNumericAttributeBase(UODAttributeSet::GetCrowdControlRecoveryAttribute(), Stats.CrowdControlResistance );
			
		}
	}
}

