// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ODDA_DataSkillListEntryMapping.generated.h"

class UODWidget_SkillListEntry_Base;
class UODListSkillDataObject_Base;
/**
 * 
 */
UCLASS()
class OD_API UODDA_DataSkillListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
	
	
public:
	TSubclassOf<UODWidget_SkillListEntry_Base> FindEntryWidgetClassByDataObject(UODListSkillDataObject_Base* InDataObject) const;

	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UODListSkillDataObject_Base>, TSubclassOf<UODWidget_SkillListEntry_Base>> DataObjectListEntryMap;
	
};
