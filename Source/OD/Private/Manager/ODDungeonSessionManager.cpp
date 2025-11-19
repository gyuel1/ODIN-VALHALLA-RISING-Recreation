// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ODDungeonSessionManager.h"

#include "AI/Spawning/ODMonsterSpawnActor.h"
#include "Character/ODMonsterBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AODDungeonSessionManager::AODDungeonSessionManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AODDungeonSessionManager::BeginPlay()
{
	Super::BeginPlay();

	//Init Test
	DungeonInit(true);

	// Release Test
	// FTimerHandle TimerHandle;
	// GetWorldTimerManager().SetTimer(
	// 	TimerHandle, this, &AODDungeonSessionManager::DungeonRelease, 100.f, false
	// );
}

void AODDungeonSessionManager::DungeonInit(bool NeedInit)
{
	if (!HasAuthority()) return;
	if (!NeedInit) return;

	DungeonRelease();

	bDungeonClearTriggered = false;
	
	// 스포너에게 던전스폰 요청
	for (auto Spawner : SpawnActors)
	{
		if (!Spawner)
		{
			continue;
		}

		Spawner->BeginMonsterSpawn(this);
	}
}

void AODDungeonSessionManager::DungeonRelease()
{
	if (!HasAuthority()) return;
	if (Monsters.IsEmpty()) return;

	TArray<TWeakObjectPtr<AODMonsterBase>> Snapshot;
	Snapshot.Reserve(Monsters.Num());
	
	for (const auto& Monster : Monsters)
	{
		Snapshot.Add(Monster);
	}

	for (const auto& Monster : Snapshot)
	{
		if (Monster.IsValid() && !Monster->IsPendingKillPending())
		{
			Monster->Destroy();
		}
	}
	
	Monsters.Empty();
}

void AODDungeonSessionManager::RegisterMonster(AODMonsterBase* Monster)
{
	if (!ensure(Monster))
	{
		return;
	}

	Monsters.Add(Monster);
	Monster->OnDestroyed.AddUniqueDynamic(this, &AODDungeonSessionManager::OnMonsterDestroyed);
	if (Monster->GetEMonsterRank() == EMonsterRank::DungeonBoss)
	{
		++DungeonBossCount;
	}
}

void AODDungeonSessionManager::OnMonsterDestroyed(AActor* Dead)
{
	if (!HasAuthority() || !Dead)
	{
		return;
	}
	
	if (AODMonsterBase* Monster = Cast<AODMonsterBase>(Dead))
	{
		Monsters.Remove(Monster);
		if (Monster->GetEMonsterRank() == EMonsterRank::DungeonBoss)
		{
			DungeonBossCount = FMath::Max(0, DungeonBossCount - 1);
			if (DungeonBossCount == 0 && !bDungeonClearTriggered)
			{
				bDungeonClearTriggered = true;
				DungeonClear();
			}
		}
	}
}

void AODDungeonSessionManager::DungeonClear()
{
	DungeonRelease();
}

#pragma region Getter
TArray<TObjectPtr<AODSpawnPoint>> AODDungeonSessionManager::GetPlayerSpawnPoints()
{
	return PlayerSpawnPoints;
}
#pragma endregion


