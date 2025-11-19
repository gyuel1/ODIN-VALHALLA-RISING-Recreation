// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ODAttributeSet.generated.h"

/*============================================================
공통 스텟

레벨 Level

체력 Health
최대체력 MaxHealth
체력자연회복력 HealthRegeneration

마나 Mana
최대마나 Mana
마나자연회복력 ManaRegeneration

이속 MovementSpeed

공격관련

공격력 AttackPower
공격속도 AttackSpeed
명중률 Accuracy
치확 CriticalChance
치피 CriticalDamage
데미지증가 DamageIncrease
상태이상 적중률 CrowdControlAccuracy

방어관련

방어력 Defense
회피율 Evasion
받는피해감소 DamageReduction
상태이상저항 CrowdControlResistance
상태이상회복 CrowdControlRecovery
============================================================*/

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class OD_API UODAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	


public:
	UODAttributeSet();

	friend class FOnDamageSourceCapture;
	friend class FOnDamageTargetCapture;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UODAttributeSet, Health)
	
	ATTRIBUTE_ACCESSORS(UODAttributeSet, Level)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, HealthRegeneration)

	ATTRIBUTE_ACCESSORS(UODAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, ManaRegeneration)

	ATTRIBUTE_ACCESSORS(UODAttributeSet, MovementSpeed)

	ATTRIBUTE_ACCESSORS(UODAttributeSet, AttackPower)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, AttackSpeed)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, Accuracy)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, CriticalChance)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, CriticalDamage)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, DamageIncrease)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, CrowdControlAccuracy)

	ATTRIBUTE_ACCESSORS(UODAttributeSet, Defense)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, Evasion)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, DamageReduction)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, CrowdControlResistance)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, CrowdControlRecovery)

	ATTRIBUTE_ACCESSORS(UODAttributeSet, EvasionProc)
	ATTRIBUTE_ACCESSORS(UODAttributeSet, CriticalProc)
	

private:

	//공용 함수
	void OnDamageReceived(const FGameplayEffectModCallbackData& Data ,const float& InDamage);
	
	//몬스터 함수
	void MonsterReportDamageEvent(const FGameplayEffectModCallbackData& Data);

	//플레이어 함수
	
	
protected:
	// ========== Attributes (protected) ==========

	// === Level ===

	UPROPERTY(ReplicatedUsing=OnRep_Level, BlueprintReadWrite, 
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Level;
	
	// === Health ===
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadWrite, Category="ODAttribute|Health",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadWrite, Category="ODAttribute|Health",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing=OnRep_HealthRegeneration, BlueprintReadWrite, Category="ODAttribute|Health",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData HealthRegeneration;

	// === Mana ===
	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadWrite, Category="ODAttribute|Mana",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Mana;

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadWrite, Category="ODAttribute|Mana",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxMana;

	UPROPERTY(ReplicatedUsing=OnRep_ManaRegeneration, BlueprintReadWrite, Category="ODAttribute|Mana",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ManaRegeneration;

	// === Movement ===
	UPROPERTY(ReplicatedUsing=OnRep_MovementSpeed, BlueprintReadWrite, Category="ODAttribute|Movement",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MovementSpeed;

	// === Offense ===
	UPROPERTY(ReplicatedUsing=OnRep_AttackPower, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData AttackPower;

	UPROPERTY(ReplicatedUsing=OnRep_AttackSpeed, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(ReplicatedUsing=OnRep_Accuracy, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Accuracy;

	UPROPERTY(ReplicatedUsing=OnRep_CriticalChance, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CriticalChance;

	UPROPERTY(ReplicatedUsing=OnRep_CriticalDamage, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CriticalDamage;

	UPROPERTY(ReplicatedUsing=OnRep_DamageIncrease, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DamageIncrease;

	UPROPERTY(ReplicatedUsing=OnRep_CrowdControlAccuracy, BlueprintReadWrite, Category="ODAttribute|Offense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CrowdControlAccuracy;

	// === Defense ===
	UPROPERTY(ReplicatedUsing=OnRep_Defense, BlueprintReadWrite, Category="ODAttribute|Defense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Defense;

	UPROPERTY(ReplicatedUsing=OnRep_Evasion, BlueprintReadWrite, Category="ODAttribute|Defense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Evasion;

	UPROPERTY(ReplicatedUsing=OnRep_DamageReduction, BlueprintReadWrite, Category="ODAttribute|Defense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DamageReduction;

	UPROPERTY(ReplicatedUsing=OnRep_CrowdControlResistance, BlueprintReadWrite, Category="ODAttribute|Defense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CrowdControlResistance;

	UPROPERTY(ReplicatedUsing=OnRep_CrowdControlRecovery, BlueprintReadWrite, Category="ODAttribute|Defense",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CrowdControlRecovery;

	// === Proc ===
	UPROPERTY(ReplicatedUsing=OnRep_EvasionProc, BlueprintReadWrite, Category="ODAttribute|Proc",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData EvasionProc;

	UPROPERTY(ReplicatedUsing=OnRep_CriticalProc, BlueprintReadWrite, Category="ODAttribute|Proc",
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CriticalProc;
	
private:
	// ========== OnRep Functions ==========

	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Accuracy(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CriticalChance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CriticalDamage(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_DamageIncrease(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CrowdControlAccuracy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_Evasion(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CrowdControlResistance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CrowdControlRecovery(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_EvasionProc(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CriticalProc(const FGameplayAttributeData& OldValue);
};
