// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_SetSightEnabled.h"

#include "AI/ODAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"


UBTS_SetSightEnabled::UBTS_SetSightEnabled()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = false;
}

void UBTS_SetSightEnabled::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bDisableOnEnter)
	{
		AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
		if (AIController)
		{
			UAIPerceptionComponent* PC = AIController->GetPerceptionComponent();
			if (PC)
			{
				PC->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
				PC->SetSenseEnabled(UAISense_Damage::StaticClass(), false);
				PC->ForgetAll();
			}
		}
	}
}

void UBTS_SetSightEnabled::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bEnableOnExit)
	{
		AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetAIOwner());
		if (AIController)
		{
			UAIPerceptionComponent* PC = AIController->GetPerceptionComponent();
			if (PC)
			{
				PC->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
				PC->SetSenseEnabled(UAISense_Damage::StaticClass(), true);
			}
		}
	}
}
