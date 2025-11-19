// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ODPlayerController.h"

#include "AsyncActions/ODAsyncAction_PushSoftWidget.h"
#include "Blueprint/UserWidget.h"
#include "Character/ODPlayer.h"
#include "Components/WidgetComponent.h"
#include "Enum/ODEnumCharacterJob.h"
#include "FunctionLibrary/ODFrontendFunctionLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Helper/ODDebugHelper.h"
#include "Player/ODPlayerState.h"
#include "Subsystem/ODUISubsystem.h"
#include "Tags/ODUITag.h"
#include "Widgets/ODWidget_PrimaryLayout.h"
#include "Widgets/NoneStackWidget/ODWidget_OverHeadPlayer.h"

void AODPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(Mode);

	CreatedRegisterPrimaryWidget();
	
	if (IsLocalController()) 
	{
		if (UODUISubsystem* Sub = UODUISubsystem::Get(this))
		{
			FString Name = Sub->GetWidgetPlayerName();
			
			ServerSetPlayerName(Name);
			
		}
	}
	
}

void AODPlayerController::ServerSetPlayerName_Implementation(const FString& PlayerName)
{
	if (AODPlayerState* PS = GetPlayerState<AODPlayerState>())
	{
		PS->SetPlayerName(PlayerName);
	}
}


void AODPlayerController::ServerRequestSpawnByJob_Implementation(EPlayerJob Job)
{
	TrySpawnBasedOnJob(Job);
}

void AODPlayerController::TrySpawnBasedOnJob(EPlayerJob Job)
{
	UWorld* World = GetWorld();
	
	if (!World)
	{
		Debug::Print(TEXT("월드 없음"));
		return;
	}

	Debug::Print(TEXT("월드 있음"));
	AActor* FoundStartSpot = nullptr;

	if (AGameModeBase* GM = World->GetAuthGameMode())
	{
		FoundStartSpot = GM->FindPlayerStart(this);
	}

	FVector LocationToSpawn = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	if (FoundStartSpot)
	{
		LocationToSpawn = FoundStartSpot->GetActorLocation();
		SpawnRotation = FoundStartSpot->GetActorRotation();
	}
	
	TSubclassOf<APawn> PawnClass = nullptr;

	
	UClass* LoadedClass = nullptr;
	
		switch (Job)
		{
		case EPlayerJob::Sorceress:
			LoadedClass = Player_Sorceress.LoadSynchronous();
			Debug::Print(TEXT("소서리스"));
			PawnClass = LoadedClass;
			break;
		case EPlayerJob::Rogue:
			LoadedClass = Player_Rogue.LoadSynchronous();
			Debug::Print(TEXT("로그"));
			PawnClass = LoadedClass;
			break;
		case EPlayerJob::ShieldMaiden:
			LoadedClass = Player_ShieldMaiden.LoadSynchronous();
			Debug::Print(TEXT("실드메이든"));
			PawnClass = LoadedClass;
			break;
		default:
			Debug::Print(TEXT("직업 클래스 경로 없음"));
			return;
		}
	
	UE_LOG(LogTemp, Error, TEXT("Loaded Class: %s"), *GetNameSafe(LoadedClass));

		if (PawnClass)
		{
			if (APawn* ExistingPawn = GetPawn())
			{
				Debug::Print(TEXT("기존폰 삭제"));
				ExistingPawn->Destroy();
			}
		}
		else
		{
			Debug::Print(TEXT("Pawn스폰실패"));
		}

		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = this->GetPawnOrSpectator();
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, LocationToSpawn, SpawnRotation, Params);
		{
		Debug::Print(TEXT("Pawn빙의"));
			Possess(NewPawn);
		}
	
}

void AODPlayerController::Client_UpdateWidgetName(const FString& PlayerName)
{
	// 1. Pawn 유효성 검사
	AODPlayer* MyPlayer = Cast<AODPlayer>(GetPawn());
	if (!MyPlayer)
	{
		// 이 로그가 AcknowledgePossession에서도 찍힌다면, 캐스팅 타입이 잘못되었거나 (AODPlayer가 아닌 AODCharacter 등)
		// AcknowledgePossession의 P 변수를 사용해야 할 수 있습니다.
		UE_LOG(LogTemp, Error, TEXT("UpdateWidgetName 실패: GetPawn()이 유효하지 않거나 AODPlayer 캐스팅 실패."));
		return;
	}

	// 2. Widget Component 유효성 검사
	UWidgetComponent* WidgetComp = MyPlayer->GetOverHeadWidgetComponent();
	if (!WidgetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateWidgetName 실패: OverHeadWidgetComponent를 찾을 수 없음 (Component가 Pawn에 없음)."));
		return;
	}
    
	// 3. User Widget Object 유효성 및 타입 검사
	UODWidget_OverHeadPlayer* OverHeadWidget = Cast<UODWidget_OverHeadPlayer>(WidgetComp->GetUserWidgetObject());
	if (!OverHeadWidget)
	{
		// WidgetComp에 설정된 User Widget Class가 없거나 UODWidget_OverHeadPlayer 타입이 아닐 경우
		UE_LOG(LogTemp, Error, TEXT("UpdateWidgetName 실패: UserWidgetObject가 유효하지 않거나 UODWidget_OverHeadPlayer 캐스팅 실패. (현재 객체 타입: %s)"), 
			WidgetComp->GetUserWidgetObject() ? *WidgetComp->GetUserWidgetObject()->GetClass()->GetName() : TEXT("None"));
		return;
	}

	// 4. 최종 성공 및 이름 설정
	OverHeadWidget->SetWidgetPlayerName(PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("UpdateWidgetName 성공: 위젯 이름 '%s'으로 설정 완료!"), *PlayerName);
}

void AODPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	// Pawn을 소유하는 시점은 캐릭터가 확실히 존재함을 보장합니다.
	if (AODPlayerState* PS = GetPlayerState<AODPlayerState>())
	{
		// PlayerState에서 현재 복제된 최신 이름을 가져옵니다.
		// 이 시점에 Client_UpdateWidgetName을 호출하여 위젯을 초기화합니다.
		Client_UpdateWidgetName(PS->GetPlayerName()); 
        
		UE_LOG(LogTemp, Warning, TEXT("PC: AcknowledgePossession에서 Overhead Widget 초기 업데이트 시도."));
	}
}



void AODPlayerController::CreatedRegisterPrimaryWidget()
{
	// 클라만 생성
	if (!IsLocalController())
		return;


	UODWidget_PrimaryLayout* PrimaryWidgetLayout = CreateWidget<UODWidget_PrimaryLayout>(this, ODPrimaryLayoutWidgetClass);
	
	PrimaryWidgetLayout->AddToViewport();

	UODUISubsystem* ODUISubSystem = UODUISubsystem::Get(this);

	// PrimaryLayOut 저장
	ODUISubSystem->RegisterCreatedPrimaryLayoutWidget(PrimaryWidgetLayout);
	

	// Hud Stack Push
	if (UODAsyncAction_PushSoftWidget* Action = UODAsyncAction_PushSoftWidget::PushSoftWidget(
		this,
		this,
		UODFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UITags::OD_Widget_LoadingScreen),
		UITags::OD_WidgetStack_Modal))
	{
		Action->Activate();
	}
	
}
