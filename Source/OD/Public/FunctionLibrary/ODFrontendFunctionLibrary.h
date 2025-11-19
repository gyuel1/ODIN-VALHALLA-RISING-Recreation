// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ODFrontendFunctionLibrary.generated.h"

enum class EBGMType : uint8;
class UODWidget_Activatable;
/**
 * 
 */
UCLASS()
class OD_API UODFrontendFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftClassPtr<UODWidget_Activatable> GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "OD.Widget")) FGameplayTag InWidgetTag);
	UFUNCTION(BlueprintPure, Category="ODFrontendFunctionLibrary")
	static UODGameInstance* GetODGameInstance(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category="ODFrontendFunctionLibrary")
	static USoundBase* GetBGM(const UObject* WorldContextObject, EBGMType BGMType);
	
	
};
