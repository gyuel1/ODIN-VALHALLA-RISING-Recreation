// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Base.h"
#include "ODListDataObject_Value.generated.h"

/**
 * 
 */


class FODOtionsDataInteractionHelper;

UCLASS(Abstract)
class OD_API UODListDataObject_Value : public UODListDataObject_Base
{
	GENERATED_BODY()

public:
	void SetDataDynamicGetter(const TSharedPtr<FODOtionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FODOtionsDataInteractionHelper>& InDynamicSetter);
	void SetDefaultValueFromString(const FString& InDefaultValue) {DefaultStringValue = InDefaultValue;}
	virtual bool HasDefaultValue() const override {return DefaultStringValue.IsSet();}
	
protected:
	FString GetDefaultValueAsString() const {return DefaultStringValue.GetValue();}
	
	TSharedPtr<FODOtionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FODOtionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue;
};
