// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODPartyComponent.h"

#include "Character/ODPlayer.h"
#include "Debug/ODLogChannels.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ODDungeonInstanceManager.h"
#include "Net/UnrealNetwork.h"
#include "Object/SpawnPoint/ODSpawnPoint.h"
#include "Party/ODPartyInfo.h"
#include "Party/ODPartyMember.h"


UODPartyComponent::UODPartyComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	Party = nullptr;
}

void UODPartyComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODPartyComponent, Party);
}

void UODPartyComponent::CreateParty_Server_Implementation()
{
	if (HasParty())
		return;

	OD_LOG_SERVER(LogOD,Warning,TEXT("제발좀"));
	if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
	{
		UODPartyInfo* MakeParty = NewObject<UODPartyInfo>(Owner);
		AddReplicatedSubObject(MakeParty);
		
		Party = MakeParty;
		//const FODPartyPacket MakePartyPacket = FODPartyPacket(Party);
		
		if (Party->CreateParty(Owner))
		{
			//OnRepPartyPacket(MakePartyPacket);
			CreateParty();
			Owner->ForceNetUpdate();
		}
	}
}


void UODPartyComponent::CreateParty_Implementation()
{
	if (HasParty())
		return;

	OD_LOG_SERVER(LogOD,Warning,TEXT("제발좀"));
	if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
	{
		UODPartyInfo* CreateParty = NewObject<UODPartyInfo>(Owner);
		AddReplicatedSubObject(CreateParty);
		
		Party = CreateParty;
		//const FODPartyPacket MakePartyPacket = FODPartyPacket(Party);
		
		if (Party->CreateParty(Owner))
		{
			//OnRepPartyPacket(MakePartyPacket);
			//OnRepPartyValue(Party);
			Owner->ForceNetUpdate();
		}
	}
}

void UODPartyComponent::RemovePartyLeaderOption_Implementation()
{
	if (!IsPartyLeader())
		return;
	
	for (UODPartyMember* PartyMember : Party->GetPartyMembers())
	{
		if (!PartyMember || !IsValid(PartyMember->GetMember()))
			continue;

		if (PartyMember->GetMember() == Cast<AODPlayer>(GetOwner()))
		{
			continue;
		}
		UODPartyComponent* PartyComponent =  PartyMember->GetMember()->GetODPartyComponent();
		PartyComponent->RemovePartyMemberOption();
	}
	RemovePartyMemberOption();
}

void UODPartyComponent::LeaveParty_Implementation()
{
	if (!IsValid(Party))
		return;

	AODPlayer* Owner = Cast<AODPlayer>(GetOwner());
	if (!Owner)
		return;

	if (IsPartyLeader())
	{
		for (UODPartyMember* PartyMember : Party->GetPartyMembers())
		{
			if (!PartyMember || !IsValid(PartyMember->GetMember()))
				continue;

			if (PartyMember->GetMember() == Cast<AODPlayer>(GetOwner()))
			{
				continue;
			}
			PartyMember->SetPartyMemberRole(EODPartyMemberRole::Leader);
			break;
		}
	}
	else
	{
		RemovePartyMemberOption();
	}
}

void UODPartyComponent::AddMember_Implementation(AODPlayer* InPlayer)
{
	if (!IsValid(InPlayer) || !IsPartyLeader() || !Party)
		return;

	if (Party->CreateMember(InPlayer))
	{
		for (UODPartyMember* Member : Party->GetPartyMembers())
		{
			if (UODPartyComponent* NewMemberPartyComponent = Member->GetMember()->GetODPartyComponent())
			{
				NewMemberPartyComponent->SetParty(Party);
				NewMemberPartyComponent->OnRepPartyValue(Party);
				
			}
		}
	}
}

void UODPartyComponent::AddMember_Server_Implementation(AODPlayer* InPlayer)
{
	if (!IsValid(InPlayer) || !IsPartyLeader() || !Party)
		return;

	if (Party->CreateMember(InPlayer))
	{
		for (UODPartyMember* Member : Party->GetPartyMembers())
		{
			if (UODPartyComponent* NewMemberPartyComponent = Member->GetMember()->GetODPartyComponent())
			{
				NewMemberPartyComponent->SetParty(Party);
				NewMemberPartyComponent->OnRepPartyValue(Party);
				AddMember(InPlayer);
			}
		}
	}
}
void UODPartyComponent::RemoveMember_Implementation(AODPlayer* InPlayer)
{
	if (!IsValid(InPlayer) || !IsPartyLeader() || !Party)
		return;
	
	if (Party->RemoveMember(InPlayer))
	{
		Party= nullptr;
	}
}



bool UODPartyComponent::HasParty() const
{
	if (Party != nullptr)
		return true;

	return false;
}

bool UODPartyComponent::IsPartyLeader() const
{
	if (!HasParty() || !GetOwner())
		return false;

	AODPlayer* Owner = Cast<AODPlayer>(GetOwner());
	if (!IsValid(Owner))
		return false;

	
	for (UODPartyMember* Member : Party->GetPartyMembers())
	{
		if (Member->GetMember() == Owner && Member->GetPartyMemberRole() == EODPartyMemberRole::Leader)
			return true;
	}

	return false;
}

void UODPartyComponent::OnRepPartyPacket_Implementation(const FODPartyPacket PartyPacket)
{
	Party = PartyPacket.PartyInfo;
}

void UODPartyComponent::ConnectDungeon_Implementation()
{
	for (UODPartyMember* Member :Party->GetPartyMembers())
	{
		Member->GetMember()->GetODPartyComponent()->ConnectDungeon_Member();
	}
	

	// TArray<AActor*> Out;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(),AODDungeonInstanceManager::StaticClass(),Out);
	//
	// if (Out.IsEmpty())
	// 	return;
	//
	// AODDungeonInstanceManager* Manager = Cast<AODDungeonInstanceManager>(Out[0]);
	// if (!Manager)
	// 	return;
	//
	// AODPlayer* Owner = Cast<AODPlayer>(GetOwner());
	// if (!Owner)
	// 	return;
	// if (Owner->HasAuthority())
	// 	Manager->ConnectPartyToDunGeon(Party,Owner);
	
}



void UODPartyComponent::OnRepPartyValue_Implementation(UODPartyInfo* InParty)
{
	Party = InParty;
}

void UODPartyComponent::SetParty_Implementation(UODPartyInfo* InParty)
{
	if (!InParty)
		Party = InParty;
	
}

void UODPartyComponent::RemovePartyMemberOption_Implementation()
{
	if (!GetOwner()->HasAuthority())
		return;
	
	if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
	{
		Party->RemoveMember(Owner);
		Party = nullptr;
	}
}

void UODPartyComponent::RemovePartyMemberOption_Client_Implementation()
{
	Party = nullptr;
}



void UODPartyComponent::ConnectDungeon_Member()
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(GetOwner(),FName("02_02_Dungeon"),true,true,LatentInfo);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AODSpawnPoint::StaticClass(), FoundActors);
	
	if (!FoundActors.IsEmpty())
	{
		for (AActor* Actor : FoundActors)
		{
			AODSpawnPoint* Enemy = Cast<AODSpawnPoint>(Actor);
			if (Enemy)
			{
				GetOwner()->SetActorLocation(Actor->GetActorLocation());
				break;
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		LevelStreamingTimerHandle,
		[this]()
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AODSpawnPoint::StaticClass(), FoundActors);
			if (!FoundActors.IsEmpty())
			{
				for (AActor* Actor : FoundActors)
				{
					AODSpawnPoint* Enemy = Cast<AODSpawnPoint>(Actor);
					if (Enemy)
					{
						GetOwner()->SetActorLocation(Actor->GetActorLocation());
						break;
					}
				}
				// FLatentActionInfo TimerLatentInfo;
				// UGameplayStatics::UnloadStreamLevel(this, FName("MidGard"), TimerLatentInfo,false);
				GetWorld()->GetTimerManager().ClearTimer(LevelStreamingTimerHandle);
			}
		},
		0.1f,
		true,
		2.f);

	
}

void UODPartyComponent::LogPartyList()
{
	if (!HasParty())
		return;
	
	AODPlayer* Owner = Cast<AODPlayer>(GetOwner());
	if ((!Owner))
		return;


	
	UE_LOG(LogODParty, Warning, TEXT("====================[%s]Start===================="), *GetOwner()->GetClass()->GetName());
	int32 i = 0;
	for (UODPartyMember* Member : Party->GetPartyMembers())
	{
		FString Role =	Member->GetPartyMemberRole() == EODPartyMemberRole::Leader ? "Leader" : "Member";
		FString MemberName = Member->GetMember() ? Member->GetMember()->GetName() : "Null Name";
		FString IsThis = Member->GetMember() == Owner ? "Owner" : "NotOwner";
		UE_LOG(LogODParty, Warning, TEXT("[%d][%s] : %s , %s"), i, *Role, *MemberName,*IsThis);
		i++;
	}
	UE_LOG(LogODParty, Warning, TEXT("====================[%s]End===================="),  *GetOwner()->GetClass()->GetName());
}

void UODPartyComponent::LogPartyList_Server_Implementation()
{
	UE_LOG(LogODParty, Warning, TEXT("서버 호출된 파티로그"));
	LogPartyList();
}

void UODPartyComponent::OnRep_Party()
{
	OD_LOG_SERVER(LogODParty,Warning, TEXT("UODPartyComponent::OnRep_Party"));
}





