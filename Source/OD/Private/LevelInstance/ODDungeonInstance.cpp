// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelInstance/ODDungeonInstance.h"
#include "Manager/ODDungeonInstanceManager.h"
#include "Character/ODPlayer.h"
#include "Helper/ODStaticFunction.h"
#include "Kismet/GameplayStatics.h"
#include "Object/SpawnPoint/ODSpawnPoint.h"
#include "Party/ODPartyInfo.h"


AODDungeonInstance::AODDungeonInstance()
	:Super()
{
	bReplicates = true;
	bUseDungeon = false;

	//거리별 최적화
	bAlwaysRelevant = false;
	bOnlyRelevantToOwner = false;
	bNetUseOwnerRelevancy = false;
	NetCullDistanceSquared = FMath::Square(10000.f);
}

AODDungeonInstance::AODDungeonInstance(const int32 InRepGraphCount)
	:Super(InRepGraphCount)
{
	bReplicates = true;
	bUseDungeon = false;
}

ULevel* AODDungeonInstance::GetLoadedLevel() const
{
	return Super::GetLoadedLevel();
}

void AODDungeonInstance::InitInstanceInfo(AODDungeonInstanceManager* InManager, const FGameplayTag& InTag,
                                          int32 InRepGraph)
{
	if (InManager)
		Manager = InManager;
	
	if (InTag.IsValid())
		LevelInstanceTag = InTag;
	
	RepGraphCount = InRepGraph;

	//스폰 포인트 찾아오기
	InitSpawnPoint();
}

void AODDungeonInstance::InitSpawnPoint()
{
	if (!HasAuthority())
		return;
	
	UWorld* World = Manager->GetWorld();
	if (!World)
		return;

	World->GetTimerManager().SetTimer(
		SpawnPointTimerHandle,
		[this,World]()
		{
			ULevel* Level = GetLoadedLevel();
			if (Level == nullptr)
				return;

			if (Level->Actors.IsEmpty())
				return;

			for (AActor* LevelActor : Level->Actors)
			{
				if (!IsValid(LevelActor))
					continue;

				if (LevelActor->IsA(AODSpawnPoint::StaticClass()))
				{
					//내 채널로 옮기기
					if (AODSpawnPoint* CastSpawnPoint = Cast<AODSpawnPoint>(LevelActor))
						SpawnPoints.Add(CastSpawnPoint);

					if (SpawnPoints.Num() >= 4)
					{
						World->GetTimerManager().ClearTimer(SpawnPointTimerHandle);
					}
				}
			}
		},
		0.1f,
		true,
		0.f);
	
	
	
}

void AODDungeonInstance::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			TArray<AActor*> Out;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AODDungeonInstanceManager::StaticClass(), Out);

			if (!Out.IsEmpty())
			{
				if (AODDungeonInstanceManager* MakeManager = Cast<AODDungeonInstanceManager>(Out[0]))
				{
					MakeManager->AddDungeon(this);
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					
				}
			}
		},
		0.1f,
		true,
		0.5f
		);

	TArray<AActor*> Out;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AODDungeonInstanceManager::StaticClass(), Out);
	
	if (Out.IsEmpty())
		return;
	
	AODDungeonInstanceManager* MakeManager = Cast<AODDungeonInstanceManager>(Out[0]);
	if (!MakeManager)
		return;
	
	MakeManager->AddDungeon(this);
}

void AODDungeonInstance::SetManager(AODDungeonInstanceManager* InManager)
{
	if (!InManager)
		Manager = InManager;
}


void AODDungeonInstance::PartyInDungeon(UODPartyInfo* InParty)
{
	if (!InParty)
		return;

	Party = InParty;
	bUseDungeon = true;

	//파티 포인트 기억하기
	for (UODPartyMember* PartyInfo : InParty->GetPartyMembers())
	{
		if (IsValid(PartyInfo->GetMember()))
		{
			AODPlayer* MakePlayer = PartyInfo->GetMember();
			const FTransform Transform = MakePlayer->GetTransform();
			PartyEntryPoint.Add(FODPartyEntryPoint(MakePlayer,Transform));
		}
	}

	//파티 던전으로 보내기
	for (int i = 0; i < InParty->GetPartyMembers().Num(); i++)
	{
		AODPlayer* CurrentPlayer = InParty->GetPartyMembers()[i]->GetMember();
		if (!IsValid(CurrentPlayer))
			continue;
		
		if (SpawnPoints.IsValidIndex(i) && IsValid(SpawnPoints[i]))
		{
			// const int32 OldChannel = ODStaticFunc::OpenChannelPlayer(CurrentPlayer,
			//                                                          GetRepGraphCount());
			// if (OldChannel == -1)
			// 	continue;
			//
			// // FTimerHandle MakeTimerHandle;
			// // CurrentPlayer->GetWorld()->GetTimerManager().SetTimer(
			// // 	MakeTimerHandle,
			// // 	[this,i,OldChannel,CurrentPlayer]()
			// // 	{
			// // 		CurrentPlayer->SetActorTransform(SpawnPoints[i]->GetActorTransform());
			// // 		ODStaticFunc::CloseChannelPlayer(CurrentPlayer, OldChannel);
			// // 	},
			// // 	1.f,
			// // 	false
			// // 	, 1.f);
			
		}
	}
	//던전 초기화
	
}

void AODDungeonInstance::PartyInDungeon_NotRepGraph(UODPartyInfo* InParty)
{
	if (!InParty)
		return;

	Party = InParty;
	bUseDungeon = true;

	//파티 포인트 기억하기
	for (UODPartyMember* PartyInfo : InParty->GetPartyMembers())
	{
		if (IsValid(PartyInfo->GetMember()))
		{
			AODPlayer* MakePlayer = PartyInfo->GetMember();
			const FTransform Transform = MakePlayer->GetTransform();
			PartyEntryPoint.Add(FODPartyEntryPoint(MakePlayer,Transform));
		}
	}

	//파티 던전으로 보내기
	for (int i = 0; i < InParty->GetPartyMembers().Num(); i++)
	{
		AODPlayer* CurrentPlayer = InParty->GetPartyMembers()[i]->GetMember();
		if (!IsValid(CurrentPlayer))
			continue;
		
		if (SpawnPoints.IsValidIndex(i) && IsValid(SpawnPoints[i]))
		{
			
			CurrentPlayer->GetWorld()->GetTimerManager().SetTimer(
				InDungeonTimerHandle,
				[this,i,CurrentPlayer]()
				{
					CurrentPlayer->SetActorTransform(SpawnPoints[i]->GetActorTransform());
				},
				1.f,
				false
				, 1.f);
		}
	}
	//던전 초기화
}

void AODDungeonInstance::PartyOutDungeon()
{
	bUseDungeon = false;

	// //파티원들 원래 월드로 되돌려주기
	// for (FODPartyEntryPoint& EntryPoint : PartyEntryPoint)
	// {
	// 	ODStaticFunc::ChangeChannelPlayer(EntryPoint.Member.Get(),0);
	// 	EntryPoint.BackToSquareOne();
	// }
	// Party = FODPartyInfo();
	// PartyEntryPoint.Reset();
	
	//던전 종료
}

