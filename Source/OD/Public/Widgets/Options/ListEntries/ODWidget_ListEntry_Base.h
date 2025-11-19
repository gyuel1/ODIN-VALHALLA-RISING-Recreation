// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ODWidget_ListEntry_Base.generated.h"

enum class EOptionsListDataModifyReason : uint8;
class UCommonTextBlock;
class UODListDataObject_Base;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	virtual void OnOwningListDataObjectSet(UODListDataObject_Base* InOwningListDataObject);

	virtual void OnOwningListDataObjectModified(UODListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
};
