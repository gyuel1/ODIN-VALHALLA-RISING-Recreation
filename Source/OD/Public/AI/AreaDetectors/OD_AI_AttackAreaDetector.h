// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ODRepGraphInterface.h"
#include "OD_AI_AttackAreaDetector.generated.h"

class AODMonsterBase;
class AODPlayer;

UCLASS()
class OD_API AOD_AI_AttackAreaDetector : public AActor, public IODRepGraphInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOD_AI_AttackAreaDetector();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly, Instanced, Category ="AttakArea")
	TObjectPtr<UShapeComponent> CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category ="AttakArea")
	TObjectPtr<UDecalComponent> DecalComponent;

#pragma region PlayerDetected
public:
	TArray<TObjectPtr<AODPlayer>> CollectValidTargets();
	
private:
	UPROPERTY()
	TSet<TWeakObjectPtr<AODPlayer>> InAreaSet;
	
	UPROPERTY()
	TSet<TWeakObjectPtr<AODPlayer>> AlreadyProcessed;
#pragma endregion

#pragma region Actor Configure
public:
	UFUNCTION()
	void Configure(float Duration, float TickInterval);
	
	UFUNCTION()
	void InitializeInitialOverlaps();

	UFUNCTION()
	void SetCachedMonster(AODMonsterBase* Monster);
	
private:
	UPROPERTY()
	float DurationSec = 0.f;

	UPROPERTY()
	float TickIntervalSec  = 0.f;

	UPROPERTY()
	TWeakObjectPtr<AODMonsterBase> CachedMonster;
#pragma endregion

#pragma region Tick Damage
public:
	UFUNCTION()
	void ApplyTickDamage();
private:
	FTimerHandle TickDamageHandle;
#pragma endregion

#pragma region RepGraph
public:	
	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;

	UFUNCTION()
	void DestroySelf();

	UFUNCTION()
	void BeginShutdown();

private:
	UPROPERTY()
	int32 RepGraphCount = 0;

	FTimerHandle ShutdownHandle;
	FTimerHandle DestroyDelayHandle;
#pragma endregion
	
#pragma region Getter
public:
	UFUNCTION()
	TSet<TWeakObjectPtr<AODPlayer>> GetInAreaSet();
#pragma endregion

#pragma region Setter
	
#pragma endregion
};
