// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_LevelUp.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class OD_API UODWidget_LevelUp : public UUserWidget
{
	GENERATED_BODY()

public:
	void LevelUp(const int32 NewLevel);

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> TextBlock_Level;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_LevelUp;
};
