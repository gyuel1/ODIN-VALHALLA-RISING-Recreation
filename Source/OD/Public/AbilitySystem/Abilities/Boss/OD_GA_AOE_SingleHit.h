// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "OD_GA_AOE_SingleHit.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_AOE_SingleHit : public UODGameplayAbility
{
	GENERATED_BODY()

protected:
	UOD_GA_AOE_SingleHit();

	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	FORCEINLINE const float& GetAbilityRange() const {return AbilityRange;};
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
	float DamagePercent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackCount;

protected:
	UFUNCTION()
	void OnMontageComplete();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendOut();
	
	UFUNCTION()
	void OnHitNotifyEvent(const FGameplayEventData InData);

	UFUNCTION()
	void OnRemoveNotifyEvent(const FGameplayEventData InData);

#pragma region AttackArea
private:
	UPROPERTY(EditDefaultsOnly, Category="AttackArea")
	FVector BoxLocationOffset = FVector(0, 300, 0);
	
	UPROPERTY(EditDefaultsOnly, Category="AttackArea")
	FVector BoxExtent = FVector(128, 250, 37.5);

	UPROPERTY(EditDefaultsOnly, Category="AttackArea")
	FRotator BoxRotator = FRotator(0, -90, 0);
	
	UPROPERTY(EditDefaultsOnly, Category="AttackArea")
	TEnumAsByte<ECollisionChannel> AttackTraceChannel = ECC_GameTraceChannel1;
#pragma endregion
};
