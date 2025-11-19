// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTD_MonsterEngageType.h"

#include "Ai/ODAIController.h"
#include "Character/ODMonsterBase.h"

// AI Controller에서 감각을 감지할 떄 몬스터 타입의 분기로 어떤감각을 사용할 시 분기하는게 감지에 대한 안정성이 높아서 Controller에 구현했으며,
// Controller에서 감지를 인지하며 Target을 설정할 떄 해당 조건을 검사하므로 나중에 삭제할 예정인 스크립트.

bool UBTD_MonsterEngageType::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AODAIController* AIController = Cast<AODAIController>(OwnerComp.GetOwner());
	if (!AIController) return false;

	AODMonsterBase* Monster = Cast<AODMonsterBase>(AIController->GetPawn());
	if (!Monster) return false;
	
	return Monster->GetEngageType() == EngageType;
}
