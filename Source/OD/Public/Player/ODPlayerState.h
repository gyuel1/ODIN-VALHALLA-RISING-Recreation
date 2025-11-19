// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ODPlayerState.generated.h"

enum class EPlayerJob : uint8;
class UODPlayerAttributeSet;
class UODPlayerInfo;
class UODAttributeSet;
class UODAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class OD_API AODPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AODPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UODAbilitySystemComponent* GetAbilitySystemComponent() const;
	UODAttributeSet* GetODAttributeSet() const;
	UODPlayerInfo* GetInfo() const;

	// UFUNCTION()
	// void OnRep_WidgetPlayerName();

	// 이름 설정 함수 (서버에서만 호출되어야 함)
	virtual void SetPlayerName(const FString& NewName) override;
	FString GetPlayerName() const { return PlayerName; }

	void SetPlayerJob(EPlayerJob NewJob);
	UFUNCTION(BlueprintPure)
	EPlayerJob GetPlayerJob() const {return PlayerJob;}

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Job")
	EPlayerJob PlayerJob;

private:
	UPROPERTY()
	TObjectPtr<UODAbilitySystemComponent> ODAsc;

	UPROPERTY()
	TObjectPtr<UODAttributeSet> ODAttributeSet;

	UPROPERTY()
	TObjectPtr<UODPlayerAttributeSet> ODPlayerAttributeSet;

	UPROPERTY()
	TObjectPtr<UODPlayerInfo> ObjectInfo;

	 UPROPERTY()
	 FString PlayerName;

	
	
	
};


