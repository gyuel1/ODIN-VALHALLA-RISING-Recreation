// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODSpawnTrigger.generated.h"

class AODMonsterSpawnActor;
class UBoxComponent;

UCLASS()
class OD_API AODSpawnTrigger : public AActor , public IODRepGraphInterface
{
	GENERATED_BODY()
	
public:	
	AODSpawnTrigger();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

private:
	UPROPERTY()
	int32 RepGraphCount;
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY()
	TObjectPtr<UBoxComponent> TriggerVolume;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(ReplicatedUsing=OnRep_Triggered)
	bool bTriggered = false;

	UFUNCTION()
	void OnRep_Triggered();
	
	UPROPERTY(EditAnywhere, Category = "SpawnActor")
	TObjectPtr<AODMonsterSpawnActor> MonsterSpawnActor;
};
