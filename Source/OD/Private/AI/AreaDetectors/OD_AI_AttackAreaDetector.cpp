// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AreaDetectors/OD_AI_AttackAreaDetector.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/Effect/Combat/OD_GE_Combat_OnDamage.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/ShapeComponent.h"
#include "Tags/ODAbilityTags.h"

using namespace GameplayEffectTags;

// Sets default values
AOD_AI_AttackAreaDetector::AOD_AI_AttackAreaDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	UBoxComponent* Box = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackAreaTrigger"));
	
	CollisionComponent = Box;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapPawn"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetupAttachment(RootComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetRelativeRotation(FRotator(-90, 0,0));
}

void AOD_AI_AttackAreaDetector::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TickDamageTimer;

	if (HasAuthority())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);  
		GetWorld()->GetTimerManager().SetTimer(TickDamageTimer,this, &ThisClass::ApplyTickDamage, 0.2f, true, 0.0f);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("BeginPlay not HasAuthority"));
	}
}

void AOD_AI_AttackAreaDetector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearAllTimersForObject(this);
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.RemoveAll(this);
		CollisionComponent->OnComponentEndOverlap.RemoveAll(this);
	}
}

#pragma region Overlap
void AOD_AI_AttackAreaDetector::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	if (auto* Player = Cast<AODPlayer>(OtherActor))
	{
		InAreaSet.Add(Player);
	}
}

void AOD_AI_AttackAreaDetector::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	if (auto* Player = Cast<AODPlayer>(OtherActor))
	{
		InAreaSet.Remove(Player);
	}
}
#pragma endregion

#pragma region PlayerDetected
TArray<TObjectPtr<AODPlayer>>  AOD_AI_AttackAreaDetector::CollectValidTargets()
{
	TArray<TObjectPtr<AODPlayer>> Out;
	for (auto It = InAreaSet.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent(); continue;
		}
		Out.Add(It->Get());
	}
	return Out;
}
#pragma endregion

#pragma region ActorConfigure
void AOD_AI_AttackAreaDetector::Configure(float Duration, float TickInterval)
{
	if (!HasAuthority())
	{
		return;
	}
	
	DurationSec = Duration;
	TickIntervalSec = TickInterval;

	if (DurationSec > 0.f)
	{
		const float Epsilon = 0.05f;
		const float ShutdownIn = FMath::Max(0.f, DurationSec - Epsilon);
		GetWorldTimerManager().SetTimer(
			ShutdownHandle, this, &AOD_AI_AttackAreaDetector::BeginShutdown,
			ShutdownIn, false
		);
		SetLifeSpan(DurationSec);
	}

	if (TickIntervalSec > 0.f)
	{
		GetWorldTimerManager().SetTimer(
			TickDamageHandle, this, &AOD_AI_AttackAreaDetector::ApplyTickDamage,
			TickIntervalSec, true, TickIntervalSec
		);
	}
}

// Spawn으로 인한 생성이 이미 해당 위치에 있던 엑터가 잡히지 않는 경우를 방지하기 위한 보강구조
void AOD_AI_AttackAreaDetector::InitializeInitialOverlaps()
{
	TArray<AActor*> InitiallyOverlapping;
	GetOverlappingActors(InitiallyOverlapping, AODPlayer::StaticClass());

	for (AActor* A : InitiallyOverlapping)
	{
		if (AODPlayer* P = Cast<AODPlayer>(A))
		{
			if (!AlreadyProcessed.Contains(P))
			{
				InAreaSet.Add(P);
				AlreadyProcessed.Add(P);								
			}
		}
	}
}

void AOD_AI_AttackAreaDetector::SetCachedMonster(AODMonsterBase* Monster)
{
	CachedMonster = Monster;
}
#pragma endregion

#pragma region TickDamage
void AOD_AI_AttackAreaDetector::ApplyTickDamage()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!CachedMonster.Get())
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = CachedMonster.Get()->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ASC->GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UOD_GE_Combat_OnDamage::StaticClass(), 1, EffectContext);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	FGameplayEffectSpec& BaseSpec = *SpecHandle.Data.Get();
	BaseSpec.AddDynamicAssetTag(OD_Effect_Combat_OnDamage);
	BaseSpec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_DamagePercent, 100);
	BaseSpec.SetSetByCallerMagnitude(AttributeDataTags::OD_Magnitude_AttackCount, 1);
	
	for (auto It = InAreaSet.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			continue;
		}

		AODPlayer* Player = It->Get();
		if (!Player)
		{
			continue;
		}

		IAbilitySystemInterface* Iface = Cast<IAbilitySystemInterface>(Player);
		if (!Iface)
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = Iface->GetAbilitySystemComponent();
		if (!TargetASC)
		{
			continue;
		}

		ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}
}
#pragma endregion

#pragma region RepGraph
int32 AOD_AI_AttackAreaDetector::GetRepGraphCount()
{
	if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(Owner))
	{
		return RGI->GetRepGraphCount();
	}

	return -1;
}

void AOD_AI_AttackAreaDetector::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}


void AOD_AI_AttackAreaDetector::DestroySelf()
{
	if (HasAuthority() && !IsActorBeingDestroyed())
	{
		Destroy();
	}
}

void AOD_AI_AttackAreaDetector::BeginShutdown()
{
	if (!HasAuthority() || IsActorBeingDestroyed())
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}
	
	TearOff();
	ForceNetUpdate();
	SetActorHiddenInGame(true);
	
	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(0.0f, 0.15f, false);
	}
	
	World->GetTimerManager().ClearTimer(TickDamageHandle);
	//World->GetTimerManager().SetTimerForNextTick(this, &AOD_AI_AttackAreaDetector::DestroySelf);
	World->GetTimerManager().SetTimer(DestroyDelayHandle, this, &AOD_AI_AttackAreaDetector::DestroySelf, 0.15f, false);
}
#pragma endregion

#pragma region Getter
TSet<TWeakObjectPtr<AODPlayer>> AOD_AI_AttackAreaDetector::GetInAreaSet()
{
	return InAreaSet;
}

#pragma endregion

#pragma region Setter

#pragma endregion

