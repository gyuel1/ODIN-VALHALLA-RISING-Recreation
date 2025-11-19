// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ODCharacterSelectManager.h"


#include "AsyncActions/ODAsyncAction_PushSoftWidget.h"
#include "Character/WidgetCharacter/ODWidgetCharacter_Select.h"
#include "FunctionLibrary/ODFrontendFunctionLibrary.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/ODUISubsystem.h"
#include "Widgets/ODWidget_Activatable.h"
#include "Widgets/ODWidget_PrimaryLayout.h"


// Sets default values
AODCharacterSelectManager::AODCharacterSelectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AODCharacterSelectManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> TempArray;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AODWidgetCharacter_Select::StaticClass(), TempArray);

	for (auto& Character : TempArray)
	{
		AODWidgetCharacter_Select* SelectCharacter = Cast<AODWidgetCharacter_Select>(Character);
		SelectCharacters.Emplace(SelectCharacter);
		//SelectCharacter->GetMesh()->OnClicked.AddDynamic(this, &ThisClass::OnCharacterClicked);
	}


	// FTimerHandle Handle;
	// GetWorld()->GetTimerManager().SetTimer(Handle, this, &ThisClass::DefaultCam, 0.0f, false); 
	
	
}

void AODCharacterSelectManager::OnCharacterClicked(AODWidgetCharacter_Select* Player)
{
	
	if (Player)
	{
		for (auto& Character : SelectCharacters)
		{
			if (Player->GetPlayerJob() == Character->GetPlayerJob())
			{
				
				Character->SetIsSelected(true);
				CameraAction(Character);
			}
			else
			{
				Character->SetIsSelected(false);
			}
		}
	}
}

void AODCharacterSelectManager::CameraAction(AODWidgetCharacter_Select* SelectCharacter)
{

	switch (SelectCharacter->GetPlayerJob())
	{
	case EPlayerJob::Rogue:
		CameraBlend("RogueCam");
		break;
	case EPlayerJob::ShieldMaiden:
		CameraBlend("ShieldMaidenCam");
		break;
	case EPlayerJob::Sorceress:
		CameraBlend("SorceressCam");
		break;
	default:
		Debug::Print(TEXT("유효하지 않은 카메라종류입니다!"));
		break;
	}
}

void AODCharacterSelectManager::CameraBlend(const FName BlendCam) const
{
	if (!CamerasMap.FindRef(BlendCam))
	{
		Debug::Print(TEXT("위젯맵 찾지못함."));
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (AActor* DefaultCam = CamerasMap.FindRef("DefaultCam"))
	{
		PC->SetViewTarget(DefaultCam);

		if (AActor* BlendCamera = CamerasMap.FindRef(BlendCam))
		{
			UGameplayStatics::PlaySound2D(this,ClickSound);
			PC->SetViewTargetWithBlend(BlendCamera, BlendTime);
		}
	}
	
}

void AODCharacterSelectManager::DefaultCam()
{
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AActor* DefaultCam = CamerasMap.FindRef("DefaultCam"))
		{
			PC->SetViewTargetWithBlend(DefaultCam, BlendTime);
		}
	}
}

void AODCharacterSelectManager::SetAllNotSelected()
{
	for (auto& Character : SelectCharacters)
	{
		Character->SetIsSelected(false);
	}
}

void AODCharacterSelectManager::InitCam(APlayerController* Controller)
{
	if (Controller)
	{
		if (AActor* DefaultCam = CamerasMap.FindRef("DefaultCam"))
		{
			Controller->SetViewTarget(DefaultCam);
		}
	}
	else
	{
		Debug::Print(TEXT("Controller Null"));
	}
}

// void AODCharacterSelectManager::CreatedPrimaryWidget()
// {
// 	if (!UGameplayStatics::GetNumLocalPlayerControllers(this))
// 	{
// 		return;
// 	}
//
// 	UODWidget_PrimaryLayout* PrimaryWidgetLayout = CreateWidget<UODWidget_PrimaryLayout>(this, ODPrimaryLayoutWidgetClass);
//
// 	checkf(PrimaryWidgetLayout, TEXT("Primary Widget is Null"))
// 	
// 	PrimaryWidgetLayout->AddToViewport();
//
// 	UODUISubsystem* ODUISubSystem = UODUISubsystem::Get(this);
//
// 	// PrimaryLayOut 저장
// 	ODUISubSystem->RegisterCreatedPrimaryLayoutWidget(PrimaryWidgetLayout);
// }




