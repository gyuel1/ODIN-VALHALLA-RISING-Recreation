// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ODCharacter.h"
#include "AbilitySystemInterface.h"
#include "ODCombatCharacter.generated.h"

class UODDamageReceivedComponent;
class UWidgetComponent;
class UODWidget_DamageQueContainer;
class UODCombatComponent;
class UODAttributeSet;
class UAttributeSet;
class UODAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCombatCharacterDele);

UCLASS()
class OD_API AODCombatCharacter : public AODCharacter, public IAbilitySystemInterface
{
	
	GENERATED_BODY()

public:
	AODCombatCharacter();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const;
	virtual void InitAbilitySystem();

	void OnAutoTargetedOutline();
	void OnAutoTargetedOutlineCleared();

	//AutoTargeted
	UPROPERTY(BlueprintAssignable)
	FCombatCharacterDele OnAutoTargetedDele;
	virtual void OnAutoTargeted();

	UPROPERTY(BlueprintAssignable)
	FCombatCharacterDele OnAutoTargetClearedDele;
	virtual void OnAutoTargetCleared();

	//OnSelectTargeted
	UPROPERTY(BlueprintAssignable)
	FCombatCharacterDele OnSelectTargetedDele;
	virtual void OnSelectTargeted();

	UPROPERTY(BlueprintAssignable)
	FCombatCharacterDele OnSelectTargetClearedDele;
	virtual void OnSelectTargetCleared();

	UFUNCTION(NetMulticast, Reliable)
	virtual void Muliticast_AddDamage(float Damage, bool bCritical = false);

	virtual AActor* GetTarget() const;

	
	FORCEINLINE UODCombatComponent* GetODCombatComponent() const {return ODCombatComponent;};
	FORCEINLINE UODDamageReceivedComponent* GetODDamageReceivedComponent() const {return ODDamageReceivedComponent;};
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UODWidget_DamageQueContainer> DamageQueContainerWidget;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UODAbilitySystemComponent> ODAsc;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UODAttributeSet> ODAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UODCombatComponent> ODCombatComponent;
	
	UPROPERTY()
	TObjectPtr<UODDamageReceivedComponent> ODDamageReceivedComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> DamageContainerWidgetComponent;

	
};
