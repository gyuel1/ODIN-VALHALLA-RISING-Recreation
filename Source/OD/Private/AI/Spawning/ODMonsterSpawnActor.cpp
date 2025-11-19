// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Spawning/ODMonsterSpawnActor.h"

#include "NavigationSystem.h"
#include "Character/ODMonsterBase.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/ODDungeonSessionManager.h"
#include "Sections/MovieSceneParticleSection.h"


// Sets default values
AODMonsterSpawnActor::AODMonsterSpawnActor()
{
	bReplicates = true;
	RepGraphCount = 0;

	//거리별 최적화
	bAlwaysRelevant = false;
	bOnlyRelevantToOwner = false;
	bNetUseOwnerRelevancy = false;
	NetCullDistanceSquared = FMath::Square(4000.f);
}

void AODMonsterSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FVector Origin = GetActorLocation();
		BeginMonsterSpawn(Origin, SpawnRadius);
	}	
}

void AODMonsterSpawnActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	for (auto& Pair : KeyMonster)
	{
		if (AActor* A = Pair.Key.Get())
		{
			A->OnDestroyed.RemoveDynamic(this, &AODMonsterSpawnActor::OnKeyMonsterDestroyed);
		}
	}
	KeyMonster.Empty();
}

void AODMonsterSpawnActor::BeginMonsterSpawn(const FVector& Origin, float Radius)
{

	if (SpawnerUsage == ESpawnerUsage::Field)
	{
		for (const auto& Entry : FieldSpawnEntries)
		{
			int32 Success = 0;
			int32 Safety = 0;
		
			while (Success < Entry.Count && Safety < Entry.Count * 100)
			{
				++Safety;
		
				SpawnLocation = CalSpawnLocation(Origin, Radius);
		
				if (!IsLocationFree(SpawnLocation, 200))
				{
					continue;
				}
			
				FTransform FieldSpawnPoint(SpawnLocation);
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				if (AODMonsterBase* SuccessSpawnMonster = GetWorld()->SpawnActor<AODMonsterBase>(Entry.MonsterClass, FieldSpawnPoint, Params))
				{
					FHitResult Hit;
					SuccessSpawnMonster->SetActorLocation(SpawnLocation, true, &Hit, ETeleportType::None);
					SuccessSpawnMonster->SetSpawner(this);
					SuccessSpawnMonster->GetPatrol().SpawnLocation = FieldSpawnPoint.GetLocation();
					++Success;
				}
			}
		}
	}
}

void AODMonsterSpawnActor::BeginMonsterSpawn(AODDungeonSessionManager* DungeonSessionManager)
{
	if (SpawnerUsage != ESpawnerUsage::Dungeon) return;
	if (!DungeonSessionManager) return;
	
	SpawnDoorIfNeeded();
		
	for (const auto& Entry : DungeonSpawnEntries)
	{
		SpawnLocation = Entry.DungeonSpawnLocation;

		UWorld* World = GetWorld();
		if (!World) return;
		
		ULevelStreaming* LS = UGameplayStatics::GetStreamingLevel(World, TEXT("02_02_Dungeon"));
		if (LS)
		{
			const FTransform XYZ = LS->LevelTransform;
			SpawnLocation += XYZ.GetLocation();
		} 
		
		FTransform DungeonSpawnPoint(SpawnLocation);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AODMonsterBase* SuccessSpawnMonster = GetWorld()->SpawnActor<AODMonsterBase>(Entry.MonsterClass, DungeonSpawnPoint, Params))
		{
			FHitResult Hit;
			SuccessSpawnMonster->SetActorLocation(SpawnLocation, true, &Hit, ETeleportType::None);
			SuccessSpawnMonster->SetActorRotation(Entry.DungeonSpawnRotation);
			SuccessSpawnMonster->SetSpawner(this);
			SuccessSpawnMonster->GetPatrol().SpawnLocation = SpawnLocation;

			if (Entry.KeyCount > 0)
			{
				DoorInfo.DestroyDoorCount += Entry.KeyCount;
				KeyMonster.Add( SuccessSpawnMonster, Entry.KeyCount);
				SuccessSpawnMonster->OnDestroyed.AddUniqueDynamic(this, &AODMonsterSpawnActor::OnKeyMonsterDestroyed);
				continue;
			}

			DungeonSessionManager->RegisterMonster(SuccessSpawnMonster);
		}
	}
}

int32 AODMonsterSpawnActor::GetRepGraphCount()
{
	return RepGraphCount;
}

void AODMonsterSpawnActor::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}


FVector AODMonsterSpawnActor::CalSpawnLocation(const FVector& Origin, float Radius)
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
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLocation;
		if (NavSys->ProjectPointToNavigation(RandomLocation, NavLocation,FVector(300.f,300.f, 3000.f)))
		{
			RandomLocation = NavLocation.Location;
		}
	}

	return RandomLocation;
}

bool AODMonsterSpawnActor::IsLocationFree(const FVector& Pos, float Radius)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(const_cast<AODMonsterSpawnActor*>(this));

	TArray<AActor*> Out;
	const bool bHasOverlap = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		Pos, Radius, ObjectTypes,APawn::StaticClass(), IgnoreActors, Out);

	return !bHasOverlap;
}

void AODMonsterSpawnActor::SpawnByTrigger()
{
	if (TriggerSpawnEntries.IsEmpty()) return;
	
	for (const auto& Entry : TriggerSpawnEntries)
	{
		SpawnLocation = Entry.DungeonSpawnLocation;

		UWorld* World = GetWorld();
		if (!World) return;
		
		ULevelStreaming* LS = UGameplayStatics::GetStreamingLevel(World, TEXT("02_02_Dungeon"));
		if (LS)
		{
			const FTransform XYZ = LS->LevelTransform;
			SpawnLocation += XYZ.GetLocation();
		} 
		
		FTransform DungeonSpawnPoint(SpawnLocation);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AODMonsterBase* SuccessSpawnMonster = GetWorld()->SpawnActor<AODMonsterBase>(Entry.MonsterClass, DungeonSpawnPoint, Params))
		{
			FHitResult Hit;
			SuccessSpawnMonster->SetActorLocation(SpawnLocation, true, &Hit, ETeleportType::None);
			SuccessSpawnMonster->SetActorRotation(Entry.DungeonSpawnRotation);
			SuccessSpawnMonster->SetSpawner(this);
			SuccessSpawnMonster->GetPatrol().SpawnLocation = SpawnLocation;

			if (SpawnerUsage == ESpawnerUsage::Dungeon)
			{
				if (Entry.KeyCount > 0)
				{
					DoorInfo.DestroyDoorCount += Entry.KeyCount;
					KeyMonster.Add( SuccessSpawnMonster, Entry.KeyCount);
					SuccessSpawnMonster->OnDestroyed.AddUniqueDynamic(this, &AODMonsterSpawnActor::OnKeyMonsterDestroyed);
				}
			}
		}
	}
}

#pragma region Door
void AODMonsterSpawnActor::OnKeyMonsterDestroyed(AActor* DestroyedActor)
{
	if (!HasAuthority()) return;

	if (!DestroyedActor) return;

	if (SpawnerUsage != ESpawnerUsage::Dungeon) return;

	if (int32* KeyPtr = KeyMonster.Find(DestroyedActor))
	{
		DoorInfo.DestroyDoorCount -= FMath::Max(0, *KeyPtr);
		KeyMonster.Remove(DestroyedActor);
	}

	if (DoorInfo.DestroyDoorCount <= 0 && CachedDoorActor)
	{
		CachedDoorActor->Destroy();
		CachedDoorActor = nullptr;

		for (auto& Pair : KeyMonster)
		{
			if (AActor* A = Pair.Key.Get())
			{
				A->OnDestroyed.RemoveDynamic(this, &AODMonsterSpawnActor::OnKeyMonsterDestroyed);
			}
		}
		KeyMonster.Empty();
	}	
}

void AODMonsterSpawnActor::SpawnDoorIfNeeded()
{
	if (!HasAuthority()) return;
	
	if (!CachedDoorActor)
	{
		if (DoorInfo.DoorClass)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform DoorSpawnLocation = DoorInfo.DoorSpawnTransform;

			UWorld* World = GetWorld();
			if (!World) return;
		
			ULevelStreaming* LS = UGameplayStatics::GetStreamingLevel(World, TEXT("02_02_Dungeon"));
			if (LS)
			{
				const FTransform XYZ = LS->LevelTransform;
				DoorSpawnLocation.SetLocation(DoorSpawnLocation.GetLocation() + XYZ.GetLocation());
			} 
			
			CachedDoorActor = GetWorld()->SpawnActor<AActor>(DoorInfo.DoorClass, DoorSpawnLocation, Params);
			if (ensure(CachedDoorActor))
			{
				CachedDoorActor->SetActorScale3D(DoorInfo.DoorSpawnTransform.GetScale3D());
			}
		}
	}
}
#pragma endregion

void AODMonsterSpawnActor::RequestRespawn(TSubclassOf<AODMonsterBase> MonsterClass, EMonsterSpawnContext SpawnContextType)
{
	if (!MonsterClass) return;

	if (SpawnContextType != EMonsterSpawnContext::Field) return;

	UWorld* World = GetWorld();
	if (!World) return;
	if (!World->IsGameWorld()) return;
#if WITH_EDITOR
	if (World->bIsTearingDown) return; 
#endif
	
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("Respawn"), MonsterClass);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 5, false);
}

void AODMonsterSpawnActor::Respawn(TSubclassOf<AODMonsterBase> MonsterClass)
{		
	if (!MonsterClass) return;
	int32 Safety = 0;

	while (Safety < 100)
	{
		++Safety;
		
		const FVector Loc = CalSpawnLocation(GetActorLocation(), SpawnRadius);
		if (!IsLocationFree(Loc, 200)) continue;

		FTransform Xform(Loc);
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (AODMonsterBase* NewMonster = GetWorld()->SpawnActor<AODMonsterBase>(MonsterClass, Xform, Params))
		{
			NewMonster->SetSpawner(this);
			NewMonster->SpawnDefaultController();
			NewMonster->GetPatrol().SpawnLocation = Xform.GetLocation();
			break;
		}
	}
}





