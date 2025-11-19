// Fill out your copyright notice in the Description page of Project Settings.


#include "Party/ODPartyMember.h"

#include "Net/UnrealNetwork.h"


UODPartyMember::UODPartyMember()
{
	
}

void UODPartyMember::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODPartyMember, Member);
	DOREPLIFETIME(UODPartyMember, PartyMemberRole);
}

void UODPartyMember::InitPartyMember(AODPlayer* InMember, const EODPartyMemberRole InPartyMemberRole)
{
	Member = InMember;
	PartyMemberRole = InPartyMemberRole;
}

void UODPartyMember::SetPartyMemberRole(const EODPartyMemberRole Role)
{
	PartyMemberRole = Role;
}
