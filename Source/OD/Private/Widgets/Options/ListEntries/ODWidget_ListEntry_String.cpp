// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ODWidget_ListEntry_String.h"

#include "Widgets/Component/ODCommonButtonBase.h"
#include "Widgets/Component/ODCommonRotator.h"
#include "Widgets/Options/DataObjects/ODListDataObject_String.h"


void UODWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);
}

void UODWidget_ListEntry_String::OnOwningListDataObjectSet(UODListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UODListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UODWidget_ListEntry_String::OnOwningListDataObjectModified(UODListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UODWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}
}

void UODWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}
}
