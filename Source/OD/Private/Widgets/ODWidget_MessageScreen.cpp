// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ODWidget_MessageScreen.h"

#include "CommonTextBlock.h"
#include "ICommonInputModule.h"
#include "Components/DynamicEntryBox.h"
#include "Components/EditableText.h"
#include "Widgets/Component/ODCommonButtonBase.h"


UMessageScreenInfoObject* UMessageScreenInfoObject::CreateOKScreen(const FText& InScreenTitle,
                                                                   const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText)
{
	UMessageScreenInfoObject* InfoObject = NewObject<UMessageScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	InfoObject->EditHintText = InEditHintText;
	InfoObject->EditText = InEditText;

	FMessageScreenButtonInfo OKButtonInfo;
	OKButtonInfo.MessageScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("확인"));

	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);
	
	return InfoObject;
}

UMessageScreenInfoObject* UMessageScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle,
	const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText)
{
	UMessageScreenInfoObject* InfoObject = NewObject<UMessageScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	InfoObject->EditHintText = InEditHintText;
	InfoObject->EditText = InEditText;

	FMessageScreenButtonInfo YesButtonInfo;
	YesButtonInfo.MessageScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("확인"));

	FMessageScreenButtonInfo NoButtonInfo;
	NoButtonInfo.MessageScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("취소"));

	InfoObject->AvailableScreenButtons.Emplace(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Emplace(NoButtonInfo);
	
	return InfoObject;
}

UMessageScreenInfoObject* UMessageScreenInfoObject::CreateOKCancelScreen(const FText& InScreenTitle,
	const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText)
{
	UMessageScreenInfoObject* InfoObject = NewObject<UMessageScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMessage;
	InfoObject->EditHintText = InEditHintText;
	InfoObject->EditText = InEditText;

	FMessageScreenButtonInfo YesButtonInfo;
	YesButtonInfo.MessageScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("확인"));

	FMessageScreenButtonInfo NoButtonInfo;
	NoButtonInfo.MessageScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("취소"));

	InfoObject->AvailableScreenButtons.Emplace(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Emplace(NoButtonInfo);
	
	return InfoObject;
}

void UODWidget_MessageScreen::InitMessageScreen(UMessageScreenInfoObject* InScreenInfoObject,
	TFunction<void(EConfirmScreenButtonType, const FText&)> ClickedButtonCallBack)
{
	check(InScreenInfoObject && CommonTextBlock_Title && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Title->SetText(InScreenInfoObject->ScreenTitle);
	CommonTextBlock_Message->SetText(InScreenInfoObject->ScreenMessage);
	EditableText->SetHintText(InScreenInfoObject->EditHintText);
	EditableText->SetText(InScreenInfoObject->EditText);

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

	for (const FMessageScreenButtonInfo& AvailableButtonInfo : InScreenInfoObject->AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		
		switch (AvailableButtonInfo.MessageScreenButtonType)
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
				const FText FinalEditText = EditableText->GetText();
				
				ClickedButtonCallBack(AvailableButtonInfo.MessageScreenButtonType, FinalEditText);
				
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
