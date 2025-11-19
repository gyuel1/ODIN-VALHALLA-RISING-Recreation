// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Component/ODCommonButtonBase.h"
#include "Widgets/Skills/Slots/ODWidget_SkillHudActive.h"
#include "ODCommonWidget_Button_SlotChange.generated.h"

class UHorizontalBox;
class UImage;
/**
 * 
 */
UCLASS()
class OD_API UODCommonWidget_Button_SlotChange : public UODCommonButtonBase
{
	GENERATED_BODY()

public:
	void AutoActiveSkillSlot();
	void AutoDeactivateSkillSlot();

	void AddSlot(const FSlateBrush& SkillBrush);
	void SetSKillHudActive(UODWidget_SkillHudActive* InSkillHudActive) {SkillHudActive = InSkillHudActive;}
	void RemoveSlot();
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SkillEmpty;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SkillIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_AutoActiveSkill;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_AutoDeactivateSkill;
	UPROPERTY()
	TObjectPtr<UODWidget_SkillHudActive> SkillHudActive;
	UPROPERTY()
	FName SKillName;
	
};
