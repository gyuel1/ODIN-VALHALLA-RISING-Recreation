// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "OD_GA_Init_Base.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Init_Base : public UODGameplayAbility
{
	GENERATED_BODY()
public:
	
	UOD_GA_Init_Base();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Effect")
	TSubclassOf<UGameplayEffect> Effect;
	
	
	
	
	
};
