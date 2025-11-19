// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/ODUIEnumTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ODAsyncAction_ConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);
/**
 * 
 */
UCLASS()
class OD_API UODAsyncAction_ConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show confirmation Screen"))
	static UODAsyncAction_ConfirmScreen* PushConfirmScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType ScreenType,
		FText InScreenTitle,
		FText InScreenMessage
		);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;


private:
	TWeakObjectPtr<UWorld> CachedOwingWorld;
	EConfirmScreenType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
	
	
};
