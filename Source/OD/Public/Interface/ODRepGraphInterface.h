// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ODRepGraphInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UODRepGraphInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OD_API IODRepGraphInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetRepGraphCount() = 0;
	virtual void SetRepGraphCount(const int32 InRepGraph) = 0;
	
};
