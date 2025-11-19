// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Debug/ODLogChannels.h"
#include "ODAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UODAssetManager();
	
	static UODAssetManager& Get();

	

	//PrimaryDataAssetType
	template <typename T>
	TArray<T*> LoadSyncPrimaryDataAsset(const FName& InAssetType);
	
};

template <typename T>
TArray<T*> UODAssetManager::LoadSyncPrimaryDataAsset(const FName& InAssetType)
{
	TArray<FPrimaryAssetId> AssetIdList;
	FPrimaryAssetType PrimaryAssetType = InAssetType;
	Get().GetPrimaryAssetIdList(PrimaryAssetType, AssetIdList);

	TArray<T*> AssetArray;
	for (const FPrimaryAssetId& MakeList : AssetIdList)
	{
		FSoftObjectPath Path = Get().GetPrimaryAssetPath(MakeList);
		if (UObject* LoadObject = Get().StreamableManager.LoadSynchronous(Path))
		{
			if (UClass* Class = Cast<UClass>(LoadObject))
			{
				if (T* PDA = Cast<T>(Class->GetDefaultObject()))
				{
					AssetArray.Add(PDA);
					
				}
			}
		}
	}
	return AssetArray;
}

