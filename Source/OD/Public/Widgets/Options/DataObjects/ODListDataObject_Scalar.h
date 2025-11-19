// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Value.h"
#include "CommonNumericTextBlock.h"
#include "ODListDataObject_Scalar.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODListDataObject_Scalar : public UODListDataObject_Value
{
	GENERATED_BODY()

public:
	OD_DATA_ACCESSOR(TRange<float>, DisplayValueRange)
	OD_DATA_ACCESSOR(TRange<float>, OutputValueRange)
	OD_DATA_ACCESSOR(float,SliderStepSize)
	OD_DATA_ACCESSOR(ECommonNumericType,DisplayNumericType)
	OD_DATA_ACCESSOR(FCommonNumberFormattingOptions,NumberFormattingOptions)

	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);

	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(float InNewValue);
	
private:
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	float StringToFloat(const FString& InString) const;
	
	TRange<float> DisplayValueRange = TRange<float>(0.f,1.f);
	TRange<float> OutputValueRange = TRange<float>(0.f,1.f);
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
};

