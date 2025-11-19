// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Gauge/ODBaseGauge.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Helper/ODDebugHelper.h"


void UODBaseGauge::SetGaugeValue(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;

	if (ODProgressBar)
	{
		if (NewMaxValue == 0)
		{
			return;
		}

		float NewPercentage = NewValue / NewMaxValue;
		ODProgressBar->SetPercent(NewPercentage);
	}

	if (ODProgressBarText)
	{
		//분수 자릿수 제한
		FNumberFormattingOptions NFO = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
		ODProgressBarText->SetText(
			FText::Format(
				FTextFormat::FromString("{0}/{1}"),
				FText::AsNumber(NewValue, &NFO),
				FText::AsNumber(NewMaxValue, &NFO)
				)
				);
	}
}

void UODBaseGauge::SetAndBoundToGameplayAttribute(class UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute)
{
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
	   .RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
	   .RemoveAll(this);
	
	// AttributeChange 델리게이트만 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
		.AddUObject(this, &ThisClass::OnValueChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
		.AddUObject(this, &ThisClass::OnMaxValueChanged);
	
	
		bool bFoundValue = false;
		bool bFoundMax = false;
		float Value = AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFoundValue);
		float MaxValue = AbilitySystemComponent->GetGameplayAttributeValue(MaxAttribute, bFoundMax);

		if (bFoundValue && bFoundMax)
		{
			SetGaugeValue(Value, MaxValue);
		}
		else
		{
			Debug::Print(TEXT("대기중"));
		}
 	


}



void UODBaseGauge::OnValueChanged(const FOnAttributeChangeData& ChangeData)
{
	SetGaugeValue(ChangeData.NewValue, CachedMaxValue);
}

void UODBaseGauge::OnMaxValueChanged(const FOnAttributeChangeData& ChangeData)
{
	SetGaugeValue(CachedValue, ChangeData.NewValue);
}

void UODBaseGauge::NativePreConstruct()
{
	Super::NativePreConstruct();

	ODProgressBar->SetFillColorAndOpacity(FillColor);
}
