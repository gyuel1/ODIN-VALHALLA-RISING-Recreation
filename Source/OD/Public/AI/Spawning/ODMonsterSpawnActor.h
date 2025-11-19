// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODMonsterSpawnActor.generated.h"

class AODDungeonSessionManager;
enum class EMonsterSpawnContext : uint8;
class AODMonsterBase;
class AODMonsterGrunt;

UENUM()
enum class ESpawnerUsage
{
	Field	UMETA(DisplayName = "Field"),
	Dungeon UMETA(DisplayName = "Dungeon"),
};

USTRUCT(BlueprintType)
struct FFieldSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AODMonsterBase> MonsterClass;

	UPROPERTY(EditAnywhere)
	int32 Count = 0;
};

USTRUCT(BlueprintType)
struct FDungeonSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AODMonsterBase> MonsterClass;

	UPROPERTY(EditAnywhere)
	FVector DungeonSpawnLocation;

	UPROPERTY(EditAnywhere)
	FRotator DungeonSpawnRotation;

	UPROPERTY(EditAnywhere)
	int32 KeyCount = 0;
};

USTRUCT(BlueprintType)
struct FDoorInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor>	DoorClass;

	UPROPERTY(EditAnywhere)
	FTransform DoorSpawnTransform;

	UPROPERTY()
	int32 DestroyDoorCount = 0;
};

UCLASS()
class OD_API AODMonsterSpawnActor : public AActor , public IODRepGraphInterface
{
	GENERATED_BODY()
	
public:	
	AODMonsterSpawnActor();

	UFUNCTION()
	void BeginMonsterSpawn(const FVector& Origin, float Radius);
	void BeginMonsterSpawn(AODDungeonSessionManager* DungeonSessionManager);

	//IODRepGraphInterface
	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

private:
	UPROPERTY()
	int32  RepGraphCount;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Value")
	ESpawnerUsage SpawnerUsage = ESpawnerUsage::Field;
	
	UPROPERTY(EditDefaultsOnly, Category = "Value")
	float SpawnRadius;

	UPROPERTY()
	FVector SpawnLocation = FVector::Zero();
	
#pragma region Field Spawning
public:
	UFUNCTION()
	void Respawn(TSubclassOf<AODMonsterBase> MonsterClass);

	UFUNCTION()
	void RequestRespawn(TSubclassOf<AODMonsterBase> MonsterClass, EMonsterSpawnContext SpawnContextType);
	
private:
	UPROPERTY(EditAnywhere, Category = "Value")
	TArray<FFieldSpawnEntry> FieldSpawnEntries;
	
	UFUNCTION()
	FVector CalSpawnLocation(const FVector& Origin, float Radius);

	UFUNCTION()
	bool IsLocationFree(const FVector& Pos, float Radius);
#pragma endregion

#pragma region Dungeon Spawning
public:
	UFUNCTION()
	void SpawnByTrigger();	
private:
	UPROPERTY(EditAnywhere, Category = "Value")
	TArray<FDungeonSpawnEntry> DungeonSpawnEntries;
	
	UPROPERTY(EditAnywhere, Category = "Value")
	TArray<FDungeonSpawnEntry> TriggerSpawnEntries;

#pragma endregion

#pragma region Door Spawn And Destory
private:
	UPROPERTY(EditDefaultsOnly, Category = "Value")
	FDoorInfo DoorInfo;

	UPROPERTY()
	AActor* CachedDoorActor;

	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, int32> KeyMonster;
public:
	UFUNCTION()
	void OnKeyMonsterDestroyed(AActor* DestroyedActor);
	
	UFUNCTION()
	void SpawnDoorIfNeeded();
#pragma endregion
	
#pragma region Common

#pragma endregion
	
};
