// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/ODAIController.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"

AODAIController::AODAIController()
{
	bReplicates = false;

	RepGraphCount = 0;

	//거리별 최적화
	bAlwaysRelevant = false;
	bOnlyRelevantToOwner = false;
	bNetUseOwnerRelevancy = false;
	NetCullDistanceSquared = FMath::Square(3500.f);
	
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>("DamageConfig");
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
}

void AODAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!HasAuthority()) return;
	
	Monster = Cast<AODMonsterBase>(InPawn);
	if (!Monster) return;

#pragma region AiPerception Setting
	if (!SightConfig)
	{
		UAISenseConfig* SightFound = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
		SightConfig = Cast<UAISenseConfig_Sight>(SightFound);
	}

	if (!DamageConfig)
	{
		UAISenseConfig* DamageFound = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Damage>();
		DamageConfig = Cast<UAISenseConfig_Damage>(DamageFound);
	}
	
	SightConfig->PeripheralVisionAngleDegrees = Monster->GetPerceptionInitValue().VisionAngleDegreeValue;
	SightConfig->SightRadius = Monster->GetPerceptionInitValue().SightRadiusValue;
	SightConfig->LoseSightRadius = Monster->GetPerceptionInitValue().LoseSightRadiusValue;
	SightConfig->SetMaxAge(10.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	AIPerceptionComponent->RequestStimuliListenerUpdate();
	AIPerceptionComponent->ConfigureSense(*SightConfig);
#pragma endregion

#pragma region BT Setting
	UBehaviorTree* BehaviorTree = (Monster->GetEMonsterRank() == EMonsterRank::Boss || Monster->GetEMonsterRank() == EMonsterRank::DungeonBoss) ? BossBT : GruntBT;
	if (!BehaviorTree) return;
	
	UBlackboardComponent* BB = nullptr;
	const bool bBBCheck = UseBlackboard(BehaviorTree->BlackboardAsset, BB);
	if (!bBBCheck || !BB) return;

	BB->SetValueAsVector(TEXT("SpawnLocation"), Monster->GetActorLocation());
	
	RunBehaviorTree(BehaviorTree);
#pragma endregion
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AODAIController::TargetPerceptionUpdated);
}

void AODAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AODAIController::TargetPerceptionUpdated);
	}
}

void AODAIController::TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	const auto SightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	const auto DamageID = UAISense::GetSenseID(UAISense_Damage::StaticClass());

	AODPlayer* Player = Cast<AODPlayer>(TargetActor);
	if (!Player) return;
	
	if (Monster->GetEMonsterRank() == EMonsterRank::Boss || Monster->GetEMonsterRank() == EMonsterRank::DungeonBoss)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			SightPlayers.Add(Player);
		}
		else
		{
			SightPlayers.Remove(Player);
		}
	}
	else if (Monster->GetEMonsterRank() == EMonsterRank::Grunt)
	{
		
		if (GetCurrentTarget() == true) return;
		
		if (Monster->GetEngageType() == EMonsterEngageType::Aggressive && Stimulus.Type == SightID)
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				SetCurrentTarget(Player);
				Monster->SetTargetPlayerActor(Player);								
			}
		}
		else if (Monster->GetEngageType() == EMonsterEngageType::Non_Aggressive && Stimulus.Type == DamageID)
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				SetCurrentTarget(Player);
				Monster->SetTargetPlayerActor(Player);
			}
		}
	}
}


void AODAIController::ClearSightPlayers()
{
	SightPlayers.Empty();
}

void AODAIController::StopBehaviorTree()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(GetBrainComponent());

	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

#pragma region Getter
TSet<TWeakObjectPtr<AODPlayer>> AODAIController::GetSightPlayers()
{
	return SightPlayers;
}

UAIPerceptionComponent* AODAIController::GetPerceptionComponent()
{
	return AIPerceptionComponent;
}

UAISenseConfig_Sight* AODAIController::GetSenseConfig()
{
	return SightConfig;
}

int32 AODAIController::GetRepGraphCount()
{
	return RepGraphCount;
}

bool AODAIController::GetCurrentTarget()
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	if (BlackboardComponent)
	{
		if (BlackboardComponent->GetValueAsObject(TargetBlackBoardKeyName))
		{
			return true;
		}		
	}
	return false;
}
#pragma endregion

#pragma region Setter
void AODAIController::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}

void AODAIController::SetCurrentTarget(AActor* NewTarget)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	if (!BlackboardComponent) return;
	
	if (NewTarget)
	{
		BlackboardComponent->SetValueAsObject(TargetBlackBoardKeyName, NewTarget);
	}
}
#pragma endregion
