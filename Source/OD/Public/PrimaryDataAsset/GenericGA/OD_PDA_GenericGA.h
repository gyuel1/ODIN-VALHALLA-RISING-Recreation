// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODCharacter.h"
#include "PrimaryDataAsset/ODPrimaryDataAsset.h"
#include "OD_PDA_GenericGA.generated.h"

class UODGameplayAbility;
/**
 * 
 */
UCLASS()
class OD_API UOD_PDA_GenericGA : public UODPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UOD_PDA_GenericGA();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	FORCEINLINE const TArray<TSubclassOf<UODGameplayAbility>>& GetGenericGiveGA() const { return GenericGiveGA; };
	FORCEINLINE const TArray<TSubclassOf<UODGameplayAbility>>& GetGenericGiveAndTryGA() const { return GenericGiveAndTryGA; };
	FORCEINLINE EODCharacterType GetODCharacterType() const { return ODCharacterType; };

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ODCharacterType", meta = (AllowPrivateAccess = true))
	EODCharacterType ODCharacterType; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Give Only GA", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UODGameplayAbility>> GenericGiveGA;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Give And Try ", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UODGameplayAbility>> GenericGiveAndTryGA;
	
	
};
