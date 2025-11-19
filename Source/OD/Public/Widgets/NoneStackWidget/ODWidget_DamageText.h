// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_DamageText.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_DamageText : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDamage(const FText Damage, UMaterialInstance* FontMaterial);
	void SetDamagePlayer(const FText Damage, UMaterialInstance* FontMaterial);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* CreatedWidgetAnim;

	UPROPERTY(meta= (BindWidgetAnim), Transient)
	UWidgetAnimation* FirstIndexDamageAnim;

	UPROPERTY(meta= (BindWidgetAnim), Transient)
	UWidgetAnimation* NextIndexDamageAnim;

	UPROPERTY(meta= (BindWidgetAnim), Transient)
	UWidgetAnimation* TextNormalEffectAnim;

	
};
