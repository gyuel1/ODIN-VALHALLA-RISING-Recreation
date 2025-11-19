// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODPlayer.h"

#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Components/ODCameraControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "AbilitySystem/Abilities/Combat/Default/OD_GA_Combat_NormalAttack.h"
#include "AI/NavigationSystemBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ODMonsterBase.h"
#include "Components/Image.h"
#include "Components/ODTargetingAndAttackComponent.h"
#include "Components/ODCombatComponent.h"
#include "Components/ODPartyComponent.h"
#include "Components/ODSkillComponent.h"
#include "Components/ODTargetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Constant/ODStencilValueConstant.h"
#include "DataAssets/Ability/ODActiveSkillDA.h"
#include "Debug/ODLogChannels.h"
#include "Elements/Columns/TypedElementAlertColumns.h"
#include "Engine/ActorChannel.h"
#include "Helper/ODDebugHelper.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Net/UnrealNetwork.h"
#include "Party/ODPartyInfo.h"
#include "Player/ODPlayerController.h"
#include "Player/ODPlayerState.h"
#include "Setting/ODGameUserSettings.h"
#include "Tags/ODAbilityTags.h"
#include "Widgets/HUD/ODWidget_HUD.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageQuePlayer.h"
#include "Widgets/NoneStackWidget/ODWidget_OverHeadPlayer.h"


AODPlayer::AODPlayer()
	:Super()
{
	bReplicates = true;
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bReplicateUsingRegisteredSubObjectList = true;
	
	CameraControllerComponent = CreateDefaultSubobject<UODCameraControllerComponent>(TEXT("CameraControllerComponent"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	//TargetComponent
	TargetComponent = CreateDefaultSubobject<UODTargetComponent>(TEXT("TargetComponent"));
	TargetingAndAttackComponent = CreateDefaultSubobject<UODTargetingAndAttackComponent>(TEXT("TargetingAndAttackComponent"));

	//AutoTargetCollision
	AutoTargetCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AutoTargetCollision"));
	AutoTargetCollision->SetupAttachment(GetRootComponent());
	AutoTargetCollision->SetSphereRadius(1000.0f);
	AutoTargetCollision->SetGenerateOverlapEvents(true);

	//SkillComponent
	SkillComponent = CreateDefaultSubobject<UODSkillComponent>(TEXT("SkillComponent"));

	//PartyComponent
	PartyComponent = CreateDefaultSubobject<UODPartyComponent>(TEXT("PartyComponent"));
	
	//Debug 자동타게팅 충돌 범위 표시 꺼진상태
	AutoTargetCollision->bHiddenInGame = true;
	
	// 위젯
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());

	ODCharacterType = EODCharacterType::Player;

	NetCullDistanceSquared = FMath::Square(6000.f);
	
}

void AODPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();
	if (APlayerController* OwningPlayerController = GetController<APlayerController>())
	{		
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->RemoveMappingContext(GameplayInputMappingContext);
			InputSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
		}
	}

	if (HasAuthority())
	{
		
	}
	else
	{
		if (!ODAsc)
		{
			InitAbilitySystem();
			UE_LOG(LogTemp, Log, TEXT("OD player initialized Client"));
		}
	}
}

void AODPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComp->BindAction(ZoomInputAction, ETriggerEvent::Triggered, this, &AODPlayer::HandleZoom);
		EnhancedInputComp->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AODPlayer::HandleLooKInput);
		EnhancedInputComp->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AODPlayer::Jump);
		EnhancedInputComp->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//키보드 이동
		EnhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AODPlayer::HandleMoveInput);
		EnhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &AODPlayer::HandleMoveInput_Completed);
		//마우스 이동
		EnhancedInputComp->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AODPlayer::OnMouseButtonPressed);
		EnhancedInputComp->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AODPlayer::OnMouseButtonReleased);
		//타겟팅
		EnhancedInputComp->BindAction(TabTargetInputAction, ETriggerEvent::Started, this, &AODPlayer::HandleTabTargetButton);
		EnhancedInputComp->BindAction(RTargetInputAction, ETriggerEvent::Started, this, &AODPlayer::HandleRTargetButton);
		EnhancedInputComp->BindAction(RTargetCancelInputAction, ETriggerEvent::Started, this, &AODPlayer::RTargetCancel);
		
		//타겟팅 공격
		EnhancedInputComp->BindAction(AttackTargetInputAction, ETriggerEvent::Triggered, this, &AODPlayer::HandleAttackTargetButton);
		EnhancedInputComp->BindAction(AutoHuntInputAction, ETriggerEvent::Started, this, &AODPlayer::HandleAutoHunt);
		//스킬사용
		EnhancedInputComp->BindAction(Skill1InputAction, ETriggerEvent::Started, this, &AODPlayer::Skill1Input);
		EnhancedInputComp->BindAction(Skill2InputAction, ETriggerEvent::Started, this, &AODPlayer::Skill2Input);
		EnhancedInputComp->BindAction(Skill3InputAction, ETriggerEvent::Started, this, &AODPlayer::Skill3Input);
		EnhancedInputComp->BindAction(Skill4InputAction, ETriggerEvent::Started, this, &AODPlayer::Skill4Input);
		EnhancedInputComp->BindAction(Skill5InputAction, ETriggerEvent::Started, this, &AODPlayer::Skill5Input);
		EnhancedInputComp->BindAction(Skill6InputAction, ETriggerEvent::Started, this, &AODPlayer::Skill6Input);
		//파티창
		EnhancedInputComp->BindAction(PartyInputAction, ETriggerEvent::Started, this, &AODPlayer::PartyUIInput);
	}
}

void AODPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled())
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(OD_STENCIL_NONE);
	}

	ConfigureOverHeadStatWidget();
	DamageQueContainerWidget = Cast<UODWidget_DamageQuePlayer>(DamageContainerWidgetComponent->GetUserWidgetObject());
}

void AODPlayer::Jump()
{
	if (ODAsc && (ODAsc->HasMatchingGameplayTag(StateTags::OD_State_Dead)|| ODAsc && ODAsc->HasMatchingGameplayTag(GameplayAbilityTags::OD_Ability_Block)))
	{
		return;
	}
	
	Super::Jump();
}

void AODPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystem();
	
	
}

void AODPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystem();
	
		
}

UAbilitySystemComponent* AODPlayer::GetAbilitySystemComponent() const
{
	if (AODPlayerState* MakePlayerState = GetPlayerState<AODPlayerState>())
	{
		if (UAbilitySystemComponent* MakeAsc = MakePlayerState->GetAbilitySystemComponent())
		{
			return MakeAsc;
		}
	}
	return nullptr;
}

void AODPlayer::InitAbilitySystem()
{
	if (AODPlayerState* MakePlayerState = GetPlayerState<AODPlayerState>())
	{
		if (UODAbilitySystemComponent* MakeAsc = Cast<UODAbilitySystemComponent>(
			MakePlayerState->GetAbilitySystemComponent()))
		{
			MakeAsc->InitAbilityActorInfo(MakePlayerState, this);
			ODAsc = MakePlayerState->GetAbilitySystemComponent();
			ODAttributeSet = MakePlayerState->GetODAttributeSet();
			Super::InitAbilitySystem();

			// if (ODAsc)
			// {
			// 	InitAbilitySystem_Client();
			// }
		
		}
	}
}

void AODPlayer::InitAbilitySystem_Client_Implementation()
{
	if (!ODAsc)
		InitAbilitySystem();
}


void AODPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AODPlayer, ReplicatedPawnName);
	DOREPLIFETIME(AODPlayer, InCombat);
}

bool AODPlayer::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	// Super 먼저 호출해서 기본 서브오브젝트들이 복제되게 함
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (!HasAuthority())
		return false;

	// 예: 컴포넌트에서 Party 포인터를 가져오는 getter
	if (PartyComponent)
	{
		WroteSomething |= Channel->ReplicateSubobject(PartyComponent->GetParty(),*Bunch,*RepFlags);
		for (UODPartyMember* PartyMember : PartyComponent->GetParty()->GetPartyMembers())
		{
			WroteSomething |= Channel->ReplicateSubobject(PartyMember, *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}

void AODPlayer::ConfigureOverHeadStatWidget()
{
	if (!OverHeadWidgetComponent) return;

	UODWidget_OverHeadPlayer* OverHeadWidget = Cast<UODWidget_OverHeadPlayer>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (!OverHeadWidget) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		// ASC 준비 안 됐으면 Timer로 재시도
		FTimerHandle RetryHandle;
		GetWorldTimerManager().SetTimer(RetryHandle, this, &AODPlayer::ConfigureOverHeadStatWidget, 0.05f, false);
		return;
	}

	OverHeadWidget->ConfigureASC(ASC);
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (LocalPlayerPawn == this)
	{
		OverHeadWidget->SetPlayerNameColor(true);
	}
	else
	{
		OverHeadWidget->SetPlayerNameColor(false);
	}
	
	GetWorldTimerManager().ClearTimer(OHSVisibilityUpdateTimerHandle);
	GetWorldTimerManager().SetTimer(OHSVisibilityUpdateTimerHandle, this, &AODPlayer::UpdateOHSVisibility, OHSCheckTime, true);
	OnRep_PawnPlayerName(); 
}

void AODPlayer::OnRep_PawnPlayerName()
{
	if (UWidgetComponent* WidgetComp = GetOverHeadWidgetComponent())
	{
		if (UODWidget_OverHeadPlayer* OverHeadWidget = Cast<UODWidget_OverHeadPlayer>(WidgetComp->GetUserWidgetObject()))
		{
			OverHeadWidget->SetWidgetPlayerName(ReplicatedPawnName);
			UE_LOG(LogTemp, Warning, TEXT("Pawn RepNotify: Overhead 위젯 이름 '%s'으로 업데이트 완료."), *ReplicatedPawnName);
		}
	}
}



void AODPlayer::SetPawnPlayerName(const FString& NewName)
{
	if (GetLocalRole() == ROLE_Authority) // 서버에서만 실행
	{
		ReplicatedPawnName = NewName;
		OnRep_PawnPlayerName(); 
	}
}

void AODPlayer::TestSkillActiveAbility()
{
	if (SkillComponent)
	{
		if (SkillComponent->TryActiveSkill(0))
			UE_LOG(LogODCooldown, Warning, TEXT("ㅇㅆㅇ"));
	}
}

void AODPlayer::TestSkillActiveAbilityByTag(FGameplayTag InTag)
{
	if (ODAsc && InTag.IsValid())
	{
		FGameplayTagContainer Container;
		Container.AddTag(InTag);
		ODAsc->TryActivateAbilitiesByTag(Container);
	}
}

void AODPlayer::TestAddSlotAbilityByTag(int32 InSlotIndex,FGameplayTag InTag)
{
	if (SkillComponent)
	{
		if (SkillComponent->GetActiveSkills().Num() > 0)
		{
			auto& Arr = SkillComponent->GetActiveSkills();
			
			if (Arr.Num() > 0 && InTag.IsValid())
				SkillComponent->AddSkillToSlot(InSlotIndex, InTag);
		}
	}
}


void AODPlayer::TestAddSlotAbility()
{
	if (SkillComponent)
	{
		if (SkillComponent->GetActiveSkills().Num() > 0)
		{
			auto& Arr = SkillComponent->GetActiveSkills();
			
			if (Arr.Num() > 0)
				SkillComponent->AddSkillToSlot(0, Arr[0]->GetTag());
		}
	}
}


void AODPlayer::TestRemoveSlotAbility()
{
	if (SkillComponent)
	{
		if (SkillComponent->GetActiveSkills().Num() > 0)
		{
			auto& Arr = SkillComponent->GetActiveSkills();
			
			if (Arr.Num() > 0)
				SkillComponent->RemoveSkillFromSlot(0);
		}
	}
}

AActor* AODPlayer::GetTarget() const
{
	if (IsValid(TargetingAndAttackComponent->CurrentTarget.Get()))
	{
		return TargetingAndAttackComponent->CurrentTarget.Get();
	}
	return nullptr;
}

void AODPlayer::Server_EndCombat_Implementation()
{
	InCombat = false;
	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("[SERVER] InCombat = false (%s)"), *GetName());
	}
}

void AODPlayer::OnRep_InCombat()
{
	/*UE_LOG(LogTemp, Warning, TEXT("[CLIENT] InCombat -> %s (%s)"),
	  InCombat ? TEXT("true") : TEXT("false"),
	  *GetName());*/
}

void AODPlayer::MarkCombatStart_Implementation()
{
	if (!InCombat)
	{
		InCombat = true;
		if (HasAuthority())
		{
			//UE_LOG(LogTemp, Warning, TEXT("[SERVER] InCombat = true (%s)"), *GetName());
		}
	}
}


void AODPlayer::ServerMouseMoveToLocation_Implementation(const FVector& Destination)
{
	TargetingAndAttackComponent->IsMovingClick = true;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Destination);
}
void AODPlayer::ServerSetMouseMovingClick_Implementation(bool MouseMoving)
{
	TargetingAndAttackComponent->IsMovingClick = MouseMoving;
}
void AODPlayer::ServerMouseStopMovement_Implementation()
{
	if (UCharacterMovementComponent* CMC = GetCharacterMovement())
	{
		CMC->StopMovementImmediately();
	}
	
}
void AODPlayer::ServerMoveToTarget_Implementation(AActor* Target)
{
	if (!Target || !HasAuthority())
	{
		return;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Target->GetActorLocation());
	
}



void AODPlayer::HandleZoom(const FInputActionValue& InputActionValue)
{
	const float AxisValue = InputActionValue.Get<float>();
	CameraControllerComponent->SetZoomAxis(AxisValue);
	
}

void AODPlayer::HandleLooKInput(const FInputActionValue& InputActionValue )
{
	CameraControllerComponent->HandleLooK(InputActionValue);
}


void AODPlayer::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	if (!ODAsc || ODAsc->HasMatchingGameplayTag(StateTags::OD_State_Dead))// || ODAsc->HasMatchingGameplayTag(GameplayAbilityTags::OD_Ability_Block))
	{
		return;
	}	
	FVector2D InputVal = InputActionValue.Get<FVector2D>();
	InputVal.Normalize();
	
	bIsUsingKeyboardForMove = true;
	
	//자동사냥 잠시 멈춤
	if (this->IsAutoHunt)
	{
		//타겟 초기화 코드 추가예정
		if (TargetingAndAttackComponent && !this->WasAutoHuntingBeforeMove)
		{
			this->WasAutoHuntingBeforeMove = true; 
			TargetingAndAttackComponent->StopAutoTargetingOff();
			this->IsAutoHunt = false;
			if (CameraControllerComponent)
			{
				CameraControllerComponent->StopLerpToNDPreset();
				CameraControllerComponent->SetAutoHuntActive(false);
			}
		}
	}
	if (TargetingAndAttackComponent->IsTargetTracking || TargetingAndAttackComponent->IsMovingClick)
	{		
		 ServerMouseStopMovement();
		
		if (UCharacterMovementComponent* CMC = GetCharacterMovement())
		{
			CMC->StopMovementImmediately();
		}    
		if (TargetingAndAttackComponent->IsTargetTracking)
		{
			TargetingAndAttackComponent->IsTargetTracking = false;
		}
    
		if (TargetingAndAttackComponent->IsMovingClick)
		{
			ServerSetMouseMovingClick(false);
			TargetingAndAttackComponent->IsMovingClick = false;
			TargetingAndAttackComponent->CachedDestination = FVector::ZeroVector;
		}
	}
	
	//캐릭터 방향 회전
	CameraControllerComponent->UpdateFromMoveAxis(InputVal);
		
	TargetingAndAttackComponent->CurrentAbilityCancel();
	AddMovementInput(GetMoveFWdDir() * InputVal.Y + CameraControllerComponent
	->GetLookRightDir() * InputVal.X);
}
//키보드가 누르고있을떄 마우스 키입력 차단
bool AODPlayer::IsKeyboardMoveActive() const
{
	return bIsUsingKeyboardForMove;
	
}

void AODPlayer::HandleMoveInput_Completed(const FInputActionValue& InputActionValue)
{	
	bIsUsingKeyboardForMove = false;
	this->WasAutoHuntingBeforeMove = false;
	//자동사냥 켜저있는중에 키보드를 때면 자동사냥 다시 시작
	
	if (TargetingAndAttackComponent)
	{
		if (bAutoHuntToggledByUser)
		{
			StartAutoHunt();
		}
	}
}

FVector AODPlayer::GetMoveFWdDir() const
{	
	return FVector::CrossProduct(CameraControllerComponent->GetLookRightDir(), FVector::UpVector);
}

void AODPlayer::OnMouseButtonPressed(const FInputActionValue& InputActionValue)
{
	if (!ODAsc || ODAsc->HasMatchingGameplayTag(StateTags::OD_State_Dead))
	{
		return;
	}
	TargetingAndAttackComponent->MouseButtonPressed();
	
}

void AODPlayer::OnMouseButtonReleased(const FInputActionValue& InputActionValue)
{
	if (!ODAsc || ODAsc->HasMatchingGameplayTag(StateTags::OD_State_Dead))
	{
		return;
	}
	CameraControllerComponent->SetMouseLookActive(false);
	if (IsKeyboardMoveActive())
	{
		return;
	}
	TargetingAndAttackComponent->MouseButtonReleased();
	//타겟이 아닐시 이동
	if (!TargetingAndAttackComponent->CachedDestination.IsZero())
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), TargetingAndAttackComponent->CachedDestination);
		ServerMouseMoveToLocation(TargetingAndAttackComponent->CachedDestination);
	}
}

void AODPlayer::HandleTabTargetButton(const FInputActionValue& InputActionValue)
{
	TargetComponent ->KeyboardTargeting();
}

void AODPlayer::HandleRTargetButton(const FInputActionValue& InputActionValue)
{
	TargetComponent->DetectTargeting();	
}

void AODPlayer::RTargetCancel(const FInputActionValue& InputActionValue)
{
	TargetComponent->DetectTargetArrayClear();
}

void AODPlayer::HandleAttackTargetButton(const FInputActionValue& InputActionValue)
{
	TargetingAndAttackComponent->TargetMove();
}

void AODPlayer::HandleAutoHunt(const FInputActionValue& InputActionValue)
{
	if (!TargetingAndAttackComponent)
	{
		return;
	}
	bAutoHuntToggledByUser = !bAutoHuntToggledByUser;
	this->IsAutoHunt = bAutoHuntToggledByUser;
	// 키보드 이동 중이 아닐 때만
	if (!bIsUsingKeyboardForMove)
	{
		if (bAutoHuntToggledByUser)
		{
			StartAutoHunt();
		}
		else
		{
			StopAutoHunt();
		}
	}
	
}


void AODPlayer::StartAutoHunt()
{
	if (!TargetingAndAttackComponent)
	{
		return;
	}
	
	IsAutoHunt = true;
	TargetingAndAttackComponent->StartAutoTargetingOn();
	//UE_LOG(LogTemp, Warning, TEXT("자동사냥 시작: ON"));
	CameraControllerComponent->StartLerpToNDPreset();
	CameraControllerComponent->SetAutoHuntActive(true);

	OnAutoHuntChanged.Broadcast(true);
}

void AODPlayer::StopAutoHunt()
{
	if (!TargetingAndAttackComponent)
	{
		return;
	}
	IsAutoHunt = false;
	TargetingAndAttackComponent->StopAutoTargetingOff();
	//UE_LOG(LogTemp, Warning, TEXT("자동사냥 중단: OFF"));
	this->WasAutoHuntingBeforeMove = false;
	CameraControllerComponent->StopLerpToNDPreset();
	CameraControllerComponent->SetAutoHuntActive(false);

	OnAutoHuntChanged.Broadcast(false);
}


void AODPlayer::UpdateOHSVisibility()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (IsValid(UODGameUserSettings::Get()))
	{
		if (UODGameUserSettings::Get()->GetCurrentPlayerInfoVisibility() == TEXT("MarkAll"))
		{
			if (LocalPlayerPawn)
			{
				//로컬플레이어와 현재캐릭터와 거리체크 (Ai-Local) (OtherPlayer-Local)
				float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
				OverHeadWidgetComponent->SetHiddenInGame(DistSquared > OHSVisibilityRange);
			}
			else
			{
				OverHeadWidgetComponent->SetHiddenInGame(false);
			}
		}

		else if (UODGameUserSettings::Get()->GetCurrentPlayerInfoVisibility() == TEXT("MarkOnly"))
		{
			if (LocalPlayerPawn == this)
			{
				OverHeadWidgetComponent->SetHiddenInGame(false);
			}
			else
			{
				OverHeadWidgetComponent->SetHiddenInGame(true);
			}
		}

		else if (UODGameUserSettings::Get()->GetCurrentPlayerInfoVisibility() == TEXT("MarkOtherPlayer"))
		{
			if (LocalPlayerPawn == this)
			{
				OverHeadWidgetComponent->SetHiddenInGame(true);
			}
			else
			{
				OverHeadWidgetComponent->SetHiddenInGame(false);
			}
		}

		else if (UODGameUserSettings::Get()->GetCurrentPlayerInfoVisibility() == TEXT("AllOff"))
		{
			OverHeadWidgetComponent->SetHiddenInGame(true);
		}
	}
}

void AODPlayer::Skill1Input(const FInputActionValue& InputActionValue)
{
	ProcessSkillInputBySlotIndex(0);
}

void AODPlayer::Skill2Input(const FInputActionValue& InputActionValue)
{
	ProcessSkillInputBySlotIndex(1);
}

void AODPlayer::Skill3Input(const FInputActionValue& InputActionValue)
{
	ProcessSkillInputBySlotIndex(2);
}

void AODPlayer::Skill4Input(const FInputActionValue& InputActionValue)
{
	ProcessSkillInputBySlotIndex(3);
}

void AODPlayer::Skill5Input(const FInputActionValue& InputActionValue)
{
	ProcessSkillInputBySlotIndex(4);
}

void AODPlayer::Skill6Input(const FInputActionValue& InputActionValue)
{
	ProcessSkillInputBySlotIndex(5);
	
}

void AODPlayer::PartyUIInput(const FInputActionValue& InputActionValue)
{
	
}

//스킬 사용 
void AODPlayer::ProcessSkillInputBySlotIndex(uint32 SlotIndex)
{
	if (!GetOwner() || !SkillComponent || !SkillComponent->GetSkillSlots().IsValidIndex(SlotIndex) || !SkillComponent->GetSkillSlots()[SlotIndex].IsValid())
	{
		
		return;
	}
    
	if (TryActivateSkillBySlotIndex(SlotIndex))
	{
		return;
	}

	TargetingAndAttackComponent->TargetMove();

}
//스킬 사용
bool AODPlayer::TryActivateSkillBySlotIndex(uint32 SlotIndex)
{
	if (SkillComponent)
	{
		return SkillComponent->TryActiveSkill(SlotIndex);
	}
	return false;
}





