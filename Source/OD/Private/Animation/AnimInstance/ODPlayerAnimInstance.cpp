// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstance/ODPlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/ODPlayer.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"


void UODPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ODPlayer = Cast<AODPlayer>(ODCombatCharacter);

	APawn* OwningPawn = TryGetPawnOwner();
	if (!OwningPawn)
	{
		return;
		
	}
	
	/*
	ODAsc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningPawn);
	if (ODAsc)
	{
		ODAsc->GetGameplayAttributeValueChangeDelegate
		(UODAttributeSet::GetHealthAttribute()).AddUObject(this, &UODPlayerAnimInstance::OnHealthChanged);//추후 대미지 판정 나오면 추가예정
	
	}*/
	
	
	OnMontageStarted.RemoveDynamic(this, &UODPlayerAnimInstance::HandleMontageStarted);
	OnMontageEnded.RemoveDynamic(this, &UODPlayerAnimInstance::HandleMontageEnded);

	OnMontageStarted.AddDynamic(this, &UODPlayerAnimInstance::HandleMontageStarted);
	OnMontageEnded.AddDynamic(this, &UODPlayerAnimInstance::HandleMontageEnded);

	IsAttacking = false;
	LastCombatTime = -1000.f;
}

void UODPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (ODPlayer)
	{
		if ( ODMovementComponent)
		{
			Velocity = ODPlayer->GetVelocity();
			IsFalling = ODMovementComponent->IsFalling();
		}
		InCombat = ODPlayer->InCombat;
		LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(ODPlayer->GetVelocity(), ODPlayer->GetActorRotation());
	}

	const float CurrentGameTime = GetGameTimeSecondsSafe();
	const bool WithinGerace = (CurrentGameTime - LastCombatTime) < CombatGraceSeconds;

	EndCombat = IsAttacking || WithinGerace;
}

void UODPlayerAnimInstance::HandleMontageStarted(UAnimMontage* Montage)
{
	if (!Montage)
	{
		return;
	}
	if (ShouldAffectCombat(Montage))
	{
		IsAttacking = true;
	}
	
}

void UODPlayerAnimInstance::HandleMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (!Montage)
	{
		return;
	}
	if (ShouldAffectCombat(Montage))
	{
		IsAttacking = false;
		LastCombatTime = GetGameTimeSecondsSafe();
	}
	
}

bool UODPlayerAnimInstance::ShouldAffectCombat(UAnimMontage* Montage) const
{
	if (AttackSlotName.IsNone())
	{
		return true;
	}
	for (const FSlotAnimationTrack& Track : Montage->SlotAnimTracks)
	{
		if (Track.SlotName == AttackSlotName)
		{
			return true;
		}
	}
	return false;
	
}

float UODPlayerAnimInstance::GetGameTimeSecondsSafe() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() : 0.f;	
}

/*
void UODPlayerAnimInstance::OnHealthChanged(const FOnAttributeChangeData& Data)
{	
	if (Data.NewValue < Data.OldValue)
	{
		 return;
	}
	const float DamageAmount = Data.NewValue - Data.OldValue;
	if (DamageAmount > 1.f)
	{
		PlayHitReact = true;
		LastCombatTime = GetGameTimeSecondsSafe();
		UE_LOG(LogTemp, Error, TEXT("!!! PLAYER HIT REACT TRUE !!!"));//추후 대미지 판정 나오면 추가예정
	}
}
*/
