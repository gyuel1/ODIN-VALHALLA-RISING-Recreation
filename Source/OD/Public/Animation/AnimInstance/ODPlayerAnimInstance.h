// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance/ODCharacterAnimInstance.h"
#include "GameplayEffectTypes.h"
#include "ODPlayerAnimInstance.generated.h"

class AODPlayer;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class OD_API UODPlayerAnimInstance : public UODCharacterAnimInstance
{
	GENERATED_BODY()

	
protected:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ODPlayer")
	AODPlayer* ODPlayer;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ODPlayer")
	bool IsFalling;	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ODPlayer")
	FVector Velocity;
	//공격 슬롯 이름 으로 모션 변경
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ODPlayer")
	FName AttackSlotName = FName(TEXT("Attack"));
	//전투 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ODPlayer")
	float CombatGraceSeconds = 4.0f;
	UPROPERTY(BlueprintReadOnly, Category = "ODPlayer")
	bool IsAttacking = false;
	//전투 상태인지
	UPROPERTY(BlueprintReadOnly, Category = "ODPlayer")
	bool InCombat = false;
	UPROPERTY(BlueprintReadOnly, Category = "ODPlayer")
	bool EndCombat = false;
	
	//마지막 전투 시간
	UPROPERTY(BlueprintReadOnly, Category = "ODPlayer")
	float LastCombatTime = 1000.f;
	

	//void OnHealthChanged(const FOnAttributeChangeData& Data); //추후 대미지 판정 나오면 추가예정
	
	UPROPERTY(BlueprintReadOnly, Category = "ODPlayer")
	bool PlayHitReact = false;

private:
	UFUNCTION()
	void HandleMontageStarted(UAnimMontage* Montage);
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool Interrupted);

	bool ShouldAffectCombat( UAnimMontage* Montage) const;

	float GetGameTimeSecondsSafe() const;

	/*
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ODAsc;//추후 대미지 판정 나오면 추가예정
	*/

	

	
	
	
	
};
