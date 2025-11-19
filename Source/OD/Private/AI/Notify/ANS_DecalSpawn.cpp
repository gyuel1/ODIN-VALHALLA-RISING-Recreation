// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Notify/ANS_DecalSpawn.h"

#include "NavigationSystem.h"
#include "AI/AreaDetectors/OD_AI_AttackAreaDetector.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"


void UANS_DecalSpawn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	switch (PatternType)
	{
		case EDecalPatternType::SingleInstant :
			{
				PatternSingleInstant(MeshComp);
				break;				
			}
		case EDecalPatternType::RepeatingFixed:

			break;
		case EDecalPatternType::RepeatingRandom:
			{
				PatternRepeatingRandom(MeshComp);
				break;
			}
		default :
			break;
	}
}

void UANS_DecalSpawn::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!CachedAttackArea) return;

	switch (PatternType)
	{
		case EDecalPatternType::SingleInstant :
			{			
				if (CachedAttackArea && CachedAttackArea->HasAuthority())
				{
					if (!CachedAttackArea->GetTearOff())
					{
						CachedAttackArea->TearOff();
					}

					CachedAttackArea->SetActorEnableCollision(false);
					CachedAttackArea->SetActorTickEnabled(false);
					
					if (UWorld* World = CachedAttackArea->GetWorld())
					{
						World->GetTimerManager().SetTimerForNextTick(
							CachedAttackArea.Get(),
							&AOD_AI_AttackAreaDetector::DestroySelf
						);
					}
				}
				CachedAttackArea = nullptr;
			}
			break;
		case EDecalPatternType::RepeatingFixed:

			break;
		case EDecalPatternType::RepeatingRandom:
			break;
		default :
			break;
	}
}

void UANS_DecalSpawn::SpawnAttackAreaAt(AActor* OwnerActor, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!AttackArea) return;
	if (PatternType == EDecalPatternType::SingleInstant && CachedAttackArea) return;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = OwnerActor;
	SpawnParameters.Instigator = Cast<AODCombatCharacter>(OwnerActor);
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	CachedAttackArea = OwnerActor->GetWorld()->SpawnActor<AOD_AI_AttackAreaDetector>(AttackArea, SpawnLocation, SpawnRotation, SpawnParameters);
	CachedAttackArea->InitializeInitialOverlaps();
	if (PatternType == EDecalPatternType::RepeatingRandom)
	{
		CachedAttackArea->Configure(RepeatingRandomParams.Duration, RepeatingRandomParams.TickInterval);
		if (AODMonsterBase* Monster = Cast<AODMonsterBase>(OwnerActor))
		{
			CachedAttackArea->SetCachedMonster(Monster);
			CachedAttackArea->SetRepGraphCount(CachedAttackArea->GetRepGraphCount());
		}
	}
}

void UANS_DecalSpawn::PatternSingleInstant(USkeletalMeshComponent* MeshComp)
{
	AActor* OwnerActor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!OwnerActor) return;
	if (!OwnerActor->HasAuthority()) return;

	UWorld* World = OwnerActor->GetWorld();
	if (!World) return;

	const FTransform BaseXf = MeshComp->GetComponentTransform();
	const float Yaw = BaseXf.Rotator().Yaw; 
	const FRotator YawRot(0.f, Yaw, 0.f);
	FVector Loc = BaseXf.GetLocation();
	
	Loc += YawRot.Vector() * SingleParams.ForwardOffset;
	
	SpawnAttackAreaAt(OwnerActor, Loc, YawRot);
}

void UANS_DecalSpawn::PatternRepeatingRandom(USkeletalMeshComponent* MeshComp)
{
	AActor* OwnerActor = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!OwnerActor) return;
	if (!OwnerActor->HasAuthority()) return;

	for (int32 i=0; i<RepeatingRandomParams.NumberSpawnCount; i++)
	{
		FVector SpawnLocation = CalSpawnLocation(MeshComp->GetWorld(), OwnerActor->GetActorLocation(), RepeatingRandomParams.RandomRadius);
		SpawnAttackAreaAt(OwnerActor, SpawnLocation);
	}
}

//바닥에 붙이기 위한 계산
FVector UANS_DecalSpawn::FixToGround(UWorld* World, const FVector& Start, float DownDistance, float UpOffset)
{
	FHitResult Hit;
	const FVector TraceStart = Start + FVector(0,0,	UpOffset);
	const FVector TraceEnd   = Start - FVector(0, 0, DownDistance);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(DecalGroundTrace), false);
	Params.bReturnPhysicalMaterial = false;

	if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		return Hit.ImpactPoint  + FVector(0,0,1.f);;
	}
	
	return Start;
}

// NavMesh를 이용한 스폰 유효 Location 계산
FVector UANS_DecalSpawn::CalSpawnLocation(UWorld* World, const FVector& Origin, float Radius)
{
	//0~2파이 랜덤 각도 생성
	float Angle = FMath::RandRange(0.f, 2 * PI);

	//반경 이내 계산
	float Distance = Radius * FMath::Sqrt(FMath::FRand());

	//랜덤 방향 벡터
	FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);

	//최종 위치
	FVector RandomLocation = Origin + Direction * Distance;

	//스폰 포인트
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World))
	{
		FNavLocation NavLocation;
		if (NavSys->ProjectPointToNavigation(RandomLocation, NavLocation,FVector(300.f,300.f, 3000.f)))
		{
			RandomLocation = NavLocation.Location;
		}
	}

	return RandomLocation;
}
