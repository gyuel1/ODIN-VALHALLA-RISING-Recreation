// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Collection.h"


void UODListSkillDataObject_Collection::AddChildListData(UODListSkillDataObject_Base* InChildListData)
{
	// 자식 리스트 데이터 초기화시 알림
	InChildListData->InitDataObject();

	
	InChildListData->SetParentData(this);
	
	ChildListDataArray.Emplace(InChildListData);
}

TArray<UODListSkillDataObject_Base*> UODListSkillDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UODListSkillDataObject_Collection::HasChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
