// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ODGameMode.generated.h"

enum class EPlayerJob : uint8;
class AODPlayer;
/**
 * 
 */
UCLASS()
class OD_API AODGameMode : public AGameMode
{
	
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	UClass* GetPawnClassByJob(EPlayerJob Job);

private:
	UPROPERTY(EditDefaultsOnly, Category="Job", meta=(AllowPrivateAccess=true))
	TArray<TSoftClassPtr<AODPlayer>> PlayerSoftClasses;

	TMap<FUniqueNetIdRepl, int32> PlayerJobOptions;
};
