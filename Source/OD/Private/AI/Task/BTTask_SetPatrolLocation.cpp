// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SetPatrolLocation.h"

#include "AI/ODAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ODMonsterGrunt.h"


EBTNodeResult::Type UBTTask_SetPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetOwner());
	if (AIController)
	{
		AODMonsterGrunt* MonsterGrunt = Cast<AODMonsterGrunt>(AIController->GetPawn());
		if (MonsterGrunt)
		{

			UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

			if (!BB) return EBTNodeResult::Failed;
			
			//FVector BaseVector = MonsterGrunt->GetPatrolPointValue().SpawnLocation;
			FVector BaseVector = BB->GetValueAsVector(TEXT("SpawnLocation"));;
			float Radius = MonsterGrunt->GetPatrolPointValue().PatrolRadius;
			FVector PatrolLocation = MonsterGrunt->GetRandomPointInRadius(BaseVector,Radius);
			
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PatrolLocation);

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
