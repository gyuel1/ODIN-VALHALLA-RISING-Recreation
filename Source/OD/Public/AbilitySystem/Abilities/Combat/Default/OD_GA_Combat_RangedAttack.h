// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "OD_GA_Combat_RangedAttack.generated.h"

class AODAbilitySpawnActorBase;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Combat_RangedAttack : public UODGameplayAbility
{
	GENERATED_BODY()
public:
	UOD_GA_Combat_RangedAttack();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	

private:
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	
	
	//BluePrint Setting
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> MontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> Effect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MontageRate;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AbilityRange;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float SKillDamagePercent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float SkillAttackCount;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed;

	
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AODAbilitySpawnActorBase> ProjectileActor;
	
	//Montage Dele
	UFUNCTION()
	void OnMontageComplete();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendOut();

	//SpawnActorSet
	UFUNCTION()
	void OnSpawnSuccess(AActor* SpawnedActor);
	UFUNCTION()
	void OnSpawnFail(AActor* SpawnedActor);

	//Notify Dele
	
	UFUNCTION()
	void OnSpawnNotifyEvent(const FGameplayEventData InData);
	UFUNCTION()
	void OnRemoveNotifyEvent(const FGameplayEventData InData);
};
