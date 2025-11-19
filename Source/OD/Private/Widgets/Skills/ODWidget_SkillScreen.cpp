// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Skills/ODWidget_SkillScreen.h"

#include "Character/ODPlayer.h"
#include "Components/ODSkillComponent.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ODPlayerController.h"
#include "Subsystem/ODUISubsystem.h"
#include "Widgets/Button/ODCommonWidget_Button_SlotChange.h"
#include "Widgets/Component/ODCommonListView.h"
#include "Widgets/Component/ODCommonTabListWidgetBase.h"
#include "Widgets/Skills/ODSkillsDataRegistry.h"
#include "Widgets/Skills/ODWidget_SkillsDetailsView.h"
#include "Widgets/Skills/DataObjects/ODListSkillDataObject_Collection.h"
#include "Widgets/Skills/ListEntries/ODWidget_SkillListEntry_Base.h"
#include "Widgets/Skills/Slots/ODWidget_SkillSlotChangeScreen.h"

void UODWidget_SkillScreen::SkillSlotChangeScreenVisibility(ESlateVisibility Invisibility)
{
	SkillSlotChangeScreen->SetVisibility(Invisibility);

	if (Invisibility == ESlateVisibility::Visible)
	{
		PlayAnimation(Anim_SkillSlotVisibility);
	}
}

void UODWidget_SkillScreen::UnlockSKill()
{

	if (SelectedItem.IsValid())
	{
		if (UODListSkillDataObject_Base* SkillData = Cast<UODListSkillDataObject_Base>(SelectedItem))
		{
			if (!SkillData->GetbIsUnlock())
			{
				FText SkillName = SkillData->GetDataDisplayName();
				FText ScreenMessage = FText::FromString(TEXT(" 스킬을 배우시겠습니까?"));
				FText AppendText = FText::Format(FText::FromString(TEXT("{0} {1}")),SkillName,ScreenMessage);

				UODUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("스킬")),
		AppendText,
		[this,SkillData](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			    SkillData->SetbIsUnlock(true);
				SkillData->GetDA_Skill()->SetbIsUnlock(true);
				
				if (SelectedEntryWidget.IsValid())
				{
					UGameplayStatics::PlaySound2D(this,UnlockSkillSfx);
					SelectedEntryWidget->UnlockSkill();
				}
			
		}
		);
	
			}
		}
	}
	
	
	
}

void UODWidget_SkillScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	TabListWidget_SkillsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnSkillsTabSelected);
	CommonListView_SkillsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	CommonListView_SkillsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
	CommonListView_SkillsList->OnItemClicked().AddUObject(this, &ThisClass::OnListViewItemClicked);
}

void UODWidget_SkillScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UODListSkillDataObject_Collection* TabCollection : GetOrCreateSkillDataRegistry()->GetRegisteredSkillsTabCollections())
	{
		if (!TabCollection)
			continue;

		const FName TabID = TabCollection->GetDataID();
		
		if (TabListWidget_SkillsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		TabListWidget_SkillsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}

	CommonListView_SkillsList->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UODWidget_SkillScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

UODSkillsDataRegistry* UODWidget_SkillScreen::GetOrCreateSkillDataRegistry()
{
	if (!CreatedOwningSkillDataRegistry)
	{
		CreatedOwningSkillDataRegistry = NewObject<UODSkillsDataRegistry>();
		CreatedOwningSkillDataRegistry->InitSkillsDataRegistry(GetOwningLocalPlayer());
	}

	
	checkf(CreatedOwningSkillDataRegistry, TEXT("Data Registry for Skill Screen is not valid"))
	
	return CreatedOwningSkillDataRegistry;
}

void UODWidget_SkillScreen::OnSkillsTabSelected(FName TabID)
{
	TArray<UODListSkillDataObject_Base*> FoundListSourceItems = GetOrCreateSkillDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	CommonListView_SkillsList->SetListItems(FoundListSourceItems);
	CommonListView_SkillsList->RequestRefresh();

	if (CommonListView_SkillsList->GetNumItems() != 0)
	{
		CommonListView_SkillsList->NavigateToIndex(0);
		CommonListView_SkillsList->SetSelectedIndex(0);
	}
}

void UODWidget_SkillScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}

	UODWidget_SkillListEntry_Base* HoveredEntryWidget = CommonListView_SkillsList->GetEntryWidgetFromItem<UODWidget_SkillListEntry_Base>(InHoveredItem);

	check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
	
}

void UODWidget_SkillScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}

	SelectedItem = InSelectedItem;
	SelectedEntryWidget = CommonListView_SkillsList->GetEntryWidgetFromItem<UODWidget_SkillListEntry_Base>(InSelectedItem);
	

	SkillsDetailsView_ListEntryInfo->UpdateDetailsViewInfo(
		CastChecked<UODListSkillDataObject_Base>(InSelectedItem));

	
}

void UODWidget_SkillScreen::OnListViewItemClicked(UObject* InClickedItem)
{
	int32 Index = SkillSlotChangeScreen->GetCurrentSelectedIndex();
	AODPlayer* Player = Cast<AODPlayer>(GetOwningPlayerController()->GetPawn());
	
	if (Index == INDEX_NONE)
	{
		return;
	}

	if (UODListSkillDataObject_Base* SkillData = Cast<UODListSkillDataObject_Base>(InClickedItem))
	{
		if (SkillData->GetbIsUnlock())
		{
			FName NewSkillName = FName(*SkillData->GetDataDisplayName().ToString());

			for (int32 i = 0; i < SkillSlotChangeScreen->GetSkillChangeSlotButtons().Num(); ++i)
			{
				if (i != Index && SkillSlotChangeScreen->GetSkillChangeSlotButtons()[i]->SKillName == NewSkillName)
				{
					SkillSlotChangeScreen->GetSkillChangeSlotButtons()[i]->RemoveSlot();
					Player->GetODSkillComponent()->RemoveSkillFromSlot(i);
				}
			}

			auto& SlotButton = SkillSlotChangeScreen->GetSkillChangeSlotButtons()[Index];
			SlotButton->SKillName = NewSkillName;
			SlotButton->AddSlot(SkillData->GetSkillIcon());
			Player->GetODSkillComponent()->AddSkillToSlot(Index,SkillData->GetSkillTag());
			UGameplayStatics::PlaySound2D(this,AddSlotSfx);
			
		}
	}
}
