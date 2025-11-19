// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"   
#include "StatsDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FStatsDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	//레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;
	//최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	//체력 자연 회복력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegeneration;
	//마나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMana;
	//마나 자연 회복력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaRegeneration;

	//이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;

	//기본 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;
	//공격속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;
	//명중률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;
	//치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalChance;
	//치명타 피해량 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalDamage;
	//데미지 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageIncrease;
	//상태이상 적중률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrowdControlAccuracy;

	//기본 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;
	//회피율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Evasion;
	//받는피해 감소
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageReduction;
	//상태이상 저항
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrowdControlResistance;
	//상태이상 회복
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrowdControlRecovery;
};

USTRUCT(BlueprintType)
struct FLevelUpStats
{
	GENERATED_BODY()

	//체력 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthIncrease;

	//체력 회복력 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegeneration;

	//마나 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaIncrease;
	
	//마력 회복력 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaRegeneration;

	//기본 공격력 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPowerIncrease;

	//기본 방어력 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefenseIncrease;
	
};
