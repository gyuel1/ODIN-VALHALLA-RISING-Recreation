// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_HudRightDefaultStack.generated.h"

class UODWidget_Hud_UserInput;

/**
 * 
 */
UCLASS()
class OD_API UODWidget_HudRightDefaultStack : public UODWidget_Activatable
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UODWidget_Hud_UserInput> GetHudUserInput() { return HudUserInput; }
	
	
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UODWidget_Hud_UserInput> HudUserInput;
	
};
