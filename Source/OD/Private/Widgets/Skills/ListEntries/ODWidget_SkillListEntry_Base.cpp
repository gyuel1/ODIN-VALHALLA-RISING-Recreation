// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/ListEntries/ODWidget_SkillListEntry_Base.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Base.h"
#include "Widgets/Skills/Slots/ODWidget_SkillSlot_SkillBook.h"


void UODWidget_SkillListEntry_Base::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, IsListItemSelected());
}

void UODWidget_SkillListEntry_Base::UnlockSkill()
{
	SetRenderOpacity(1.0f);
	TextBlock_SkillSub->SetVisibility(ESlateVisibility::Hidden);
}

void UODWidget_SkillListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UODListSkillDataObject_Base>(ListItemObject));
}

void UODWidget_SkillListEntry_Base::OnOwningListDataObjectSet(UODListSkillDataObject_Base* InOwningListDataObject)
{
	if (CommonText_SkillDisplayName)
	{
		CommonText_SkillDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	
	SkillSlot->GetImage_SkillIcon()->SetBrush(InOwningListDataObject->GetSkillIcon());
	SkillSlot->SetSkillActivationAndGrade(InOwningListDataObject->GetActivationType(), InOwningListDataObject->GetSkillGrade());

	if (InOwningListDataObject->GetbIsUnlock())
	{
		SetRenderOpacity(1.0f);
		TextBlock_SkillSub->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetRenderOpacity(0.5f);
		TextBlock_SkillSub->SetVisibility(ESlateVisibility::Visible);
	}
}
