// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Sounds/ODDataAsset_Sounds.h"
#include "Engine/GameInstance.h"
#include "ODGameInstance.generated.h"

class UODDataAsset_Sounds;
/**
 * 
 */
UCLASS()
class OD_API UODGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UODGameInstance();

public:
	UFUNCTION(BlueprintCallable)
	USoundBase* GetBGM(EBGMType BGMType) {return DA_Sounds->GetBGM(BGMType);}
	
protected:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	UFUNCTION()
	void OnPreLoadMap(const FString& MapName);

	UFUNCTION()
	void OnPostLoadMap(UWorld* LoadedWorld);

	UPROPERTY()
	UUserWidget* BlackScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingWidgetClassRef;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UODDataAsset_Sounds> DA_Sounds;
	
	
};
