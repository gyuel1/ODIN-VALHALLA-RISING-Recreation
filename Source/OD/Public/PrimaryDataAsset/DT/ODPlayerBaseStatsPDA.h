// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryDataAsset/ODPrimaryDataAsset.h"
#include "ODPlayerBaseStatsPDA.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class OD_API UODPlayerBaseStatsPDA : public UODPrimaryDataAsset
{
	GENERATED_BODY()

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable",
		meta = (AllowPrivateAccess = true, Categories="OD.BaseStats"))
	TMap<FGameplayTag, TSoftObjectPtr<UDataTable>> DTBaseStatsMap;
};
