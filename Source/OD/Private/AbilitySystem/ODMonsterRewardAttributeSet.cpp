// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ODMonsterRewardAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


UODMonsterRewardAttributeSet::UODMonsterRewardAttributeSet()
{
}

void UODMonsterRewardAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UODMonsterRewardAttributeSet, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODMonsterRewardAttributeSet, Exp, COND_None, REPNOTIFY_Always);
}

bool UODMonsterRewardAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UODMonsterRewardAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UODMonsterRewardAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UODMonsterRewardAttributeSet, Gold, OldValue);
}

void UODMonsterRewardAttributeSet::OnRep_Exp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UODMonsterRewardAttributeSet, Exp, OldValue);
}
