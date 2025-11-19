// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/State/OD_GA_Combat_Death.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Ai/ODAIController.h"
#include "AI/Spawning/ODMonsterSpawnActor.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags/ODAbilityTags.h"

using namespace TriggerTags;


UOD_GA_Combat_Death::UOD_GA_Combat_Death()

{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;


	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Combat.State.Death")));

	FAbilityTriggerData Trig;
	Trig.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	Trig.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("OD.State.Dead"));
	AbilityTriggers.Add(Trig);
}

bool UOD_GA_Combat_Death::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayTagContainer* SourceTags,
                                             const FGameplayTagContainer* TargetTags,
                                             FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UOD_GA_Combat_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (MontageArray.IsEmpty())
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	const int32 Index = PickIndexDeterministic(ActivationInfo, MontageArray.Num());
	if (!MontageArray[Index])
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	//이동막기  및 방향 동기화
	if (AODCombatCharacter* MakeCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
		{
			if (MakeCharacter->IsA(AODPlayer::StaticClass()))
			{
				ENetMode NetMode = MakeCharacter->GetNetMode();
				
				//MakeMove->RotationRate = FRotator::ZeroRotator;
				MakeMove->bOrientRotationToMovement = false;
				MakeMove->MaxWalkSpeed = 0.f;
			}
		}
	}


	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (K2_HasAuthority())
		{
			if (ActorInfo->AvatarActor.IsValid())
			{
				if (AODMonsterBase* Monster = Cast<AODMonsterBase>(ActorInfo->AvatarActor))
				{
					if (AODAIController* Controller = Cast<AODAIController>(Monster->GetController()))
					{
						Controller->StopBehaviorTree();
					}

					if (Monster->GetbNeedsDeathMesh())
					{
						// USkeletalMeshComponent* Main = Monster->GetMesh();
						// Main->SetAnimationMode(EAnimationMode::AnimationBlueprint);
						// Main->SetSkeletalMesh(Monster->GetCachedDieMesh()->GetSkeletalMeshAsset());
						// Main->InitAnim(true);

						UWorld* World = Monster->GetWorld();
						const FTransform Xform = Monster->GetActorTransform();

						FActorSpawnParameters Params;
						Params.SpawnCollisionHandlingOverride =
							ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						Params.Owner = Monster;
						Params.Instigator = Monster->GetInstigator();

						World->SpawnActor<AActor>(Monster->GetMonsterDieClass(), Xform, Params);
						EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
						return;
					}
				}
				if (!ASC->HasMatchingGameplayTag(OD_Transaction_FieldMobReward))
				{
					ASC->AddLooseGameplayTag(OD_Transaction_FieldMobReward);
				}
			}
		}
		if (ActorInfo->AvatarActor.Get()->IsA(AODPlayer::StaticClass()))
		{
			const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Init.Regeneration"));
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(Tag);

			ASC->RemoveActiveEffectsWithGrantedTags(TagContainer);
		}

		const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
		ASC->AddLooseGameplayTag(BlockTag);
	}



	//Montage
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontageArray[Index],
		MontageRate);

	MontageTask->OnCompleted.AddDynamic(this, &UOD_GA_Combat_Death::OnMontageComplete);
	MontageTask->OnCancelled.AddDynamic(this, &UOD_GA_Combat_Death::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UOD_GA_Combat_Death::OnMontageInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UOD_GA_Combat_Death::OnMontageBlendOut);
	MontageTask->ReadyForActivation();

	
}

void UOD_GA_Combat_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(TEXT("OD.State.Dead"));
	const FGameplayTag BlockTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Block"));
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(DeadTag);
		ASC->RemoveLooseGameplayTag(BlockTag);
	}
	
	if (K2_HasAuthority())
	{
		if (ActorInfo->AvatarActor.Get()->IsA(AODPlayer::StaticClass()))
		{
			const FGameplayTag FillVitalsTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Init.FillVitals"));
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FillVitalsTag);

			GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(TagContainer);
			
			const FGameplayTag RegenerationTag = FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Init.Regeneration"));
			FGameplayTagContainer ReContainer;
			ReContainer.AddTag(RegenerationTag);
			GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(ReContainer);
			
		}
		if (AODMonsterBase* Monster = Cast<AODMonsterBase>(GetOwningActorFromActorInfo()))
		{
			Monster->Destroy();
		}
	}
	if (AODCombatCharacter* MakeCharacter = Cast<AODCombatCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (MakeCharacter->IsA(AODPlayer::StaticClass()))
		{
			if (UCharacterMovementComponent* MakeMove = MakeCharacter->GetCharacterMovement())
			{
				MakeMove->bOrientRotationToMovement = true;
				MakeMove->MaxWalkSpeed = 600.f;
			}
		}
	}

	//Task 비우기
	if (MontageTask && MontageTask->GetState() != EGameplayTaskState::Finished)
	{
		MontageTask->EndTask();
	}
	MontageTask = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UOD_GA_Combat_Death::OnMontageComplete()
{
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_Death::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_Death::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UOD_GA_Combat_Death::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
