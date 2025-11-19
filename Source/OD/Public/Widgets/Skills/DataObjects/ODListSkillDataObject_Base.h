// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Helper/ODMacros.h"
#include "UObject/NoExportTypes.h"
#include "ODListSkillDataObject_Base.generated.h"

enum class ODSkillActivation : uint8;
enum class EGradeType : uint8;
class UImage;
struct FSkillDetailInfo;
/**
 * 
 */
UCLASS(Abstract)
class OD_API UODListSkillDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
	OD_DATA_ACCESSOR(FName, DataID)
    OD_DATA_ACCESSOR(FText, DataDisplayName)
    OD_DATA_ACCESSOR(FText, SkillDescriptionText)
    OD_DATA_ACCESSOR(FText, DisabledRichText)
    OD_DATA_ACCESSOR(UODListSkillDataObject_Base*, ParentData)
	OD_DATA_ACCESSOR(FSlateBrush, SkillIcon)
	OD_DATA_ACCESSOR(EGradeType, SkillGrade)
	OD_DATA_ACCESSOR(ODSkillActivation, ActivationType)
	OD_DATA_ACCESSOR(TWeakObjectPtr<UODListSkillDataObject_Base>, DA_Skill)
	OD_DATA_ACCESSOR(bool, bIsUnlock);
	OD_DATA_ACCESSOR(FGameplayTag, SkillTag);
	TArray<FSkillDetailInfo>& GetSkillNormalDetailInfo() {return SkillNormalDetailInfo;}
	TArray<FSkillDetailInfo>& GetSkillSpecialDetailInfo() {return SkillSpecialDetailInfo;}
	

	void InitDataObject();
	virtual TArray<UODListSkillDataObject_Base*> GetAllChildListData() const {return TArray<UODListSkillDataObject_Base*>();}
	virtual bool HasChildListData() const {return false;}

protected:
	// 하위클래스는 필요 초기회시 재정의함 
	virtual void OnDataObjectInitialized();
	
private:
	FName DataID;
	FText DataDisplayName;
	FText SkillDescriptionText;
	FText DisabledRichText;
	EGradeType SkillGrade;
	ODSkillActivation ActivationType;
	bool bIsUnlock;
	FGameplayTag SkillTag;

	UPROPERTY(Transient)
	UODListSkillDataObject_Base* ParentData;

	UPROPERTY(Transient)
	FSlateBrush SkillIcon;

	UPROPERTY(Transient)
	TArray<FSkillDetailInfo> SkillNormalDetailInfo;
	
	UPROPERTY(Transient)
	TArray<FSkillDetailInfo> SkillSpecialDetailInfo;

	UPROPERTY(Transient)
	TWeakObjectPtr<UODListSkillDataObject_Base> DA_Skill;
};
