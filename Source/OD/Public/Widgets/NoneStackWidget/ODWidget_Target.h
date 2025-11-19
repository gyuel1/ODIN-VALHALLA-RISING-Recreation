// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_Target.generated.h"

class UHorizontalBox;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_Target : public UUserWidget
{
	GENERATED_BODY()

public:
	void VisibilityTargetMain(ESlateVisibility Option);
	void VisibilityTargetSub(ESlateVisibility Option);

	void TargetMainActiveAnimation();
	
	

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_TargetMain;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_TargetSub;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* TargetMainActiveAnim;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* TargetSubActiveAnim;

	
	
};
