// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Structs/StatsDataStruct.h"
#include "ODCombatComponent.generated.h"

struct FGameplayAbilitySpec;
class UODAbilityDA;
class AODCombatCharacter;
class UODActiveSkillDA;
class UODPassiveSkillDA;
class UODSkillDA;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OD_API UODCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UODCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ODCombatInitialize(AODCombatCharacter* InOwner);
	
	FGameplayAbilitySpecHandle FindAbilitySpecHandleByTag(const FGameplayTag& InTag);
	FGameplayAbilitySpec* FindAbilitySpecByTag(const FGameplayTag& InTag, UAbilitySystemComponent* InASC);
	
	//스텟 초기화
	virtual void InitStats();

	FORCEINLINE const FStatsDataStruct& GetBaseStats() const {return Stats;};

protected:
	
	//전투 스텟
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats",meta = (AllowPrivateAccess = true))
	FStatsDataStruct Stats;
	

	//몽타주가 있는 현재 캐릭터만의 고유 어빌리티 배열
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category="BasicAbility", meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UODAbilityDA>> ActiveAbilityArray;
	

	//====================부여된 어빌리티 스펙을 담고 있는 배열=======================//
	
	//패시브 어빌리티 이펙트 스펙 - 배우면 패시브 컨테이너에 등록 (부여및 첫 실행)
	UPROPERTY()
	TMap<FGameplayTag, FActiveGameplayEffectHandle> PassiveAbilityEffectHandle;
	
	//액티브 어빌리티 어빌리티 스펙 - 배우면 액티브 컨테이너에 등록 (부여만)  포함
	UPROPERTY()
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> ActiveAbilityHandle;


};
