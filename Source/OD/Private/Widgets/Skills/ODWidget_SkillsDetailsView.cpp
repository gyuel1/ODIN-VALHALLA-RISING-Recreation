// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/ODWidget_SkillsDetailsView.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "DataAssets/Ability/ODSkillDA.h"
#include "Helper/ODDebugHelper.h"
#include "Helper/ODStaticFunction.h"
#include "Widgets/Skills/ODWidget_SkillDetailDescription.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Base.h"
#include "Widgets/Skills/Slots/ODWidget_SkillSlot_SkillBook.h"


void UODWidget_SkillsDetailsView::UpdateDetailsViewInfo(UODListSkillDataObject_Base* InDataObject)
{
	if (!InDataObject)
	{
		return;
	}

	VerticalBox_NormalSkillDetail->ClearChildren();
	VerticalBox_SpecialSkillDetail->ClearChildren();

	CommonTextBlock_SkillName->SetText(InDataObject->GetDataDisplayName());
	CommonTextBlock_SkillDescription->SetText(InDataObject->GetSkillDescriptionText());
	SkillSlot->GetImage_SkillIcon()->SetBrush(InDataObject->GetSkillIcon());
	FString EnumName = UEnum::GetValueAsString(InDataObject->GetSkillGrade());
	SkillSlot->SetSkillActivationAndGrade(InDataObject->GetActivationType(), InDataObject->GetSkillGrade());
	SkillSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	;

	if (InDataObject->GetSkillNormalDetailInfo().IsValidIndex(0))
	{
		for (FSkillDetailInfo DetailInfo : InDataObject->GetSkillNormalDetailInfo())
		{
			if (!SkillDetailDescription.IsValid())
			{
				SkillDetailDescription.LoadSynchronous();
			}
			
			if (UClass* NewWidgetClass = SkillDetailDescription.Get())
			{
				UODWidget_SkillDetailDescription* NewDetailDescription = CreateWidget<UODWidget_SkillDetailDescription>(GetWorld(), NewWidgetClass);
				NewDetailDescription->SetBuffTexture(DetailInfo.BuffDescriptionType);
				NewDetailDescription->SetSkillDescription(DetailInfo.SkillDetailDescriptionText);
				if (UVerticalBoxSlot* NewSlot = VerticalBox_NormalSkillDetail->AddChildToVerticalBox(NewDetailDescription))
				{
					NewSlot->SetPadding(FMargin(0.f,3.f,0.f,0.f));
				}
			}
		}
	}

	if (InDataObject->GetSkillSpecialDetailInfo().IsValidIndex(0))
	{
		for (FSkillDetailInfo DetailInfo : InDataObject->GetSkillSpecialDetailInfo())
		{
			if (!SkillDetailDescription.IsValid())
			{
				SkillDetailDescription.LoadSynchronous();
			}
			
			if (UClass* NewWidgetClass = SkillDetailDescription.Get())
			{
				UODWidget_SkillDetailDescription* NewDetailDescription = CreateWidget<UODWidget_SkillDetailDescription>(GetWorld(), NewWidgetClass);
				NewDetailDescription->SetBuffTexture(DetailInfo.BuffDescriptionType);
				NewDetailDescription->SetSkillDescription(DetailInfo.SkillDetailDescriptionText);
				if (UVerticalBoxSlot* NewSlot = VerticalBox_SpecialSkillDetail->AddChildToVerticalBox(NewDetailDescription))
				{
					NewSlot->SetPadding(FMargin(0.f,5.f,0.f,0.f));
				}
			}
		}
	}
	
}

void UODWidget_SkillsDetailsView::ClearDetailsViewInfo()
{
	CommonTextBlock_SkillName->SetText(FText::GetEmpty());
	CommonTextBlock_SkillDescription->SetText(FText::GetEmpty());
	SkillSlot->SetVisibility(ESlateVisibility::Collapsed);
	VerticalBox_NormalSkillDetail->ClearChildren();
	VerticalBox_SpecialSkillDetail->ClearChildren();
}

void UODWidget_SkillsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SkillSlot->SetSizeBox_SkillDetailView();
	ClearDetailsViewInfo();
}
