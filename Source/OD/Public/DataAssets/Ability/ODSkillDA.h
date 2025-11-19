// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "DataAssets/Ability/ODAbilityDA.h"
#include "ODSkillDA.generated.h"

enum class EGradeType : uint8;
enum class EBuffAttribute : uint8;

UENUM(BlueprintType)
enum class ODSkillActivation : uint8
{
	Active,
	Passive
};

USTRUCT(BlueprintType)
struct FSkillDescription
{
	GENERATED_BODY()

	//TArray로 할지 DT로 할지 고민/ 프로토는 TArray
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true, MultiLine = "true"))
	FText Description;

	//UPROPERTY()
	//커스텀 설명 만들어야할듯
};

USTRUCT(BlueprintType)
struct FSkillDetailInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true, MultiLine = "true"))
	EBuffAttribute BuffDescriptionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true, MultiLine = "true"))
	FText SkillDetailDescriptionText;
	
};

USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_BODY()

	//스킬 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	FName SkillName;

	//스킬 타입 (액티브 패시브)
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	ODSkillActivation SkillType;

	//스킬을 배웠는지
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	bool bSkillUnlock;

	//스킬 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	TObjectPtr<UPaperSprite> SkillIcon;

	// 스킬 등급
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	EGradeType SkillGrade;

	//스킬설명 구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	FSkillDescription SkillDescription;

	// 기본 스킬 디테일 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	TArray<FSkillDetailInfo> SkillNormalDetailInfo;

	// 특수 스킬 디테일 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Skill Info",meta = (AllowPrivateAccess = true))
	TArray<FSkillDetailInfo> SkillSpecialDetailInfo;
	
	
};

UCLASS()
class OD_API UODSkillDA : public UODAbilityDA
{
	GENERATED_BODY()

public:
	UODSkillDA();

	//Read Only
	FORCEINLINE const FSkillInfo& GetSkillInfo() const {return SkillInfo;}

	void UnlockSkill();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Info",  meta = (AllowPrivateAccess = true))
	FSkillInfo SkillInfo;
	
};
