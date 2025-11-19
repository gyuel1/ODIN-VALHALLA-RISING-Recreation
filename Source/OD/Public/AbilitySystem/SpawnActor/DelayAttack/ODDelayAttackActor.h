// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/SpawnActor/ODAbilitySpawnActorBase.h"
#include "ODDelayAttackActor.generated.h"

/**
 * 
 */
UCLASS()
class OD_API AODDelayAttackActor : public AODAbilitySpawnActorBase
{
	GENERATED_BODY()
public:
	
	AODDelayAttackActor();

	virtual void InitAbilityInfo(UAbilitySystemComponent* InOwnerASC, AODCombatCharacter* InTarget, const FODSkillParameters InSkillParameters, AGameplayCueNotify_Actor* InCueActor = nullptr) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void OnTickTrigger();

private:
	
	UPROPERTY()
	FODSkillDelayAttackParams DelayAttackParams;

	UPROPERTY()
	bool TickTrigger;

	UPROPERTY()
	bool DestroyTrigger;
	
	UPROPERTY()
	float CheckDelaySeconds;


	
	
};
