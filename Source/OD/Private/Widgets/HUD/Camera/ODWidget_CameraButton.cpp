// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/Camera/ODWidget_CameraButton.h"

#include "Character/ODPlayer.h"
#include "Components/ODCameraControllerComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UODWidget_CameraButton::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this,0);
	PlayerCharacter = Cast<AODPlayer>(Character);
}

void UODWidget_CameraButton::NativeOnClicked()
 {
 	Super::NativeOnClicked();

 	int32 NextMode = (static_cast<int32>(CurrentMode) + 1) % MaxIndex;
 	CurrentMode = static_cast<ECameraMode>(NextMode);

 	UE_LOG(LogTemp, Error, TEXT("Camera Mode : %d"),NextMode);


 	switch (CurrentMode)
 	{
 	case ECameraMode::NormalView:
 		ButtonText->SetText(FText::FromString("N"));
 		PlayerCharacter->CameraControllerComponent->NextPreset();
 		break;
 	case ECameraMode::QuarterView:
 		ButtonText->SetText(FText::FromString("Q"));
 		PlayerCharacter->CameraControllerComponent->NextPreset();
 		break;
 	case ECameraMode::DefaultView:
 		ButtonText->SetText(FText::FromString("D"));
 		PlayerCharacter->CameraControllerComponent->NextPreset();
 		break;
 	}
 }
