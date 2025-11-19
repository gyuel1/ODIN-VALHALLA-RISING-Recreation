// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ODWidget_ListEntry_Base.h"

#include "CommonTextBlock.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Base.h"


void UODWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UODListDataObject_Base>(ListItemObject));
}

void UODWidget_ListEntry_Base::OnOwningListDataObjectSet(UODListDataObject_Base* InOwningListDataObject)
{
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	if (!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
}

void UODWidget_ListEntry_Base::OnOwningListDataObjectModified(UODListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	
}
