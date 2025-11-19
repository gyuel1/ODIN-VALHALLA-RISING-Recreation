// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODMonsterBase.h"

#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "AI/Spawning/ODMonsterSpawnActor.h"
#include "Components/ODRewardComponent.h"
#include "Components/WidgetComponent.h"
#include "FunctionLibrary/ODFrontendFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/ODUISubsystem.h"
#include "Tags/ODUITag.h"
#include "Widgets/ODWidget_Activatable.h"
#include "Widgets/Gauge/ODBaseGauge.h"
#include "Widgets/HUD/MonsterInfo/ODWidget_HUDMonsterInfo.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageQueMonster.h"
#include "Widgets/NoneStackWidget/ODWidget_OverHeadMonster.h"
#include "Widgets/NoneStackWidget/ODWidget_Target.h"

AODMonsterBase::AODMonsterBase()
	:Super()
{
	bReplicates = true;
	AODMonsterBase::SetReplicateMovement(true);
	
	
	AutoPossessAI  = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	ODAsc = CreateDefaultSubobject<UODAbilitySystemComponent>(TEXT("ODAsc"));
	ODAsc->SetIsReplicated(true);

	ODAttributeSet = CreateDefaultSubobject<UODAttributeSet>(TEXT("ODAttributeSet"));
	// ODMonsterRewardAttributeSet = CreateDefaultSubobject<UODMonsterRewardAttributeSet>(TEXT("ODMonsterRewardAttributeSet"));

	ODRewardComponent = CreateDefaultSubobject<UODRewardComponent>(TEXT("RewardComponent"));
	
	// 위젯
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());
	TargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidgetComponent"));
	TargetWidgetComponent->SetupAttachment(GetRootComponent());
	
}

void AODMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	//지연로딩을 위해 호출타이밍 조절해야함
	InitAbilitySystem();

	if (bNeedsDeathMesh)
	{
		SwitchDeathMeshIfNeed(false);
	}

	ConfigureOverHeadStatWidget();
	ConfigureTargetWidget();
	DamageQueContainerWidget = Cast<UODWidget_DamageQueMonster>(DamageContainerWidgetComponent->GetUserWidgetObject());
}

void AODMonsterBase::InitAbilitySystem()
{
	Super::InitAbilitySystem();
	ODAsc->InitAbilityActorInfo(this,this);
}

void AODMonsterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (MonsterInfo)
	{
		MonsterInfo->DeactivateWidget();
		MonsterInfo->RemoveFromParent();
	}
	
	if (!HasAuthority()) return;
	
	if (!IsValid(MySpawner)) return;
	
	MySpawner->RequestRespawn(GetClass(), SpawnContext);
}

void AODMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	GetCharacterMovement()->MaxWalkSpeed = MonsterMoveSpeeds.MaxWalkSpeed;
}

void AODMonsterBase::OnAutoTargeted()
{
	Super::OnAutoTargeted();

	if (OverHeadWidget)
	{
		OverHeadWidget->VisibilityGauge(ESlateVisibility::Visible);
		OverHeadWidget->VisibilityName(ESlateVisibility::Visible);
	}
}

void AODMonsterBase::OnAutoTargetCleared()
{
	Super::OnAutoTargetCleared();

	if (OverHeadWidget)
	{
		OverHeadWidget->VisibilityGauge(ESlateVisibility::Hidden);
		OverHeadWidget->VisibilityName(ESlateVisibility::Hidden);
	}
}

void AODMonsterBase::OnSelectTargeted()
{
	Super::OnSelectTargeted();

	if (!TargetWidget) return;

	TargetWidget->VisibilityTargetMain(ESlateVisibility::Visible);
	TargetWidget->TargetMainActiveAnimation();

	if (OverHeadWidget)                                                                   
	{                                                                                     
		OverHeadWidget->VisibilityGauge(ESlateVisibility::Visible);                       
		OverHeadWidget->VisibilityName(ESlateVisibility::Visible);                        
	}                                                                                     
	
	CreatedHudMonsterInfo();
}

void AODMonsterBase::OnSelectTargetCleared()
{
	Super::OnSelectTargetCleared();

	if (!TargetWidget) return;

	if (OverHeadWidget)                                                            
	{                                                                              
		OverHeadWidget->VisibilityGauge(ESlateVisibility::Hidden);                 
		OverHeadWidget->VisibilityName(ESlateVisibility::Hidden);                  
	}                                                                              
	TargetWidget->VisibilityTargetMain(ESlateVisibility::Hidden);
	TargetWidget->VisibilityTargetSub(ESlateVisibility::Hidden);
	MonsterInfo->DeactivateWidget();
	MonsterInfo->RemoveFromParent();
}

int32 AODMonsterBase::GetRepGraphCount()
{
	

	return 0;
}

void AODMonsterBase::SetRepGraphCount(const int32 InRepGraph)
{
	Super::SetRepGraphCount(InRepGraph);
}

void AODMonsterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

#pragma region DieMeshSwitch 
void AODMonsterBase::SwitchDeathMeshIfNeed(bool bVisibleDieMesh)
{
	if (!bNeedsDeathMesh)
	{
		return;
	}

	Multicast_SetVisibleDieMesh(bVisibleDieMesh);
	
}

USkeletalMeshComponent* AODMonsterBase::FindDieMesh()
{
	static const FName DieTag("DieMesh");
	
	if (CachedDieMesh)
		return CachedDieMesh;
	
	TArray<USkeletalMeshComponent*> Components;
	GetComponents<USkeletalMeshComponent>(Components);

	for (USkeletalMeshComponent* Component : Components)
	{
		if (Component && Component->ComponentHasTag(DieTag))
		{
			CachedDieMesh = Component;
			break;
		}
	}
	
	return CachedDieMesh;
}

void AODMonsterBase::Multicast_SetVisibleDieMesh_Implementation(bool bVisible)
{
	if (!CachedDieMesh)
		FindDieMesh();
	
	if (!CachedDieMesh || !GetMesh()) return;
	
	CachedDieMesh->SetVisibility(bVisible);
	CachedDieMesh->SetHiddenInGame(!bVisible);
	CachedDieMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetVisibility(!bVisible);
	GetMesh()->SetHiddenInGame(bVisible);
	GetMesh()->SetCollisionEnabled(!bVisible ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	
	GetAbilitySystemComponent()->RefreshAbilityActorInfo();

	CachedDieMesh->SetSimulatePhysics(false);
}
#pragma endregion

#pragma region Getter
AActor* AODMonsterBase::GetTarget() const
{
	return TargetPlayerActor; 
}

FPerceptionInitValue AODMonsterBase::GetPerceptionInitValue()
{
	return PerceptionInitValue;
}

EMonsterEngageType AODMonsterBase::GetEngageType()
{
	return EngageType;
}

FMonsterMoveSpeed AODMonsterBase::GetMonsterMoveSpeed()
{
	return MonsterMoveSpeeds;
}

EMonsterSpawnContext AODMonsterBase::GetMonsterSpawnContext()
{
	return SpawnContext;
}

EMonsterRank AODMonsterBase::GetEMonsterRank()
{
	return MonsterRank;
}

AODMonsterSpawnActor* AODMonsterBase::GetSpawnActor()
{
	return MySpawner;
}

FTargetSystemValue& AODMonsterBase::GetPatrol()
{
	return TargetSystemValue;
}

USkeletalMeshComponent* AODMonsterBase::GetCachedDieMesh()
{
	return CachedDieMesh;
}

TSubclassOf<AODMonsterBase> AODMonsterBase::GetMonsterDieClass()
{
	return MonsterClass;
}

bool AODMonsterBase::GetbNeedsDeathMesh()
{
	return bNeedsDeathMesh;
}
#pragma endregion

#pragma region Setter
void AODMonsterBase::SetTargetPlayerActor(AActor* TargetActor)
{
	TargetPlayerActor = TargetActor;
}

void AODMonsterBase::SetSpawner(AODMonsterSpawnActor* Spawner)
{
	MySpawner = Spawner;
}
#pragma endregion

void AODMonsterBase::ConfigureOverHeadStatWidget()
{
	if (!OverHeadWidgetComponent) return;

	OverHeadWidget = Cast<UODWidget_OverHeadMonster>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (!OverHeadWidget) return;


	OverHeadWidget->SetName(this);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC || ASC->GetNumericAttribute(UODAttributeSet::GetHealthAttribute()) <= 0)
	{
		// ASC 준비 안 됐으면 Timer로 재시도
		FTimerHandle RetryHandle;
		GetWorldTimerManager().SetTimer(RetryHandle, this, &ThisClass::ConfigureOverHeadStatWidget, 0.05f, false);
		return;
	}

	OverHeadWidget->ConfigureASC(ASC);
	
}

void AODMonsterBase::ConfigureHudMonsterInfo()
{
	
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	
	if (!ASC)
	{
		// ASC 준비 안 됐으면 Timer로 재시도
		FTimerHandle RetryHandle;
		GetWorldTimerManager().SetTimer(RetryHandle, this, &ThisClass::ConfigureHudMonsterInfo, 0.05f, false);
		Debug::Print(TEXT("준비중"));
		return;
	}


	MonsterInfo->SetName(this);
	MonsterInfo->ConfigureASC(ASC);
	
	

	// 초기값 강제 업데이트
	bool bFoundValue = false;
	bool bFoundMax = false;
	float CurrentValue = ASC->GetGameplayAttributeValue(UODAttributeSet::GetHealthAttribute(), bFoundValue);
	float MaxValue = ASC->GetGameplayAttributeValue(UODAttributeSet::GetMaxHealthAttribute(), bFoundMax);

	if (bFoundValue && bFoundMax)
	{
		UE_LOG(LogTemp, Error, TEXT("MaxHP: %f"), MaxValue);
		UE_LOG(LogTemp, Error, TEXT("CurrentHP: %f"), CurrentValue);
		MonsterInfo->HPGauge->SetGaugeValue(CurrentValue, MaxValue);
	}
	
}

void AODMonsterBase::ConfigureTargetWidget()
{
	if (!TargetWidgetComponent) return;

	TargetWidget = Cast<UODWidget_Target>(TargetWidgetComponent->GetUserWidgetObject());

	if (!TargetWidget) return;
	
}

void AODMonsterBase::CreatedHudMonsterInfo()
{
	UODUISubsystem* ODUISubSystem = UODUISubsystem::Get(this);
	
	ODUISubSystem->PushSoftWidgetToStackAsync(UITags::OD_WidgetStack_GameMenu, UODFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UITags::OD_Widget_MonsterInfo),
		[this](EAsyncPushWidgetState InPushState, UODWidget_Activatable* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				{
					PushedWidget->SetOwningPlayer(UGameplayStatics::GetPlayerController(this, 0));
					MonsterInfo = nullptr;
					MonsterInfo = Cast<UODWidget_HUDMonsterInfo>(PushedWidget);
					break;
				}	
				
			case EAsyncPushWidgetState::AfterPush:
				ConfigureHudMonsterInfo();
				break;
			default:
				break;
			}
			
		}
		);
}
