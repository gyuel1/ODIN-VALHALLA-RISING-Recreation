// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/Skill/OD_GA_Skill_Base.h"
#include "AbilitySystem/SpawnActor/ODAbilitySpawnActorBase.h"
#include "Components/ODSkillComponent.h"
#include "DataAssets/Ability/ODActiveSkillDA.h"
#include "Tags/ODAbilityTags.h"
#include "OD_GA_Active_Base.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

using namespace GameplayAbilityTags;
using namespace AttributeDataTags;
using namespace GameplayEffectTags;
using namespace GameplayAbilitySkillTags;
using namespace StateTags;

UENUM()
enum class EHasAbilitySpawnActor
{
	None,
	HasSpawnActor
};
UENUM()
enum class EHasAbilityHitNiagara
{
	None,
	HasNiagara
};


UCLASS()
class OD_API UOD_GA_Active_Base : public UOD_GA_Skill_Base
{
	GENERATED_BODY()
	
public:
	UOD_GA_Active_Base();

	//================= Ability  ===================//

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
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	
	//================= Cost  ===================//

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	
	//================= Cooldown  ===================//

	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	UFUNCTION(Client, Reliable)
	virtual void OnSkillCooldown(UODSkillComponent* InODSkillComponent) const;
	

	//==================SkillData===================//

	virtual UODActiveSkillDA* GetActiveSkillDA(const FGameplayAbilityActorInfo* ActorInfo) const;
	
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> AbilityMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float MontageRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	EHasAbilitySpawnActor HasAbilitySpawnActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta=(AllowPrivateAccess=true, EditCondition="HasAbilitySpawnActor == EHasAbilitySpawnActor::HasSpawnActor",
			EditConditionHides))
	TSubclassOf<AODAbilitySpawnActorBase> AbilitySpawnActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	EHasAbilityHitNiagara HasAbilityHitNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta=(AllowPrivateAccess=true, EditCondition="HasAbilityHitNiagara == EHasAbilityHitNiagara::HasNiagara",
			EditConditionHides))
	TObjectPtr<UNiagaraSystem> HitNiagaraSystem;

	UFUNCTION(Client, Reliable)
	virtual void OnHitNiagaraSystem(AActor* InTarget);
	

	
	
	UPROPERTY()
	FGameplayTagContainer CooldownTags;
	
	FGameplayEffectSpecHandle CreateHitEffect(const TSubclassOf<UGameplayEffect>& InEffect,
	                                          const float& SkillDamagePercent = 100.f,
	                                          const float& SkillAttackCount = 1.f,
	                                          const FGameplayTag& InTag = FGameplayTag(OD_Effect_Combat_OnDamage));
	
	virtual bool LockPlayerInput();
	virtual void UnlockPlayerInput();
};


