// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/ODFrontendFunctionLibrary.h"

#include "Enum/ODUIEnumSound.h"
#include "Kismet/GameplayStatics.h"
#include "Setting/ODUIDeveloperSetting.h"
#include "Systems/ODGameInstance.h"


TSoftClassPtr<UODWidget_Activatable> UODFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(
	FGameplayTag InWidgetTag)
{
	const UODUIDeveloperSetting* ODUIDeveloperSettings = GetDefault<UODUIDeveloperSetting>();

	checkf(ODUIDeveloperSettings->ODWidgetMaps.Contains(InWidgetTag), TEXT("DeveloperSetting Don't Found Map In Tag"));

	return ODUIDeveloperSettings->ODWidgetMaps.FindRef(InWidgetTag);
}

UODGameInstance* UODFrontendFunctionLibrary::GetODGameInstance(const UObject* WorldContextObject)
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return Cast<UODGameInstance>(GI);
	}
	return nullptr;
}

USoundBase* UODFrontendFunctionLibrary::GetBGM(const UObject* WorldContextObject, EBGMType BGMType)
{
	UODGameInstance* GI = GetODGameInstance(WorldContextObject);
	
	if (!GI->GetBGM(BGMType))
		return nullptr;

	return GI->GetBGM(BGMType);
}
