// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ODSkillComponent.h"
#include "ODWidget_Hud_UserInput.generated.h"

class UODCommonButtonBase;
class UButton;
class UODWidget_SkillHudActive;
class UImage;
class UPaperSprite;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_Hud_UserInput : public UUserWidget
{
	GENERATED_BODY()
public:
	TArray<TObjectPtr<UODWidget_SkillHudActive>> Get_SKillHudActives() {return SKillHudActives;}

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickButtonJump();
	UFUNCTION()
	void OnClickButtonAttack();
	UFUNCTION()
	void OnClickButtonAuto();

	UFUNCTION()
	void OnAutoHuntChanged(bool bIsAutoHunt);

	void SetAttackTexture();

	
	
private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UODWidget_SkillHudActive>> SKillHudActives;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_SkillHudActive> SKillHudActive1;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_SkillHudActive> SKillHudActive2;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_SkillHudActive> SKillHudActive3;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_SkillHudActive> SKillHudActive4;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_SkillHudActive> SKillHudActive5;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_SkillHudActive> SKillHudActive6;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Jump;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Attack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonButtonBase> Button_Auto;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* Anim_OnClick_Jump;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* Anim_OnClick_Attack;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_AutoSelect;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_Attack;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TSoftObjectPtr<UPaperSprite>> Image_PaperSpriteJob;

	UPROPERTY()
	TWeakObjectPtr<AODPlayer> Player;


	int32 CurrentSelectedIndex = INDEX_NONE;

	
};
