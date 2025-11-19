// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryDataAsset/ODPrimaryDataAsset.h"
#include "ODBaseStatsPDA.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODBaseStatsPDA : public UODPrimaryDataAsset
{
	GENERATED_BODY()
public:
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(blueprintCallable)
	UDataTable* FindPlayerStatsDT(const FGameplayTag& InTag);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable",
		meta = (AllowPrivateAccess = true, Categories="OD.BaseStats"))
	TMap<FGameplayTag, TObjectPtr<UDataTable>> BaseStatsMap;
	
};
