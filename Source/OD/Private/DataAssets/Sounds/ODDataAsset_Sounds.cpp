// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Sounds/ODDataAsset_Sounds.h"


USoundBase* UODDataAsset_Sounds::GetBGM(EBGMType BGMType)
{
	if (USoundBase* FoundSound = BGMs.FindRef(BGMType))
	{
		return FoundSound;
	}

	UE_LOG(LogTemp, Error, TEXT("UODDataAsset_Sounds : GetBGM Is Null"));
	return nullptr;
}
