// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Component/ODCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Helper/ODDebugHelper.h"


void UODCommonButtonBase::SetButtonText(FText Text)
{
	if (CommonTextBlock_ButtonText && !Text.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUseUpperCaseForButtonText ? Text.ToUpper() : Text);
	}
}

void UODCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UODCommonButtonBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UODCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UODCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
}

void UODCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
}

void UODCommonButtonBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void UODCommonButtonBase::NativeOnClicked()
{
	Super::NativeOnClicked();
}




