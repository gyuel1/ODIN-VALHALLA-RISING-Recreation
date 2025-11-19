// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SpawnActor/DelayAttack/ODDelayAttackActor.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/ODCombatCharacter.h"


AODDelayAttackActor::AODDelayAttackActor()
	:Super()
{
	
	TickTrigger = false;
	DestroyTrigger = false;
	CheckDelaySeconds = 0.f;
	
}

void AODDelayAttackActor::InitAbilityInfo(UAbilitySystemComponent* InOwnerASC, AODCombatCharacter* InTarget,
	const FODSkillParameters InSkillParameters, AGameplayCueNotify_Actor* InCueActor)
{
	Super::InitAbilityInfo(InOwnerASC, InTarget, InSkillParameters, InCueActor);
	
	DelayAttackParams = InSkillParameters.DelayAttackParams;

	
}

void AODDelayAttackActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AODDelayAttackActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (HasAuthority())
	{
		if (!Target.IsValid() || !OwnerASC.IsValid() || DestroyTrigger)
			Destroy();
		
		if (TickTrigger)
		{
			if (CheckDelaySeconds >= DelayAttackParams.DealySeconds)
			{
				OnParamsDamageToTarget(DelayAttackParams.DamageType,
				                       DelayAttackParams.DelayAttackPercent,
				                       Target.Get());

				//여기서 이펙트 실행
				OnHitNiagaraSystem(Target.Get());
				DestroyTrigger = true;
				TickTrigger = false;
			}
			CheckDelaySeconds += DeltaSeconds;
		}
	}
}

void AODDelayAttackActor::OnTickTrigger()
{
	TickTrigger = true;
}





