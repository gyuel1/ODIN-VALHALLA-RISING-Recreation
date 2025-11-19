// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "OD_GA_Boss_DashAttack.generated.h"

class AODMonsterBase;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Boss_DashAttack : public UODGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDistance = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashSpeed = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float EndLeadTime = 0.25f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	bool bLockFacingDuringDash = true;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	TObjectPtr<UAnimMontage> DashAttackEndMontage;

	bool bIsAbilityEnding = false;

	UPROPERTY()
	TWeakObjectPtr<AODMonsterBase> CachedMonster;

public:
	UFUNCTION()
	void OnMoveFinished();

	UFUNCTION()
	void OnMontageCompleted();
};
