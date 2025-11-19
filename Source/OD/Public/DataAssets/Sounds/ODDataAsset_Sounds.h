// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ODDataAsset.h"
#include "ODDataAsset_Sounds.generated.h"

/**
 * 
 */

enum class EBGMType : uint8;

UCLASS()
class OD_API UODDataAsset_Sounds : public UODDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	USoundBase* GetBGM(EBGMType BGMType);
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM", meta = (AllowPrivateAccess = true))
	TMap<EBGMType,TObjectPtr<USoundBase>> BGMs;
	
	
};
