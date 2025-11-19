// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_OverHeadPlayer.h"

#include "Components/TextBlock.h"
#include "Helper/ODDebugHelper.h"
#include "Player/ODPlayerController.h"
#include "Player/ODPlayerState.h"
#include "Widgets/Gauge/ODBaseGauge.h"


void UODWidget_OverHeadPlayer::NativeConstruct()
{
	Super::NativeConstruct();
}

void UODWidget_OverHeadPlayer::ConfigureASC(class UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HPGauge->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UODAttributeSet::GetHealthAttribute(), UODAttributeSet::GetMaxHealthAttribute());
		MPGauge->SetAndBoundToGameplayAttribute(AbilitySystemComponent, UODAttributeSet::GetManaAttribute(), UODAttributeSet::GetMaxManaAttribute());
	}
}

void UODWidget_OverHeadPlayer::SetWidgetPlayerName(const FString& WidgetName)
{
	PlayerName->SetText(FText::FromString(WidgetName));
	
}

void UODWidget_OverHeadPlayer::SetPlayerNameColor(bool IsPlayer)
{
	if (IsPlayer)
	{
		PlayerName->SetColorAndOpacity(PlayerNameColor);
	}
	else
	{
		PlayerName->SetColorAndOpacity(OtherPlayerNameColor);
	}
}


