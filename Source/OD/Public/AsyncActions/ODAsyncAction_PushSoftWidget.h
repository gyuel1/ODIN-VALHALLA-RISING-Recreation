// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ODAsyncAction_PushSoftWidget.generated.h"

class UODWidget_Activatable;
class AODPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UODWidget_Activatable*, PushedWidget);
/**
 * 
 */
UCLASS()
class OD_API UODAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"))
	static UODAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController,
		TSoftClassPtr<UODWidget_Activatable> InSoftWidgetClass, UPARAM(meta = (Categories = "OD.WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget = true);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;
	
private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<UODWidget_Activatable> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget;
};
