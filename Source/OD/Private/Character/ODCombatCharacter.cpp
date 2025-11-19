// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODCombatCharacter.h"
#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "AbilitySystem/ODAttributeSet.h"
#include "Character/ODMonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/ODCombatComponent.h"
#include "Components/ODDamageReceivedComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/WidgetComponent.h"
#include "Constant/ODStencilValueConstant.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Tags/ODAbilityTags.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageQueContainer.h"
#include "Widgets/NoneStackWidget/ODWidget_OverHeadMonster.h"

using namespace AttributeDataTags;

AODCombatCharacter::AODCombatCharacter()
	: Super()
{
	bReplicates = true;
	
	ODCombatComponent = CreateDefaultSubobject<UODCombatComponent>(TEXT("ODCombatComponent"));
	ODCombatComponent->SetIsReplicated(true);

	ODDamageReceivedComponent = CreateDefaultSubobject<UODDamageReceivedComponent>(TEXT("ODDamageReceivedComponent"));
	ODDamageReceivedComponent->SetIsReplicated(true);
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);

	//Trace Response #ECC_GameTraceChannel2 = DetectTarget
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);

	
	//Trace Response #ECC_GameTraceChannel3 = AttackCollision
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);

	//Object Response
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	DamageContainerWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageContainerWidgetComponent"));
	DamageContainerWidgetComponent->SetupAttachment(GetRootComponent());

	//거리별 최적화
	bAlwaysRelevant = false;
	bOnlyRelevantToOwner = false;
	bNetUseOwnerRelevancy = false;
	NetCullDistanceSquared = FMath::Square(3500.f);
}

void AODCombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh())
		GetMesh()->SetReceivesDecals(false);
}


UAbilitySystemComponent* AODCombatCharacter::GetAbilitySystemComponent() const
{
	if (!ODAsc)
	{
		return nullptr;
	}
	return ODAsc;
}

UAttributeSet* AODCombatCharacter::GetAttributeSet() const
{
	if (!ODAttributeSet)
	{
		return nullptr;
	}
	return ODAttributeSet;
}

void AODCombatCharacter::InitAbilitySystem()
{
	ODCombatComponent->InitStats();
	ODCombatComponent->ODCombatInitialize(this);
}

void AODCombatCharacter::OnAutoTargetedOutline()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = OD_STENCIL_TARGET_OUTLINE;
}

void AODCombatCharacter::OnAutoTargetedOutlineCleared()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->CustomDepthStencilValue = OD_STENCIL_NONE;
}

void AODCombatCharacter::OnAutoTargeted()
{
	if (GetNetMode() == NM_DedicatedServer)
		return;
	
	OnAutoTargetedOutline();

	//Client Only
	if (OnAutoTargetedDele.IsBound())
	{
		OnAutoTargetedDele.Broadcast();
	}
	
}

void AODCombatCharacter::OnAutoTargetCleared()
{
	if (GetNetMode() == NM_DedicatedServer)
		return;
	
	OnAutoTargetedOutlineCleared();

	//Client Only
	if (OnAutoTargetClearedDele.IsBound())
	{
		OnAutoTargetClearedDele.Broadcast();
	}
}

void AODCombatCharacter::OnSelectTargeted()
{
	if (GetNetMode() == NM_DedicatedServer)
		return;
	
	//Client Only
	if (OnSelectTargetedDele.IsBound())
	{
		OnSelectTargetedDele.Broadcast();
	}
}

void AODCombatCharacter::OnSelectTargetCleared()
{
	if (GetNetMode() == NM_DedicatedServer)
		return;
	
		//Client Only
		if (OnSelectTargetClearedDele.IsBound())
		{
			OnSelectTargetClearedDele.Broadcast();
		}
}

void AODCombatCharacter::Muliticast_AddDamage_Implementation(float Damage, bool bCritical)
{
	if (DamageQueContainerWidget)
	{
		DamageQueContainerWidget->AddDamage(Damage,bCritical);
	}
}

AActor* AODCombatCharacter::GetTarget() const
{
	return nullptr;
}
