// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODWidgetCharacter.h"
#include "Enum/ODEnumCharacterJob.h"
#include "ODWidgetCharacter_Select.generated.h"

/**
 * 
 */


class AODCharacterSelectManager;

UCLASS()
class OD_API AODWidgetCharacter_Select : public AODWidgetCharacter
{
	GENERATED_BODY()

public:
	AODWidgetCharacter_Select();
	
	// UFUNCTION()
	// void OnCharacterClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
	EPlayerJob GetPlayerJob() {return CharacterJob;}
	UFUNCTION(BlueprintPure)
	bool GetIsSelected() const {return bIsSelected;}
	UFUNCTION(BlueprintCallable)
	void SetIsSelected(bool InSelected) {bIsSelected = InSelected;}

protected:
	UPROPERTY()
	TArray<TObjectPtr<AODWidgetCharacter_Select>> SelectPlayers;
	UPROPERTY()
	bool bIsSelected = false;

private:
	UPROPERTY(EditAnywhere, Category = "Job", meta = (AllowPrivateAccess = "true"))
	EPlayerJob CharacterJob;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AODCharacterSelectManager* CharacterSelectManager;


};
