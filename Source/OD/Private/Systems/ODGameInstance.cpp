// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/ODGameInstance.h"

#include "Helper/ODDebugHelper.h"
#include "Widgets/HUD/Credit/ODWidget_Credits.h"


UODGameInstance::UODGameInstance()
{
	
}

void UODGameInstance::Init()
{
	Super::Init();
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnPostLoadMap);
}

void UODGameInstance::Shutdown()
{
	Super::Shutdown();

	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UODGameInstance::OnPreLoadMap(const FString& MapName)
{
	if (IsRunningDedicatedServer())
		return; 
	
	FString TargetMap = FPackageName::GetShortName(MapName);
	
	if (TargetMap.Equals(TEXT("TitleMenu"), ESearchCase::IgnoreCase))
	{
		return;
	}

	if (GEngine && GEngine->GameViewport && !BlackScreenWidget)
	{
		UClass* WidgetClass = LoadingWidgetClassRef.Get();
		
		if (!WidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("로딩 위젯 클래스를 불러올 수 없음."));
			return;
		}
		BlackScreenWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		if (BlackScreenWidget)
		{
			GEngine->GameViewport->AddViewportWidgetContent(BlackScreenWidget->TakeWidget(), 1000);
		}
	}
}

void UODGameInstance::OnPostLoadMap(UWorld* LoadedWorld)
{
	if (IsRunningDedicatedServer())
		return;
	
	FString CurrentMapName = LoadedWorld ? LoadedWorld->GetMapName() : TEXT("");
	FString ShortName = FPackageName::GetShortName(CurrentMapName);

	if (ShortName.Equals(TEXT("TitleMenu"), ESearchCase::IgnoreCase))
	{
		return;
	}

	if (BlackScreenWidget)
	{
		FTimerHandle Timer;
		LoadedWorld->GetTimerManager().SetTimer(Timer, [this]()
		{
			if (GEngine && GEngine->GameViewport && BlackScreenWidget)
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(BlackScreenWidget->TakeWidget());
				BlackScreenWidget = nullptr;
			}
		}, 2.0f, false);

	
	}
}
