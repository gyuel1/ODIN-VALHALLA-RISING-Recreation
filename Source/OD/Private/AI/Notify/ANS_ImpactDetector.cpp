// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Notify/ANS_ImpactDetector.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tags/ODAbilityTags.h"


void UANS_ImpactDetector::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	if (!MeshComp->GetGenerateOverlapEvents())
	{
		MeshComp->SetGenerateOverlapEvents(true);
	}

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &UANS_ImpactDetector::OnMeshOverlapBegin);
}

void UANS_ImpactDetector::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{

	if (!MeshComp) return;
	MeshComp->OnComponentBeginOverlap.RemoveDynamic(this, &UANS_ImpactDetector::OnMeshOverlapBegin);

	for (auto T : ImpactList)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *T.GetName());
	}
	
	ImpactList.Empty();
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UANS_ImpactDetector::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AODPlayer* Player = Cast<AODPlayer>(OtherActor);
	if (Player == nullptr) return;

	for (auto DamagedPlayer : ImpactList)
	{
		if (DamagedPlayer == Player) return;
	}

	if (!Player->HasAuthority()) return;
		
	ImpactList.Add(Player);

	
}