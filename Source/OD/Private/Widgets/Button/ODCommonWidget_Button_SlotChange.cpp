// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Button/ODCommonWidget_Button_SlotChange.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Helper/ODDebugHelper.h"
#include "Widgets/Skills/Slots/ODWidget_SkillHudActive.h"


void UODCommonWidget_Button_SlotChange::AutoActiveSkillSlot()
{
	HorizontalBox_AutoDeactivateSkill->SetVisibility(ESlateVisibility::Hidden);
	HorizontalBox_AutoActiveSkill->SetVisibility(ESlateVisibility::Visible);
}

void UODCommonWidget_Button_SlotChange::AutoDeactivateSkillSlot()
{
	HorizontalBox_AutoActiveSkill->SetVisibility(ESlateVisibility::Hidden);
	HorizontalBox_AutoDeactivateSkill->SetVisibility(ESlateVisibility::Visible);
}

void UODCommonWidget_Button_SlotChange::AddSlot(const FSlateBrush& SkillBrush)
{
	Image_SkillIcon->SetBrush(SkillBrush);
	Image_SkillEmpty->SetVisibility(ESlateVisibility::Hidden);
	Image_SkillIcon->SetVisibility(ESlateVisibility::Visible);
	SkillHudActive->AddSlot(SkillBrush);
	
}


void UODCommonWidget_Button_SlotChange::RemoveSlot()
{
	SKillName = NAME_None;
	Image_SkillIcon->SetVisibility(ESlateVisibility::Hidden);
	Image_SkillEmpty->SetVisibility(ESlateVisibility::Visible);
	Debug::Print(TEXT("스킬 제거됨"));
	SkillHudActive->RemoveSlot();
}
