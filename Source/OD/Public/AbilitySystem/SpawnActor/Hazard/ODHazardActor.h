// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SpawnActor/ODAbilitySpawnActorBase.h"
#include "ODHazardActor.generated.h"

class USphereComponent;

USTRUCT()
struct FTargetHitTimer
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AODCombatCharacter> Target;

	UPROPERTY()
	float Timer;

	FTargetHitTimer()
	{
		Target.Reset();
		Timer = 0.0f;
	}
	FTargetHitTimer(AODCombatCharacter* InTarget)
	{
		Target = InTarget;
		Timer = 0.0f;
	}
	
	bool operator==(const FTargetHitTimer& Other) const
	{
		return Target == Other.Target;
	}
};

UCLASS()
class OD_API AODHazardActor : public AODAbilitySpawnActorBase
{
	GENERATED_BODY()

public:
	AODHazardActor();
	
	virtual void InitAbilityInfo(UAbilitySystemComponent* InOwnerASC, AODCombatCharacter* InTarget, const FODSkillParameters InSkillParameters, AGameplayCueNotify_Actor* InCueActor = nullptr) override;
	
	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaSeconds) override;

	bool HasHazardTimer(AODCombatCharacter* InTarget);
	

private:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(AllowPrivateAccess=true))
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY()
	FODSkillHazardParams HazardPrams;
	
	UPROPERTY()
	float HazardTickTimer;
	
	UPROPERTY()
	TArray<FTargetHitTimer> HitTargetTimers;

	UPROPERTY()
	TSet<AODCombatCharacter*> InHazardTargets;

private:
	UFUNCTION()
	void OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
										   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										   bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
};
