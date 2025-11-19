// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SpawnActor/ODAbilitySpawnActorBase.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Components/DecalComponent.h"
#include "Debug/ODLogChannels.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ReplicationGraph/ODReplicationGraph.h"
#include "Tags/ODAbilityTags.h"

AODAbilitySpawnActorBase::AODAbilitySpawnActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	
	NetUpdateFrequency = 100.f;          
	MinNetUpdateFrequency = 66.f;
	bAlwaysRelevant = true;
	
	RepGraphCount = 0;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	RootComponent->SetMobility(EComponentMobility::Movable);
	SetNetDormancy(DORM_Never);
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraComponent->SetupAttachment(Root);

	HitkNiagaraSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitkNiagaraSystemComponent"));
	HitkNiagaraSystemComponent->SetupAttachment(Root);
	HitkNiagaraSystemComponent->SetAutoActivate(false);
	HitkNiagaraSystemComponent->SetActive(true);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(Root);
	
	OnDamageEffect = UOD_GE_Combat_OnDamage::StaticClass();
	
	SkillParameters = FODSkillParameters();
	
	//거리별 최적화
        bAlwaysRelevant = false;
        bOnlyRelevantToOwner = false;
        bNetUseOwnerRelevancy = false;
        NetCullDistanceSquared = FMath::Square(6000.f);

	TargetLocation = FVector();
}


void AODAbilitySpawnActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AODAbilitySpawnActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AODAbilitySpawnActorBase, TargetLocation);
}

void AODAbilitySpawnActorBase::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();

	if (!DecalComponent->GetDecalMaterial())
	{
		DecalComponent->SetVisibility(false);
	}
}



void AODAbilitySpawnActorBase::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true); 
}

int32 AODAbilitySpawnActorBase::GetRepGraphCount()
{
	return RepGraphCount;
}

void AODAbilitySpawnActorBase::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}

void AODAbilitySpawnActorBase::InitAbilityInfo(UAbilitySystemComponent* InOwnerASC,
						 AODCombatCharacter* InTarget,
						 const FODSkillParameters InSkillParameters,
						 AGameplayCueNotify_Actor* InCueActor)
{
	if (!IsValid(InOwnerASC) || !IsValid(InTarget))
	{
		Destroy();
		return;
	}
	OwnerASC = InOwnerASC;
	Target = InTarget;
	
	AODCombatCharacter* MakeOwner = Cast<AODCombatCharacter>(OwnerASC->GetAvatarActor());
	if (!MakeOwner)
		Destroy();
	SetRepGraphCount(MakeOwner->GetRepGraphCount());

	TargetLocation = Target.Get()->GetActorLocation();
	SkillParameters = InSkillParameters;

	
	if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(OwnerASC->GetAvatarActor()))
	{
		SetRepGraphCount(RGI->GetRepGraphCount());
		UE_LOG(LogOD,Warning,TEXT("%d"),GetRepGraphCount());
	}

	if (MakeOwner->GetController())
	{
		SetOwner(MakeOwner->GetController());
	}
		
	
}

void AODAbilitySpawnActorBase::OnDamageToTarget()
{
	if (Target.IsValid())
		OnDamageToTarget(Target.Get());
}

void AODAbilitySpawnActorBase::OnDamageToTarget(AODCombatCharacter* InTarget)
{
	if (!OwnerASC.IsValid() || !InTarget || !OnDamageEffect || !HasAuthority())
		return;
	
	if (AODCombatCharacter* ActiveCharacter = Cast<AODCombatCharacter>(OwnerASC.Get()->GetAvatarActor()))
	{
		if (IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(InTarget))
		{
			if (UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent())
			{
				//Make EffectSpec
				FGameplayEffectContextHandle EffectContextHandle = OwnerASC.Get()->
					MakeEffectContext();
				EffectContextHandle.AddSourceObject(ActiveCharacter);
					
				FGameplayEffectSpecHandle SpecHandle = OwnerASC.Get()->MakeOutgoingSpec(OnDamageEffect, 1, EffectContextHandle);
					

				FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
				Spec.AddDynamicAssetTag(GameplayEffectTags::OD_Effect_Combat_OnDamage);

				const FODSkillDamageParms MakeDamageParams = SkillParameters.DamageParms;

				Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_DamagePercent, MakeDamageParams.DamagePercent);
				Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_AttackCount, MakeDamageParams.DamageCount);
					
				OwnerASC.Get()->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(), TargetASC);
			}
		}
	}
}

void AODAbilitySpawnActorBase::OnParamsDamageToTarget(const EODSkillDamageType InDamageType,const float& InActorDamagePercent,
	AODCombatCharacter* InTarget)
{
	
	if (!OwnerASC.IsValid() || !InTarget || !OnDamageEffect || !HasAuthority() || InDamageType == EODSkillDamageType::None )
		return;

	if (AODCombatCharacter* ActiveCharacter = Cast<AODCombatCharacter>(OwnerASC.Get()->GetAvatarActor()))
	{
		if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(InTarget))
		{
			if (UAbilitySystemComponent* TargetAsc = AscInterface->GetAbilitySystemComponent())
			{
				//Make EffectSpec
				FGameplayEffectContextHandle EffectContextHandle = OwnerASC.Get()->
																			MakeEffectContext();
				EffectContextHandle.AddSourceObject(ActiveCharacter);

				FGameplayEffectSpecHandle SpecHandle = OwnerASC.Get()->MakeOutgoingSpec(
					OnDamageEffect, 1, EffectContextHandle);

				FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
				Spec.AddDynamicAssetTag(GameplayEffectTags::OD_Effect_Combat_OnDamage);


				//Hazard DamagePercent Set
				const FODSkillDamageParms MakeDamageParams = SkillParameters.DamageParms;
				float MakeDamagePercent = 1.f;

				if (InDamageType == EODSkillDamageType::BaseValue)
				{
					MakeDamagePercent = InActorDamagePercent;
				}
				else if (InDamageType == EODSkillDamageType::PercentOfPercent)
				{
					MakeDamagePercent = MakeDamageParams.DamagePercent * (InActorDamagePercent / 100.f);
				}

				Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_DamagePercent, MakeDamagePercent);
				Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_AttackCount, 1.f);

				//ApplyEffect
				OwnerASC.Get()->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(), TargetAsc);
			}
		}
	}
}

bool AODAbilitySpawnActorBase::IsCanTargetingToEnemy(AODCombatCharacter* InTarget)
{
	if (!OwnerASC.IsValid() || !InTarget)
		return false;

	if (OwnerASC->GetAvatarActor())
	{
		if (OwnerASC->GetAvatarActor()->IsA(AODMonsterBase::StaticClass()))
		{
			return InTarget->IsA(AODPlayer::StaticClass());
		}

		if (OwnerASC->GetAvatarActor() && OwnerASC->GetAvatarActor()->IsA(AODPlayer::StaticClass()))
		{
			if (InTarget->IsA(AODMonsterBase::StaticClass()))
			{
				return true;
			}
		}
	}
	return false;
}

void AODAbilitySpawnActorBase::OnHitNiagaraSystem_Implementation(AODCombatCharacter* InTarget)
{
	if (!HitkNiagaraSystemComponent || !HitkNiagaraSystemComponent->GetAsset() || !InTarget)
		return;

	UNiagaraSystem* MakeNiagaraSystem = HitkNiagaraSystemComponent->GetAsset();
	if (!MakeNiagaraSystem)
		return;
	

	UNiagaraComponent* Com = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MakeNiagaraSystem,
		GetActorLocation(),
		GetActorRotation());

	if (Com)
	{
		if (InTarget->GetRootComponent())
			Com->AttachToComponent(InTarget->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

