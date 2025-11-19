// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTD_MonsterStatus.h"

#include "AI/ODAIController.h"
#include "Character/ODMonsterGrunt.h"


bool UBTD_MonsterStatus::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetOwner());
	if (!AIController) return false;

	AODMonsterGrunt* Monster = Cast<AODMonsterGrunt>(AIController->GetPawn());
	if (!Monster) return false;
	
	return Monster->GetCurrentStatus() == DesignatedMonsterStatus;
}
