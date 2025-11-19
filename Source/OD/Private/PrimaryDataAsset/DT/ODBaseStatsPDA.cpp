// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryDataAsset/DT/ODBaseStatsPDA.h"


FPrimaryAssetId UODBaseStatsPDA::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("DataTable",GetFName());
}

UDataTable* UODBaseStatsPDA::FindPlayerStatsDT(const FGameplayTag& InTag)
{
	if (BaseStatsMap.Contains(InTag))
	{
		return BaseStatsMap.Find(InTag)->Get();
	}
	return nullptr;
}
