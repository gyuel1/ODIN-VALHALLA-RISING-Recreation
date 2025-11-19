// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Skill/Active/OD_GA_Active_Base.h"
#include "OD_GA_Active_Valkyrie_002.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Active_Valkyrie_002 : public UOD_GA_Active_Base
{
	GENERATED_BODY()
public:
	UOD_GA_Active_Valkyrie_002();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	//Skill Value
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	float DashSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	float ArriveRange;

	//Dash Tick
	FTSTicker::FDelegateHandle DashTickHandle;


	void StartDash();
	void EndDash();
	bool TickDash(float DeltaSeconds);
	

	//Montage Dele
	UFUNCTION()
	void OnMontageComplete();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendOut();

	// UFUNCTION(NetMulticast,Reliable)
	// void SetActorLocation_NetMulticast(AODPlayer* InPlayer, const FVector InLocation );
	UFUNCTION(Client,Reliable)
	void SetActorLocation_Client(AODPlayer* InPlayer, const FVector InLocation );

	//Notify Dele
	UFUNCTION()
	void OnHitNotifyEvent(const FGameplayEventData InData);
	//Notify Dele
	UFUNCTION()
	void OnDashNotifyEvent(const FGameplayEventData InData);
	UFUNCTION()
	void OnRemoveNotifyEvent(const FGameplayEventData InData);
	
	
	
	
	
};
