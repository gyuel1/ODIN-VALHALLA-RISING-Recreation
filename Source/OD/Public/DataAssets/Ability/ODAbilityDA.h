// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "DataAssets/ODDataAsset.h"
#include "ODAbilityDA.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODAbilityDA : public UODDataAsset
{
	GENERATED_BODY()
public:
	UODAbilityDA();
	
	FORCEINLINE const FGameplayTag& GetTag() const {return Tag;};
	FORCEINLINE const TSubclassOf<UODGameplayAbility>& GetAbility() const {return Ability;};
protected:
	//능력의 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Ability",meta = (AllowPrivateAccess = true, Categories="OD.Active,OD.Passive,OD.Ability"))
	FGameplayTag Tag;

	//능력 TSubClassOf
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "Ability",meta = (AllowPrivateAccess = true))
	TSubclassOf<UODGameplayAbility> Ability;

	
	
};
