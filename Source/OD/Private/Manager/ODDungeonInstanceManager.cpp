// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ODDungeonInstanceManager.h"

#include "Debug/ODLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AODDungeonInstanceManager::AODDungeonInstanceManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AODDungeonInstanceManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AODDungeonInstanceManager, DungeonInstances);
}

int32 AODDungeonInstanceManager::GetRepGraphCount()
{
	return 0;
}

void AODDungeonInstanceManager::SetRepGraphCount(const int32 InRepGraph)
{
	return;
}

void AODDungeonInstanceManager::AddDungeon(AODDungeonInstance* InDungeonInstance)
{
	if (!InDungeonInstance)
		DungeonInstances.Add(InDungeonInstance);
}


void AODDungeonInstanceManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (!DungeonInstanceInfos.IsEmpty())
	{
		int RepGraphCount = 1;
		for (FDungeonInstanceInfo& Info : DungeonInstanceInfos)
		{
			if (!Info.IsValid())
				continue;

			AODDungeonInstance* Inst = GetWorld()->SpawnActorDeferred<AODDungeonInstance>(
				Info.DungeonInstanceClass, Info.DungeonTransform, this);
			if (Inst)
			{
				Inst->InitInstanceInfo(this, Info.DungeonTag, RepGraphCount);
				Inst->SetReplicates(true);
				DungeonInstances.Add(Inst);

				UGameplayStatics::FinishSpawningActor(Inst, Info.DungeonTransform);
				RepGraphCount++;
			}
		}
	}
	
}

void AODDungeonInstanceManager::ConnectPartyToDunGeon(UODPartyInfo* InParty, AODPlayer* InPlayer)
{
	if (!IsValid(InPlayer) || !InPlayer->GetODPartyComponent()->IsPartyLeader())
		return;
	
	//연결 가능한 던전찾기
	if (DungeonInstances.IsEmpty())
	{
		UE_LOG(LOGODDungeon,Warning,TEXT("Dungeon Empty"));	
		return;
	}
	AODDungeonInstance* MakeDungeon = nullptr;
	for (AODDungeonInstance* Dungeon :DungeonInstances)
	{
		if (!Dungeon)
			continue;

		if (Dungeon->IsOccupied())
			continue;
			
		MakeDungeon = Dungeon;
		break;
	}
	if (!MakeDungeon)
		return;

	//찾은 던전에 파티 연결해주기
	//MakeDungeon->PartyInDungeon(InParty);
	MakeDungeon->PartyInDungeon_NotRepGraph(InParty);
}

void AODDungeonInstanceManager::ConnectPartyToDunGeon_Server_Implementation(UODPartyInfo* InParty, AODPlayer* InPlayer)
{
	ConnectPartyToDunGeon(InParty,InPlayer);
}



