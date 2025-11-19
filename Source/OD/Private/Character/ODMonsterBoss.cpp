// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODMonsterBoss.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Abilities/Boss/OD_GA_Boss_Enrage.h"
#include "Tags/ODAbilityTags.h"


AODMonsterBoss::AODMonsterBoss()
:Super()
{
	ODCharacterType = EODCharacterType::FieldBossMonster;
}

void AODMonsterBoss::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;
	
	ASC->GetGameplayAttributeValueChangeDelegate(UODAttributeSet::GetHealthAttribute())
		   .AddUObject(this, &ThisClass::OnBossEnrage);

	ASC->GetGameplayAttributeValueChangeDelegate(UODAttributeSet::GetMaxHealthAttribute())
	   .AddUObject(this, &ThisClass::OnBossEnrage);
}

void AODMonsterBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
}

void AODMonsterBoss::OnBossEnrage(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;
	
	static FGameplayTagContainer EnrageTag;
	EnrageTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Boss.Enrage")));
	
	const float CurrentHp = ASC->GetNumericAttribute(UODAttributeSet::GetHealthAttribute());
	const float MaxHP = ASC->GetNumericAttribute(UODAttributeSet::GetMaxHealthAttribute());

	static const FGameplayTag Tag_Enraged = FGameplayTag::RequestGameplayTag(TEXT("OD.Effect.Enrage.Status"));	

	const bool bOnEnrage = (MaxHP > 0) && (CurrentHp / MaxHP <= 0.5f);

	if (!bOnEnrage || ASC->HasMatchingGameplayTag(Tag_Enraged))
	{
		GetWorldTimerManager().ClearTimer(EnrageRetryHandle);
		return;
	}

	if (!ASC->TryActivateAbilitiesByTag(EnrageTag))
	{
		GetWorldTimerManager().SetTimer(EnrageRetryHandle, this, &AODMonsterBoss::HandleEnrageRetry, 0.01f, true);
	}
}

void AODMonsterBoss::HandleEnrageRetry()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(this) || !ASC) { GetWorldTimerManager().ClearTimer(EnrageRetryHandle); return; }

	static const FGameplayTag Tag_Enraged = FGameplayTag::RequestGameplayTag(TEXT("OD.Effect.Enrage.Status"));
	if (ASC->HasMatchingGameplayTag(Tag_Enraged))
	{
		GetWorldTimerManager().ClearTimer(EnrageRetryHandle);
		return;
	}

	static FGameplayTagContainer EnrageTag; EnrageTag.Reset(); EnrageTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Boss.Enrage")));
	if (ASC->TryActivateAbilitiesByTag(EnrageTag))
	{
		GetWorldTimerManager().ClearTimer(EnrageRetryHandle);
		return;
	}
}
