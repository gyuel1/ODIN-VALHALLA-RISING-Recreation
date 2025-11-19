// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "ODBaseGauge.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class OD_API UODBaseGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGaugeValue(float NewValue, float NewMaxValue);
	void SetAndBoundToGameplayAttribute(class UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute);

	// Bind Delegates
	void OnValueChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxValueChanged(const FOnAttributeChangeData& ChangeData);

protected:
	virtual void NativePreConstruct() override;
	
	
protected:
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UProgressBar> ODProgressBar;
	
	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor FillColor;
	
	UPROPERTY(meta = (BindWidgetOptional));
	TObjectPtr<UTextBlock> ODProgressBarText;


private:
	float CachedValue;
	float CachedMaxValue;
	
};
