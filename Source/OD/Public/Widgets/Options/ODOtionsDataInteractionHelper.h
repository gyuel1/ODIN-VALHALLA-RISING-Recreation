// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UODGameUserSettings;
/**
 * 
 */
class OD_API FODOtionsDataInteractionHelper
{

public:
	FODOtionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InStringValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UODGameUserSettings> CachedWeakGameUserSettings;
};
