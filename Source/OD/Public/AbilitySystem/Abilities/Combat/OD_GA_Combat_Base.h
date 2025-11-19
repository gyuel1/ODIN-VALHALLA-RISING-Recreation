// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "OD_GA_Combat_Base.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Combat_Base : public UODGameplayAbility
{
	GENERATED_BODY()

protected:
	int32 PickIndexDeterministic(const FGameplayAbilityActivationInfo& Info, int32 Count);

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	
	//BluePrint Setting
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> MontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> Effect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MontageRate;
	
	
	
	
	
};
