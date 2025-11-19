// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ODTargetComponent.h"
#include "Character/ODCharacter.h"
#include "Components/ActorComponent.h"
#include "ODTargetingAndAttackComponent.generated.h"

class AActor;
class AODCombatCharacter;
class UODTargetComponent;
class UODCameraControllerComponent;
class UODAbilitySystemComponent;
class UODCombatComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OD_API UODTargetingAndAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Sets default values for this component's properties
	UODTargetingAndAttackComponent();
	
	FVector CachedDestination;
	UPROPERTY(Replicated)
	TObjectPtr<AActor> CurrentTarget;
	UPROPERTY(Replicated)
	TObjectPtr<AActor> CurrentAutoTarget;

	UFUNCTION(server, reliable)
	void SetServerCurrentTarget(AActor* InActor);
	//공격 종료후 방어
	UFUNCTION(Server, Reliable)
	void Server_CheckTarget(AODCombatCharacter* TargetToCheck);
	UFUNCTION(Client, Reliable)
	void Client_ForceTargetResetAndSearch();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopMovementOnTargetDead();

	UFUNCTION(Server, Reliable)
	void Server_SetNewTarget(AODCombatCharacter* Target);
	
		
	void AttemptInitializeODAsc();
	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsMovingClick = false;
	UPROPERTY(VisibleAnywhere, Replicated)
	bool bTargetIsDeadOrBlocked = false;
	UPROPERTY(VisibleAnywhere, Replicated)
	bool bTargetDeadOrBlocked = false;
		
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	TObjectPtr<UODAbilitySystemComponent> ODAsc;
	UPROPERTY()
	TObjectPtr<UODTargetComponent> TargetComponent;
	UPROPERTY()
	TObjectPtr<UODCameraControllerComponent> CameraControllerComponent;
	
	//어빌리티 찾기 타이머
	FTimerHandle ASCInitTimerHandle;

	

public:
	//F타겟 설정 및 기본공격
	UFUNCTION()
	void CurrentAbilityCancel();
	bool IsCurrentAbilityCancel();
	
	UFUNCTION()
	void TargetAttack();
	UFUNCTION()
	void TargetMove();
	UFUNCTION()
	void TileClick();
	//마우스 타겟
	UFUNCTION()
	void MouseButtonReleased();
	UFUNCTION()
	void MouseButtonPressed();
	//공격 어빌리티
	UFUNCTION(Server,Reliable)
	void AttackAndSkill();
	UFUNCTION()
	void AutoHuntLoop();
	UFUNCTION()
	void StartAutoTargetingOn();
	UFUNCTION()
	void StopAutoTargetingOff();
	//타겟 체크중
	UPROPERTY()
	FTimerHandle TargetCheckTimer;
	//타겟 추적
	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsTargetTracking = false;
	
	bool MouseButtonHeld = false;
	bool bHitSuccessful = false;
	FTimerHandle MouseHoldTime;
	FTimerHandle AutoHuntLoopTimer;
	FHitResult HitResult;
	

private:
	//연속 클릭방지 타임 갱신
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float LastClickTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MinClickInterval = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float ShortPressThreshold = 0.2f;
	//카메라 회전
	UPROPERTY(EditDefaultsOnly)
	bool IsLooking = false;
	//float AttackRange = 0.f;
};
