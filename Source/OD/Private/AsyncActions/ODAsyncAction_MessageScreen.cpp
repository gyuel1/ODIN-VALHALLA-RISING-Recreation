// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/ODAsyncAction_MessageScreen.h"

#include "Subsystem/ODUISubsystem.h"


UODAsyncAction_MessageScreen* UODAsyncAction_MessageScreen::PushConfirmScreen(const UObject* WorldContextObject,
                                                                              EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMessage, FText CachedScreenEditHintText,
                                                                              FText CachedScreenEditText)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UODAsyncAction_MessageScreen* Node = NewObject<UODAsyncAction_MessageScreen>();
			Node->CachedOwingWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;
			Node->CachedScreenEditHintText = CachedScreenEditHintText;
			Node->CachedScreenEditText = CachedScreenEditText;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}

void UODAsyncAction_MessageScreen::Activate()
{
	UODUISubsystem::Get(CachedOwingWorld.Get())->PushMessageScreenToModalStackAsync(
		CachedScreenType,
		CachedScreenTitle,
		CachedScreenMessage,
		CachedScreenEditHintText,
		CachedScreenEditText,
		[this](EConfirmScreenButtonType ClickedButtonType, const FText& FinalScreenEditText)
		{
			this->CachedScreenEditText = FinalScreenEditText;
			OnButtonClicked.Broadcast(ClickedButtonType,this->CachedScreenEditText);

			SetReadyToDestroy();
		}
		);
}
