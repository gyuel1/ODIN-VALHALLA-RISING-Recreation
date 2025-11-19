// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ODPlayerAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Debug/ODLogChannels.h"
#include "Net/UnrealNetwork.h"
#include "Tags/ODAbilityTags.h"

using namespace TriggerTags;


UODPlayerAttributeSet::UODPlayerAttributeSet()
{
	Gold.SetBaseValue(0.0f);
	CurrentExp.SetBaseValue(0.0f);
	MaxExp.SetBaseValue(0.0f);
}

void UODPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UODPlayerAttributeSet, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODPlayerAttributeSet, CurrentExp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODPlayerAttributeSet, MaxExp, COND_None, REPNOTIFY_Always);
	
}

bool UODPlayerAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UODPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//레벨업
	if (Data.EvaluatedData.Attribute == GetCurrentExpAttribute())
	{
		const float MakeCurrentExp = GetCurrentExp();
		const float MakeMaxExp = GetMaxExp();
		if (MakeCurrentExp >= MakeMaxExp)
		{
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				float Level = ASC->GetNumericAttribute(UODAttributeSet::GetLevelAttribute());
				if (Level < 10.f)
				{
					Level++;
					ASC->SetNumericAttributeBase(UODAttributeSet::GetLevelAttribute(),Level);
					
					//레벨업 태그부여 레벨업 어빌리티 실행
					ASC->AddLooseGameplayTag(OD_Update_LevelUp);
					//UI 레벨업 알림
					
					SetCurrentExp(MakeCurrentExp - MakeMaxExp);
				}
				else
				{
					SetCurrentExp(GetMaxExp());
				}
			}
		}
	}

	//몬스터 사냥으로 얻은 골드와 경험치 (무조건 필드몬스터를 사냥하고받는 경험치와 골드만 밑의 코드로 통과)
	if (Data.EffectSpec.GetDynamicAssetTags().IsValid() && Data.EffectSpec.GetDynamicAssetTags().HasTag(OD_Effect_Transaction_FieldReward))
	{
		if (Data.EvaluatedData.Attribute == GetGoldAttribute())
		{
			UE_LOG(LogODTransaction,Warning,TEXT("Gold : %f"), GetGold());
		}
		if (Data.EvaluatedData.Attribute == GetCurrentExpAttribute())
		{
			UE_LOG(LogODTransaction,Warning,TEXT("Exp : %f"), GetCurrentExp());
		}
	}
}



void UODPlayerAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UODPlayerAttributeSet, Gold, OldValue);
}

void UODPlayerAttributeSet::OnRep_CurrentExp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UODPlayerAttributeSet, CurrentExp, OldValue);
}

void UODPlayerAttributeSet::OnRep_MaxExp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UODPlayerAttributeSet, MaxExp, OldValue);
}
