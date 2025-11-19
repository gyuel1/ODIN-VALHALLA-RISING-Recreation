// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Value.h"
#include "ODListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODListDataObject_String : public UODListDataObject_Value
{
	GENERATED_BODY()

public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);
	void AdvanceToNextOption();
	void BackToPreviousOption();

protected:
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
protected:
	FString CurrentStringValue;
	FText CurrentDisplayText;
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

public:
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const {return AvailableOptionsTextArray;}
	FORCEINLINE FText GetCurrentDisplayText() const {return CurrentDisplayText;}
};


