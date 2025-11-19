// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/ODSetBaseStatsEffect.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "PrimaryDataAsset/DT/ODBaseStatsPDA.h"
#include "Subsystem/ODGameDataSubsystem.h"



UODSetBaseStatsEffect::UODSetBaseStatsEffect()
{
	// DurationPolicy = EGameplayEffectDurationType::Infinite;
	//
	// StackingType = EGameplayEffectStackingType::AggregateBySource;
	// StackLimitCount = 1;
	//
	// TArray<FODSetBaseStatsEffectData> ModifierInfoArray;
	//
	// auto AddMod = [&](const FGameplayAttribute& Attr, const FName& Key)
	// {
	// 	FGameplayModifierInfo M;
	// 	M.Attribute = Attr;               
	// 	ModifierInfoArray.Add(FODSetBaseStatsEffectData(M, Key));
	// };
	//
	// // === Health ===
	// AddMod(UODAttributeSet::GetHealthAttribute(),               "Health");
	// AddMod(UODAttributeSet::GetMaxHealthAttribute(),            "MaxHealth");
	// AddMod(UODAttributeSet::GetHealthRegenerationAttribute(),   "HealthRegeneration");
	//
	// // === Mana ===
	// AddMod(UODAttributeSet::GetManaAttribute(),                 "Mana");
	// AddMod(UODAttributeSet::GetMaxManaAttribute(),              "MaxMana");
	// AddMod(UODAttributeSet::GetManaRegenerationAttribute(),     "ManaRegeneration");
	//
	// // === Movement ===
	// AddMod(UODAttributeSet::GetMovementSpeedAttribute(),        "MovementSpeed");
	//
	// // === Offense ===
	// AddMod(UODAttributeSet::GetAttackPowerAttribute(),          "AttackPower");
	// AddMod(UODAttributeSet::GetAttackSpeedAttribute(),          "AttackSpeed");
	// AddMod(UODAttributeSet::GetAccuracyAttribute(),             "Accuracy");
	// AddMod(UODAttributeSet::GetCriticalChanceAttribute(),       "CriticalChance");
	// AddMod(UODAttributeSet::GetCriticalDamageAttribute(),       "CriticalDamage");
	// AddMod(UODAttributeSet::GetDamageIncreaseAttribute(),       "DamageIncrease");
	// AddMod(UODAttributeSet::GetCrowdControlAccuracyAttribute(), "CrowdControlAccuracy");
	//
	// // === Defense ===
	// AddMod(UODAttributeSet::GetDefenseAttribute(),              "Defense");
	// AddMod(UODAttributeSet::GetEvasionAttribute(),              "Evasion");
	// AddMod(UODAttributeSet::GetDamageReductionAttribute(),      "DamageReduction");
	// AddMod(UODAttributeSet::GetCrowdControlResistanceAttribute(),"CrowdControlResistance");
	// AddMod(UODAttributeSet::GetCrowdControlRecoveryAttribute(), "CrowdControlRecovery");
	//
	//
	// for (FODSetBaseStatsEffectData& Data : ModifierInfoArray)
	// {
	// 	Data.Info.ModifierOp = EGameplayModOp::Additive;
	// 	FName TagName = FName(FString::Printf(TEXT("OD.Attribute.BaseStats.%s"), *Data.Name.ToString()));
	// 	
	// 	FSetByCallerFloat SetByCallerTag;
	// 	SetByCallerTag.DataTag = FGameplayTag::RequestGameplayTag(TagName);
	// 	Data.Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerTag);
	// 	Modifiers.Add(Data.Info);
	// }
}

