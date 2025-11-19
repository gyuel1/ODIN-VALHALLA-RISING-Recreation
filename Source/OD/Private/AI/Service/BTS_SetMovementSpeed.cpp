// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_SetMovementSpeed.h"

#include "AI/ODAIController.h"
#include "Character/ODMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTS_SetMovementSpeed::UBTS_SetMovementSpeed()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = false;
}

void UBTS_SetMovementSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;
	
	AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
	if (!Monster) return;
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterMoveSpeed().MaxRunSpeed;
}

void UBTS_SetMovementSpeed::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
	const AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
	if (!Monster) return;


	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetMonsterMoveSpeed().MaxWalkSpeed;
}
