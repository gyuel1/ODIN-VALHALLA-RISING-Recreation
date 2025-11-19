// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "OD_GA_Boss_Enrage.generated.h"

class UBehaviorTreeComponent;
class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Boss_Enrage : public UODGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//Montage Dele
	UFUNCTION()
	void OnMontageComplete();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendOut();
private:

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	
	//BluePrint Setting
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> MontageArray;

	UPROPERTY(EditDefaultsOnly, Category="Enrage")
	TSubclassOf<UGameplayEffect> EnrageStatusEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MontageRate;

public:
	int32 PickIndexDeterministic(const FGameplayAbilityActivationInfo& Info, int32 Count);

};
