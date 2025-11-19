// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ODWidget_OverHeadPlayer.generated.h"

class UImage;
class UTextBlock;
class UODBaseGauge;
/**
 * 
 */
UCLASS()
class OD_API UODWidget_OverHeadPlayer : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void ConfigureASC(class UAbilitySystemComponent* AbilitySystemComponent);
	void SetWidgetPlayerName(const FString& WidgetName);
	void SetPlayerNameColor(bool IsPlayer);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODBaseGauge> HPGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UODBaseGauge> MPGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> GuildMark;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> GuildName;

	UPROPERTY(EditAnywhere, Category = "Color")
	FSlateColor PlayerNameColor;

	UPROPERTY(EditAnywhere, Category = "Color")
	FSlateColor OtherPlayerNameColor;
	
	
	
};
