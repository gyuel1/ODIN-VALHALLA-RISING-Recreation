// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ODGameMode.h"
#include "Enum/ODEnumCharacterJob.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ODPlayerState.h"
#include "ReplicationGraph/ODReplicationGraph.h"


void AODGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (UNetDriver* Driver = GetNetDriver())
	{
		if (UReplicationDriver* RepDriver = Driver->GetReplicationDriver())
		{
			if (UODReplicationGraph* RepGraph = Cast<UODReplicationGraph>(RepDriver))
			{
				RepGraph->CreatePlayerNode(NewPlayer);
			}
		}
	}
	
}

void AODGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (UNetDriver* Driver = GetNetDriver())
	{
		if (UReplicationDriver* RepDriver = Driver->GetReplicationDriver())
		{
			if (UODReplicationGraph* RepGraph = Cast<UODReplicationGraph>(RepDriver))
			{
				if (APlayerController* PC = Cast<APlayerController>(Exiting))
					RepGraph->RemovePlayerNode(PC);
			}
		}
	}
}

void AODGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	FString JobValueString = UGameplayStatics::ParseOption(Options, TEXT("Job"));
    
	if (!JobValueString.IsEmpty())
	{
		int32 JobCode = FCString::Atoi(*JobValueString);
       
		if (UniqueId.IsValid())
		{
			PlayerJobOptions.Add(UniqueId, JobCode);
			FString UniqueIdString = TEXT("Invalid ID");
			
			if (UniqueId.GetUniqueNetId().IsValid())
			{
				UniqueIdString = UniqueId.GetUniqueNetId()->ToString(); 
			}
          
			UE_LOG(LogTemp, Warning, TEXT("PreLogin: Job %d saved for ID: %s"), JobCode, *UniqueIdString);
		}
	}
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AODGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	FUniqueNetIdRepl UniqueId = NewPlayer->PlayerState ? NewPlayer->PlayerState->GetUniqueId() : FUniqueNetIdRepl();
    
	if (UniqueId.IsValid())
	{
		if (const int32* JobCodePtr = PlayerJobOptions.Find(UniqueId))
		{
			EPlayerJob SelectedJob = (EPlayerJob)(*JobCodePtr);
          
			// 직업에 따라 DefaultPawnClass 변경
			DefaultPawnClass = GetPawnClassByJob(SelectedJob);

			AODPlayerState* PS = Cast<AODPlayerState>(NewPlayer->PlayerState);
			if (PS)
			{
				PS->SetPlayerJob(SelectedJob); // 서버에서 PlayerState의 직업 설정
			}

			// 사용한 직업 정보는 제거
			PlayerJobOptions.Remove(UniqueId);
          
			UE_LOG(LogTemp, Error, TEXT("플레이어 직업 (PreLogin TMap 조회) : %s"), *DefaultPawnClass->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Job option not found in TMap for ID. Using default pawn from constructor."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player UniqueId is invalid. Using default pawn from constructor."));
	}
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

UClass* AODGameMode::GetPawnClassByJob(EPlayerJob Job)
{
	switch (Job)
	{
	case EPlayerJob::ShieldMaiden:
		return PlayerSoftClasses[0].LoadSynchronous();
	case EPlayerJob::Rogue:
		return PlayerSoftClasses[1].LoadSynchronous();
	case EPlayerJob::Sorceress:
		return PlayerSoftClasses[2].LoadSynchronous();
	default:
		return DefaultPawnClass;
	}
}
