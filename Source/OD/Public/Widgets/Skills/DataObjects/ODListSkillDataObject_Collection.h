// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Base.h"
#include "ODListSkillDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODListSkillDataObject_Collection : public UODListSkillDataObject_Base
{
	GENERATED_BODY()
	
public:
	void AddChildListData(UODListSkillDataObject_Base* InChildListData);
	
	virtual TArray<UODListSkillDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasChildListData() const override;

private:
	UPROPERTY(Transient)
	TArray<UODListSkillDataObject_Base*> ChildListDataArray;
};
