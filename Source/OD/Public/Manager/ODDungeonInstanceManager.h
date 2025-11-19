// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "LevelInstance/ODDungeonInstance.h"
#include "ODDungeonInstanceManager.generated.h"



USTRUCT(BlueprintType)
struct FDungeonInstanceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DungeonInstanceInfo")
	TSubclassOf<AODDungeonInstance> DungeonInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DungeonInstanceInfo")
	FTransform DungeonTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DungeonInstanceInfo")
	FGameplayTag DungeonTag;
	
	FORCEINLINE bool IsValid() const
	{
		return DungeonInstanceClass.Get() && DungeonTag.IsValid();
	}
};

UCLASS()
class OD_API AODDungeonInstanceManager : public AActor, public IODRepGraphInterface
{
	GENERATED_BODY()

public:
	AODDungeonInstanceManager();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

	void AddDungeon(AODDungeonInstance* InDungeonInstance);

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ConnectPartyToDunGeon_Server(UODPartyInfo* InParty,AODPlayer* InPlayer);
	UFUNCTION(BlueprintCallable)
	void ConnectPartyToDunGeon(UODPartyInfo* InParty,AODPlayer* InPlayer);

	FORCEINLINE TArray<FDungeonInstanceInfo>& GetDungeonInstances() {return DungeonInstanceInfos;}



private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DungeonInstanceInfo",meta = (AllowPrivateAccess = true,Categories="OD.Level"))
	TArray<FDungeonInstanceInfo> DungeonInstanceInfos;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="DungeonInstanceInfo",
		meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<AODDungeonInstance>> DungeonInstances;
};
