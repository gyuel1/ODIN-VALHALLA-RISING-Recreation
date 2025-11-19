// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ODListDataObject_Value.h"


void UODListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FODOtionsDataInteractionHelper>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

void UODListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FODOtionsDataInteractionHelper>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}
