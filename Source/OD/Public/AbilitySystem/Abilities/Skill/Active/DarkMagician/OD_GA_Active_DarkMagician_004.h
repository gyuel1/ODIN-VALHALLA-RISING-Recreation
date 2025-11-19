// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Skill/Active/OD_GA_Active_Base.h"
#include "OD_GA_Active_DarkMagician_004.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Active_DarkMagician_004 : public UOD_GA_Active_Base
{
	GENERATED_BODY()
public:
	UOD_GA_Active_DarkMagician_004();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	
private:
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category="Delay Attack",meta=(AllowPrivateAccess=true))
	// FODSkillDelayAttackParams  DelayAttackParams;
	
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
	void OnHitNotifyEvent(const FGameplayEventData InData);

	UFUNCTION()
	void OnNiagaraNotifyEvent(const FGameplayEventData InData);

	
	UFUNCTION()
	void OnRemoveNotifyEvent(const FGameplayEventData InData);
	
};
