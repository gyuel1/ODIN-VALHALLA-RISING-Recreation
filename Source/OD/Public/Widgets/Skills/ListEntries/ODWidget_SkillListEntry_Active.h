// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Skills/ListEntries/ODWidget_SkillListEntry_Base.h"
#include "ODWidget_SkillListEntry_Active.generated.h"

class UTextBlock;
class UODWidget_SkillSlot_SkillBook;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_SkillListEntry_Active : public UODWidget_SkillListEntry_Base
{
	GENERATED_BODY()

	// SkillSlot 초기화
	virtual void OnOwningListDataObjectSet(UODListSkillDataObject_Base* InOwningListDataObject) override;


	
};
