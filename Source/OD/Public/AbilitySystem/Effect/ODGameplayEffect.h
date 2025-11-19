// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ODGameplayEffect.generated.h"

USTRUCT()
struct FODSetBaseStatsEffectData
{
	GENERATED_BODY()
	FODSetBaseStatsEffectData()
		: Info(), Name(NAME_None)
	{
	}

	FODSetBaseStatsEffectData(const FGameplayModifierInfo& InInfo, const FName& InName)
		: Info(InInfo), Name(InName)
	{
	}
	
	FGameplayModifierInfo Info;
	FName Name;
};

UCLASS()
class OD_API UODGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
};
