// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/OD_ANS_Tag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Character/ODCombatCharacter.h"



void UOD_ANS_Tag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
}

void UOD_ANS_Tag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !Tag.IsValid())
		return;
	
	if (AODCombatCharacter* CombatCharacter = Cast<AODCombatCharacter>(MeshComp->GetOwner()))
	{
		//캐릭터의 매쉬와 같은가?
		if (CombatCharacter->GetMesh() != MeshComp)
			return;

		//LeaderPoseComponent 부모를 따라가는가?
		if (MeshComp->LeaderPoseComponent.IsValid())
			return;

		//권위가 없는데 공격판정을 하는가?
		FGameplayTag MatchTag = FGameplayTag::RequestGameplayTag("OD.Notify.Combat");
		if (!CombatCharacter->HasAuthority() && Tag.MatchesTag(MatchTag))
			return;
		
		
		if (UAbilitySystemComponent* ASC = CombatCharacter->GetAbilitySystemComponent())
		{
			FGameplayEventData Data;
			Data.EventTag = Tag;
			Data.Instigator = CombatCharacter;
			Data.Target = CombatCharacter->GetTarget();
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatCharacter,Tag,Data);
		}
	}
}
