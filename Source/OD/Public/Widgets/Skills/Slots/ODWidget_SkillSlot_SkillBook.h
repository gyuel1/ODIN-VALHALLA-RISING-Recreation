// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Helper/ODMacros.h"
#include "ODWidget_SkillSlot_SkillBook.generated.h"

enum class EGradeType : uint8;
/**
 * 
 */
enum class ODSkillActivation : uint8;
class UImage;
class USizeBox;
class UODDataAsset_GradeInside;
UCLASS()
class OD_API UODWidget_SkillSlot_SkillBook : public UUserWidget
{
	GENERATED_BODY()
public:
	UImage* GetImage_SlotBackGround() {return Image_SlotBackGround;}
	UImage* GetImage_SlotGradeColor() {return Image_SlotGradeColor;}
	UImage* GetImage_SkillIcon() {return Image_SkillIcon;}
	USizeBox* GetImage_SizeBox_SkillIcon() {return SizeBox_SkillIcon;}
	USizeBox* GetImage_SizeBox_SkillBackGround() {return SizeBox_SkillBackGround;}

	void SetSizeBox_SkillBook();
	void SetSizeBox_SkillDetailView();
	void SetSkillActivationAndGrade(ODSkillActivation ActivationType, EGradeType SkillGrade);


	
	
private:
	UPROPERTY(EditAnywhere, Category = "BackGround", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> SoftImage_BackGroundActive;
	UPROPERTY(EditAnywhere, Category = "BackGround", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> SoftImage_BackGroundPassive;
	
	UPROPERTY(EditAnywhere, Category="DA", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODDataAsset_GradeInside> DA_GradeInside;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SlotBackGround;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SlotGradeColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SkillIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_SkillIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_SkillBackGround;
	
};
