// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ODListDataObject_Collection.h"


void UODListDataObject_Collection::AddChildListData(UODListDataObject_Base* InChildListData)
{
	InChildListData->InitDataObject();

	InChildListData->SetParentData(this);
	
	ChildListDataArray.Add(InChildListData);
}

TArray<UODListDataObject_Base*> UODListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UODListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
