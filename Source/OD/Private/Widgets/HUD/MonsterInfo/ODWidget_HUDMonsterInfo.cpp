// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/MonsterInfo/ODWidget_HUDMonsterInfo.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Widgets/Gauge/ODBaseGauge.h"
#include "Character/ODMonsterBase.h"
#include "Components/TextBlock.h"


void UODWidget_HUDMonsterInfo::ConfigureASC(class UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HPGauge->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UODAttributeSet::GetHealthAttribute(), UODAttributeSet::GetMaxHealthAttribute());
	}
}

void UODWidget_HUDMonsterInfo::SetName(class AODMonsterBase* Target)
{
	MonsterName->SetText(Target->GetDisplayName());
}
