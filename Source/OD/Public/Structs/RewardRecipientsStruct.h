// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODCombatCharacter.h"
#include "Party/ODPartyInfo.h"
#include "RewardRecipientsStruct.generated.h"

USTRUCT(Blueprintable)
struct FRewardRecipients
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AODCombatCharacter> RewardRecipientActor = nullptr;

	UPROPERTY()
	float Weight = 0.f;

	FRewardRecipients()
	{
		RewardRecipientActor = nullptr;
		Weight = 0.f;
	}

	FRewardRecipients(AODCombatCharacter* InRewardRecipientActor,const float& InWeight)
	{
		RewardRecipientActor = InRewardRecipientActor;
		Weight = InWeight;
	}
};

USTRUCT(Blueprintable)
struct FRewardRecipientParty
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UODPartyInfo> RewardRecipientActors;
	
	UPROPERTY()
	FGuid PartyCode;

	UPROPERTY()
	float Weight = 0.f;

	FRewardRecipientParty()
	{
		RewardRecipientActors = nullptr;
		PartyCode = FGuid();
		Weight = 0.f;
	}

	FRewardRecipientParty(const FGuid InPartyCode)
	{
		RewardRecipientActors = nullptr;
		Weight = 0.f;

		PartyCode = InPartyCode;
	}
};


