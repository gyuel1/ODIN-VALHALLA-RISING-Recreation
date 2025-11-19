// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryDataAsset/DT/ODPlayerBaseStatsPDA.h"


FPrimaryAssetId UODPlayerBaseStatsPDA::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("DataTable",GetFName());
}
