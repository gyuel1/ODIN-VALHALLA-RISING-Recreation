// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ODPartyComponent.h"
#include "LevelInstance/ODLevelInstanceBase.h"
#include "ODDungeonInstance.generated.h"

class AODSpawnPoint;
class AODDungeonInstanceManager;

//파티멤버와 파티멤버가 진입했던 기존월드의 위치를 저장
USTRUCT()
struct FODPartyEntryPoint
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AODPlayer> Member;

	UPROPERTY()
	FTransform Transform;

	FODPartyEntryPoint()
	{
		Member = nullptr;
		Transform = FTransform();
	}
	FODPartyEntryPoint(AODPlayer* InMember, const FTransform& InTransform)
	{
		Member = InMember;
		Transform = InTransform;
	}

	void BackToSquareOne()
	{
		if (Member.IsValid())
			Member->SetActorTransform(Transform);
	}
};

UCLASS()
class OD_API AODDungeonInstance : public AODLevelInstanceBase
{
	GENERATED_BODY()
	
public:
	AODDungeonInstance();
	AODDungeonInstance(const int32 InRepGraphCount);

	virtual ULevel* GetLoadedLevel() const override;
	
	virtual void InitInstanceInfo(AODDungeonInstanceManager* InManager ,const FGameplayTag& InTag, int32 InRepGraph);
	virtual void InitSpawnPoint();

	virtual void BeginPlay() override;

	void SetManager(AODDungeonInstanceManager* InManager);

	
	

	//파티 진입
	void PartyInDungeon(UODPartyInfo* InParty);

	void PartyInDungeon_NotRepGraph(UODPartyInfo* InParty);

	void PartyOutDungeon();
	//=================== Get Set =========================//

	FORCEINLINE bool IsOccupied() const { return bUseDungeon; };
	
private:

	FTimerHandle TimerHandle;

	FTimerHandle InDungeonTimerHandle;

	FTimerHandle SpawnPointTimerHandle;

	UPROPERTY()
	TObjectPtr<AODDungeonInstanceManager> Manager;

	UPROPERTY()
	FGameplayTag LevelInstanceTag;
	
	UPROPERTY()
	TObjectPtr<UODPartyInfo> Party;

	UPROPERTY()
	TArray<FODPartyEntryPoint> PartyEntryPoint;

	UPROPERTY()
	TArray<AODSpawnPoint*> SpawnPoints;
	
	UPROPERTY()
	bool bUseDungeon;
	
};


