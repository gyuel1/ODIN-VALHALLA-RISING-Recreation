// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ODUISubsystem.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/ODFrontendFunctionLibrary.h"
#include "Helper/ODDebugHelper.h"
#include "Widgets/ODWidget_Activatable.h"
#include "Widgets/ODWidget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/ODWidget_ConfirmScreen.h"
#include "Tags/ODUITag.h"
#include "Widgets/ODWidget_MessageScreen.h"


UODUISubsystem* UODUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UODUISubsystem>(World->GetGameInstance());
	}
	
	return nullptr;
}

void UODUISubsystem::RegisterCreatedPrimaryLayoutWidget(UODWidget_PrimaryLayout* InCreateWidget)
{
	if (!InCreateWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("PrimaryLayout is null on RegisterCreatedPrimaryLayoutWidget!"));
		return;
	}

	if (CreatedPrimaryLayout)
	{
		UE_LOG(LogTemp, Warning, TEXT("PrimaryLayout already registered! Old: %s, New: %s"), 
		   *CreatedPrimaryLayout->GetName(), *InCreateWidget->GetName());
		// 중복Set 임시
		CreatedPrimaryLayout = InCreateWidget;
		return; // 이미 등록
	}

	CreatedPrimaryLayout = InCreateWidget;
}

void UODUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UODWidget_Activatable> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UODWidget_Activatable*)> AsyncPushStateCallBack)
{
	// 먼저 Subsystem이 살아 있는지 안전하게 확인
    if (!IsValid(this))
    {
        AsyncPushStateCallBack(EAsyncPushWidgetState::Failed, nullptr);
        return;
    }

    // PrimaryLayout 준비 확인
    if (!CreatedPrimaryLayout)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreatedPrimaryLayout not ready, delaying Push..."));

        // WeakPtr로 안전하게 재시도
        TWeakObjectPtr<UODUISubsystem> WeakThis(this);

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis, InWidgetStackTag, InSoftWidgetClass, AsyncPushStateCallBack]()
        {
            if (!WeakThis.IsValid())
            {
                AsyncPushStateCallBack(EAsyncPushWidgetState::Failed, nullptr);
                return;
            }

            WeakThis->PushSoftWidgetToStackAsync(InWidgetStackTag, InSoftWidgetClass, AsyncPushStateCallBack);

        }, 0.1f, false);

        return;
    }

    // Soft Widget Async Load
    UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        InSoftWidgetClass.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda([InSoftWidgetClass, this, InWidgetStackTag, AsyncPushStateCallBack]()
        {
            UClass* LoadedWidgetClass = InSoftWidgetClass.Get();

            if (!LoadedWidgetClass || !CreatedPrimaryLayout)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load widget class or PrimaryLayout missing!"));
                AsyncPushStateCallBack(EAsyncPushWidgetState::Failed, nullptr);
                return;
            }

            UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

            if (!FoundWidgetStack)
            {
                UE_LOG(LogTemp, Error, TEXT("WidgetStack not found for tag %s"), *InWidgetStackTag.ToString());
                AsyncPushStateCallBack(EAsyncPushWidgetState::Failed, nullptr);
                return;
            }

            // Widget 생성
            UODWidget_Activatable* CreatedWidget = FoundWidgetStack->AddWidget<UODWidget_Activatable>(
                LoadedWidgetClass,
                [AsyncPushStateCallBack](UODWidget_Activatable& CreatedWidgetInstance)
                {
                    AsyncPushStateCallBack(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
                });

            AsyncPushStateCallBack(EAsyncPushWidgetState::AfterPush, CreatedWidget);
        })
    );
}

void UODUISubsystem::PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle,
	const FText& InScreenMessage, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallBack)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;
	
	switch (InScreenType)
	{
	case EConfirmScreenType::OK:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::OKCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKCancelScreen(InScreenTitle, InScreenMessage);
		break;
	case EConfirmScreenType::Unknown:
		break;
	default:
		break;
	}

	check(CreatedInfoObject);


		PushSoftWidgetToStackAsync(
			UITags::OD_WidgetStack_Modal,
			UODFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UITags::OD_Widget_ConfirmScreen),
			[CreatedInfoObject, ButtonClickedCallBack](EAsyncPushWidgetState InPushState, UODWidget_Activatable* PushedWidget)
			{
				if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
				{
					UODWidget_ConfirmScreen* CreatedConfirmScreen = CastChecked<UODWidget_ConfirmScreen>(PushedWidget);
					CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallBack);
				}
			}
			);
}

void UODUISubsystem::PushMessageScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle,
	const FText& InScreenMessage, const FText& InEditHintText, const FText& InEditText,
	TFunction<void(EConfirmScreenButtonType, const FText&)> ButtonClickedCallBack)
{
	UMessageScreenInfoObject* CreatedInfoObject = nullptr;
	
	switch (InScreenType)
	{
	case EConfirmScreenType::OK:
		CreatedInfoObject = UMessageScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMessage, InEditHintText, InEditText);
		break;
	case EConfirmScreenType::YesNo:
		CreatedInfoObject = UMessageScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMessage, InEditHintText, InEditText);
		break;
	case EConfirmScreenType::OKCancel:
		CreatedInfoObject = UMessageScreenInfoObject::CreateOKCancelScreen(InScreenTitle, InScreenMessage, InEditHintText, InEditText);
		break;
	case EConfirmScreenType::Unknown:
		break;
	default:
		break;
	}

	check(CreatedInfoObject);


	PushSoftWidgetToStackAsync(
		UITags::OD_WidgetStack_Modal,
		UODFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UITags::OD_Widget_MessageScreen),
		[CreatedInfoObject, ButtonClickedCallBack](EAsyncPushWidgetState InPushState, UODWidget_Activatable* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UODWidget_MessageScreen* CreatedConfirmScreen = CastChecked<UODWidget_MessageScreen>(PushedWidget);
				CreatedConfirmScreen->InitMessageScreen(CreatedInfoObject, ButtonClickedCallBack);
			}
		}
		);
}



bool UODUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (const auto* GI = Cast<UGameInstance>(Outer))
	{
		if (GI->IsDedicatedServerInstance())
		{
			return false; // 서버는 생성 안 함
		}

		if (GI->GetWorld() &&
			(GI->GetWorld()->WorldType == EWorldType::EditorPreview ||
			 GI->GetWorld()->WorldType == EWorldType::Editor))
		{
			return false; // 에디터 프리뷰 월드에서는 생성 안 함
		}

		// 상속 중복 방지
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		return FoundClasses.IsEmpty();
	}

	return false;
}
