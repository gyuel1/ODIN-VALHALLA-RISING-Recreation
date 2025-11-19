// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ODPartyMember.h"
#include "UObject/NoExportTypes.h"
#include "ODPartyInfo.generated.h"

#define MAX_PARTY_HEADCOUNT 4

class AODPlayer;
class UODPartyMember;
/**
 * 
 */
UCLASS()
class OD_API UODPartyInfo : public UObject
{
	GENERATED_BODY()
public:
	UODPartyInfo();

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool IsNameStableForNetworking() const override { return true; }
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool CreateParty(AODPlayer* InLeaderPlayer);
	bool CreateMember(AODPlayer* InPlayer, const EODPartyMemberRole InRole = EODPartyMemberRole::Member);
	bool RemoveMember(AODPlayer* InPlayer);

	FORCEINLINE FGuid GetPartyCode() const {return PartyCode;}
	bool IsPartyMember(AODPlayer* InPlayer);

	
	FORCEINLINE TArray<TObjectPtr<UODPartyMember>>& GetPartyMembers() {return PartyMembers;};
private:
	
	UPROPERTY(Replicated)
	TArray<TObjectPtr<UODPartyMember>> PartyMembers;

	UPROPERTY(Replicated)
	FGuid PartyCode;
	
	
};
