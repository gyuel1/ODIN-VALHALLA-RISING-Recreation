// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_ReturnToStartIfFar.h"

#include "AI/ODAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ODMonsterGrunt.h"


UBTS_ReturnToStartIfFar::UBTS_ReturnToStartIfFar()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick	= true;
}

void UBTS_ReturnToStartIfFar::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	AODMonsterBase* MonsterBase = Cast<AODMonsterBase>(AIController->GetPawn());
	if (!MonsterBase) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		const FVector SpawnPoint = MonsterBase->GetPatrol().SpawnLocation;
		const float Dist = FVector::Dist(MonsterBase->GetActorLocation(), SpawnPoint);

		if (Dist > MonsterBase->GetPatrol().MaxTrackingRadius)
		{
			BB->ClearValue(TargetActorKey.SelectedKeyName);
			MonsterBase->SetTargetPlayerActor(nullptr);
			BB->SetValueAsBool(bReturnSpawnLocationKey.SelectedKeyName, true);
			AIController->StopMovement();
		}
	}
}
