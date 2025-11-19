// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ODDamageReceivedComponent.generated.h"


class AODCombatCharacter;

USTRUCT(BlueprintType)
struct FDamageReceivedCharacterInfo : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AODCombatCharacter> CombatCharacter;

	UPROPERTY()
	float AccumulatedDamage;

	
	FDamageReceivedCharacterInfo()
	{
		CombatCharacter = nullptr;
		AccumulatedDamage = 0.f;
	}
	FDamageReceivedCharacterInfo(AODCombatCharacter* InCharacter, const float& InDamage = 0.f)
	{
		CombatCharacter = InCharacter;
		AccumulatedDamage = InDamage;
	}

	void PreReplicatedRemove(const struct FDamageReceivedCharacterList& InArraySerializer);
	void PostReplicatedAdd(const struct FDamageReceivedCharacterList& InArraySerializer);
	void PostReplicatedChange(const struct FDamageReceivedCharacterList& InArraySerializer);
};

USTRUCT(BlueprintType)
struct FDamageReceivedCharacterList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDamageReceivedCharacterInfo> Items;

	FDamageReceivedCharacterList()
	{
		Items = {};
	}
	

	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FDamageReceivedCharacterInfo, FDamageReceivedCharacterList>( Items, DeltaParms, *this );
	}

	FDamageReceivedCharacterInfo* FindDamageReceivedCharacter(AODCombatCharacter* InCharacter)
	{
		for (FDamageReceivedCharacterInfo& Character : Items)
		{
			if (Character.CombatCharacter == InCharacter)
			{
				return &Character;
			}
		}
		return nullptr;
	}
};

template<>
struct TStructOpsTypeTraits< FDamageReceivedCharacterList > : public TStructOpsTypeTraitsBase2< FDamageReceivedCharacterList >
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OD_API UODDamageReceivedComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UODDamageReceivedComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(Server, Reliable)
	void AddDamageReceivedArray(AODCombatCharacter* InCharacter, const float InDamage);
	void AddDamageReceivedArray_Internal(AODCombatCharacter* InCharacter, float InDamage);

	UFUNCTION(Server, Reliable)
	void ClearDamageReceivedArray();
	void ClearDamageReceivedArray_Internal();

	FORCEINLINE const FDamageReceivedCharacterList& GetDamageReceivedArray() const {return DamageReceivedArray;};


private:
	UPROPERTY(Replicated)
	FDamageReceivedCharacterList DamageReceivedArray;

	
};
