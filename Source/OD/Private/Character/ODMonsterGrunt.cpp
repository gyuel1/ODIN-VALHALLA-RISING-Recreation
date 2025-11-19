// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODMonsterGrunt.h"

#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"

AODMonsterGrunt::AODMonsterGrunt()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	ODCharacterType = EODCharacterType::FieldGruntMonster;
}

void AODMonsterGrunt::NextPatrolStatus()
{
	float RandomValue = FMath::FRand();
	
	if (IdleProbability > RandomValue)
	{
		CurrentStatus = EMonsterStatus::Idle;
	}
	else
	{
		CurrentStatus = EMonsterStatus::Walking;
	}
}

FVector AODMonsterGrunt::GetRandomPointInRadius(const FVector& SpawnLocation, float Radius)
{
	//후에 내 위치에서 일정거리 밖이면서 원안의 좌표를 구하는 식으로 변경예정
	
	//0~2파이 랜덤 각도 생성
	float Angle = FMath::RandRange(0.f, 2 * PI);

	//반경 이내 계산
	float Distance = FMath::RandRange(0.f, Radius);

	//랜덤 방향 벡터
	FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);

	//최종 위치
	FVector RandomPoint = SpawnLocation + Direction * Distance;

	//NavmeshSystem에 맞춰서 Z축 보정
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLocation;
		if (NavSys->ProjectPointToNavigation(RandomPoint, NavLocation))
		{
			RandomPoint = NavLocation.Location;
		}
	}
	
	return RandomPoint;	
}

void AODMonsterGrunt::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Getter
EMonsterStatus AODMonsterGrunt::GetCurrentStatus()
{
	return CurrentStatus;
}

float AODMonsterGrunt::GetIdleProbability()
{
	return IdleProbability;
}

FRandomPatrolPointValue& AODMonsterGrunt::GetPatrolPointValue()
{
	return PatrolPointValue;
}

#pragma endregion