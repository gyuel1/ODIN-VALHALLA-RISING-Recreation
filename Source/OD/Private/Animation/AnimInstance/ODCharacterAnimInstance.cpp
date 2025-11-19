// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/ODCharacterAnimInstance.h"


#include "KismetAnimationLibrary.h"
#include "Character/ODCombatCharacter.h"
#include "Helper/ODDebugHelper.h"
#include "GameFramework/CharacterMovementComponent.h"


void UODCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* OwningPawn = TryGetPawnOwner();
	if (OwningPawn)
	{
		ODCombatCharacter = Cast<AODCombatCharacter>(OwningPawn);
		if (ODCombatCharacter)
		{
			ODMovementComponent = ODCombatCharacter->GetCharacterMovement();
		}
	}
	// ...
}

void UODCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!ODCombatCharacter || !ODMovementComponent)
	{
		return;
	}	
	GroundSpeed = ODCombatCharacter->GetVelocity().Size2D();

	bHasAcceleration = ODMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	bIsMoving = (GroundSpeed > 0.1) || bHasAcceleration;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(ODCombatCharacter->GetVelocity(), ODCombatCharacter->GetActorRotation());

}
