// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "ODWidget_PrimaryLayout.generated.h"

/**
 * 
 */
class UCommonActivatableWidgetContainerBase;
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InTag) const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "OD.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);
	
	
	
private:
	UPROPERTY(Transient)
	TMap<FGameplayTag,TObjectPtr<UCommonActivatableWidgetContainerBase>> RegisteredWidgetStackMap;
	
};
