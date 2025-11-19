// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Spawning/ODSpawnTrigger.h"

#include "AI/Spawning/ODMonsterSpawnActor.h"
#include "Character/ODPlayer.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AODSpawnTrigger::AODSpawnTrigger()
{
	bReplicates = true;
	RepGraphCount = 0;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	TriggerVolume->SetCollisionProfileName(TEXT("OverlapPawn"));
	TriggerVolume->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f)); //기본값. 실제 값은 BP에서 수정

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AODSpawnTrigger::OnBeginOverlap);
	//거리별 최적화
	bAlwaysRelevant = false;
	bOnlyRelevantToOwner = false;
	bNetUseOwnerRelevancy = false;
	NetCullDistanceSquared = FMath::Square(5000.f);
}

void AODSpawnTrigger::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AODSpawnTrigger, bTriggered);
}

int32 AODSpawnTrigger::GetRepGraphCount()
{
	return RepGraphCount;
}

void AODSpawnTrigger::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}

// Called when the game starts or when spawned
void AODSpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AODSpawnTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherActor->IsA(AODPlayer::StaticClass())) return;

	if (!HasAuthority()) return;
	if (bTriggered) return;
	
	if (MonsterSpawnActor == nullptr) return;
	
	Cast<AODMonsterSpawnActor>(MonsterSpawnActor)->SpawnByTrigger();
	bTriggered = true;
	OnRep_Triggered();
}

void AODSpawnTrigger::OnRep_Triggered()
{
	TriggerVolume->SetGenerateOverlapEvents(false);
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

