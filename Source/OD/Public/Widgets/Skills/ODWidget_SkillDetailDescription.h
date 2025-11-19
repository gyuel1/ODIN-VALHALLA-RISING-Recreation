// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_SkillDetailDescription.generated.h"

enum class EBuffAttribute : uint8;
class UImage;
class UCommonRichTextBlock;
class UODDataAsset_BuffIcon;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_SkillDetailDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetBuffTexture(EBuffAttribute BuffAttribute);
	void SetSkillDescription(const FText& SkillDescription);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_BuffIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonRichTextBlock> CommonRichTextBlock_SkillDescription;
	UPROPERTY(EditAnywhere, Category="DA", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODDataAsset_BuffIcon> DA_BuffIcon;
	
	
};
