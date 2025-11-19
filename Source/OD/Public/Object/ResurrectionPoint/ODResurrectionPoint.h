// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationCoreLibrary.h"
#include "GameplayTagContainer.h"
#include "Components/ArrowComponent.h"
#include "Object/ODObjectBase.h"
#include "ODResurrectionPoint.generated.h"


/**
 * 
 */
UCLASS()
class OD_API AODResurrectionPoint : public AODObjectBase
{
	GENERATED_BODY()

	
	FORCEINLINE const FGameplayTag& GetResurrectionPointTag() const {return ResurrectionPointTag;}
	FORCEINLINE const FTransform& GetResurrectionPoint() const {return ResurrectionPoint->GetComponentTransform();};
	

public:
	AODResurrectionPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> ResurrectionPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResurrectionPointInfo")
	FGameplayTag ResurrectionPointTag;

	
	
	
	
	
};
