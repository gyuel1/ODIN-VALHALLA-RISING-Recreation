// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/Slots/ODWidget_SkillHudActive.h"

#include "Character/ODPlayer.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ODSkillComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"


void UODWidget_SkillHudActive::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UODWidget_SkillHudActive::SetSKillNumberText(FText SKillNumber)
{
	TextBlock_SkillNumber->SetText(SKillNumber);
}

void UODWidget_SkillHudActive::AddSlot(const FSlateBrush& SkillBrush)
{
	Player = Cast<AODPlayer>(UGameplayStatics::GetPlayerCharacter(this,0));
	
	Image_SkillEmpty->SetVisibility(ESlateVisibility::Hidden);
	Image_SkillActive->SetBrush(SkillBrush);
	Image_SkillActive->SetVisibility(ESlateVisibility::Visible);
	HorizontalBox_NonAuto->SetVisibility(ESlateVisibility::Hidden);
	HorizontalBox_Auto->SetVisibility(ESlateVisibility::Visible);
	
	Player->GetODSkillComponent()->GetSkillSlots()[SkillIndex].WidgetCooldownDele.AddDynamic(this,&ThisClass::CoolTime);
	Player->GetODSkillComponent()->GetSkillSlots()[SkillIndex].WidgetActivateDele.AddDynamic(this,&ThisClass::PlayAnimation_TryActivate);
}

void UODWidget_SkillHudActive::RemoveSlot()
{
	Player = Cast<AODPlayer>(UGameplayStatics::GetPlayerCharacter(this,0));
	
	Image_SkillActive->SetVisibility(ESlateVisibility::Hidden);
	Image_SkillEmpty->SetVisibility(ESlateVisibility::Visible);
	HorizontalBox_Auto->SetVisibility(ESlateVisibility::Hidden);
	HorizontalBox_NonAuto->SetVisibility(ESlateVisibility::Visible);

	Player->GetODSkillComponent()->GetSkillSlots()[SkillIndex].WidgetCooldownDele.RemoveDynamic(this,&ThisClass::CoolTime);
	Player->GetODSkillComponent()->GetSkillSlots()[SkillIndex].WidgetActivateDele.RemoveDynamic(this,&ThisClass::PlayAnimation_TryActivate);
}

void UODWidget_SkillHudActive::CoolTime(FSkillCooldownDuration InDuration)
{
	if (InDuration.CurrentCooldown <= 0)
	{
		TextBlock_CoolTime->SetVisibility(ESlateVisibility::Hidden);
		ProgressBar_CoolTime->SetPercent(0);
		return;
	}
	
	TextBlock_CoolTime->SetVisibility(ESlateVisibility::Visible);
	
	float Ratio = 1.0f - (InDuration.CurrentCooldown / InDuration.TotalCooldown);
	
	ProgressBar_CoolTime->SetPercent(Ratio);

	FText Text = FText::Format(FText::FromString("{0}"), FText::AsNumber(InDuration.CurrentCooldown , &FNumberFormattingOptions()
		.SetMinimumFractionalDigits(1)
		.SetMaximumFractionalDigits(1)));

	TextBlock_CoolTime->SetText(Text);
}

void UODWidget_SkillHudActive::PlayAnimation_TryActivate()
{
	PlayAnimation(Anim_TryActivate);
}


