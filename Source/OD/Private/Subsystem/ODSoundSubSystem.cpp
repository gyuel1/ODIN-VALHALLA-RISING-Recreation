// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ODSoundSubSystem.h"

#include "Components/AudioComponent.h"
#include "Setting/ODGameUserSettings.h"


void UODSoundSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &ThisClass::OnPostWorldInitialization);
}


void UODSoundSubSystem::PlayBGM(USoundBase* InSound, float InDuration)
{
	if (!BGMComponent || !InSound)
	{
		UE_LOG(LogTemp, Error, TEXT("UODSoundSubSystem::PlayBGM: BGMComponent Or InSound Null"));
		return;
	}

	if (BGMComponent->IsPlaying() || InDuration > 0.f)
	{
		FadeStartVolume = BGMComponent->VolumeMultiplier;
		FadeTargetVolume = 0.f;
		FadeDurationElapsed = 0.f;
		FadeTotalDuration = InDuration;
		NextBGM = InSound;
		bIsFading = true;
		GetWorld()->GetTimerManager().SetTimer(BGMFadeTimerHandle, this, &ThisClass::UpDateBGMFade, 0.01f, true);
	}
	else
	{
		BGMComponent->SetSound(InSound);
		BGMComponent->SetVolumeMultiplier(MasterVolume * BGMVolume);
		BGMComponent->Play();
	}
}

void UODSoundSubSystem::UpDateBGMFade()
{
	FadeDurationElapsed += 0.01f;
	float Alpha = FMath::Clamp(FadeDurationElapsed / FadeTotalDuration, 0.f, 1.f);
	float NewVolume = FMath::Lerp(FadeStartVolume, FadeTargetVolume, Alpha);
	BGMComponent->SetVolumeMultiplier(NewVolume);

	if (Alpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(BGMFadeTimerHandle);

		if (NextBGM)
		{
			BGMComponent->SetSound(NextBGM);
			BGMComponent->Play();

			FadeStartVolume = 0.f;
			FadeTargetVolume = MasterVolume * BGMVolume;
			FadeDurationElapsed = 0.f;
			FadeTotalDuration = 1.f; 
			NextBGM = nullptr;

			GetWorld()->GetTimerManager().SetTimer(BGMFadeTimerHandle, this, &ThisClass::UpDateBGMFade, 0.01f, true);
		}
		else
		{
			bIsFading = false;
		}
	}
}

void UODSoundSubSystem::OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues)
{
	if (!InWorld || !InWorld->IsGameWorld())
		return;
	
	if (BGMComponent)
	{
		BGMComponent->DestroyComponent();
		BGMComponent = nullptr;
	}

	BGMComponent = NewObject<UAudioComponent>(InWorld->GetWorldSettings());
	
	if (BGMComponent)
	{
		BGMComponent->bAutoActivate = false;
		BGMComponent->SetUISound(true);
		BGMComponent->RegisterComponent();
		BGMComponent->SetVolumeMultiplier(MasterVolume * BGMVolume);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SoundSubsystem: World is null!"));
	}
}

void UODSoundSubSystem::SetBGMVolume(float InVolume)
{
	UODGameUserSettings* UserSetting = UODGameUserSettings::Get();
	BGMVolume = UserSetting->GetBGMVolume();
	BGMVolume = FMath::Clamp(InVolume, 0.f, 1.f);
	
	if (BGMComponent)
		BGMComponent->SetVolumeMultiplier(MasterVolume * BGMVolume);
		
}

void UODSoundSubSystem::SetMasterVolume(float InVolume)
{
	UODGameUserSettings* UserSetting = UODGameUserSettings::Get();
	MasterVolume = UserSetting->GetOverAllVolume();
	MasterVolume = FMath::Clamp(InVolume, 0.f, 1.f);
}
