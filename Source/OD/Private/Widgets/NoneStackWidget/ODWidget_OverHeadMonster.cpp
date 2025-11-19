// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_OverHeadMonster.h"

#include "AbilitySystem/ODAttributeSet.h"
#include "Character/ODMonsterBase.h"
#include "Components/TextBlock.h"
#include "Widgets/Gauge/ODBaseGauge.h"


void UODWidget_OverHeadMonster::ConfigureASC(class UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HPGauge->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UODAttributeSet::GetHealthAttribute(), UODAttributeSet::GetMaxHealthAttribute());
	}
	
}

void UODWidget_OverHeadMonster::SetName(AODMonsterBase* Target)
{
	MonsterName->SetText(Target->GetDisplayName());
}

void UODWidget_OverHeadMonster::VisibilityName(ESlateVisibility Option)
{
	MonsterName->SetVisibility(Option);
}

void UODWidget_OverHeadMonster::VisibilityGauge(ESlateVisibility Option)
{
	HPGauge->SetVisibility(Option);
}
