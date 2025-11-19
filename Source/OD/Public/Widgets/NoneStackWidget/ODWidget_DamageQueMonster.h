// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageQueContainer.h"
#include "ODWidget_DamageQueMonster.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODWidget_DamageQueMonster : public UODWidget_DamageQueContainer
{
	GENERATED_BODY()
	
public:
	virtual void AddDamage(int32 DamageValue = 0, bool IsCritical = false) override;
	

private:
	UPROPERTY(meta =(BindWidgetAnim), Transient)
	UWidgetAnimation* CriticalStringAnimation;
};
