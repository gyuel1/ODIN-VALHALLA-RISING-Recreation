// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODObjectBase.generated.h"

UCLASS()
class OD_API AODObjectBase : public AActor , public IODRepGraphInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AODObjectBase();

	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

private:
	UPROPERTY()
	int32 RepGraphCount;
	


	
};
