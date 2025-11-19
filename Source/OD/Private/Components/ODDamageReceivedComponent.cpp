// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODDamageReceivedComponent.h"

#include "Character/ODCombatCharacter.h"
#include "Debug/ODLogChannels.h"
#include "Helper/ODDebugHelper.h"
#include "Net/UnrealNetwork.h"


void FDamageReceivedCharacterInfo::PreReplicatedRemove(const struct FDamageReceivedCharacterList& InArraySerializer)
{

}

void FDamageReceivedCharacterInfo::PostReplicatedAdd(const struct FDamageReceivedCharacterList& InArraySerializer)
{
	
}

void FDamageReceivedCharacterInfo::PostReplicatedChange(const struct FDamageReceivedCharacterList& InArraySerializer)
{

}

UODDamageReceivedComponent::UODDamageReceivedComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UODDamageReceivedComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODDamageReceivedComponent, DamageReceivedArray);
}

void UODDamageReceivedComponent::BeginPlay()
{
	Super::BeginPlay();
	//DamageReceivedArray.OwningObject = this;
}

void UODDamageReceivedComponent::AddDamageReceivedArray_Internal(AODCombatCharacter* InCharacter, float InDamage)
{
	if (!IsValid(InCharacter) || InDamage <= 0.f)
		return;
	
	if (FDamageReceivedCharacterInfo* Info = DamageReceivedArray.FindDamageReceivedCharacter(InCharacter))
	{
		Info->AccumulatedDamage += InDamage;
		DamageReceivedArray.MarkItemDirty(*Info);		
	}
	else
	{
		FDamageReceivedCharacterInfo& MakeInfo = DamageReceivedArray.Items.Emplace_GetRef(InCharacter,InDamage);
		DamageReceivedArray.MarkItemDirty(MakeInfo);
	}

	OD_LOG_SERVER(LogODDamageReceived,Warning,TEXT("================[%s]Start================"),*GetOwner()->GetName());
	for (FDamageReceivedCharacterInfo& Info :DamageReceivedArray.Items)
	{
		if (!Info.CombatCharacter)
		{
			OD_LOG_SERVER(LogODDamageReceived, Warning, TEXT("IsNull Character"));
			continue;
		}
		
		OD_LOG_SERVER(LogODDamageReceived,Warning,TEXT("[%s] : %f"),*Info.CombatCharacter->GetName(),Info.AccumulatedDamage)
	}
	OD_LOG_SERVER(LogODDamageReceived,Warning,TEXT("=================[%s]End================="),*GetOwner()->GetName());
}

void UODDamageReceivedComponent::ClearDamageReceivedArray_Internal()
{
	if (!GetOwner())
		return;
	DamageReceivedArray.Items.Reset();
	DamageReceivedArray.MarkArrayDirty();
	
	GetOwner()->ForceNetUpdate();
}


void UODDamageReceivedComponent::AddDamageReceivedArray_Implementation(AODCombatCharacter* InCharacter,
                                                                       const float InDamage)
{
	AddDamageReceivedArray_Internal(InCharacter,InDamage);
}

void UODDamageReceivedComponent::ClearDamageReceivedArray_Implementation()
{
	ClearDamageReceivedArray_Internal();
}



