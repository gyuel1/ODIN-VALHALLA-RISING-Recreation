// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SpawnActor/Projectiles/ODProjectileActor.h"

#include "Character/ODCombatCharacter.h"


AODProjectileActor::AODProjectileActor()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileSpeed = 0.f;
}



void AODProjectileActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	const FVector SourceLocation = GetActorLocation();
	//대상 좌표 캐싱 
	if (Target.IsValid())
		TargetLocation = Target.Get()->GetActorLocation();

	//충돌 검사 (거리 검사 맞으면 데미지주고 자폭)
	const float Distance = FVector::Dist(SourceLocation, TargetLocation);
	if (Distance <= 60.f)
	{
		if (HasAuthority())
		{
			OnHitNiagaraSystem(Target.Get());
			OnDamageToTarget();
			Destroy();
		}
		return;
	}
	
	//방향 동기화 좌표바라보기
	FRotator LookAtRot = (TargetLocation - SourceLocation).Rotation();
	SetActorRotation(FRotator(LookAtRot.Pitch, LookAtRot.Yaw,
											 LookAtRot.Roll));

	//해당 방향으로 이동
	FVector NewLocation = GetActorForwardVector() * ProjectileSpeed * DeltaSeconds;
	const float MoveDistance = FVector::Dist(NewLocation, TargetLocation);
	
	const float AbsTargetDistance =  FMath::Abs(MoveDistance);
	const float AbsMoveDistance =  FMath::Abs(MoveDistance);
	
	if (AbsTargetDistance < AbsMoveDistance)
	{
		SetActorLocation_NetMulticast(TargetLocation);
	}
	else
	{
		SetActorLocation_NetMulticast(SourceLocation + NewLocation);
	}

	ForceNetUpdate();
}

void AODProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileSpeed = SkillParameters.ProjectileParms.ProjectileSpeed;
	
}

void AODProjectileActor::SetActorLocation_NetMulticast_Implementation(const FVector InLocation)
{
	SetActorLocation(InLocation);
}
