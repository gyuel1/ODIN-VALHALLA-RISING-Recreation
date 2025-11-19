// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveToAttackRange.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/Abilities/Combat/Default/OD_GA_Combat_NormalAttack.h"
#include "Ai/ODAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Components/ODCombatComponent.h"


EBTNodeResult::Type UBTTask_MoveToAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
		if (TargetActor.IsValid())
		{

			//사거리 가져오기 AttackRange Setting
			AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
			if (!Monster)
			{
				//(LogTemp, Error, TEXT("UBTTask_MoveToAttackRange : MonsterBase"));
				return EBTNodeResult::Failed;
			}
			
			UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
			if (!ASC)
			{
				//UE_LOG(LogTemp, Error, TEXT("UBTTask_MoveToAttackRange : ASC"));
				return EBTNodeResult::Failed;
			}
			
			UODCombatComponent* CombatComponent = Monster->GetODCombatComponent();
			if (!CombatComponent)
			{
				//UE_LOG(LogTemp, Error, TEXT("UBTTask_MoveToAttackRange : CombatComponent"));
				return EBTNodeResult::Failed;
			}
			
			FGameplayTag Tag = FGameplayTag::RequestGameplayTag("OD.Ability.Combat.NormalAttack");
			if (!Tag.IsValid())
			{
				//UE_LOG(LogTemp, Error, TEXT("UBTTask_MoveToAttackRange : Tag"));
				return EBTNodeResult::Failed;
			}
			
			FGameplayAbilitySpec* Spec = CombatComponent->FindAbilitySpecByTag(Tag, ASC);
			if (!Spec)
			{
				//(LogTemp, Error, TEXT("UBTTask_MoveToAttackRange : Spec"));
				return EBTNodeResult::Failed;
			}
			
			UOD_GA_Combat_NormalAttack* Attack = Cast<UOD_GA_Combat_NormalAttack>(Spec->Ability);
			if (!Attack)
			{
				//UE_LOG(LogTemp, Error, TEXT("UBTTask_MoveToAttackRange : Attack"));
				return EBTNodeResult::Failed;
			}
			
 			AttackRange = Attack->GetAbilityRange();
			
			AIController->MoveToActor(TargetActor.Get(), 0.f, true, true, false, nullptr, true);
			bNotifyTick = true;
			return EBTNodeResult::InProgress;			
		}		
	}
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToAttackRange::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
		if (Monster)
		{
			const float Dist = FVector::Dist(Monster->GetActorLocation(), TargetActor.Get()->GetActorLocation());
			if (Dist <= AttackRange)
			{
				AIController->StopMovement();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}		
	}
	
}
