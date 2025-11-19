// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ODDataAsset_DataListEntryMapping.generated.h"

class UODWidget_ListEntry_Base;
class UODListDataObject_Base;
/**
 * 
 */
UCLASS()
class OD_API UODDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UODWidget_ListEntry_Base> FindEntryWidgetClassByDataObject(UODListDataObject_Base* InDataObject) const;

	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UODListDataObject_Base>, TSubclassOf<UODWidget_ListEntry_Base>> DataObjectListEntryMap;
	
};
