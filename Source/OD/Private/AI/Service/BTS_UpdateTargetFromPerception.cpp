// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_UpdateTargetFromPerception.h"

#include "Ai/ODAIController.h"
#include "Perception/AIPerceptionComponent.h"


/*
 *    다른 방식으로 구현했으므로 해당 방식을 사용하지 않음.
 *    추후 삭제 할 예정 (9/16)
 */


void UBTS_UpdateTargetFromPerception::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<AODAIController>(OwnerComp.GetOwner());
	if (!AIController) return;
}

void UBTS_UpdateTargetFromPerception::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

}
