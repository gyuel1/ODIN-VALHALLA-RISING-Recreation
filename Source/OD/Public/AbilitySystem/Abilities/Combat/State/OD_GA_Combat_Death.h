// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Combat/State/OD_GA_Combat_State.h"
#include "OD_GA_Combat_Death.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Combat_Death : public UOD_GA_Combat_State
{
	GENERATED_BODY()

public:
	UOD_GA_Combat_Death();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	//Montage Dele
	UFUNCTION()
	void OnMontageComplete();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendOut();
	
};
