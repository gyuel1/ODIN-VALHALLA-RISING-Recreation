// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_DamageQueContainer.generated.h"

class UODWidget_DamageText;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_DamageQueContainer : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void AddDamage(int32 DamageValue = 0, bool IsCritical = false);

protected:
	void StartOrResetClearTimer();
	UFUNCTION()
	void PlayFadeOutAndRemove();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> DamageBox;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UODWidget_DamageText> DamageTextClass;

	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 MaxCount = 5;

	UPROPERTY(EditAnywhere, Category = "Color")
	UMaterialInstance* OrangeMaterial;

	UPROPERTY(EditAnywhere, Category = "Color")
	UMaterialInstance* CreamMaterial;

	UPROPERTY(EditAnywhere, Category = "Color")
	UMaterialInstance* RedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float MaxLiftTime = 5.0f;
	
	FTimerHandle DamageRemoveTimerHandle;
	
};
