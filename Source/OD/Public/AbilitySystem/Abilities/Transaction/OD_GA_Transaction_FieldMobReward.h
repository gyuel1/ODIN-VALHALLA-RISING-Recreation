// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Transaction/OD_GA_Transaction_Base.h"
#include "OD_GA_Transaction_FieldMobReward.generated.h"



UCLASS()
class OD_API UOD_GA_Transaction_FieldMobReward : public UOD_GA_Transaction_Base
{
	GENERATED_BODY()

public:
	UOD_GA_Transaction_FieldMobReward();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
};
