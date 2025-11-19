// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/ODAsyncAction_PushSoftWidget.h"

#include "Helper/ODDebugHelper.h"
#include "Player/ODPlayerController.h"
#include "Subsystem/ODUISubsystem.h"
#include "Widgets/ODWidget_Activatable.h"

UODAsyncAction_PushSoftWidget* UODAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
	APlayerController* OwningPlayerController, TSoftClassPtr<UODWidget_Activatable> InSoftWidgetClass,
	UPARAM(meta = (Categories = "OD.WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	// SoftWidgetClass가 비어있으면 크래쉬
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack Was Passed a Null Soft Widget Class"))

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UODAsyncAction_PushSoftWidget* Node = NewObject<UODAsyncAction_PushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}
	return nullptr;
}

void UODAsyncAction_PushSoftWidget::Activate()
{
	UODUISubsystem* ODUISubSystem = UODUISubsystem::Get(CachedOwningWorld.Get());
	
	ODUISubSystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState InPushState, UODWidget_Activatable* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				break;
			case EAsyncPushWidgetState::AfterPush:
				AfterPush.Broadcast(PushedWidget);

				if (bCachedFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}
				SetReadyToDestroy();
				break;
			default:
				break;
			}
			
		}
		);
}
