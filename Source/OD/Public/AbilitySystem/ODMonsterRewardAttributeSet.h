// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ODMonsterRewardAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class OD_API UODMonsterRewardAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UODMonsterRewardAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	
	//Gold
	UPROPERTY(ReplicatedUsing=OnRep_Gold, BlueprintReadWrite, 
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Gold;
	
	
	//CurrentEXp
	UPROPERTY(ReplicatedUsing=OnRep_Exp, BlueprintReadWrite, 
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Exp;

	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Exp(const FGameplayAttributeData& OldValue);
	
	
};
