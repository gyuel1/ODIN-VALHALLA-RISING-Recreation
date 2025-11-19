// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Skill/Active/OD_GA_Active_Base.h"
#include "AbilitySystem/SpawnActor/DelayAttack/ODDelayAttackActor.h"
#include "OD_GA_Active_Valkyrie_003.generated.h"

USTRUCT()
struct FDelayAttackLink
{
	GENERATED_BODY()

	UPROPERTY()
	AODCombatCharacter* Enemy;

	UPROPERTY()
	AODDelayAttackActor* DelayAttackActor;


	FDelayAttackLink()
	{
		Enemy = nullptr;
		DelayAttackActor = nullptr;
	}

	FDelayAttackLink(AODCombatCharacter* InEnemy, AODDelayAttackActor* InDelayAttackActor)
	{
		Enemy = InEnemy;
		DelayAttackActor = InDelayAttackActor;
	}
	FDelayAttackLink(AODCombatCharacter* InEnemy)
	{
		Enemy = InEnemy;
		DelayAttackActor = nullptr;
	}

	bool operator== (const FDelayAttackLink& Other) const
	{
		return Other.Enemy == Enemy;
	}
	
};

class AODAbilitySpawnActorBase;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Active_Valkyrie_003 : public UOD_GA_Active_Base
{
	GENERATED_BODY()

public:
	UOD_GA_Active_Valkyrie_003();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

private:
	//딜레이어택
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category="Delay Attack",meta=(AllowPrivateAccess=true))
	TSubclassOf<AODDelayAttackActor> DelayActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category="Delay Attack",meta=(AllowPrivateAccess=true))
	FODSkillDelayAttackParams  DelayAttackParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,category="Range Attack", meta=(AllowPrivateAccess=true))
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,category="Range Attack", meta=(AllowPrivateAccess=true))
	float AttackOffsetRange;

	

	UPROPERTY()
	TArray<FDelayAttackLink> DelayAttackArray;

	int32 CheckAttack;

	//Montage Dele
	UFUNCTION
	()
	void OnMontageComplete();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendOut();

	//SpawnActorSet
	UFUNCTION()
	void OnSpawnSuccess(AActor* SpawnedActor);
	UFUNCTION()
	void OnSpawnFail(AActor* SpawnedActor);

	//Notify Dele
	UFUNCTION()
	void OnHitNotifyEvent(const FGameplayEventData InData);
	UFUNCTION()
	void OnSpawnNotifyEvent(const FGameplayEventData InData);
	UFUNCTION()
	void OnRemoveNotifyEvent(const FGameplayEventData InData);
};
