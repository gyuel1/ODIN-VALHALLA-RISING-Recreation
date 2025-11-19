// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/Active/Valkyrie/OD_GA_Active_Valkyrie_002.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODPlayer.h"
#include "Debug/ODLogChannels.h"


UOD_GA_Active_Valkyrie_002::UOD_GA_Active_Valkyrie_002()
	: Super()
{
	AbilityTags.AddTag(OD_Active_Valkyrie_002);

	DashSpeed = 0.f;
	ArriveRange = 150.f;
}

bool UOD_GA_Active_Valkyrie_002::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayTagContainer* SourceTags,
                                                    const FGameplayTagContainer* TargetTags,
                                                    FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOD_GA_Active_Valkyrie_002::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!IsActive())
		return;

	UODActiveSkillDA* DA = GetActiveSkillDA(ActorInfo);
	if (!DA)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	const FActiveSkillData& SkillData = DA->GetActiveSkillData();

	if (!IsValid(AbilityMontage))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}


	//UAbilityTask_WaitGameplayEvent::WaitGameplayEvent / notify Event
	if (K2_HasAuthority())
	{
		//Hit Event / OD_Notify_Combat_Hit_1
		auto* WaitHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyTags::OD_Notify_Combat_Hit_1,
			nullptr,
			true,
			true);
		WaitHitTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnHitNotifyEvent);
		WaitHitTask->ReadyForActivation();

		//Dash Event / OD_Notify_Trigger_2
		auto* WaitDashTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			NotifyTags::OD_Notify_Trigger_2,
			nullptr,
			true,
			true);
		WaitDashTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnDashNotifyEvent);
		WaitDashTask->ReadyForActivation();
		
	}
	
	//Remove Block Tag  / OD_Notify_Trigger_1
	auto* WaitRemoveTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		NotifyTags::OD_Notify_Trigger_1,
		nullptr,
		true,
		true);
	WaitRemoveTask->EventReceived.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnRemoveNotifyEvent);
	WaitRemoveTask->ReadyForActivation();
	
	//Montage
	auto* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage,
		MontageRate);
	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Active_Valkyrie_002::OnMontageBlendOut);
	MontageTask->ReadyForActivation();





	if (K2_HasAuthority())
		CommitAbility(Handle, ActorInfo, ActivationInfo);
}

void UOD_GA_Active_Valkyrie_002::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Active_Valkyrie_002::StartDash()
{
	if (!DashTickHandle.IsValid())
	{
		DashTickHandle = FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateUObject(this, &UOD_GA_Active_Valkyrie_002::TickDash), 0.0f);
	}
}

void UOD_GA_Active_Valkyrie_002::EndDash()
{
	if (DashTickHandle.IsValid())
	{
		UE_LOG(LogOD,Warning,TEXT("DashEnd"));
		FTSTicker::GetCoreTicker().RemoveTicker(DashTickHandle);
		DashTickHandle.Reset();
	}
}

bool UOD_GA_Active_Valkyrie_002::TickDash(float DeltaSeconds)
{
	AODPlayer* Player = Cast<AODPlayer>(CurrentActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		EndDash();
		return false;
	}
	AODCombatCharacter* Target = Cast<AODCombatCharacter>(Player->GetTarget());
	if (!IsValid(Target))
	{
		EndDash();
		return false;
	}

	//도착 거리 체크
	const FVector OwnerLocation = Player->GetActorLocation();
	const FVector TargetLocation = Target->GetActorLocation();
	
	const float DistanceToTarget = FMath::Abs(FVector::Dist(OwnerLocation,TargetLocation));
	if (ArriveRange >= DistanceToTarget)
	{
		EndDash();
		return false;
	}

	//아직 도착을 안했으면 돌진하고 반복
	FRotator LookAtRot = (TargetLocation - OwnerLocation).Rotation();
	Player->SetActorRotation(FRotator(Player->GetActorRotation().Roll, LookAtRot.Yaw,
	                                  Player->GetActorRotation().Pitch));

	const FVector CurrentTickDashLocation = Player->GetActorForwardVector()* DashSpeed * DeltaSeconds;
	const FVector CurrentActorLocation = OwnerLocation + CurrentTickDashLocation;

	Player->SetActorLocation(CurrentActorLocation);
	if (K2_HasAuthority())
	{
		SetActorLocation_Client(Player, CurrentActorLocation);
	}
	
	UE_LOG(LogOD,Warning,TEXT("Dash"));
	return true;
}

void UOD_GA_Active_Valkyrie_002::OnMontageComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_002::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_002::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Active_Valkyrie_002::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

// void UOD_GA_Active_Valkyrie_002::SetActorLocation_NetMulticast_Implementation(AODPlayer* InPlayer,
// 	const FVector InLocation)
// {
// 	InPlayer->SetActorLocation(InLocation);
// }

void UOD_GA_Active_Valkyrie_002::SetActorLocation_Client_Implementation(AODPlayer* InPlayer, const FVector InLocation)
{
	InPlayer->SetActorLocation(InLocation);
}

void UOD_GA_Active_Valkyrie_002::OnHitNotifyEvent(const FGameplayEventData InData)
{
	if (!K2_HasAuthority())
		return;

	UODActiveSkillDA* DA = GetActiveSkillDA(CurrentActorInfo);

	//Tag OD.Notify.Combat.Hit.1
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Combat_Hit_1))
	{
		UE_LOG(LogTemp, Warning, TEXT("001 Notify Hit"));

		//Effect
		if (AODPlayer* ActiveCharacter = Cast<AODPlayer>(GetAvatarActorFromActorInfo()))
		{
			if (AActor* Target = ActiveCharacter->GetTarget())
			{
				if (UAbilitySystemComponent* TargetAsc =
					UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
				{
					if (DA)
					{
						const FActiveSkillData& SkillData = DA->GetActiveSkillData();

						const FGameplayEffectSpecHandle& EffectSpecHandle = CreateHitEffect(
							UOD_GE_Combat_OnDamage::StaticClass(),
							SkillData.DamagePercent,
							SkillData.DamageCount);
						
						OnHitNiagaraSystem(Target);
						GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
							*EffectSpecHandle.Data.Get(), TargetAsc);
					}
				}
			}
		}
	}
}

void UOD_GA_Active_Valkyrie_002::OnDashNotifyEvent(const FGameplayEventData InData)
{
	if (K2_HasAuthority())
	{
		if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_2))
		{
			UE_LOG(LogOD,Warning,TEXT("OD_Notify_Trigger_2"));
			StartDash();
		}
	}
}

void UOD_GA_Active_Valkyrie_002::OnRemoveNotifyEvent(const FGameplayEventData InData)
{
	if (InData.EventTag.MatchesTagExact(NotifyTags::OD_Notify_Trigger_1))
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveLooseGameplayTag(OD_Ability_Block);
		}
	}
}
