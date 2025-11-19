// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/ODRepGraphInterface.h"
#include "ODAIController.generated.h"

class AODPlayer;
class UAISenseConfig_Damage;
struct FAIStimulus;
class AODMonsterBase;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class OD_API AODAIController : public AAIController , public IODRepGraphInterface
{
	GENERATED_BODY()

public:
	AODAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual int32 GetRepGraphCount() override;
	virtual void SetRepGraphCount(const int32 InRepGraph) override;
private:
	UPROPERTY()
	int32 RepGraphCount;

private:
	UPROPERTY(EditDefaultsOnly, Category="Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category="Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditDefaultsOnly, Category="Perception")
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	
	UPROPERTY()
	TObjectPtr<AODMonsterBase> Monster;

	UPROPERTY(EditDefaultsOnly, Category = "AI Behavior")
	FName TargetBlackBoardKeyName = "TargetActor";

public:
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void StopBehaviorTree();

#pragma region AI Behavior
private:
	UPROPERTY(EditDefaultsOnly, Category="AI Behavior")
	TObjectPtr<UBehaviorTree> GruntBT;

	UPROPERTY(EditDefaultsOnly, Category="AI Behavior")
	TObjectPtr<UBehaviorTree> BossBT;
#pragma endregion

#pragma region Boss Target Logic
private:	
	UPROPERTY()
	TSet<TWeakObjectPtr<AODPlayer>> SightPlayers;

public:
	UFUNCTION()
	void ClearSightPlayers();
#pragma endregion

#pragma region Getter
public:
	bool GetCurrentTarget();

	UFUNCTION()
	UAIPerceptionComponent* GetPerceptionComponent();

	UFUNCTION()
	UAISenseConfig_Sight* GetSenseConfig();

	UFUNCTION()
	TSet<TWeakObjectPtr<AODPlayer>> GetSightPlayers();
#pragma endregion

#pragma region Setter
public:
	void SetCurrentTarget(AActor* NewTarget);
	

#pragma endregion
	
};
