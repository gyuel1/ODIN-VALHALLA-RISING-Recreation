// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ODSkillComponent.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "Character/ODPlayer.h"
#include "DataAssets/Ability/ODActiveSkillDA.h"
#include "DataAssets/Ability/ODPassiveSkillDA.h"
#include "Helper/ODDebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/ODPlayerController.h"
#include "Systems/ODAssetManager.h"
#include "Tags/ODAbilityTags.h"
#include "Widgets/HUD/ODWidget_HUD.h"
#include "Widgets/HUD/UserInput/ODWidget_Hud_UserInput.h"
#include "Widgets/Skills/Slots/ODWidget_SkillHudActive.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


void FSkillSlot::AssignSlot(UODActiveSkillDA* InActiveDA , UAbilitySystemComponent* InASC)
{
	if (!InActiveDA)
		return;
	
	UPaperSprite* Icon = nullptr;
	if (TSoftObjectPtr<UPaperSprite> SoftIcon = InActiveDA->GetSkillInfo().SkillIcon)
	{
		if (!SoftIcon.IsNull())
		{
			Icon = SoftIcon.LoadSynchronous();
		}
	}
	SkillIcon = Icon;
	
	const FGameplayTag& Tag = InActiveDA->GetTag();
	if (Tag.IsValid())
		SkillTag = Tag;

	ASC = InASC;
}

void FSkillSlot::ClearSlot()
{
	SkillTag = FGameplayTag();
	SkillIcon = nullptr;
}


void FSkillSlot::OnSkillCooldown_Server(const FActiveGameplayEffectHandle InCooldownHandle)
{
	if (InCooldownHandle.IsValid())
	{
		CooldownHandle = InCooldownHandle;
	}
}

void FSkillSlot::OnSkillCooldown_Client(const FGameplayTagContainer InTagContainer)
{
	UE_LOG(LogTemp, Error, TEXT("OnSkillCooldown_Client"))
	
	if (!ASC || !InTagContainer.IsValid())
		return;

	//UI한테 스킬 쿨타임 알려주기
	ENetMode NetMode = ASC->GetNetMode();
	ASC->GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		[this,NetMode,InTagContainer]()
		{
			//핸들 찾았어? ->핸들찾기
			if (!CooldownHandle.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("스킬컴포넌트 핸들 찾음"))
				TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandle = ASC->GetActiveEffectsWithAllTags(InTagContainer);
				if (ActiveGameplayEffectHandle.IsEmpty())
					return;
				

				CooldownHandle = ActiveGameplayEffectHandle[0];
			}
			else
			{
				FSkillCooldownDuration Duration = GetCooldownDuration();
				if (Duration.IsValid())
				{
					UE_LOG(LogTemp, Error, TEXT("WidgetCooldown Duration Valid"));
					// OD_LOG_SERVER_ONEPARAM(NetMode, LogODCooldown, Warning, TEXT("[ID : %s ]Total : %f , Current : %f"),
					//                        *CooldownHandle.ToString(),Duration.TotalCooldown, Duration.CurrentCooldown);

					//WidgetCooldownDele 델리게이트
					if (WidgetCooldownDele.IsBound())
					{
						UE_LOG(LogTemp, Error, TEXT("WidgetCooldownDele BroadCast"))
						WidgetCooldownDele.Broadcast(Duration);
					}
	
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("WidgetCooldown Duration UnValid"));
					ClearCooldown();
					ASC->GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
				}
			}
		},
		0.010f,
		true);
	
}




bool FSkillSlot::IsCooldown()
{
	return CooldownHandle.IsValid() && GetCooldownDuration().CurrentCooldown > 0.f;
}

void FSkillSlot::ClearCooldown()
{
	CooldownHandle = FActiveGameplayEffectHandle(INDEX_NONE);
	CooldownDuration = FSkillCooldownDuration();

	if (WidgetCooldownDele.IsBound())
	{
		WidgetCooldownDele.Broadcast(CooldownDuration);
	}
	
}

FSkillCooldownDuration FSkillSlot::GetCooldownDuration()
{
	
	if (UAbilitySystemComponent* MakeASC = CooldownHandle.GetOwningAbilitySystemComponent())
	{
		const FActiveGameplayEffect* CooldownEffect =  MakeASC->GetActiveGameplayEffect(CooldownHandle);
		if (!CooldownEffect)
		{
			UE_LOG(LogTemp, Error, TEXT("CooldownEffect null"))
			return FSkillCooldownDuration();
		}
		
		const float& Total = CooldownEffect->GetDuration();
		const float& Current = FMath::Max(0.f, CooldownEffect->GetTimeRemaining(MakeASC->GetWorld()->GetTimeSeconds()));
		
		return FSkillCooldownDuration(Total,Current);
	}
	
	UE_LOG(LogTemp, Error, TEXT("CooldownHandle.GetOwningAbilitySystemComponent() null"))
	return FSkillCooldownDuration();
}

// Sets default values for this component's properties
UODSkillComponent::UODSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	

	SetIsReplicatedByDefault(true);

	//스킬 슬롯의 칸 개수는 6
	SkillSlotArray.Reserve(6);
	SkillSlotArray.Init(FSkillSlot(), 6);
}

void UODSkillComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}


void UODSkillComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UODSkillComponent,PassiveSkillArray);
	DOREPLIFETIME(UODSkillComponent,ActiveSkillArray);
	DOREPLIFETIME(UODSkillComponent,SkillSlotArray);
	
}

TArray<TObjectPtr<UODPassiveSkillDA>>& UODSkillComponent::GetPassiveSkills() 
{
	return PassiveSkillArray;
}

TArray<TObjectPtr<UODActiveSkillDA>>& UODSkillComponent::GetActiveSkills() 
{
	return ActiveSkillArray;
}

TArray<FSkillSlot>& UODSkillComponent::GetSkillSlots()
{
	return SkillSlotArray;
}

FSkillSlot* UODSkillComponent::FindSkillSlotByTag(const FGameplayTag& InTag)
{
	if (!InTag.IsValid())
		return nullptr;
	
	for (FSkillSlot& SkillSlot : SkillSlotArray)
	{
		if (SkillSlot.IsValid() && SkillSlot.SkillTag == InTag)
			return &SkillSlot;
	}

	return nullptr;
}

UODSkillDA* UODSkillComponent::FindSkillDAByTag(const FGameplayTag& InTag)
{
	for (UODPassiveSkillDA* PassiveSkillDA : PassiveSkillArray)
	{
		if (!IsValid(PassiveSkillDA))
			continue;

		if (PassiveSkillDA->GetTag().IsValid() && PassiveSkillDA->GetTag() == InTag)
		{
			return PassiveSkillDA;
		}
	}


	for (UODActiveSkillDA* ActiveSkillDA : ActiveSkillArray)
	{
		if (!IsValid(ActiveSkillDA))
			continue;

		if (ActiveSkillDA->GetTag().IsValid() && ActiveSkillDA->GetTag() == InTag)
		{
			return ActiveSkillDA;
		}
	}


	return nullptr;
}

bool UODSkillComponent::IsUnlockSkill(const FGameplayTag& InTag)
{
	//유효하지 않은 태그
	if (!InTag.IsValid())
		return false;

	// 스킬이 배워져 있는지?
	if (UODSkillDA* SkillDA = FindSkillDAByTag(InTag))
		return SkillDA->GetSkillInfo().bSkillUnlock;
	
	return false;
}

void UODSkillComponent::UnlockSkill_Implementation(const FGameplayTag& InTag)
{
	//유효하지 않은 태그
	if (!InTag.IsValid())
		return;
	
	if (!IsUnlockSkill(InTag))
	{
		if (UODSkillDA* SkillDA = FindSkillDAByTag(InTag))
		{
			SkillDA->UnlockSkill();
		}
	}
}


void UODSkillComponent::AddSkillToSlot(const uint32& InIndex, const FGameplayTag& InTag)
{
	AddSkillToSlot_Internal(InIndex,InTag);
	if (GetOwner() && !GetOwner()->HasAuthority())
		AddSkillToSlot_Server(InIndex, InTag);
}

void UODSkillComponent::AddSkillToSlot_Internal(const uint32& InIndex, const FGameplayTag& InTag)
{
	if (!SkillSlotArray.IsValidIndex(InIndex) || !InTag.IsValid())
		return;

	if (FSkillSlot* Slot = FindSkillSlotByTag(InTag))
	{
		Slot->ClearSlot();
	}

	
	RemoveSkillFromSlot(InIndex);
	if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent())
		{
			UODActiveSkillDA* DA = FindSkillDAByTag<UODActiveSkillDA>(InTag);
			if (!DA || !DA->GetAbility() || !DA->GetTag().IsValid())
				return;

			if (Owner->HasAuthority())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DA->GetAbility(), 1);
				AbilitySpec.DynamicAbilityTags.AddTag(DA->GetTag());

				ASC->GiveAbility(AbilitySpec);
			}
				SkillSlotArray[InIndex].AssignSlot(DA, ASC);
		}
	}
}
void UODSkillComponent::AddSkillToSlot_Server_Implementation(const uint32 InIndex, const FGameplayTag InTag)
{
	AddSkillToSlot_Internal(InIndex,InTag);
}

void UODSkillComponent::RemoveSkillFromSlot(const uint32& InIndex)
{
	RemoveSkillFromSlot_Internal(InIndex);
	if (GetOwner() && !GetOwner()->HasAuthority())
		RemoveSkillFromSlot_Server(InIndex);
}


void UODSkillComponent::RemoveSkillFromSlot_Internal(const uint32& InIndex)
{
	if (!SkillSlotArray.IsValidIndex(InIndex) || !SkillSlotArray[InIndex].IsValid() || SkillSlotArray[InIndex].IsCooldown())
	{
		return;
	}

	if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent())
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(SkillSlotArray[InIndex].SkillTag);

			TArray<FGameplayAbilitySpec*> Specs;
			ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer,Specs);
			
			for (FGameplayAbilitySpec* Spec : Specs)
			{
				ASC->CancelAbilityHandle(Spec->Handle);
				
				if (GetOwner() && !GetOwner()->HasAuthority())
					ASC->ClearAbility(Spec->Handle);
			}
		}
	}
	SkillSlotArray[InIndex].ClearSlot();
}


void UODSkillComponent::RemoveSkillFromSlot_Server_Implementation(const uint32 InIndex)
{
	RemoveSkillFromSlot_Internal(InIndex);
}

bool UODSkillComponent::TryActiveSkill(const uint32& SlotIndex)
{
	if (!GetOwner() || !SkillSlotArray[SlotIndex].IsValid())
		return false;

	if (AODPlayer* Owner = Cast<AODPlayer>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent())
		{
			FGameplayTag SkillTag = SkillSlotArray[SlotIndex].SkillTag;
			FGameplayTagContainer SkillTagContainer;
			SkillTagContainer.AddTag(SkillTag);
			
			UE_LOG(LogODCooldown, Warning, TEXT("%s"), *SkillTag.ToString());

			if (SkillTagContainer.IsValid())
			{
				
				if ( ASC->TryActivateAbilitiesByTag(SkillTagContainer))
				{
					GetOwner()->ForceNetUpdate();
					OnSkillSlotAnimation(SlotIndex);
					
					return true;
				}
			}
		}
	}
		return false;
}

void UODSkillComponent::OnSkillSlotAnimation_Implementation(const int32 Inindex)
{
	
	if (GetSkillSlots()[Inindex].WidgetActivateDele.IsBound())
	{
		GetSkillSlots()[Inindex].WidgetActivateDele.Broadcast();
	}
	
}

// Called when the game starts
void UODSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

