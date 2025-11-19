// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "ODAnimInstanceBase.generated.h"

/**
 * 
 */
class ACharacter;
class UCharacterMovementComponent;
UCLASS()
class OD_API UODAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
//태그 사용하게 되면 추가작성
/*protected:
	UFUNCTION(BlueprintPure, meta=(NotBlueprintThreadSafe))
	bool OwnerHasTag(FGameplayTag Tag) const;*/

	
	
};
