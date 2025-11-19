// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ODAsyncAction_MessageScreen.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageScreenButtonClickedDelegate,
	EConfirmScreenButtonType, ClickedButtonType,
	FText, EditText);
/**
 * 
 */
UCLASS()
class OD_API UODAsyncAction_MessageScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Message Screen"))
	static UODAsyncAction_MessageScreen* PushConfirmScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType ScreenType,
		FText InScreenTitle,
		FText InScreenMessage,
		FText CachedScreenEditHintText,
	    FText CachedScreenEditText
		);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnMessageScreenButtonClickedDelegate OnButtonClicked;


private:
	TWeakObjectPtr<UWorld> CachedOwingWorld;
	EConfirmScreenType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
	FText CachedScreenEditHintText;
	FText CachedScreenEditText;
	
	
};
