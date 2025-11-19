// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ODGameplayAbility.generated.h"

class UODAbilityDA;
/**
 * 
 */
UCLASS()
class OD_API UODGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UODGameplayAbility();

	virtual void SetAbilityDataAsset(UODAbilityDA* InAbilityDA);

	

protected:
	//Random
	int32 PickIndexDeterministic(const FGameplayAbilityActivationInfo& Info, int32 Count);
		
	
	
	
	
};
