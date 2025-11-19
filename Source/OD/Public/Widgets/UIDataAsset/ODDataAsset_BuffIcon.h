// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ODDataAsset.h"
#include "ODDataAsset_BuffIcon.generated.h"

/**
 * 
 */

enum class EBuffAttribute : uint8;
class UPaperSprite;

USTRUCT(BlueprintType)
struct FBuffInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Buff Texture",meta = (AllowPrivateAccess = true))
	TMap<EBuffAttribute, TSoftObjectPtr<UPaperSprite>> BuffSprite;
};

UCLASS()
class OD_API UODDataAsset_BuffIcon : public UODDataAsset
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const FBuffInfo& GetBuffInfo() const {return BuffInfo;}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff Texture",  meta = (AllowPrivateAccess = true))
	FBuffInfo BuffInfo;
	
	
};
