// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GameFramework/Actor.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODAbilitySpawnActorBase.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class UODGameplayEffect;
class AODCombatCharacter;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EODSkillDamageType : uint8
{
	None,    
	BaseValue,
	PercentOfPercent
};

USTRUCT(BlueprintType)
struct FODSkillDamageParms
{
	GENERATED_BODY()
	
	UPROPERTY()
	float DamagePercent = 100.f;
	UPROPERTY()
	float DamageCount = 1.f;

	FODSkillDamageParms()
	{
	}
	
	FODSkillDamageParms(const float& InDamagePercent,const float InDamageCount)
	{
		DamagePercent = InDamagePercent;
		DamageCount = InDamageCount;
	}
};

USTRUCT(BlueprintType)
struct  FODSkillProjectileParms
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed = 0;

	FODSkillProjectileParms()
	{
		ProjectileSpeed = 0;
	}

	FODSkillProjectileParms(const float InProjectileSpeed)
	{
		ProjectileSpeed = InProjectileSpeed;
	}
};

USTRUCT(BlueprintType)
struct FODSkillHazardParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EODSkillDamageType HazardDamageType = EODSkillDamageType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HazardDamagePercent  = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HazardRange = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartHazardTick = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoopHazardTick = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasSpawnDamage = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LoopHazardTickCount = 1;
	
	FODSkillHazardParams()
	{
		HazardDamageType = EODSkillDamageType::None;
		HazardDamagePercent = 1.f;
		HazardRange = 250.f;
		StartHazardTick = 0.f;
		LoopHazardTick = 0.5f;
		HasSpawnDamage = false;
		LoopHazardTickCount = 1;
		
	}

	FODSkillHazardParams(EODSkillDamageType InHazardDamageType,
						const float& InHazardDamagePercent,
						const float& InHazardRange,
						const float& InStartHazardTick = 0.0f,
						const float& InLoopHazardTick = 0.5,
						bool InHasSpawnDamage = false,
						const int32& InHLoopHazardTickCount = 1)
	{
		HazardDamageType = InHazardDamageType;
		HazardDamagePercent = InHazardDamagePercent;
		HazardRange = InHazardRange;
		StartHazardTick = InStartHazardTick;
		LoopHazardTick = InLoopHazardTick;
		HasSpawnDamage = InHasSpawnDamage;
		LoopHazardTickCount = InHLoopHazardTickCount;
	}
};
USTRUCT(BlueprintType)
struct FODSkillDelayAttackParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EODSkillDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DealySeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayAttackPercent;
	

	FODSkillDelayAttackParams()
	{
		DamageType = EODSkillDamageType::None;
		DealySeconds = 0.f;
		DelayAttackPercent = 0.f;
	}
	FODSkillDelayAttackParams(const EODSkillDamageType InDamageType,const float& InDealyCount, const float& InDelayAttackPercent)
	{
		DamageType = InDamageType;
		DealySeconds = 0.f;
		DelayAttackPercent = 0.f;
	}
	
};


USTRUCT(BlueprintType)
struct FODSkillParameters
{
	GENERATED_BODY()

	UPROPERTY()
	FODSkillDamageParms DamageParms = FODSkillDamageParms();

	UPROPERTY()
	FODSkillProjectileParms ProjectileParms = FODSkillProjectileParms();

	UPROPERTY()
	FODSkillHazardParams HazardPrams =  FODSkillHazardParams();

	UPROPERTY()
	FODSkillDelayAttackParams DelayAttackParams = FODSkillDelayAttackParams();
	

	FODSkillParameters()
	{
	}

	//Basic
	FODSkillParameters(const FODSkillDamageParms& InDamageParms)
	{
		DamageParms = InDamageParms;
	}
	
	//ProjectileParms
	FODSkillParameters(const FODSkillDamageParms& InDamageParms, const FODSkillProjectileParms& InProjectileParms)
	{
		DamageParms = InDamageParms;
		ProjectileParms = InProjectileParms;
	}

	//HazardPrams (!HasOnDamage)
	FODSkillParameters(const FODSkillHazardParams& InHazardPrams)
	{
		HazardPrams = InHazardPrams;
		HazardPrams.HasSpawnDamage = false;
	}
	
	//HazardPrams (HasOnDamage)
	FODSkillParameters(const FODSkillDamageParms& InDamageParms, const FODSkillHazardParams& InHazardPrams)
	{
		DamageParms = InDamageParms;
		HazardPrams = InHazardPrams;
	}
	FODSkillParameters(const FODSkillDamageParms& InDamageParms, const FODSkillDelayAttackParams& InDelayAttackParams)
	{
		DamageParms = InDamageParms;
		DelayAttackParams = InDelayAttackParams;
	}

	
	
};


UCLASS()
class OD_API AODAbilitySpawnActorBase : public AActor , public IODRepGraphInterface
{
	GENERATED_BODY()

	//Life Cycle
public:	
	AODAbilitySpawnActorBase();
	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreRegisterAllComponents() override;
	
protected:
	virtual void BeginPlay() override;

	//RepGraphInterface
public:
	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

private:
	int32 RepGraphCount = -1;

	//Custom Func
public:
	virtual void InitAbilityInfo(UAbilitySystemComponent* InOwnerASC,
	                     AODCombatCharacter* InTarget,
	                     const FODSkillParameters InSkillParameters,
	                     AGameplayCueNotify_Actor* InCueActor = nullptr);
	
protected:
	virtual void OnDamageToTarget();
	virtual void OnDamageToTarget(AODCombatCharacter* InTarget);
	virtual void OnParamsDamageToTarget(const EODSkillDamageType InDamageType, const float& InActorDamagePercent, AODCombatCharacter* InTarget);

	virtual bool IsCanTargetingToEnemy(AODCombatCharacter* InTarget);

	UFUNCTION(Client, Reliable)
	virtual void OnHitNiagaraSystem(AODCombatCharacter* InTarget);

	//virtual void NiagaraPlayOnceDestroy(UNiagaraSystem* InNiagaraSystem);

	//Root
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> Root;
	
	//UNiagaraComponent
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "NiagaraSystem")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;


	UPROPERTY(EditDefaultsOnly,BlueprintReadwrite)
	TObjectPtr<UNiagaraComponent> HitkNiagaraSystemComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "NiagaraSystem")
	TObjectPtr<UDecalComponent> DecalComponent;
	
	//Ability Info
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> OwnerASC;
	UPROPERTY()
	TWeakObjectPtr<AODCombatCharacter> Target;

	//OnDamageEffect;
	UPROPERTY()
	TSubclassOf<UODGameplayEffect> OnDamageEffect;

	//SkillParameters
	UPROPERTY()
	FODSkillParameters SkillParameters;
	
	//Target Location Cache
	UPROPERTY(Replicated)
	FVector TargetLocation;
	

	//cue?

	
	
	
};
