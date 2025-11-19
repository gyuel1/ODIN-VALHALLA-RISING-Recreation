// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODTargetComponent.h"

#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Components/SphereComponent.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

using namespace Debug;


// Sets default values for this component's properties
UODTargetComponent::UODTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DetectTargetArrayIndex = 0;
	SetIsReplicatedByDefault(true);
}

void UODTargetComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODTargetComponent, CurrentSelectTarget);
}


void UODTargetComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UODTargetComponent::UninitializeComponent()
{
	Super::UninitializeComponent();
	StopAutoTargeting();
	if (AutoTargetCollision)
	{
		AutoTargetCollision->OnComponentBeginOverlap.Clear();
		AutoTargetCollision->OnComponentEndOverlap.Clear();
	}
}

void UODTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Called when the game starts
void UODTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
		{
			ComponentOwner = Owner;
			AutoTargetCollision = Owner->GetAutoTargetCollision();
			StartAutoTargeting();
		}
	}
	if (AutoTargetCollision)
	{
		AutoTargetCollision->OnComponentBeginOverlap.AddDynamic(
			this, &UODTargetComponent::OnAutoTargetCollisionBeginOverlap);
		AutoTargetCollision->OnComponentEndOverlap.AddDynamic(
			this, &UODTargetComponent::OnAutoTargetCollisionEndOverlap);
	}
}

void UODTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	StopAutoTargeting();
}

void UODTargetComponent::StartAutoTargeting()
{
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(AutoTargetingTimerHandle))
		return;

	GetWorld()->GetTimerManager().SetTimer
	(AutoTargetingTimerHandle,
	 FTimerDelegate::CreateUObject(this, &UODTargetComponent::TimerFunction),
	 0.05f,
	 true);
}

void UODTargetComponent::StopAutoTargeting()
{
	if (!GetWorld())
	{
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(AutoTargetingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoTargetingTimerHandle);
	}
}

void UODTargetComponent::KeyboardTargeting()
{
	if (!DetectTargetArray.IsEmpty())
	{
		//만약 목록에 있어도 유효한 객체가 없으면 빠져나가기
		int32 TargetIndex = INDEX_NONE;
		if (IsValid(CurrentSelectTarget))
		{
			TargetIndex = DetectTargetArray.IndexOfByKey(CurrentSelectTarget.Get());
		}
		if (TargetIndex == INDEX_NONE)
		{
			TargetIndex = DetectTargetArrayIndex;
		}

		int32 Index = TargetIndex;
		for (int32 i = TargetIndex; i < TargetIndex + DetectTargetArray.Num(); i++)
		{
			Index = (Index + 1) % DetectTargetArray.Num();
			if (DetectTargetArray[Index].Get())
			{
				if (IsValid(CurrentSelectTarget))
				{
					if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
					{
						CurrentSelectTarget.Get()->OnSelectTargetCleared();
					}
				}
				CurrentSelectTarget = DetectTargetArray[Index].Get();
				ServerSetSelectTarget(DetectTargetArray[Index].Get());
				if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
				{
					CurrentSelectTarget.Get()->OnSelectTargeted();
				}
				DetectTargetArrayIndex = Index;
				return;
			}
		}
	}

	if (!SoftTargetArray.IsEmpty())
	{
		if (IsValid(CurrentSelectTarget))
		{
			int32 TargetIndex = SoftTargetArray.IndexOfByKey(CurrentSelectTarget.Get());
			if (TargetIndex != INDEX_NONE)
			{
				int32 Index = TargetIndex + 1;
				for (int32 j = Index; j < SoftTargetArray.Num(); j++)
				{
					if (SoftTargetArray[j].IsValid())
					{
						if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
						{
							CurrentSelectTarget.Get()->OnSelectTargetCleared();
						}

						CurrentSelectTarget = SoftTargetArray[j].Get();
						ServerSetSelectTarget(SoftTargetArray[j].Get());

						if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
						{
							CurrentSelectTarget.Get()->OnSelectTargeted();
						}
						return;
					}
				}
			}
		}
	}
	SoftTargeting();
}

void UODTargetComponent::DetectTargeting()
{
	DetectTargetArray.Empty();
	DetectTargetArray = DetectInRange(1900.f);
	DetectTargetArrayIndex = 0;
	if (!DetectTargetArray.IsEmpty())
	{
		if (IsValid(CurrentSelectTarget))
		{
			if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
			{
				CurrentSelectTarget.Get()->OnSelectTargetCleared();
			}
		}
		CurrentSelectTarget = DetectTargetArray[DetectTargetArrayIndex].Get();
		ServerSetSelectTarget(DetectTargetArray[DetectTargetArrayIndex].Get());
		if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
		{
			CurrentSelectTarget.Get()->OnSelectTargeted();
		}

		SoftTargetArray.Empty();
	}
}

void UODTargetComponent::DetectTargetArrayClear()
{
	DetectTargetArray.Empty();
	if (IsValid(CurrentSelectTarget))
	{
		CurrentSelectTarget->OnSelectTargetCleared();
	}
	CurrentSelectTarget = nullptr;
	ServerSetSelectTarget(nullptr);
}

void UODTargetComponent::SoftTargeting()
{
	SoftTargetArray.Empty();
	SoftTargetArray = DetectInRange(1500.f);

	if (!SoftTargetArray.IsEmpty())
	{
		if (IsValid(CurrentSelectTarget))
		{
			if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
			{
				CurrentSelectTarget.Get()->OnSelectTargetCleared();
			}
		}
		CurrentSelectTarget = SoftTargetArray[0].Get();
		ServerSetSelectTarget(SoftTargetArray[0].Get());
		if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
		{
			CurrentSelectTarget.Get()->OnSelectTargeted();
		}
	}
}


void UODTargetComponent::SelectTargeting(AODCombatCharacter* InTarget)
{
	if (!InTarget)
		return;

	if (IsValid(CurrentSelectTarget))
	{
		if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
		{
			CurrentSelectTarget.Get()->OnSelectTargetCleared();
		}
	}
	CurrentSelectTarget = InTarget;
	ServerSetSelectTarget(InTarget);
	if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
	{
		CurrentSelectTarget.Get()->OnSelectTargeted();
	}
}

void UODTargetComponent::TimerFunction()
{
	SetAutoTarget();
	ClearOutOfRangeTarget();
}

void UODTargetComponent::SetAutoTarget()
{
	if (!GetOwner() || !GetWorld())
		return;

	if (AutoTargetArray.IsEmpty() && CurrentAutoTarget.IsValid())
	{
		if (ComponentOwner.IsValid() && ComponentOwner->IsLocallyControlled())
		{
			if (IsValid(CurrentSelectTarget))
			{
				if (CurrentSelectTarget.Get() != CurrentAutoTarget.Get())
				{
					CurrentAutoTarget->OnAutoTargetCleared();
				}
				else
				{
					CurrentAutoTarget->OnAutoTargetedOutlineCleared();
				}
			}
			else
			{
				CurrentAutoTarget->OnAutoTargetCleared();
			}
		}
		CurrentAutoTarget.Reset();
		return;
		
	}

	AODCombatCharacter* ClosestTarget = nullptr;
	for (TWeakObjectPtr<AODCombatCharacter>& MakeAutoTarget : AutoTargetArray)
	{
		//플레이어일경우 추후에 추가 예정
		if (AODPlayer* Player = Cast<AODPlayer>(MakeAutoTarget.Get()))
			continue;

		if (AODMonsterBase* Monster = Cast<AODMonsterBase>(MakeAutoTarget.Get()))
		{
			if (!ClosestTarget)
			{
				ClosestTarget = Monster;
			}
			else
			{
				const float ClosestCharacterDistance = FVector::Dist(GetOwner()->GetActorLocation(),
				                                                     ClosestTarget->GetActorLocation());
				const float MonsterDistance =
					FVector::Dist(GetOwner()->GetActorLocation(), Monster->GetActorLocation());

				if (ClosestCharacterDistance > MonsterDistance)
				{
					ClosestTarget = Monster;
				}
			}
		}
	}

	if (!ClosestTarget)
		return;

	if (CurrentAutoTarget.Get() && CurrentAutoTarget.Get() != ClosestTarget)
	{
		const float ClosestCharacterDistance = FVector::Dist(GetOwner()->GetActorLocation(),
		                                                     ClosestTarget->GetActorLocation());
		const float CurrentAutoTargetDistance = FVector::Dist(GetOwner()->GetActorLocation(),
		                                                      CurrentAutoTarget.Get()->GetActorLocation());
		if (ClosestCharacterDistance < CurrentAutoTargetDistance)
		{
			if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
			{
				if (IsValid(CurrentSelectTarget))
				{
					if (CurrentSelectTarget.Get() != CurrentAutoTarget.Get())
					{
						CurrentAutoTarget.Get()->OnAutoTargetCleared();
					}
					else
					{
						CurrentAutoTarget.Get()->OnAutoTargetedOutlineCleared();
					}
				}
				else
				{
					CurrentAutoTarget.Get()->OnAutoTargetCleared();
				}
			}
			CurrentAutoTarget = ClosestTarget;
			if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
			{
				//if (IsValid(CurrentSelectTarget) && CurrentSelectTarget.Get() != CurrentAutoTarget.Get())
				CurrentAutoTarget->OnAutoTargeted();
			}
		}
	}
	else
	{
		CurrentAutoTarget = ClosestTarget;
		if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
		{
			//if (IsValid(CurrentSelectTarget) && CurrentSelectTarget.Get() != CurrentAutoTarget.Get())
			CurrentAutoTarget->OnAutoTargeted();
		}
	}
}

void UODTargetComponent::OnAutoTargetCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                           bool bFromSweep, const FHitResult& SweepResult)
{
	//플레이어 충돌판정은 추후 재설정
	if (AODPlayer* Player = Cast<AODPlayer>(OtherActor))
		return;

	if (AODMonsterBase* Monster = Cast<AODMonsterBase>(OtherActor))
	{
		AutoTargetArray.Add(Monster);
	}
}


void UODTargetComponent::OnAutoTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//플레이어 충돌판정은 추후 재설정
	if (AODPlayer* Player = Cast<AODPlayer>(OtherActor))
		return;

	if (AODMonsterBase* Monster = Cast<AODMonsterBase>(OtherActor))
	{
		if (AutoTargetArray.Contains(Monster))
		{
			AutoTargetArray.Remove(Monster);
		}
	}

	//범위 밖으로 나간 액터와 지금 선택된 타겟이 같을경우
	if (CurrentAutoTarget.IsValid() && CurrentAutoTarget.Get() == OtherActor)
	{
		if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
		{
			if (IsValid(CurrentSelectTarget))
			{
				if (CurrentSelectTarget.Get() != CurrentAutoTarget.Get())
				{
					CurrentAutoTarget.Get()->OnAutoTargetCleared();
				}
				else
				{
					CurrentAutoTarget.Get()->OnAutoTargetedOutlineCleared();
				}
			}
			else
			{
				CurrentAutoTarget.Get()->OnAutoTargetCleared();
			}
		}
		CurrentAutoTarget.Reset();
	}
}

TArray<TWeakObjectPtr<AODCombatCharacter>> UODTargetComponent::DetectInRange(const float& InRange)
{
	if (!ComponentOwner.IsValid())
		return {};

	TArray<FHitResult> OutActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	//ECC_GameTraceChannel2 / DetectTarget
	const TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2);

	const FVector Center = GetOwner()->GetActorLocation();
	const float Radius = InRange;

	UKismetSystemLibrary::SphereTraceMulti(
		this,
		Center,
		Center,
		Radius,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutActors,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		3.0f
	);

	TSet<TWeakObjectPtr<AODCombatCharacter>> MakeTargetArray = {};
	for (FHitResult& HitResult : OutActors)
	{
		if (AODCombatCharacter* HitActor = Cast<AODCombatCharacter>(HitResult.GetActor()))
		{
			if (AODPlayer* HitPlayer = Cast<AODPlayer>(HitActor))
			{
				//플레이어 로직 추가 예정
				continue;
			}
			MakeTargetArray.Add(HitActor);
		}
	}

	TArray<TWeakObjectPtr<AODCombatCharacter>> Result = MakeTargetArray.Array();
	FVector OwnerLoc = ComponentOwner->GetActorLocation();

	Result.Sort([OwnerLoc](const TWeakObjectPtr<AODCombatCharacter>& L,
	                       const TWeakObjectPtr<AODCombatCharacter>& R)
	{
		const AODCombatCharacter* LA = L.Get();
		const AODCombatCharacter* RA = R.Get();

		if (!LA || !RA)
			return LA != nullptr;

		const float DL = FVector::Dist(LA->GetActorLocation(), OwnerLoc);
		const float DR = FVector::Dist(RA->GetActorLocation(), OwnerLoc);

		return DL < DR;
	});


	return Result;
}

void UODTargetComponent::ClearOutOfRangeTarget()
{
	if (IsValid(CurrentSelectTarget))
	{
		const float Range = FVector::Dist(GetOwner()->GetActorLocation(),
		                                  CurrentSelectTarget->GetActorLocation());
		if (Range >= 2500.f)
		{
			if (ComponentOwner.IsValid() && ComponentOwner.Get()->IsLocallyControlled())
			{
				CurrentSelectTarget.Get()->OnSelectTargetCleared();
			}
			CurrentSelectTarget = nullptr;
			ServerSetSelectTarget(nullptr);
		}
	}
}

void UODTargetComponent::ServerSetSelectTarget_Implementation(AODCombatCharacter* NewTarget)
{
	if (IsValid(NewTarget))
	{
		CurrentSelectTarget = NewTarget;
	}
}

AODCombatCharacter* UODTargetComponent::GetTarget()
{
	if (IsValid(CurrentSelectTarget))
	{
		return CurrentSelectTarget.Get();
	}
	else if (CurrentAutoTarget.IsValid())
	{
		return CurrentAutoTarget.Get();
	}

	return nullptr;
}
