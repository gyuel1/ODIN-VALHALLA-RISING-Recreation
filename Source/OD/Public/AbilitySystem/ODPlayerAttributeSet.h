// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Tags/ODAbilityTags.h"
#include "ODPlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

using namespace GameplayEffectTags;

UCLASS()
class OD_API UODPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UODPlayerAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	
	ATTRIBUTE_ACCESSORS(UODPlayerAttributeSet, Gold);
	ATTRIBUTE_ACCESSORS(UODPlayerAttributeSet, CurrentExp)
	ATTRIBUTE_ACCESSORS(UODPlayerAttributeSet, MaxExp)

protected:
	
	//Gold
	UPROPERTY(ReplicatedUsing=OnRep_Gold, BlueprintReadWrite,
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Gold;

	//CurrentEXp
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExp, BlueprintReadWrite,
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CurrentExp;

	//MaxExp
	UPROPERTY(ReplicatedUsing=OnRep_MaxExp, BlueprintReadWrite,
		meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxExp;
	
	
	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_CurrentExp(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxExp(const FGameplayAttributeData& OldValue);
	
};
