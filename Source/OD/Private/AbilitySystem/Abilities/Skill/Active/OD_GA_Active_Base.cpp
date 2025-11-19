// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/OD_GA_Active_Base.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Skill/OD_GE_Skill_Cooldown.h"
#include "AbilitySystem/Effect/Skill/OD_GE_Skill_Cost.h"
#include "Character/ODPlayer.h"
#include "Debug/ODLogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags/ODAbilityTags.h"


UOD_GA_Active_Base::UOD_GA_Active_Base()
	: Super()
{
	ActivationBlockedTags.AddTag(OD_Ability_Block);

	MontageRate = 1.f;

	CostGameplayEffectClass = UOD_GE_Skill_Cost::StaticClass();
	CooldownGameplayEffectClass = UOD_GE_Skill_Cooldown::StaticClass();

	HasAbilitySpawnActor = EHasAbilitySpawnActor::None;
}

bool UOD_GA_Active_Base::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayTagContainer* SourceTags,
                                            const FGameplayTagContainer* TargetTags,
                                            FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	if (HasAbilitySpawnActor == EHasAbilitySpawnActor::HasSpawnActor)
	{
		if (!AbilitySpawnActor)
		{
			UE_LOG(LOGODAbility,Warning,TEXT("소환 액터 유무에 대해 HasSpawnActor 설정을 했으나 소환할 액터를 등록하지 않았음. HasSpawnActor Is Null"));	
			return false;
		}
	}
			
	
	
	if (AODPlayer* Player = Cast<AODPlayer>(ActorInfo->AvatarActor.Get()))
	{
		
		if (AActor* Target = Player->GetTarget())
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
				Target))
			{
				
				if (ASC->HasMatchingGameplayTag(OD_State_Dead))
					return false;

				
				if (UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo))
				{
					const float SkillRange = DA->GetActiveSkillData().SkillRange;

					const FVector& PLayerLoc = Player->GetActorLocation();
					const FVector& TargetLoc = Target->GetActorLocation();
					const float& Distance = FVector::Dist(PLayerLoc, TargetLoc);

					if (Distance <= SkillRange)
					{
						
						return true;
					}
				}
			}
		}
	}

	return false;
}

void UOD_GA_Active_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo);
	if (!DA || !AbilityMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AddLooseGameplayTag(OD_Ability_Block);
	}
	
	if (!LockPlayerInput())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}



	
	/*===============================
	 :	자식에서 EndAbility 꼭 만들기 (몽타주에 바인딩 되어있으면 괜찮음)
	 :	자식에서 CommitAbility 호출하기 (쿨타운,코스트 적용)
	 ===============================*/
	

	
}

void UOD_GA_Active_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	UnlockPlayerInput();
	
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(OD_Ability_Block);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_Base::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (AbilityTags.IsValid())
	{
		FGameplayTag MakeCooldownTag = FGameplayTag::RequestGameplayTag(
			FName(FString::Printf(TEXT("%s.Cooldown"), *AbilityTags.GetByIndex(0).ToString())));
		CooldownTags.AddTag(MakeCooldownTag);
	}
}

bool UOD_GA_Active_Base::CommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

bool UOD_GA_Active_Base::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   FGameplayTagContainer* OptionalRelevantTags) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
		return false;

	if (UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo))
	{
		const float& CurrentMana = ASC->GetNumericAttribute(UODAttributeSet::GetManaAttribute());
		const float& MakeSkillCost = DA->GetActiveSkillData().SkillCost;

		return CurrentMana > MakeSkillCost;
	}
	
	return false;
}




void UOD_GA_Active_Base::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo);
	if (!CostGameplayEffectClass || !DA)
		return;
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return;

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CostGameplayEffectClass, GetAbilityLevel(),
																 ContextHandle);

	FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
	Spec.AddDynamicAssetTag(GameplayEffectTags::OD_Effect_Skill_Cost);

	const float& MakeSKillCost = DA->GetActiveSkillData().SkillCost;
	Spec.SetSetByCallerMagnitude(OD_Magnitude_Skill_Cost, -MakeSKillCost);
	
	ApplyGameplayEffectSpecToOwner(Handle,ActorInfo,ActivationInfo,SpecHandle);

	//코스트 소모를 스킬별로 알려야하나? AttributeSet에서 OD_Magnitude_Skill_Cost 태그로 판단하고 Ui연동만 하면 되나?
	
}


bool UOD_GA_Active_Base::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UOD_GA_Active_Base::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo);
	if (!CooldownGameplayEffectClass || !DA)
		return;
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
		return;

	AODPlayer* Player = Cast<AODPlayer>(GetAvatarActorFromActorInfo());
	if (!Player)
		return;

	if (CooldownTags.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveEffectsWithTags(CooldownTags);
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownGameplayEffectClass,
																 GetAbilityLevel(), ContextHandle);

	FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();

	if (CooldownTags.IsValid())
		Spec.DynamicGrantedTags.AddTag(CooldownTags.GetByIndex(0));

	const float& MakeSkillCooldown =  DA->GetActiveSkillData().SkillCooldown;
	Spec.SetSetByCallerMagnitude(OD_Magnitude_Skill_Cooldown,MakeSkillCooldown);

	if (UODSkillComponent* SkillComponent = Player->GetODSkillComponent())
	{
		if (!AbilityTags.IsValid())
			return;
	
		if (FSkillSlot* SkillSlot = SkillComponent->FindSkillSlotByTag(AbilityTags.GetByIndex(0)))
		{
			const FActiveGameplayEffectHandle& EffectHandle = ApplyGameplayEffectSpecToOwner(
				Handle, ActorInfo, ActivationInfo, SpecHandle);
			if (K2_HasAuthority())
			{
				SkillSlot->OnSkillCooldown_Server(EffectHandle);
				//클라 동기화
				OnSkillCooldown(SkillComponent);
			}
		}
	}
}


const FGameplayTagContainer* UOD_GA_Active_Base::GetCooldownTags() const
{
	if (CooldownTags.IsValid())
		return &CooldownTags;

	return nullptr;
}

void UOD_GA_Active_Base::OnSkillCooldown_Implementation(UODSkillComponent* InODSkillComponent) const
{
	if (FSkillSlot* SkillSlot = InODSkillComponent->FindSkillSlotByTag(AbilityTags.GetByIndex(0)))
	{
		SkillSlot->OnSkillCooldown_Client(CooldownTags);
	}
}


UODActiveSkillDA* UOD_GA_Active_Base::GetActiveSkillDA(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		return nullptr;
	
	if (AODPlayer* Player = Cast<AODPlayer>( ActorInfo->AvatarActor.Get()))
	{
		if (UODSkillComponent* SC = Player->GetODSkillComponent())
		{
			if (!AbilityTags.IsValid())
				return nullptr;
		
			if (UODActiveSkillDA* MakeActiveSkillDA = SC->FindSkillDAByTag<UODActiveSkillDA>(AbilityTags.GetByIndex(0)))
			{
				return MakeActiveSkillDA;
			}
		}
	}
	return nullptr;
}


void UOD_GA_Active_Base::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
}


void UOD_GA_Active_Base::OnHitNiagaraSystem_Implementation(AActor* InTarget)
{
	if (!HitNiagaraSystem || !IsValid(InTarget))
		return;

	UNiagaraComponent* Com = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		HitNiagaraSystem,
		InTarget->GetActorLocation(),
		InTarget->GetActorRotation());

	if (Com)
	{
		if (InTarget->GetRootComponent())
			Com->AttachToComponent(InTarget->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}


FGameplayEffectSpecHandle UOD_GA_Active_Base::CreateHitEffect(const TSubclassOf<UGameplayEffect>& InEffect,
                                                              const float& SkillDamagePercent,
                                                              const float& SkillAttackCount, const FGameplayTag& InTag)
{
	if (!InEffect.Get())
		return FGameplayEffectSpecHandle();

	//Make EffectSpec
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->
		MakeEffectContext();
	EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->
		MakeOutgoingSpec(
			InEffect, 1, EffectContextHandle);

	FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
	
	Spec.AddDynamicAssetTag(OD_Effect_Combat_OnDamage);
	
	Spec.SetSetByCallerMagnitude(OD_Magnitude_DamagePercent, SkillDamagePercent);
	Spec.SetSetByCallerMagnitude(OD_Magnitude_AttackCount, SkillAttackCount);

	if (InTag.IsValid())
	{
		Spec.AddDynamicAssetTag(InTag);
	}

	return SpecHandle;
}

bool UOD_GA_Active_Base::LockPlayerInput()
{
	//이동막기  및 방향 동기화
	if (AODPlayer* MakeCharacter = Cast<AODPlayer>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
		{
			AActor* Target = nullptr;
			Target = MakeCharacter->GetTarget();
			if (Target)
			{
				//MakeMove->RotationRate = FRotator::ZeroRotator;
				FVector MyLoc = MakeCharacter->GetActorLocation();
				FVector TargetLoc = Target->GetActorLocation();
				FRotator LookAtRot = (TargetLoc - MyLoc).Rotation();

				MakeCharacter->SetActorRotation(FRotator(MakeCharacter->GetActorRotation().Roll, LookAtRot.Yaw,
														 MakeCharacter->GetActorRotation().Pitch));
				MakeMove->bOrientRotationToMovement = false;
				MakeMove->MaxWalkSpeed = 0.f;
				return true;
			}
		}
	}
	return false;
}

void UOD_GA_Active_Base::UnlockPlayerInput()
{
	if (AODPlayer* MakeCharacter = Cast<AODPlayer>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
		{
			MakeMove->bOrientRotationToMovement = true;
			MakeMove->MaxWalkSpeed = 600.f;
		}
	}
}
