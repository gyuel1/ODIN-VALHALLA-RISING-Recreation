// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ODWidget_SkillListEntry_Base.generated.h"

class UCommonTextBlock;
class UODWidget_SkillSlot_SkillBook;
class UTextBlock;
/**
 * 
 */
class UODListSkillDataObject_Base;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_SkillListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnListEntryWidgetHovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

public:
	void UnlockSkill();
	
protected:
	//~ Begin IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry
	
	virtual void OnOwningListDataObjectSet(UODListSkillDataObject_Base* InOwningListDataObject);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODWidget_SkillSlot_SkillBook* SkillSlot;

	UPROPERTY(BluePrintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_SkillSub; 

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SkillDisplayName;
};
