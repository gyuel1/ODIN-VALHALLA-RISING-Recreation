// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ODOtionsDataInteractionHelper.h"

#include "Setting/ODGameUserSettings.h"


FODOtionsDataInteractionHelper::FODOtionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath)
	: CachedDynamicFunctionPath(InSetterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UODGameUserSettings::Get();
}

FString FODOtionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath,OutStringValue);

	return OutStringValue;
}

void FODOtionsDataInteractionHelper::SetValueFromString(const FString& InStringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(CachedWeakGameUserSettings.Get(), CachedDynamicFunctionPath, InStringValue);
}
