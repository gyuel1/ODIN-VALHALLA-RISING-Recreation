// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ODListDataObject_Base.h"

#include "Setting/ODGameUserSettings.h"


void UODListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UODListDataObject_Base::OnDataObjectInitialized()
{
}

void UODListDataObject_Base::NotifyListDataModified(UODListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeImmediatly)
	{
		UODGameUserSettings::Get()->ApplySettings(true);
	}
}
