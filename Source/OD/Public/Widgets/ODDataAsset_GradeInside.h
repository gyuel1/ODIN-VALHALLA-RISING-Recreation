// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ODDataAsset.h"
#include "ODDataAsset_GradeInside.generated.h"

enum class EGradeType : uint8;
class UPaperSprite;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FGradeTexture
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Grade Texture",meta = (AllowPrivateAccess = true))
	TMap<EGradeType, TSoftObjectPtr<UPaperSprite>> CircleSlotTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Grade Texture",meta = (AllowPrivateAccess = true))
	TMap<EGradeType, TSoftObjectPtr<UPaperSprite>> SquareSlotTextures;
};

UCLASS()
class OD_API UODDataAsset_GradeInside : public UODDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE const FGradeTexture& GetGradeTexture() const {return GradeTexture;}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grade Texture",  meta = (AllowPrivateAccess = true))
	FGradeTexture GradeTexture;
};
