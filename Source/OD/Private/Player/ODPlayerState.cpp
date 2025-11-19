// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ODPlayerState.h"

#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/ODPlayerAttributeSet.h"
#include "Character/ODPlayer.h"
#include "Info/ODPlayerInfo.h"
#include "Net/UnrealNetwork.h"
#include "Player/ODPlayerController.h"


AODPlayerState::AODPlayerState()
{

	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 66.f;
	
	ODAsc = CreateDefaultSubobject<UODAbilitySystemComponent>(TEXT("ODAsc"));
	ODAsc->SetIsReplicated(true);
	ODAsc->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ODAttributeSet = CreateDefaultSubobject<UODAttributeSet>(TEXT("ODAttributeSet"));
	ODPlayerAttributeSet = CreateDefaultSubobject<UODPlayerAttributeSet>(TEXT("ODPlayerAttributeSet"));
	
	ObjectInfo = CreateDefaultSubobject<UODPlayerInfo>(TEXT("ObjectInfo"));
}

void AODPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AODPlayerState, PlayerJob);
}

void AODPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

UODAbilitySystemComponent* AODPlayerState::GetAbilitySystemComponent() const
{
	return ODAsc;
}

UODAttributeSet* AODPlayerState::GetODAttributeSet() const
{
	return ODAttributeSet;
}

UODPlayerInfo* AODPlayerState::GetInfo() const
{
	return ObjectInfo;
}

// void AODPlayerState::OnRep_WidgetPlayerName()
// {
// 	if (AODPlayerController* PC = GetOwner<AODPlayerController>())
// 	{
// 		PC->Client_UpdateWidgetName(PlayerName);
//
// 		UE_LOG(LogTemp, Warning, TEXT("Client: PlayerName 복제 완료 -> 위젯 업데이트 요청"));
// 	}
// }

void AODPlayerState::SetPlayerName(const FString& NewName)
{
	Super::SetPlayerName(NewName);
	
	PlayerName = NewName;

	// 서버에서 Pawn에 이름 전달 ***
	if (GetLocalRole() == ROLE_Authority) // 서버에서만 실행
	{
		if (AODPlayerController* PC = GetOwner<AODPlayerController>())
		{
			if (AODPlayer* Player = Cast<AODPlayer>(PC->GetPawn()))
			{
				Player->SetPawnPlayerName(NewName); 
			}
		}
	}
}

void AODPlayerState::SetPlayerJob(EPlayerJob NewJob)
{
	if (HasAuthority())
	{
		PlayerJob = NewJob;
	}
}



