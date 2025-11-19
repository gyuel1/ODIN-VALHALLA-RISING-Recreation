// Fill out your copyright notice in the Description page of Project Settings.


#include "Party/ODPartyInfo.h"

#include "Character/ODPlayer.h"
#include "Net/UnrealNetwork.h"


UODPartyInfo::UODPartyInfo()
{
	PartyMembers.Reset();
	PartyCode = FGuid();
}

void UODPartyInfo::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODPartyInfo, PartyMembers);
	DOREPLIFETIME(UODPartyInfo, PartyCode);
	
}

bool UODPartyInfo::CreateParty(AODPlayer* InLeaderPlayer)
{
	if (!IsValid(InLeaderPlayer))
		return false;

	//PartyCode = FGuid();
	return CreateMember(InLeaderPlayer,EODPartyMemberRole::Leader);
}

bool UODPartyInfo::CreateMember(AODPlayer* InPlayer, const EODPartyMemberRole InRole)
{
	if (PartyMembers.Num() >= MAX_PARTY_HEADCOUNT)
		return false;

	for (UODPartyMember* FindMember :PartyMembers)
	{
		if (FindMember->GetMember() == InPlayer)
		{
			return false;
		}
	}
		
	UODPartyMember* NewMember = NewObject<UODPartyMember>(InPlayer);
	InPlayer->AddReplicatedSubObject(NewMember);

	NewMember->InitPartyMember(InPlayer,InRole);
	PartyMembers.Add(NewMember);
	return true;
	
}

bool UODPartyInfo::RemoveMember(AODPlayer* InPlayer)
{
	if (!IsValid(InPlayer))
		return false;
	
	for (UODPartyMember* FindMember :PartyMembers)
	{
		if (FindMember->GetMember() == InPlayer)
		{
			PartyMembers.Remove(FindMember);
			return true;
		}
	}
	return false;
	
}

bool UODPartyInfo::IsPartyMember(AODPlayer* InPlayer)
{
	for (UODPartyMember* PartyMember : PartyMembers)
	{
		if (PartyMember->GetMember() ==	InPlayer)
			return true;
	}
	return false;
}
