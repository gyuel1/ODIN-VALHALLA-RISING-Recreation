// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODDungeonSessionManager.generated.h"

class AODMonsterBase;
class AODMonsterSpawnActor;
class AODSpawnPoint;

UCLASS()
class OD_API AODDungeonSessionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AODDungeonSessionManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="PlayerSpawnPointActor")
	TArray<TObjectPtr<AODSpawnPoint>> PlayerSpawnPoints;
	
#pragma region Dungeon Init
public:
	UFUNCTION()
	void DungeonInit(bool NeedInit);
private:
	UPROPERTY(EditAnywhere, Category="Spawners")
	TArray<TObjectPtr<AODMonsterSpawnActor>> SpawnActors;
#pragma endregion

#pragma region Dungeon Release
public:
	UFUNCTION()
	void DungeonRelease();

	UFUNCTION()
	void RegisterMonster(AODMonsterBase* Monster);

	UFUNCTION()
	void OnMonsterDestroyed(AActor* Dead);
	
private:
	UPROPERTY()
	TSet<TWeakObjectPtr<AODMonsterBase>> Monsters;

	UPROPERTY()
	int32 DungeonBossCount = 0;
#pragma endregion

#pragma region Dungeon Clear
public:
	UFUNCTION()
	void DungeonClear();

private:
	UPROPERTY()
	bool bDungeonClearTriggered = false;
#pragma endregion

#pragma region Getter
public:
	TArray<TObjectPtr<AODSpawnPoint>> GetPlayerSpawnPoints();
#pragma endregion
	
};
