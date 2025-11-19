// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/ODAssetManager.h"

#include "Curves/CurveLinearColor.h"
#include "Debug/ODLogChannels.h"


UODAssetManager::UODAssetManager()
	: Super()
{
}

UODAssetManager& UODAssetManager::Get()
{
	if (UODAssetManager* AssetManager = Cast<UODAssetManager>(GEngine->AssetManager))
	{
		return *AssetManager;
	}
	return *NewObject<UODAssetManager>();
}


