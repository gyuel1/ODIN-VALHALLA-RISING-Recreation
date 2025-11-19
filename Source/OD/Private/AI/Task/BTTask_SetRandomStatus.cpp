// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetRandomStatus.h"

#include "AI/ODAIController.h"
#include "Character/ODMonsterGrunt.h"

EBTNodeResult::Type UBTTask_SetRandomStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetOwner());
	if (AIController)
	{
		AODMonsterGrunt* Monster = Cast<AODMonsterGrunt>(AIController->GetPawn());
		if (Monster)
		{
			Monster->NextPatrolStatus();
			//OwnerComp.RequestExecution(EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
