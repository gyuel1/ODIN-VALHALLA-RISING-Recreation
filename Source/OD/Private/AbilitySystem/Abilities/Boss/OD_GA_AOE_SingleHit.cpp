// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Boss/OD_GA_AOE_SingleHit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Engine/OverlapResult.h"
#include "Tags/ODAbilityTags.h"
#include "DrawDebugHelpers.h"

using namespace GameplayEffectTags;

UOD_GA_AOE_SingleHit::UOD_GA_AOE_SingleHit()
{
	MontageRate = 1.0f;
	DamagePercent = 100.f;
	AttackCount = 1.f;

	Effect = UOD_GE_Combat_OnDamage::StaticClass();
}

bool UOD_GA_AOE_SingleHit::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		if (AODCombatCharacter* CombatCharacter = Cast<AODCombatCharacter>(ActorInfo->AvatarActor))
		{
			if (AActor* Target = CombatCharacter->GetTarget())
			{
				//Dead Check
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target))
				{
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
					{
						const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(TEXT("OD.State.Dead"));
						if (ASC->HasMatchingGameplayTag(DeadTag))
						{
							return false;
						}
					}
				}
			}
			//monster
			if (CombatCharacter->IsA(AODMonsterBase::StaticClass()))
			{
				return true;
			}
		}
	}
	
	return false;
}

void UOD_GA_AOE_SingleHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (MontageArray.IsEmpty())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const int32 Index = PickIndexDeterministic(ActivationInfo, MontageArray.Num());
	if (!MontageArray[Index])
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	//Block Tag
	const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AddLooseGameplayTag(BlockTag);
	}


	//Montage
	if (IsValid(MontageTask) && MontageTask->GetState() != EGameplayTaskState::Finished)
		return;

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontageArray[Index],
		MontageRate);

	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_AOE_SingleHit::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_AOE_SingleHit::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_AOE_SingleHit::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_AOE_SingleHit::OnMontageBlendOut);
	MontageTask->ReadyForActivation();


	//UAbilityTask_WaitGameplayEvent::WaitGameplayEvent / notify Event
	if (K2_HasAuthority())
	{
		//Hit Event
		const FGameplayTag NotifyHitTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Combat.Hit.1"));
		auto* WaitHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyHitTag,
			nullptr,
			true,
			true);
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_AOE_SingleHit::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();
	}

	//Remove Block Tag
	const FGameplayTag NotifyRemoveTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Trigger.1"));
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyRemoveTag,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_AOE_SingleHit::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();
}

void UOD_GA_AOE_SingleHit::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	//Remove Block Tag
	const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(BlockTag);
	}

	//EndTask
	if (MontageTask && MontageTask->GetState() != EGameplayTaskState::Finished)
	{
		MontageTask->EndTask();
	}
	MontageTask = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_AOE_SingleHit::OnHitNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority())
	{
		return;
	}
	
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return;
	}

#pragma region Combat_AOE_BoxOverlap
	AODCombatCharacter* ActiveCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo());
	if (!ActiveCharacter) return;
	
	USkeletalMeshComponent* Mesh = ActiveCharacter->GetMesh();
	const FRotator YawOnly(0.f, Mesh->GetComponentRotation().Yaw, 0.f);
	const FQuat		 Rot	 = YawOnly.Quaternion();
	const FVector	 Center  = Avatar->GetActorLocation() + Rot.RotateVector(BoxLocationOffset); 

	TArray<FOverlapResult> Hits;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GA_AOE), false, Avatar);
	const FCollisionShape Shape = FCollisionShape::MakeBox(BoxExtent);
	Avatar->GetWorld()->OverlapMultiByChannel(Hits, Center, Rot, ECC_GameTraceChannel3, Shape, Params);
#pragma endregion

#if WITH_EDITOR
#pragma region Debug View Box
	const FColor Color = FColor::Green;
	const bool   bPersistent = false;   // true면 지속 표시
	const float  LifeTime   = 2.0f;     // 2초 표시
	const uint8  DepthPrio  = 0;
	const float  Thickness  = 2.0f;

	DrawDebugBox(Avatar->GetWorld(),
				 Center,
				 BoxExtent,           // half-extents
				 Rot,                 // FQuat
				 Color,
				 bPersistent,
				 LifeTime,
				 DepthPrio,
				 Thickness);
#pragma endregion
#endif
		
	TArray<TWeakObjectPtr<AActor>> Targets;
	Targets.Reserve(Hits.Num());
	for (const FOverlapResult& Result : Hits)
	{
		if (AODPlayer* Player = Cast<AODPlayer>(Result.GetActor()))
		{
			Targets.Add(Player);
		}
	}
	
	if (Targets.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No Actor Found"));
		Hits.Empty();
		return;
	}

#pragma region GAS_Damage_ApplyGE
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Avatar);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(Effect, 1, EffectContextHandle);

	FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();
	Spec.AddDynamicAssetTag(OD_Effect_Combat_OnDamage);

	Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_DamagePercent, DamagePercent);
	Spec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_AttackCount, AttackCount);

	for (auto Player : Targets)
	{
		AODPlayer* P = Cast<AODPlayer>(Player.Get());
		if (!P)
		{
			UE_LOG(LogTemp, Error, TEXT("Player is NULL"));
			continue;
		}

		AODMonsterBase* Monster = Cast<AODMonsterBase>(ActiveCharacter);
		if (!Monster)
		{
			UE_LOG(LogTemp, Error, TEXT("Monster is NULL"));
			continue;
		}
		Monster->SetTargetPlayerActor(P);
		
		UE_LOG(LogTemp, Error, TEXT("HitNotifyEvent"));
		if (IAbilitySystemInterface* Iface = Cast<IAbilitySystemInterface>(Monster->GetTarget()))
		{
			if (UAbilitySystemComponent* TargetASC = Iface->GetAbilitySystemComponent())
			{
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}
		}
	}
#pragma endregion
}

void UOD_GA_AOE_SingleHit::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	//Tag OD.Notify.Trigger.1
	if (InData.EventTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("OD.Notify.Trigger.1"))))
	{
		//Block Tag
		const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(BlockTag);
		}
	}
}



#pragma region AnimMontage Dele
void UOD_GA_AOE_SingleHit::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_AOE_SingleHit::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_AOE_SingleHit::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_AOE_SingleHit::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
#pragma endregion
