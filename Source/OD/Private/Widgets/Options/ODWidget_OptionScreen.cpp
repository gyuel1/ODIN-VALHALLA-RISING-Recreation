// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ODWidget_OptionScreen.h"

#include "ICommonInputModule.h"
#include "Helper/ODDebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "Setting/ODGameUserSettings.h"
#include "Subsystem/ODUISubsystem.h"
#include "Widgets/Component/ODCommonListView.h"
#include "Widgets/Component/ODCommonTabListWidgetBase.h"
#include "Widgets/Options/ODOptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ODListDataObject_Collection.h"


void UODWidget_OptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
		FBindUIActionArgs (
			ResetAction,
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
			);

		Debug::Print(TEXT("ResetAction 생성"));
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
			)
			);
	
	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
}

void UODWidget_OptionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UODListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}

		const FName TabID = TabCollection->GetDataID();
		
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UODWidget_OptionScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	UODGameUserSettings::Get()->ApplySettings(true);
}

UODOptionsDataRegistry* UODWidget_OptionScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UODOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("Data Registry for Options Screen is not valid"))
	
	return CreatedOwningDataRegistry;
}

void UODWidget_OptionScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UODUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("설정")),
		FText::FromString(TEXT("현재 탭에 모든 변경사항을 초기화하시겠습니까?")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bHasDataFailedToReset = false;
			
			for (UODListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if (!DataToReset)
				{
					continue;
				}

				if (DataToReset->TryResetBackToDefaultValue())
				{
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT("Was Reset"));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT("Faild to Reset"));
				}
			}

			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();
			    RemoveActionBinding(ResetActionHandle);
			}
			
			bIsResettingData = false;
		}
		);
}

void UODWidget_OptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UODWidget_OptionScreen::OnListVieListDataModified(UODListDataObject_Base* ModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}

void UODWidget_OptionScreen::OnOptionsTabSelected(FName TabID)
{
	TArray<UODListDataObject_Base*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UODListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListVieListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}
