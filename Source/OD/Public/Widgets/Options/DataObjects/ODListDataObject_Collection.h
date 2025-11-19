// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Base.h"
#include "ODListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODListDataObject_Collection : public UODListDataObject_Base
{
	GENERATED_BODY()

public:
	void AddChildListData(UODListDataObject_Base* InChildListData);
	virtual TArray<UODListDataObject_Base*> GetAllChildListData() const;
	virtual bool HasAnyChildListData() const;
	
private:
	UPROPERTY(Transient)
	TArray<UODListDataObject_Base*> ChildListDataArray;
};
