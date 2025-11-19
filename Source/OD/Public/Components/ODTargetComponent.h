// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ODTargetComponent.generated.h"


class AODPlayer;
class USphereComponent;
class AODCombatCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OD_API UODTargetComponent : public UActorComponent
{
	GENERATED_BODY()

	

public:
	UODTargetComponent();

	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//=====Targeting=====//
public:
	UFUNCTION(BlueprintCallable)
	AODCombatCharacter* GetTarget();

	//AutoTargeting
	void StartAutoTargeting();
	void StopAutoTargeting();

	//SelectTargeting
	//다음 타겟 선택 (키보드 Tab기능)
	UFUNCTION(BlueprintCallable)
	void KeyboardTargeting();
	
	//인게임 주변 감지 기능 (키보드 R기능)
	UFUNCTION(BlueprintCallable)
	void DetectTargeting();

	//주변감지 목록 지우기
	UFUNCTION(BlueprintCallable)
	void DetectTargetArrayClear();
	
	//Tab으로 하는 타겟팅 로직 (직접사용 x)
	UFUNCTION(BlueprintCallable)
	void SoftTargeting();
	
	//마우스 클릭으로 하는 타겟팅
	UFUNCTION(BlueprintCallable)
	void SelectTargeting(AODCombatCharacter* InTarget);

	AODCombatCharacter* GetCurrentAutoTarget() const { return CurrentAutoTarget.Get(); }
	void GetAutoTarget()  { return SetAutoTarget(); }

private:
	UPROPERTY()
	TWeakObjectPtr<AODPlayer> ComponentOwner;


private:

	void TimerFunction();
	
	//AutoTargeting
	UFUNCTION(BlueprintCallable)
	void SetAutoTarget();
	UFUNCTION()
	void OnAutoTargetCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                       bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAutoTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//범위감지
	TArray<TWeakObjectPtr<AODCombatCharacter>> DetectInRange(const float& InRange);


	//SelectTargeting
	//거리가 멀어지면 현재 타겟 선택 해제
	void ClearOutOfRangeTarget();
	
	UPROPERTY(Replicated)
	TObjectPtr<AODCombatCharacter> CurrentSelectTarget;

	UFUNCTION(Server, Reliable)           
	void ServerSetSelectTarget(AODCombatCharacter* NewTarget);
	
	UPROPERTY()
	TArray<TWeakObjectPtr<AODCombatCharacter>> SoftTargetArray;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<AODCombatCharacter>> DetectTargetArray;
	
	UPROPERTY()
	uint32 DetectTargetArrayIndex;
	
	//USphereComponent
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<USphereComponent> AutoTargetCollision;

	//AutoTargeting
	UPROPERTY()
	FTimerHandle AutoTargetingTimerHandle;
	UPROPERTY()
	TWeakObjectPtr<AODCombatCharacter> CurrentAutoTarget;
	UPROPERTY()
	TSet<TWeakObjectPtr<AODCombatCharacter>> AutoTargetArray;
};
