// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ODListDataObject_Scalar.h"

#include "Widgets/Options/ODOtionsDataInteractionHelper.h"


FCommonNumberFormattingOptions UODListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;
}

FCommonNumberFormattingOptions UODListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UODListDataObject_Scalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString())
		);
	}

	return 0.0f;
}

void UODListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewValue
			);
		
		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));

		NotifyListDataModified(this);
	}
}

bool UODListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}

	return false;
}

bool UODListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}

	return false;
}

float UODListDataObject_Scalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;
	LexFromString(OutConvertedValue, *InString);

	return OutConvertedValue;
}

