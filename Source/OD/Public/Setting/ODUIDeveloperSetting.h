// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "ODUIDeveloperSetting.generated.h"

class UODWidget_Activatable;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "OD UI Settings"))
class OD_API UODUIDeveloperSetting : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "OD.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UODWidget_Activatable>> ODWidgetMaps;
	
	
};
