// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ODPlayerController.generated.h"

class AODPlayer;
class UODWidget_HUD;
class UODWidget_PrimaryLayout;
enum class EPlayerJob : uint8;
/**
 * 
 */
UCLASS()
class OD_API AODPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	FORCEINLINE UODWidget_HUD* GetHUDWidget() const {return ODHUDWidget;}

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerName(const FString& PlayerName);
	UFUNCTION(Server, Reliable)
	void ServerRequestSpawnByJob(EPlayerJob Job);
	void TrySpawnBasedOnJob(EPlayerJob Job);

	UFUNCTION()
	void Client_UpdateWidgetName(const FString& PlayerName);

protected:
	// Pawn을 소유했을 때 호출됩니다.
	virtual void AcknowledgePossession(APawn* P) override;

private:
	void CreatedRegisterPrimaryWidget();

private:
	UPROPERTY(EditAnywhere, Category="DefaultPawn")
	TSoftClassPtr<AODPlayer> Player_Sorceress;
	UPROPERTY(EditAnywhere, Category="DefaultPawn")
	TSoftClassPtr<AODPlayer> Player_Rogue;
	UPROPERTY(EditAnywhere, Category="DefaultPawn")
	TSoftClassPtr<AODPlayer> Player_ShieldMaiden;
	



#pragma region Widgets
	UPROPERTY(EditDefaultsOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UODWidget_PrimaryLayout> ODPrimaryLayoutWidgetClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UODWidget_HUD> ODHUDWidget;
	
#pragma endregion Widgets
};
