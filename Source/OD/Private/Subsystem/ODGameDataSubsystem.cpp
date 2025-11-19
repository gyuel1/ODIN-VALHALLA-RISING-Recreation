// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ODGameDataSubsystem.h"
#include "Debug/ODLogChannels.h"
#include "PrimaryDataAsset/DT/ODBaseStatsPDA.h"
#include "Systems/ODAssetManager.h"


UODGameDataSubsystem::UODGameDataSubsystem()
{
}

void UODGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Server Only
	if (GetWorld() && GetWorld()->GetNetMode() != NM_Client)
	{
		BaseStatsMap = UODAssetManager::Get().LoadSyncPrimaryDataAsset<
			UODBaseStatsPDA>("DataTable");
	}
}

