// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_SkillsDetailsView.generated.h"

class UODWidget_SkillDetailDescription;
class UODWidget_SkillSlot_SkillBook;
class UODListSkillDataObject_Base;
class UCommonTextBlock;
class UVerticalBox;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_SkillsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UODListSkillDataObject_Base* InDataObject);
	void ClearDetailsViewInfo();

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_SkillName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_SkillDescription;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODWidget_SkillSlot_SkillBook* SkillSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox_NormalSkillDetail;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox_SpecialSkillDetail;

	UPROPERTY(EditAnywhere, Category= "Class", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UODWidget_SkillDetailDescription> SkillDetailDescription;
	
	
};
