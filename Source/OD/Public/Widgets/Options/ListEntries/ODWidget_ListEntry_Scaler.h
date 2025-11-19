// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Scalar.h"
#include "Widgets/Options/ListEntries/ODWidget_ListEntry_Base.h"
#include "ODWidget_ListEntry_Scaler.generated.h"

/**
 * 
 */

class UCheckBox;
class UProgressBar;
class UCommonNumericTextBlock;
class UODCommonButtonBase;
class UAnalogSlider;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_ListEntry_Scaler : public UODWidget_ListEntry_Base
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UODListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UODListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	
private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_SettingSlider;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UProgressBar* ProgressBar_SettingFill;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget,AllowPrivateAccess = "true"))
	UCheckBox* CheckBox_SettingMute;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonButtonBase* CommonButton_PreviousOption; // 감소버튼
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonButtonBase* CommonButton_NextOption; // 증가버튼

	UPROPERTY(Transient)
	UODListDataObject_Scalar* CachedOwningScalerDataObject;

};
