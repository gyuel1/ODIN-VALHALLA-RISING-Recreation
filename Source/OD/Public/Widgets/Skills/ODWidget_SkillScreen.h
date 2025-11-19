// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_SkillScreen.generated.h"

class UODWidget_SkillListEntry_Base;
class UODWidget_SkillSlotChangeScreen;
class UODCommonTabListWidgetBase;
class UODWidget_SkillsDetailsView;
class UODCommonListView;
class UODSkillsDataRegistry;

/**
 * 
 */
UCLASS()
class OD_API UODWidget_SkillScreen : public UODWidget_Activatable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SkillSlotChangeScreenVisibility(ESlateVisibility Invisibility);
	UFUNCTION(BlueprintCallable)
	void UnlockSKill();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	

private:
	UODSkillsDataRegistry* GetOrCreateSkillDataRegistry();
		
	UFUNCTION()
	void OnSkillsTabSelected(FName TabID);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);
	void OnListViewItemClicked(UObject* InClickedItem);
	

	UPROPERTY(Transient)
	UODSkillsDataRegistry* CreatedOwningSkillDataRegistry;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonTabListWidgetBase* TabListWidget_SkillsTabs;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODCommonListView* CommonListView_SkillsList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODWidget_SkillsDetailsView* SkillsDetailsView_ListEntryInfo;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UODWidget_SkillSlotChangeScreen* SkillSlotChangeScreen;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_SkillSlotVisibility;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UObject> SelectedItem;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UODWidget_SkillListEntry_Base> SelectedEntryWidget;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = ( AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> AddSlotSfx;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = ( AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> UnlockSkillSfx;
};
