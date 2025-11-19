// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Ability/ODSkillDA.h"
#include "ODActiveSkillDA.generated.h"


UENUM(BlueprintType)
enum class EODSkillTargeting : uint8
{
	Self,
	Target,
	Field
};

UENUM(BlueprintType)
enum class EODSkillRole : uint8
{
	Attack,
	Buff,
};

UENUM(BlueprintType)
enum class EODTargetScope : uint8
{
	Single,
	AreaOfEffect
};

UENUM(BlueprintType)
enum class EODSkillRangeType : uint8
{
	Melee,
	Ranged
};

USTRUCT(BlueprintType)
struct FActiveSkillData
{
	GENERATED_BODY()

	FActiveSkillData()
	{
		RequiredLevel = 1.f;
		DamagePercent = 100.f;
		DamageCount = 1.f;
		SkillCost = 0.f;
		SkillCooldown = 1.f;
		SkillRange = 250.f;
		MaxTargetCount = 1;
	}
	
	
	//스킬을 배우기 위한 요구 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "ActiveInfo",
		meta = (AllowPrivateAccess = true, ClampMin="1", UIMin="1"))
	float RequiredLevel;
	
	//스킬의 공격퍼센트 %단위로 기입 EX) 기본공격력의 3배 = 300.f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Active Info", meta = (AllowPrivateAccess = true))
	float DamagePercent;
	
	//스킬의 공격 횟수 (실제로 몇타가 들어가는가? 멀티타겟일때 3명에게 3번씩을 공격해도 카운트는 3)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Active Info",
		meta = (AllowPrivateAccess = true, ClampMin="1", UIMin="1"))
	float DamageCount;
	
	//스킬 사용 코스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Active Info",
		meta = (AllowPrivateAccess = true, ClampMin="0", UIMin="0"))
	float SkillCost;
	
	//스킬 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Active Info",
		meta = (AllowPrivateAccess = true, ClampMin="0", UIMin="0"))
	float SkillCooldown;
	
	//스킬 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Active Info",
			meta = (AllowPrivateAccess = true))
	float SkillRange;
	
	//최대 적용가능한 타겟의 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Active Info",
		meta = (AllowPrivateAccess = true, ClampMin="1", UIMin="1"))
	int32 MaxTargetCount;
	
};

UCLASS()
class OD_API UODActiveSkillDA : public UODSkillDA
{
	GENERATED_BODY()

public:
	UODActiveSkillDA();

	FORCEINLINE const FActiveSkillData& GetActiveSkillData() const {return ActiveSkillData;};
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Active Skill Data",  meta = (AllowPrivateAccess = true))
	FActiveSkillData ActiveSkillData;
	
};
