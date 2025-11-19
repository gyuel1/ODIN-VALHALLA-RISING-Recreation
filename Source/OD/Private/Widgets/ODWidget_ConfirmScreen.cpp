// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ODWidget_ConfirmScreen.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Component/ODCommonButtonBase.h"
#include "ICommonInputModule.h"

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle,
                                                                   const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("확인"));

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("확인"));

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("취소"));

	InfoObject->AvailableScreenButtons.Emplace(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Emplace(NoButtonInfo);
	
	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;

	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("확인"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("취소"));

	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);
	
	return InfoObject;
}

void UODWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallBack)
{
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);

	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// 이전 버튼을 모두 지움
		DynamicEntryBox_Buttons->Reset<UODCommonButtonBase>(
			[](UODCommonButtonBase& ExistingButton)
			{
				// Clear
				ExistingButton.OnClicked().Clear();
			}
			);
	}

	check(!InScreenInfoObject->AvailableScreenButtons.IsEmpty());

	for (const FConfirmScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		
		switch (AvailableButtonInfo.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::Cancelled:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		case EConfirmScreenButtonType::Closed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		default:
			break;
		}
		UODCommonButtonBase* AddedButton = DynamicEntryBox_Buttons->CreateEntry<UODCommonButtonBase>();
		AddedButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		AddedButton->SetTriggeringInputAction(InputActionRowHandle);
		AddedButton->OnClicked().AddLambda(
			[ClickedButtonCallBack,AvailableButtonInfo,this]()
			{
				ClickedButtonCallBack(AvailableButtonInfo.ConfirmScreenButtonType);

				DeactivateWidget();
			}
			);
	}

	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		// 가장 마지막에 추가된 버튼 포커스
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}
}
