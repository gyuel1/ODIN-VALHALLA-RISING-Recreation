// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_HUDMonsterInfo.generated.h"

class UOverlay;
class UTextBlock;
class UODBaseGauge;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_HUDMonsterInfo : public UODWidget_Activatable
{
	GENERATED_BODY()
	
public:
	void ConfigureASC(class UAbilitySystemComponent* AbilitySystemComponent);
	void SetName(class AODMonsterBase* Target);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODBaseGauge> HPGauge;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> HUDMonsterInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MonsterName;
	
	
};
