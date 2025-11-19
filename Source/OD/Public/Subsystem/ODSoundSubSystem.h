// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ODSoundSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODSoundSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// BGM
	UFUNCTION(BlueprintCallable)
	void PlayBGM(USoundBase* InSound, float InDuration = 0.f);
	void UpDateBGMFade();
	
	void OnPostWorldInitialization(UWorld* InWorld, UWorld::InitializationValues IVS);
	void OnWorldTearDown(UWorld* InWorld);

	// Volume Setting
public:
	void SetBGMVolume(float InVolume);
	void SetMasterVolume(float InVolume);
	


private:
	UPROPERTY()
	UAudioComponent* BGMComponent;
	
	float MasterVolume = 1.f;
	float BGMVolume = 1.f;
	float SFXVolume = 1.f;
	
	FTimerHandle BGMFadeTimerHandle;
	float FadeStartVolume;
	float FadeTargetVolume;
	float FadeDurationElapsed;
	float FadeTotalDuration;
	bool bIsFading = false;

	USoundBase* NextBGM;

	TArray<TObjectPtr<USoundBase>> Bgms;
	
};
