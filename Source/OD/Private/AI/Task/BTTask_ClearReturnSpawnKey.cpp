// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ClearReturnSpawnKey.h"

#include "AbilitySystemComponent.h"
#include "AI/ODAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ODMonsterBase.h"
#include "Components/ODDamageReceivedComponent.h"
#include "Tags/ODAbilityTags.h"

EBTNodeResult::Type UBTTask_ClearReturnSpawnKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
		if (AIController)
		{
			AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
			if (Monster)
			{
				UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
				if (ASC)
				{
					FGameplayTag Tag = FGameplayTag(GameplayAbilityTags::OD_Ability_Init_FillVitals);
					FGameplayTagContainer Container;
					Container.AddTag(Tag);
					ASC->TryActivateAbilitiesByTag(Container);

					//Monster->GetODDamageReceivedComponent()->ClearDamageReceivedArray_Internal();

					FGameplayTag ResetTag = FGameplayTag(GameplayEffectTags::OD_Effect_Enrage_Status);
					if (ASC->HasMatchingGameplayTag(ResetTag))
					{
						FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(ResetTag));
						TArray<FActiveGameplayEffectHandle> Handles = ASC->GetActiveEffects(Query);
						for (const FActiveGameplayEffectHandle& H : Handles)
						{
							ASC->RemoveActiveGameplayEffect(H, 1);
						}
					}
					
					BB->SetValueAsBool(GetSelectedBlackboardKey(), false);
					if (Monster->GetEMonsterRank() == EMonsterRank::Boss || Monster->GetEMonsterRank() == EMonsterRank::DungeonBoss)
					{
						BB->ClearValue(TEXT("TargetActor"));
						AIController->ClearSightPlayers();
					}

					return EBTNodeResult::Succeeded;
				}
			}
		}		
	}
	
	return EBTNodeResult::Failed;
}
