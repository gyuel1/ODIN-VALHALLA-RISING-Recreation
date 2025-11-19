// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODMonsterBase.h"
#include "ODMonsterBoss.generated.h"

class UGameplayAbility;
struct FGameplayTagContainer;
struct FOnAttributeChangeData;
class AOD_AI_AttackAreaDetector;
/**
 * 
 */
UCLASS()
class OD_API AODMonsterBoss : public AODMonsterBase
{
	GENERATED_BODY()
	// 거인 우두머리, 하티, 스콜, 펜리르
	
	// 소환 패턴 > 스포너에 요청

public:
	AODMonsterBoss();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void OnBossEnrage(const FOnAttributeChangeData& Data);
	void HandleEnrageRetry();
private:
	FTimerHandle EnrageRetryHandle;
	bool bEnrageQueued = false;
};
