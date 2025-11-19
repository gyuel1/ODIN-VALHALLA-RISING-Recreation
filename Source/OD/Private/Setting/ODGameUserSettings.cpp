// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/ODGameUserSettings.h"

#include "Helper/ODDebugHelper.h"


UODGameUserSettings::UODGameUserSettings()
	: OverallVolume(1.f)
{
}

UODGameUserSettings* UODGameUserSettings::Get()
{
	if (GEngine)
	{
		return Cast<UODGameUserSettings>(GEngine->GetGameUserSettings());
	}

	Debug::Print(TEXT("UODGameUserSettings : nullptr"));
	return nullptr;
}

void UODGameUserSettings::SetOverAllVolume(float InVolume)
{
	OverallVolume = InVolume;
	
}

void UODGameUserSettings::SetBGMVolume(float InVolume)
{
	BGMVolume = InVolume;
}

void UODGameUserSettings::SetSFXVolume(float InVolume)
{
	SFXVolume = InVolume;
}

void UODGameUserSettings::SetMessageVolume(float InVolume)
{
	MessageVolume = InVolume;
}

void UODGameUserSettings::SetSpeechVolume(float InVolume)
{
	SpeechVolume = InVolume;
}
