// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ODWidget_Activatable.h"
#include "ODWidget_Credits.generated.h"

/**
 * 
 */
class UODWidget_CreditBase;
UCLASS()
class OD_API UODWidget_Credits : public UODWidget_Activatable
{
	GENERATED_BODY()

public:
	FORCEINLINE UODWidget_CreditBase* GetCreditGold() {return CreditGold;}
	FORCEINLINE UODWidget_CreditBase* GetCreditDiamond() {return CreditDiamond;} 

private:
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UODWidget_CreditBase> CreditGold;

	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UODWidget_CreditBase> CreditDiamond;
	
};
