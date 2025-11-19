// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SpawnActor/Hazard/ODHazardActor.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Effect/ODGameplayEffect.h"
#include "Character/ODCombatCharacter.h"
#include "Components/DecalComponent.h"

#include "Components/SphereComponent.h"
#include "Helper/ODStaticFunction.h"
#include "Tags/ODAbilityTags.h"


AODHazardActor::AODHazardActor()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Root);
	SphereComponent->SetSphereRadius(250.f);
	SphereComponent->SetGenerateOverlapEvents(true);

	HazardTickTimer = 0.f;
}

void AODHazardActor::InitAbilityInfo(UAbilitySystemComponent* InOwnerASC, AODCombatCharacter* InTarget,
                                     const FODSkillParameters InSkillParameters, AGameplayCueNotify_Actor* InCueActor)
{
	Super::InitAbilityInfo(InOwnerASC, InTarget, InSkillParameters, InCueActor);

	HazardPrams = SkillParameters.HazardPrams;
	SphereComponent->SetSphereRadius(HazardPrams.HazardRange);
	NiagaraComponent->SetRelativeScale3D(FVector(HazardPrams.HazardRange / 100.f));
	DecalComponent->DecalSize = FVector(HazardPrams.HazardRange /2 , HazardPrams.HazardRange,HazardPrams.HazardRange);

	
}

void AODHazardActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(
			this, &AODHazardActor::OnSphereCollisionBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(
			this, &AODHazardActor::OnSphereCollisionEndOverlap);
	}
}

void AODHazardActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.Clear();
		SphereComponent->OnComponentEndOverlap.Clear();
	}
}

void AODHazardActor::BeginPlay()
{
	Super::BeginPlay();

	/*================================
	 *	스폰시 데미지 있으면 OnDamage
	 *	스폰시 데미지가 없는데 StartTickSeconds 가 0이면 OnHazardDamage
	 *	범위내 적을 배열에 저장
	 ================================*/

	if (Target.IsValid())
	{
		NiagaraComponent->SetRelativeRotation(Target->GetActorRotation());
	}

	if (HasAuthority())
	{
		if (OwnerASC.IsValid() && OwnerASC->GetAvatarActor())
		{
			TArray<AODCombatCharacter*> BeginHitTargets = ODStaticFunc::DetectSphereInRange<AODCombatCharacter>(
				this, HazardPrams.HazardRange,ECC_GameTraceChannel3);

			for (AODCombatCharacter* BeginHitTarget : BeginHitTargets)
			{
				if (!IsCanTargetingToEnemy(BeginHitTarget))
					continue;

				//범위내 대상 체크 배열
				InHazardTargets.Add(BeginHitTarget);

				//공격당한 적의 HazardTick Timer
				FTargetHitTimer MakeHitTimer(BeginHitTarget);
				HitTargetTimers.Add(MakeHitTimer);

				if (HazardPrams.HasSpawnDamage)
				{
					OnHitNiagaraSystem(MakeHitTimer.Target.Get());
					OnDamageToTarget(MakeHitTimer.Target.Get());
				}
				else
				{
					if (HazardPrams.StartHazardTick >= 0.f)
					{
						OnParamsDamageToTarget(HazardPrams.HazardDamageType,
											   HazardPrams.HazardDamagePercent,
											   MakeHitTimer.Target.Get());
					}
				}
			}
		}
	}
}




/*================================
 *	대상의 시간을 체크 ->  대상이 범위 내에 있고 넘었으면 데미지 시간초기화
 *	대상이 처음 범위안으로 들어올때? 배열에 적이 있는지 확인하고 없으면 데미지 주고 추가
 *	대상이 범위밖으로 나갈때 -> 들어온 대상 배열에서 제거
 *
 *	그럼 그냥 배열 2개가 맞는건가?
 *
 *	지금 범위내에 있는지 없는지도 체크해야하고 시간도 체크해야하고 
 ================================*/


void AODHazardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		if (HazardTickTimer >= HazardPrams.StartHazardTick + (HazardPrams.LoopHazardTick * HazardPrams.
			LoopHazardTickCount))
			Destroy();

		if (HazardTickTimer >= HazardPrams.StartHazardTick)
		{
			TArray<FTargetHitTimer> RemoveArray;
			//Damage
			for (FTargetHitTimer& HitTarget : HitTargetTimers)
			{
				if (HitTarget.Target.IsValid() && HitTarget.Timer >= HazardPrams.LoopHazardTick)
				{
					if (InHazardTargets.Contains(HitTarget.Target.Get()))
					{
						OnParamsDamageToTarget(HazardPrams.HazardDamageType,
						                       HazardPrams.HazardDamagePercent,
						                       HitTarget.Target.Get());
						HitTarget.Timer -= HazardPrams.LoopHazardTick;
					}
					else
					{
						RemoveArray.Add(HitTarget);
					}
				}
				HitTarget.Timer += DeltaSeconds;
			}
			//Remove
			for (FTargetHitTimer& RemoveTarget : RemoveArray)
			{
				if (HitTargetTimers.Contains(RemoveTarget))
				{
					HitTargetTimers.Remove(RemoveTarget);
				}
			}
		}
		HazardTickTimer += DeltaSeconds;
	}
}

bool AODHazardActor::HasHazardTimer(AODCombatCharacter* InTarget)
{
	for (FTargetHitTimer& HitTarget : HitTargetTimers)
	{
		if (HitTarget.Target == InTarget)
			return true;
	}
	return false;
}

void AODHazardActor::OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	if (AODCombatCharacter* MakeTarget = Cast<AODCombatCharacter>(OtherActor))
	{
		if (IsCanTargetingToEnemy(MakeTarget))
		{
			if (!InHazardTargets.Contains(MakeTarget))
			{
				InHazardTargets.Add(MakeTarget);
				if (!HasHazardTimer(MakeTarget))
				{
					OnParamsDamageToTarget(HazardPrams.HazardDamageType,
											   HazardPrams.HazardDamagePercent,
											   MakeTarget);
					HitTargetTimers.Add(FTargetHitTimer(MakeTarget));
				}
			}
		}
	}
}

void AODHazardActor::OnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AODCombatCharacter* MakeTarget = Cast<AODCombatCharacter>(OtherActor))
	{
		if (InHazardTargets.Contains(MakeTarget))
			InHazardTargets.Remove(MakeTarget);
	}
}

