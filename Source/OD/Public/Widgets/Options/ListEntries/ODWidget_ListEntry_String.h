// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/ODWidget_ListEntry_Base.h"
#include "ODWidget_ListEntry_String.generated.h"

class UODCommonRotator;
class UODCommonButtonBase;
class UODListDataObject_String;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_ListEntry_String : public UODWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void OnOwningListDataObjectSet(UODListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UODListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;

private:
	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonButtonBase* CommonButton_PreviousOption; // 감소버튼

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonRotator* CommonRotator_AvailableOptions; // 사용가능한

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonButtonBase* CommonButton_NextOption; // 증가버튼

	UPROPERTY(Transient)
	UODListDataObject_String* CachedOwningStringDataObject;
	
};
