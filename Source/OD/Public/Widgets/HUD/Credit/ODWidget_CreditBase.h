// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_CreditBase.generated.h"

/**
 * 
 */
class UTextBlock;
class UButton;

UCLASS()
class OD_API UODWidget_CreditBase : public UUserWidget
{
	GENERATED_BODY()
	

public:
	FORCEINLINE UTextBlock* GetCreditValue() {return CreditValueTextBlock;}
	
private:
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UTextBlock> CreditValueTextBlock;

	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UButton> CreditButton;
	
};
