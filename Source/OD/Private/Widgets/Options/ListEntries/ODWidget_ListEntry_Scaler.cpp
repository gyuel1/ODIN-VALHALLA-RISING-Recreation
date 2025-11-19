// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ODWidget_ListEntry_Scaler.h"

#include "Widgets/Options/DataObjects/ODListDataObject_Scalar.h"
#include "AnalogSlider.h"
#include "Components/ProgressBar.h"

void UODWidget_ListEntry_Scaler::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AnalogSlider_SettingSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChanged);
}

void UODWidget_ListEntry_Scaler::OnOwningListDataObjectSet(UODListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningScalerDataObject = CastChecked<UODListDataObject_Scalar>(InOwningListDataObject);

	CommonNumeric_SettingValue->SetNumericType(CachedOwningScalerDataObject->GetDisplayNumericType());
	CommonNumeric_SettingValue->FormattingSpecification = CachedOwningScalerDataObject->GetNumberFormattingOptions();
	CommonNumeric_SettingValue->SetCurrentValue(CachedOwningScalerDataObject->GetCurrentValue());

	AnalogSlider_SettingSlider->SetMinValue(CachedOwningScalerDataObject->GetDisplayValueRange().GetLowerBoundValue());
	AnalogSlider_SettingSlider->SetMaxValue(CachedOwningScalerDataObject->GetDisplayValueRange().GetUpperBoundValue());
	AnalogSlider_SettingSlider->SetStepSize(CachedOwningScalerDataObject->GetSliderStepSize());
	AnalogSlider_SettingSlider->SetValue(CachedOwningScalerDataObject->GetCurrentValue());
}

void UODWidget_ListEntry_Scaler::OnOwningListDataObjectModified(UODListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningScalerDataObject)
	{
		CommonNumeric_SettingValue->SetCurrentValue(CachedOwningScalerDataObject->GetCurrentValue());
		AnalogSlider_SettingSlider->SetValue(CachedOwningScalerDataObject->GetCurrentValue());
	}
}

void UODWidget_ListEntry_Scaler::OnSliderValueChanged(float Value)
{
	if (CachedOwningScalerDataObject)
	{
		CachedOwningScalerDataObject->SetCurrentValueFromSlider(Value);
	}

	ProgressBar_SettingFill->SetPercent(AnalogSlider_SettingSlider->GetValue());
}
