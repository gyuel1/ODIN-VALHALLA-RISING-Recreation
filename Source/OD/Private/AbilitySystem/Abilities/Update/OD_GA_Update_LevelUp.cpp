// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Update/OD_GA_Update_LevelUp.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Character/ODPlayer.h"
#include "Components/ODCombatComponent.h"
#include "Debug/ODLogChannels.h"
#include "Tags/ODAbilityTags.h"

using namespace GameplayAbilityTags;
using namespace TriggerTags;

UOD_GA_Update_LevelUp::UOD_GA_Update_LevelUp()
	:Super()
{
	AbilityTags.AddTag(OD_Ability_Update_LevelUp);

	FAbilityTriggerData Trig;
	Trig.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	Trig.TriggerTag = FGameplayTag(OD_Update_LevelUp);
	AbilityTriggers.Add(Trig);
}

bool UOD_GA_Update_LevelUp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
		return false;

	const float Level = ASC->GetNumericAttributeBase(UODAttributeSet::GetLevelAttribute());
	if (Level > 10.f)
		return false;
	
	return true;
	
}

void UOD_GA_Update_LevelUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	AODPlayer* Avatar = Cast<AODPlayer>(ActorInfo->AvatarActor);
	UODCombatComponent* CombatComponent = Avatar->GetODCombatComponent();
	
	if (!K2_HasAuthority() || !ASC || !Avatar || !CombatComponent)
	{
		EndAbility(Handle, ActorInfo,ActivationInfo, true,false);
		return;
	}

	
	//레벨에 맞는 스텟 갱신
	const float CurrentLevel = ASC->GetNumericAttribute(UODAttributeSet::GetLevelAttribute());
	const FLevelUpStats LevelUpStats = Avatar->GetLevelUpStats();
	const FStatsDataStruct BaseStats = CombatComponent->GetBaseStats();
	

	const float NewMaxHealth = BaseStats.MaxHealth + (CurrentLevel - 1.f) * LevelUpStats.HealthIncrease;
	const float NewHealthRegeneration = BaseStats.HealthRegeneration + (CurrentLevel - 1.f) * LevelUpStats.HealthRegeneration;

	const float NewMaxMana = BaseStats.MaxMana + (CurrentLevel - 1.f) * LevelUpStats.ManaIncrease;
	const float NewManaRegeneration = BaseStats.ManaRegeneration + (CurrentLevel - 1.f) * LevelUpStats.ManaRegeneration;

	const float NewAttackPower = BaseStats.AttackPower + (CurrentLevel - 1.f) * LevelUpStats.AttackPowerIncrease;
	const float NewDefense = BaseStats.Defense + (CurrentLevel - 1.f) * LevelUpStats.DefenseIncrease;

	
	ASC->SetNumericAttributeBase(UODAttributeSet::GetMaxHealthAttribute(),NewMaxHealth);
	ASC->SetNumericAttributeBase(UODAttributeSet::GetHealthRegenerationAttribute(),NewHealthRegeneration);
	
	ASC->SetNumericAttributeBase(UODAttributeSet::GetMaxManaAttribute(),NewMaxMana);
	ASC->SetNumericAttributeBase(UODAttributeSet::GetManaRegenerationAttribute(),NewManaRegeneration);
	
	ASC->SetNumericAttributeBase(UODAttributeSet::GetAttackPowerAttribute(),NewAttackPower);
	ASC->SetNumericAttributeBase(UODAttributeSet::GetDefenseAttribute(),NewDefense);
	

	UE_LOG(LogOD,Warning,TEXT("Level Up Ability Done."));
	
	//레벨에 맞는 경험치통 갱신
	ASC->AddLooseGameplayTag(OD_Init_MaxExp);
	
	//체력 마나 최대로 체워주기
	ASC->AddLooseGameplayTag(OD_Init_FillVitals);
	
	//자동회복 갱신은 postAttributeSetChange에서 함.



	
	EndAbility(Handle, ActorInfo,ActivationInfo, true,false);
}

void UOD_GA_Update_LevelUp::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(OD_Update_LevelUp);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
