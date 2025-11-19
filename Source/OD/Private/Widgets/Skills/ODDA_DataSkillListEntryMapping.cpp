// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/ODDA_DataSkillListEntryMapping.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Base.h"


TSubclassOf<UODWidget_SkillListEntry_Base> UODDA_DataSkillListEntryMapping::FindEntryWidgetClassByDataObject(
	UODListSkillDataObject_Base* InDataObject) const
{
	check(InDataObject);

	for (UClass* DataObjectClass = InDataObject->GetClass(); DataObjectClass; DataObjectClass = DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UODListSkillDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UODListSkillDataObject_Base>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}
	
	return nullptr;
}
