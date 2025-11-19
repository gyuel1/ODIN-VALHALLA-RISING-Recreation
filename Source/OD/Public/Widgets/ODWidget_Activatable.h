// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ODWidget_Activatable.generated.h"

class AODPlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OD_API UODWidget_Activatable : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure)
	AODPlayerController* GetOwningPlayerController();
	

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AODPlayerController> CachedOwningPC;
	
	
};
