// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_SelectBossTarget.h"

#include "AI/ODAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"


EBTNodeResult::Type UBTTask_SelectBossTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (!BB || !AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	if (AIController->GetSightPlayers().Num() <= 0)
	{
		return EBTNodeResult::Failed;
	}
	
	TArray<AODPlayer*> TargetList;
	TargetList.Reserve(AIController->GetSightPlayers().Num());

	for (const auto& Player : AIController->GetSightPlayers())
	{
		if (Player.IsValid())
		{
			TargetList.Add(Player.Get());
		}
	}

	if (TargetList.Num() == 0)
	{
		BB->ClearValue(GetSelectedBlackboardKey());
		return EBTNodeResult::Succeeded;
	}
	
	const int32 TargetIndex = FMath::RandRange(0, TargetList.Num() - 1);

	BB->SetValueAsObject(GetSelectedBlackboardKey(), TargetList[TargetIndex]);

	AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}	

	Monster->SetTargetPlayerActor(TargetList[TargetIndex]);
	
	return EBTNodeResult::Succeeded;
}
