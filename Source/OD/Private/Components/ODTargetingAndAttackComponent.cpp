// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODTargetingAndAttackComponent.h"
#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODNpcBase.h"
#include "Character/ODPlayer.h"
#include "Components/ODCameraControllerComponent.h"
#include "Components/ODTargetComponent.h"
#include "Debug/ODLogChannels.h"
#include "Helper/ODDebugHelper.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "DataAssets/Ability/ODActiveSkillDA.h"
#include "DataAssets/Ability/ODSkillDA.h"
#include "Tags/ODAbilityTags.h"
#include "Widgets/NoneStackWidget/ODWidget_Target.h"


// Sets default values for this component's properties
UODTargetingAndAttackComponent::UODTargetingAndAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	// ...
}

void UODTargetingAndAttackComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODTargetingAndAttackComponent, IsMovingClick);
	DOREPLIFETIME(UODTargetingAndAttackComponent, CurrentTarget);
	DOREPLIFETIME(UODTargetingAndAttackComponent, bTargetIsDeadOrBlocked);
	DOREPLIFETIME(UODTargetingAndAttackComponent, bTargetDeadOrBlocked);
	DOREPLIFETIME(UODTargetingAndAttackComponent, IsTargetTracking);		
	
}



void UODTargetingAndAttackComponent::AttemptInitializeODAsc()
{
	//어빌리티 찾기
	if (AODCombatCharacter* CombatCharacter = Cast<AODCombatCharacter>(GetOwner()))
	{
		UAbilitySystemComponent* FoundASC = CombatCharacter->GetAbilitySystemComponent(); 

		if (FoundASC)
		{
			ODAsc = Cast<UODAbilitySystemComponent>(FoundASC);

			if (ODAsc)
			{
				// 성공했으면 타이머를 멈춥
				GetWorld()->GetTimerManager().ClearTimer(ASCInitTimerHandle); 
				return;
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("ODAsc 초기화 재시도..."));
	}
}



// Called when the game starts
void UODTargetingAndAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	//어빌리티 불러오기 
	if (!ODAsc)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ASCInitTimerHandle,
			this,
			&UODTargetingAndAttackComponent::AttemptInitializeODAsc,
			0.1f, 
			true 
		);
		AttemptInitializeODAsc(); 
	}

	if (!TargetComponent && GetOwner())
	{
		TargetComponent = GetOwner()->FindComponentByClass<UODTargetComponent>();
		
	}

	 if (!CameraControllerComponent && GetOwner())
    {
        CameraControllerComponent = GetOwner()->FindComponentByClass<UODCameraControllerComponent>();    
    }

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MouseHoldTime);
	}
}

void UODTargetingAndAttackComponent::CurrentAbilityCancel()
{
	//수정예정
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag("OD.Ability.Block");
	if (ODAsc)
	{
		
		if (!ODAsc->HasMatchingGameplayTag(Tag))
		{
			ODAsc->CurrentMontageStop(0.15f);
		}
	}
}

bool UODTargetingAndAttackComponent::IsCurrentAbilityCancel()
{
	if (!ODAsc)
		return false;

	FGameplayTag Tag = FGameplayTag::RequestGameplayTag("OD.Ability.Block");
	if (ODAsc->HasMatchingGameplayTag(Tag))
		return false;
	
	ODAsc->CurrentMontageStop(0.15f);
	return true;
	
	
}

void UODTargetingAndAttackComponent::TargetAttack()
{
	
	//F공격 부분
	if (CurrentTarget == nullptr)
	{
		bTargetDeadOrBlocked = false;
		return;
	}
	if (!IsTargetTracking || !IsValid(CurrentTarget))
	{
		GetWorld()->GetTimerManager().ClearTimer(TargetCheckTimer);
		IsTargetTracking = false;
		if (AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner()))
		{

			//여기 테스트로 수정함
			if (UAbilitySystemComponent* ASC = ODPlayer->GetAbilitySystemComponent())
			{
				if (ASC->GetCurrentMontage() && ASC->HasMatchingGameplayTag(GameplayAbilityTags::OD_Ability_Block))
					return;
			}
			
			ODPlayer->ServerMouseStopMovement();
			if (UCharacterMovementComponent* CMC = ODPlayer->GetCharacterMovement())
			{
				CMC->StopMovementImmediately();
			}
		}
		if (AODMonsterBase* MonsterTarget = Cast<AODMonsterBase>(CurrentTarget))
		{
			if (MonsterTarget->GetTargetWidget())
			{
				MonsterTarget->GetTargetWidget()->VisibilityTargetSub(ESlateVisibility::Hidden);
			}
		}
					
		//UE_LOG(LogTemp, Warning, TEXT("타겟이 없습니다."));
		
		return ;
	}
	AODCombatCharacter* Target = Cast<AODCombatCharacter>(CurrentTarget.Get());
	if (IsValid(Target))
	{
		Server_CheckTarget(Target);
	}
	if (CurrentTarget != nullptr)
	{
		bTargetDeadOrBlocked = true;
		AttackAndSkill();		
	}
	
}

void UODTargetingAndAttackComponent::TargetMove()
{
	if (IsTargetTracking) // 추적이 중지된 상태면 이동도 막음
	{
		return;
	}

	if (TargetComponent)
	{
		AActor* ATargetActor = TargetComponent->GetTarget();
	
		if (!IsValid(ATargetActor))
		{
			return;
		}
		// 타겟 이 있으면
		CurrentTarget  = ATargetActor;
		SetServerCurrentTarget(ATargetActor);
		
		UE_LOG(LogOD,Warning,TEXT("<UNK> <UNK> <UNK>: %s"), *CurrentTarget->GetName());
		IsTargetTracking = true;
		
		// 타이머로 주기적으로 타겟 체크
		GetWorld()->GetTimerManager().SetTimer(
		TargetCheckTimer,
		this,
		&UODTargetingAndAttackComponent::TargetAttack,
		0.05f,  
		true  
		);
		AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner());
		if (ODPlayer && ODPlayer->GetController())
		{
			ODPlayer->ServerMoveToTarget(CurrentTarget);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(ODPlayer->GetController(), CurrentTarget->GetActorLocation());
		}
	}
}
void UODTargetingAndAttackComponent::MouseButtonPressed()
{
	MouseButtonHeld = true;
	IsLooking = false;

	 
	//카메라 가 회전 인지 캐릭터 이동인지 판단
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			MouseHoldTime,
			[this]()
		{
			if (!IsValid(CameraControllerComponent))
			{
				return;
			}
			if (MouseButtonHeld)
			{
				IsLooking = true;
				CameraControllerComponent->SetMouseLookActive(true);			
			}
		},
		ShortPressThreshold,
		false
		);
	}
}

void UODTargetingAndAttackComponent::AttackAndSkill_Implementation()
{
	AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner());
	if (!ODPlayer)
	{
		return;
	}

	if (!IsCurrentAbilityCancel())
		return;
	
	const int32 NumSkillSlots = 6;
	for (int32 Index = 0; Index < NumSkillSlots; ++Index)
	{
		// 추후 거리 쿨타임 등 추가예정
		// 자동사냥으로 발동 시도
		if (ODPlayer->TryActivateSkillBySlotIndex(Index)) 
		{
			ODPlayer->MarkCombatStart();
			return;
		}
	}
		
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("OD.Ability.Combat.NormalAttack")));
	if (ODAsc)
	{
		if (ODAsc->TryActivateAbilitiesByTag(TagContainer))
		{
			ODPlayer->MarkCombatStart();
			
		}
	}
}

// void UODTargetingAndAttackComponent::AttackAndSkill()
// {
// 	
// }


void UODTargetingAndAttackComponent::MouseButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(MouseHoldTime);
	MouseButtonHeld = false;
	if (IsLooking)
	{
		IsLooking = false;
		return;
	}
		
	//연속 클릭 방지
	const float WorldTime = GetWorld()->GetTimeSeconds();
	if (WorldTime - LastClickTime < MinClickInterval)
	{
		return;
	}
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return;
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC) return;
		
	FVector WorldLocation, WorldDirection;//카메라 근처 시작점 , 카메라 근처 방향
	//충돌 감지와 마우스 커서 아래 충돌이 발생했는지 판단용
	//내 캐릭터는 제외 하는부분 
	if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))//마우스 좌표(2D)를 3D 좌표로 변환
	{
		FCollisionQueryParams TraceParams(FName(TEXT("MouseTrace")), true, GetOwner());
		bHitSuccessful = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			WorldLocation, // 트레이스 시작점 (카메라 근처)
			WorldLocation + WorldDirection  * 10000.0f, // 트레이스 끝점 
			ECollisionChannel::ECC_Visibility,
			TraceParams 
		);
		
	}
	//몬스터,npc,플레이어 타겟 지정
	if (bHitSuccessful)
	{
		
		AActor* HitActor = HitResult.GetActor();
		AODCharacter* CharacterTarget = Cast<AODCharacter>(HitActor);
		
		
		if (CharacterTarget)
		{
			AActor* CurrentSelectedTarget = TargetComponent->GetTarget();

			// 첫타겟과 같은 타겟을 누르면
			if (AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner()))
			{
				if (CurrentSelectedTarget == CharacterTarget)
				{
					if (AODMonsterBase* MonsterTarget = Cast<AODMonsterBase>(CharacterTarget))
					{
						TargetMove();
						if (MonsterTarget->GetTargetWidget())
						{
							MonsterTarget->GetTargetWidget()->VisibilityTargetSub(ESlateVisibility::Visible);
						}
					}
					UAIBlueprintHelperLibrary::SimpleMoveToLocation(ODPlayer->GetController(), CurrentSelectedTarget->GetActorLocation());
					ODPlayer->ServerMoveToTarget(CurrentSelectedTarget);
					OD_LOG_SERVER(LogTemp, Warning, TEXT("현재 : %s"), *CurrentSelectedTarget->GetName());
					return; 
				}
			}
			//추후 npc생성시 수정
			/*if (AODNpcBase* Npc = Cast<AODNpcBase>(CharacterTarget))
			{	
				TargetComponent->SelectTargeting(Npc);				
			}*/
			if (AODMonsterBase* Monster = Cast<AODMonsterBase>(CharacterTarget))
			{
				TargetComponent->SelectTargeting(Monster);// 몬스터를 타겟으로 지정
							
			}
			else if (AODPlayer* OtherPlayer  = Cast<AODPlayer>(CharacterTarget))
			{
				if (OtherPlayer != GetOwner()) 
				{
					TargetComponent->SelectTargeting(OtherPlayer);								
				}
			}
			
		}
		//땅찍기
		if (HitActor && !CharacterTarget)
		{
			TileClick();		
		}
	
	}
	else
	{
		//UI
		TargetComponent->SelectTargeting(nullptr);				
	}
	IsMovingClick = true;
	//연속 클릭방지 타임 갱신
	LastClickTime = WorldTime;	
}
void UODTargetingAndAttackComponent::TileClick()
{
	AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner());
	//공격으로 가는중 클릭시
	if (IsTargetTracking)
	{
		GetWorld()->GetTimerManager().ClearTimer(TargetCheckTimer); 
		IsTargetTracking = false;
	}
	if (TargetComponent)
	{
		TargetComponent->SelectTargeting(nullptr);
		
		if (ODPlayer->IsAutoHunt)
		{
			
			ODPlayer->WasAutoHuntingBeforeMove = true;
       
			StopAutoTargetingOff();
			ODPlayer->IsAutoHunt = false;
		}
	}
	
	//내비 메시 처리
	if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation NavLoc;
		CachedDestination =	NavSys->ProjectPointToNavigation(HitResult.Location, NavLoc) ? NavLoc.Location : HitResult.Location;
	}
	else
	{
		CachedDestination = HitResult.Location;
	}
}

void UODTargetingAndAttackComponent::SetServerCurrentTarget_Implementation(AActor* InActor)
{
	if (IsValid(InActor))
	{
		CurrentTarget = InActor;
	}
}
//몬스터 죽으면 캐릭이 이동하는거 방어 부분
void UODTargetingAndAttackComponent::Server_CheckTarget_Implementation(AODCombatCharacter* TargetToCheck)
{
	if (!IsValid(TargetToCheck) || TargetToCheck != CurrentTarget.Get())
	{
		return;
	}
	if (UAbilitySystemComponent* TargetASC = TargetToCheck->GetAbilitySystemComponent())
	{
		//몬스터 태그
		if (TargetASC->HasMatchingGameplayTag(StateTags::OD_State_Dead) ||
			TargetASC->HasMatchingGameplayTag(GameplayAbilityTags::OD_Ability_Combat_State_Death))
		{
			CurrentTarget = nullptr;
			SetServerCurrentTarget(nullptr);
			Multicast_StopMovementOnTargetDead();

			Client_ForceTargetResetAndSearch();
		}
	}
}
void UODTargetingAndAttackComponent::Client_ForceTargetResetAndSearch_Implementation()
{
	//클라에도 복제
	CurrentTarget = nullptr;
	bTargetIsDeadOrBlocked = true;
	bTargetDeadOrBlocked = false;
}
void UODTargetingAndAttackComponent::Server_SetNewTarget_Implementation(AODCombatCharacter* Target)
{
	if (IsValid(Target))
	{
		CurrentTarget = Target;
		SetServerCurrentTarget(Target);
	}
}
void UODTargetingAndAttackComponent::Multicast_StopMovementOnTargetDead_Implementation()
{
	// 내비게이션, 이동 명령 중단
	if (AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner()))
	{
		if (UCharacterMovementComponent* CharacterMovement = ODPlayer->GetCharacterMovement())
		{
			CharacterMovement->StopMovementImmediately();
		}

		if (ODPlayer->GetController())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(ODPlayer->GetController(), ODPlayer->GetActorLocation());
		}
	}

	IsTargetTracking = false; // 타겟 추적 비활성화
	CurrentTarget = nullptr;  // 타겟 초기화

}
void UODTargetingAndAttackComponent::AutoHuntLoop()
{
	//자동사냥
	if (bTargetDeadOrBlocked)
	{
		return;
	}

	AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner());
	//여기 테스트로 수정함
	if (UAbilitySystemComponent* ASC = ODPlayer->GetAbilitySystemComponent())
	{
		if (ASC->GetCurrentMontage() && ASC->HasMatchingGameplayTag(GameplayAbilityTags::OD_Ability_Block))
			return;	
	}
	
	if (!ODPlayer|| !TargetComponent)
	{
		return;
	}
	AODCombatCharacter* Target = Cast<AODCombatCharacter>(CurrentTarget.Get());
	if (!IsValid(Target))
	{
		TargetComponent->GetAutoTarget();
		AODCombatCharacter* AAutoTarget = TargetComponent->GetCurrentAutoTarget();
		
		if (IsValid(AAutoTarget))
		{
			Server_SetNewTarget(AAutoTarget);
			bTargetIsDeadOrBlocked = false;
		}
		return; 		
	}
	if (IsValid(Target))
	{
		Server_CheckTarget(Target);
		Target = Cast<AODCombatCharacter>(CurrentTarget.Get());
		if (bTargetIsDeadOrBlocked || !IsValid(Target)) 
		{
			return ; 
		}
	}
	//타겟과 실제 거리
	float DistanceToTarget = (ODPlayer->GetActorLocation() - Target->GetActorLocation()).Size();
	
	// 타겟 이 있으면
	if (IsValid(Target))
	{
		const float MaxDistanceToKeepAutoTarget = 1500.0f;
		if (DistanceToTarget > MaxDistanceToKeepAutoTarget)
		{
			CurrentTarget = nullptr;
			SetServerCurrentTarget(nullptr);
			return;
		}
		
		float CurrentAttackRange = 300.f;
		
		if (DistanceToTarget > CurrentAttackRange)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(ODPlayer->GetController(), Target->GetActorLocation());
			ODPlayer->ServerMoveToTarget(Target);
		}
			
		AttackAndSkill();
	
	}
}

void UODTargetingAndAttackComponent::StartAutoTargetingOn()
{
	//자동사냥 시작
	if (TargetComponent)
	{
		TargetComponent->StartAutoTargeting(); 
	}
	    
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(AutoHuntLoopTimer))
		return;

	GetWorld()->GetTimerManager().SetTimer
	(AutoHuntLoopTimer,
	 this,
	 &UODTargetingAndAttackComponent::AutoHuntLoop,
	 0.005f,
	 true);
}

void UODTargetingAndAttackComponent::StopAutoTargetingOff()
{
	//자동사냥 멈추기
	GetWorld()->GetTimerManager().ClearTimer(AutoHuntLoopTimer);    
	
	if (TargetComponent)
	{		
		TargetComponent->StopAutoTargeting(); 
	}
	
	IsTargetTracking = false;
    
	if (AODPlayer* ODPlayer = Cast<AODPlayer>(GetOwner()))
	{
		
		ODPlayer->ServerMouseStopMovement(); 
		if (UCharacterMovementComponent* CMC = ODPlayer->GetCharacterMovement())
		{
			CMC->StopMovementImmediately(); 
		}
	}
}





