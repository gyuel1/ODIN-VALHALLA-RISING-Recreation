// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ODDataAsset_DataListEntryMapping.h"

#include "Widgets/Options/DataObjects/ODListDataObject_Base.h"


TSubclassOf<UODWidget_ListEntry_Base> UODDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(
	UODListDataObject_Base* InDataObject) const
{
	check(InDataObject);

	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UODListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UODListDataObject_Base>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}
	
	return nullptr;
}
