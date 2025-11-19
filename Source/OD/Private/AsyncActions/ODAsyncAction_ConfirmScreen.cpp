// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/ODAsyncAction_ConfirmScreen.h"

#include "Subsystem/ODUISubsystem.h"


UODAsyncAction_ConfirmScreen* UODAsyncAction_ConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject,
                                                                              EConfirmScreenType ScreenType,  FText InScreenTitle, FText InScreenMessage)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UODAsyncAction_ConfirmScreen* Node = NewObject<UODAsyncAction_ConfirmScreen>();
			Node->CachedOwingWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}

void UODAsyncAction_ConfirmScreen::Activate()
{
	UODUISubsystem::Get(CachedOwingWorld.Get())->PushConfirmScreenToModalStackAsync(
		CachedScreenType,
		CachedScreenTitle,
		CachedScreenMessage,
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			OnButtonClicked.Broadcast(ClickedButtonType);

			SetReadyToDestroy();
		}
		);
}
