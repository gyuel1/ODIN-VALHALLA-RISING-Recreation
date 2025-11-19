// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ODGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UODGameUserSettings();
	
	static UODGameUserSettings* Get();

	// ***GameTab Setting***
	UFUNCTION()
	FString GetCurrentPlayerInfoVisibility() const {return CurrentPlayerInfoVisibility;}
	UFUNCTION()
	void SetCurrentPlayerInfoVisibility(const FString& InNewPlayerInfoVisibility) {CurrentPlayerInfoVisibility = InNewPlayerInfoVisibility;}
	// ***GameTab Setting***

	// ***AudioTab Setting***
#pragma region Sound GetSet
	UFUNCTION()
	float GetOverAllVolume() const {return OverallVolume;}
	UFUNCTION()
	void SetOverAllVolume(float InVolume);
	
	UFUNCTION()
	float GetBGMVolume() const {return BGMVolume;}
	UFUNCTION()
	void SetBGMVolume(float InVolume);
	
	UFUNCTION()
	float GetSFXVolume() const {return SFXVolume;}
	UFUNCTION()
	void SetSFXVolume(float InVolume);
	
	UFUNCTION()
	float GetMessageVolume() const {return MessageVolume;}
	UFUNCTION()
	void SetMessageVolume(float InVolume);
	
	UFUNCTION()
	float GetSpeechVolume() const {return SpeechVolume;}
	UFUNCTION()
	void SetSpeechVolume(float InVolume);
#pragma endregion

	
private:
	// ***GameTab Setting***
	UPROPERTY(Config)
	FString CurrentPlayerInfoVisibility;
	// ***GameTab Setting***

	// ***AudioTab Setting***
	UPROPERTY(Config)
	float OverallVolume;
	UPROPERTY(Config)
	float BGMVolume;
	UPROPERTY(Config)
	float SFXVolume;
	UPROPERTY(Config)
	float MessageVolume;
	UPROPERTY(Config)
	float SpeechVolume;
	// ***AudioTab Setting***

	
};
