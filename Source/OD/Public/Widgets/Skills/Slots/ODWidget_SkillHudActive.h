// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ODSkillComponent.h"
#include "ODWidget_SkillHudActive.generated.h"

class AODPlayer;
class UProgressBar;
class UTextBlock;
class UHorizontalBox;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_SkillHudActive : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetSKillNumberText(FText SKillNumber);
	void AddSlot(const FSlateBrush& SkillBrush);
	void RemoveSlot();
	UFUNCTION()
	void CoolTime(FSkillCooldownDuration InDuration);
	UFUNCTION()
	void PlayAnimation_TryActivate();
	
	
	

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_ActiveSkill;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SkillEmpty;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_SkillActive;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_NonAuto;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_Auto;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_SkillNumber;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> ProgressBar_CoolTime;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_CoolTime;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_TryActivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SkillIndex;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AODPlayer> Player;
	
};
