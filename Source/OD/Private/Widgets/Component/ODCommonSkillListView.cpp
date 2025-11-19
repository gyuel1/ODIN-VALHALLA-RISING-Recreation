// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Component/ODCommonSkillListView.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Skills/ODDA_DataSkillListEntryMapping.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Base.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Collection.h"
#include "Widgets/Skills/ListEntries/ODWidget_SkillListEntry_Base.h"


UUserWidget& UODCommonSkillListView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                   TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UODListSkillDataObject_Base* DataObject = Cast<UODListSkillDataObject_Base>(Item);

	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
	
	if (DataObject && DataListEntryMapping)
	{
		if (TSubclassOf<UODWidget_SkillListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(DataObject))
		{
			return GenerateTypedEntry<UODWidget_SkillListEntry_Base>(FoundWidgetClass,OwnerTable);
		}
	}
    
	// 유효하지 않은 경우 부모 함수 호출 또는 오류 처리
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UODCommonSkillListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UODListSkillDataObject_Collection>();
}

#if WITH_EDITOR
void UODCommonSkillListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
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
