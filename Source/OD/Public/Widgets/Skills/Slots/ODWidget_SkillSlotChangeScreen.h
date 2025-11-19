// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_SkillSlotChangeScreen.generated.h"


class UODWidget_Hud_UserInput;
class UUniformGridPanel;
class UODCommonWidget_Button_SlotChange;

UCLASS()
class OD_API UODWidget_SkillSlotChangeScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	int32 GetCurrentSelectedIndex() const {return CurrentSelectedIndex;}
	TArray<TObjectPtr<UODCommonWidget_Button_SlotChange>>& GetSkillChangeSlotButtons() {return SkillChangeSlotButtons;}

	UFUNCTION()
	void OnSkillSlotChangeClicked(int ClickedIndex);

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UODCommonWidget_Button_SlotChange>> SkillChangeSlotButtons;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UUniformGridPanel> UniformGridPanel_ChangeSkillSlotGridPanel;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonWidget_Button_SlotChange> SKillChangeSlotIndex1;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonWidget_Button_SlotChange> SKillChangeSlotIndex2;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonWidget_Button_SlotChange> SKillChangeSlotIndex3;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonWidget_Button_SlotChange> SKillChangeSlotIndex4;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonWidget_Button_SlotChange> SKillChangeSlotIndex5;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UODCommonWidget_Button_SlotChange> SKillChangeSlotIndex6;

	UPROPERTY()
	TObjectPtr<UODWidget_Hud_UserInput> HudUserInput;

	int32 CurrentSelectedIndex = INDEX_NONE;
	
	
};
