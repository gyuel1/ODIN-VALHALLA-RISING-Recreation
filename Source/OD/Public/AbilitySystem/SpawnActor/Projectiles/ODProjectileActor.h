// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SpawnActor/ODAbilitySpawnActorBase.h"
#include "ODProjectileActor.generated.h"

/**
 * 
 */
UCLASS()
class OD_API AODProjectileActor : public AODAbilitySpawnActorBase
{
	GENERATED_BODY()

public:
	AODProjectileActor();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:

	float ProjectileSpeed;

	UFUNCTION(NetMulticast,reliable)
	void SetActorLocation_NetMulticast(const FVector InLocation);
};
