// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_OverHeadMonster.generated.h"

class UTextBlock;
class AODMonsterBase;
class UODBaseGauge;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_OverHeadMonster : public UUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureASC(class UAbilitySystemComponent* AbilitySystemComponent);
	void SetName(AODMonsterBase* Target);
	void VisibilityName(ESlateVisibility Option);
	void VisibilityGauge(ESlateVisibility Option);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODBaseGauge> HPGauge;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MonsterName;
	
	
	
};
