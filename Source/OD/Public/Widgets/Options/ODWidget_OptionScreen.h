// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_OptionScreen.generated.h"

enum class EOptionsListDataModifyReason : uint8;
class UODListDataObject_Base;
class UODCommonListView;
class UODCommonTabListWidgetBase;
class UODOptionsDataRegistry;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_OptionScreen : public UODWidget_Activatable
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
private:
	UODOptionsDataRegistry* GetOrCreateDataRegistry();

	UFUNCTION(BlueprintCallable)
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	void OnListVieListDataModified(UODListDataObject_Base* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	UPROPERTY(meta = (BindWidget))
	UODCommonTabListWidgetBase* TabListWidget_OptionsTabs;
	
	UPROPERTY(meta = (BindWidget))
	UODCommonListView* CommonListView_OptionsList;

	UPROPERTY(Transient)
	UODOptionsDataRegistry* CreatedOwningDataRegistry;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UPROPERTY(Transient)
	TArray<UODListDataObject_Base*> ResettableDataArray;

	bool bIsResettingData = false;
};

