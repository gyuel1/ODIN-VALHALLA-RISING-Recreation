// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODMonsterBase.h"
#include "ODMonsterGrunt.generated.h"

USTRUCT(BlueprintType)
struct FRandomPatrolPointValue : public FTargetSystemValue
{
	GENERATED_BODY()
	
	//몬스터 패트롤 최대 거리
	UPROPERTY(EditDefaultsOnly, Category="Value")
	float PatrolRadius = 100.f;
};


/**
 * 
 */
UCLASS()
class OD_API AODMonsterGrunt : public AODMonsterBase
{
	GENERATED_BODY()
public:
	AODMonsterGrunt();		

	UFUNCTION()
	void NextPatrolStatus();
	
	UFUNCTION()
	FVector GetRandomPointInRadius(const FVector& SpawnLocation, float Radius);

	virtual void BeginPlay() override;

protected:
	//몬스터 patrol중 idle과 walk 전환 확률
	UPROPERTY(EditDefaultsOnly, Category="Value")
	float IdleProbability = 0.5f;

	//몬스터 상태 (Patrol에 사용됨으로 Boss가 사용할 일 없다고 판단) 추후 Ability Tag로 판단예정
	UPROPERTY(VIsibleAnywhere, Category="Monster Status")
	EMonsterStatus CurrentStatus = EMonsterStatus::Idle;
	
	UPROPERTY(EditDefaultsOnly, Category="Value")
	FRandomPatrolPointValue PatrolPointValue;
	
public:
	UFUNCTION()
	EMonsterStatus GetCurrentStatus();

	UFUNCTION()
	float GetIdleProbability();

	UFUNCTION()
	FRandomPatrolPointValue& GetPatrolPointValue();
};
