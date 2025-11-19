// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance/ODAnimInstanceBase.h"
#include "ODCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODCharacterAnimInstance : public UODAnimInstanceBase
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

protected:
	UPROPERTY()
	class AODCombatCharacter* ODCombatCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* ODMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ODCombatCharacter")
	float GroundSpeed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ODCombatCharacter")
	bool bHasAcceleration;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ODCombatCharacter")
	bool bIsMoving;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ODCombatCharacter")
	float LocomotionDirection;
	
	
	
	
};
