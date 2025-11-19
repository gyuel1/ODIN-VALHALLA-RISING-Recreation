// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ODPartyMember.generated.h"

UENUM()
enum class EODPartyMemberRole
{
	Leader,
	Member
};


class AODPlayer;
/**
 * 
 */
UCLASS()
class OD_API UODPartyMember : public UObject
{
	GENERATED_BODY()
public:
	UODPartyMember();
	
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool IsNameStableForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void InitPartyMember(AODPlayer* InMember, const EODPartyMemberRole InPartyMemberRole);
	
	FORCEINLINE AODPlayer* GetMember() const {return Member;}
	FORCEINLINE EODPartyMemberRole GetPartyMemberRole() const {return PartyMemberRole;}
	void SetPartyMemberRole(const EODPartyMemberRole Role);

private:
	
	UPROPERTY(Replicated)
	TObjectPtr<AODPlayer> Member;
	
	UPROPERTY(Replicated)
	EODPartyMemberRole PartyMemberRole;
	
	
	
	
	
};
