// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ODGameDataSubsystem.generated.h"

class UODBaseStatsPDA;
/**
 * 
 */
UCLASS()
class OD_API UODGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UODGameDataSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	
private:
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UODBaseStatsPDA>> BaseStatsMap;
	
};
