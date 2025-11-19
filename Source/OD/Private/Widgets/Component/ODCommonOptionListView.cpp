// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Component/ODCommonOptionListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/ODDataAsset_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Collection.h"
#include "Widgets/Options/ListEntries/ODWidget_ListEntry_Base.h"


UUserWidget& UODCommonOptionListView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                    TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UODListDataObject_Base* DataObject = Cast<UODListDataObject_Base>(Item);

	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	
	if (DataObject && DataListEntryMapping)
	{
		if (TSubclassOf<UODWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(DataObject))
		{
			return GenerateTypedEntry<UODWidget_ListEntry_Base>(FoundWidgetClass,OwnerTable);
		}
	}
    
	// 유효하지 않은 경우 부모 함수 호출 또는 오류 처리
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UODCommonOptionListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UODListDataObject_Collection>();
}

#if WITH_EDITOR
void UODCommonOptionListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("The Variable DataListEntryMapping has no valid data asset assigned")+
			GetClass()->GetName() + 
			TEXT(" Needs a valid data Asset to Function Properly")
			));
	}
}
#endif
