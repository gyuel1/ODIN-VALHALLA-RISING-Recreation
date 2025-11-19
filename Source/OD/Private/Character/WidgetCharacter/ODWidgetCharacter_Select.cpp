// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WidgetCharacter/ODWidgetCharacter_Select.h"

#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ODCharacterSelectManager.h"


AODWidgetCharacter_Select::AODWidgetCharacter_Select()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetNotifyRigidBodyCollision(true);
	
}

// void AODWidgetCharacter_Select::OnCharacterClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
// {
// 	Debug::Print(TEXT("캐릭터 클릭"));
// 	
//
// 	
// }

void AODWidgetCharacter_Select::BeginPlay()
{
	Super::BeginPlay();

	
	if (GetMesh())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
		{
			GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
		}
		//GetMesh()->OnClicked.AddDynamic(this, &ThisClass::OnCharacterClicked);
	}
	
	CharacterSelectManager = Cast<AODCharacterSelectManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AODCharacterSelectManager::StaticClass()));
}
