// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ODAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/ODAbilitySystemComponent.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Components/ODDamageReceivedComponent.h"
#include "Debug/ODLogChannels.h"
#include "Helper/ODDebugHelper.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Damage.h"
#include "Player/ODPlayerState.h"
#include "Tags/ODAbilityTags.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageQueContainer.h"

using namespace StateTags;
using namespace GameplayEffectTags;
using namespace TriggerTags;


UODAttributeSet::UODAttributeSet()
{
    Health.SetCurrentValue(0.0f);
    Mana.SetCurrentValue(0.0f);
    EvasionProc.SetCurrentValue(0.0f);
    CriticalProc.SetCurrentValue(0.0f);
    
}

bool UODAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    if (!Super::PreGameplayEffectExecute(Data))
        return false;

    if (Data.EffectSpec.GetDynamicAssetTags().IsValid() && Data.EffectSpec.GetDynamicAssetTags().HasTag(OD_Effect_Combat_OnDamage))
    {
        if (Data.EvaluatedData.Attribute == GetHealthAttribute())
        {
            float Damage = Data.EvaluatedData.Magnitude * -1;
            const float AccumulatedDamage = FMath::Clamp(Damage, 0.0f,GetHealth());

            //받은 데미지 저장
            OnDamageReceived(Data, AccumulatedDamage);
        }
    }
    
    return true;
}

void UODAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    
    if (!GetWorld() || !IsValid(GetOwningActor()))
        return;

    const float NewMaxMana = FMath::Max(GetMaxMana(), 1.f);
    const float ManaClamped = FMath::Clamp(GetMana(), 0.f, NewMaxMana);
    SetMana(ManaClamped);

    const float NewMaxHealth = FMath::Max(GetMaxHealth(), 1.f);
    const float HealthClamped = FMath::Clamp(GetHealth(), 0.f, NewMaxHealth);
    SetHealth(HealthClamped);

    //OnDamage
    if (Data.EffectSpec.GetDynamicAssetTags().IsValid() && Data.EffectSpec.GetDynamicAssetTags().HasTag(OD_Effect_Combat_OnDamage))
    {
        //회피값이 들어오면
       if (Data.EvaluatedData.Attribute == GetEvasionProcAttribute())
       {
           const float ProcValue = Data.EvaluatedData.Magnitude;
           if (ProcValue >= 1.f)
           {
               if (GetOwningActor()->IsA(AODMonsterBase::StaticClass()))
               {
                   AODMonsterBase* Monster = Cast<AODMonsterBase>(GetOwningActor());
                   Monster->Muliticast_AddDamage(0);
               }
               else if (GetOwningActor()->IsA(AODPlayerState::StaticClass()))
               {
                   AODPlayerState* PS = Cast<AODPlayerState>(GetOwningActor());
                   AODPlayer* Player = Cast<AODPlayer>(PS->GetAbilitySystemComponent()->GetAvatarActor());
                   Player->Muliticast_AddDamage(0);
               }
               SetEvasionProc(0.f);
           }
               
       }
        //체력 변경이 들어오면
       if (Data.EvaluatedData.Attribute == GetHealthAttribute())
       {
           const float Damage = Data.EvaluatedData.Magnitude * -1;
           bool bIsCritical = false;
           
           if (GetCriticalProc() >= 1.f)
           {
               SetCriticalProc(0.f);
               bIsCritical = true;
           }
           // UI출력
           if (GetOwningActor()->IsA(AODMonsterBase::StaticClass()))
           {
               AODMonsterBase* Monster = Cast<AODMonsterBase>(GetOwningActor());
               Monster->Muliticast_AddDamage(Damage,bIsCritical);
           }

           else if (GetOwningActor()->IsA(AODPlayerState::StaticClass()))
           {
               AODPlayerState* PS = Cast<AODPlayerState>(GetOwningActor());
               AODPlayer* Player = Cast<AODPlayer>(PS->GetAbilitySystemComponent()->GetAvatarActor());
               Player->Muliticast_AddDamage(Damage, bIsCritical);
           }
       }
    }
    
    //Health에 대한 Effect가 들어왔는가?
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {

        //Monster
        if (GetOwningActor()->IsA(AODMonsterBase::StaticClass()))
        {
            MonsterReportDamageEvent(Data);
        }

        //Player
        if (GetOwningActor()->IsA(AODPlayer::StaticClass()))
        {
        }
        
        //Dead
        if (HealthClamped <= 0.f)
        {
            if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
            {
                if (!ASC->HasMatchingGameplayTag(OD_State_Dead))
                {
                    ASC->AddLooseGameplayTag(OD_State_Dead);
                }
            }
        }
    }
}

void UODAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UODAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
    Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
    
    if (GetOwningActor() && GetOwningActor()->IsA(AODPlayerState::StaticClass()))
    {
        if (Attribute == GetHealthRegenerationAttribute() || Attribute == GetManaRegenerationAttribute())
        {
            if (GetOwningActor() && GetOwningActor()->HasAuthority())
            {
                if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
                {
                    ASC->AddLooseGameplayTag(TriggerTags::OD_Init_Regeneration);
                }
            }
        }
    }
}

void UODAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // === Level ===

    DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, Level, COND_None, REPNOTIFY_Always);

	// === Health ===
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);

	// === Mana ===
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	// === Movement ===
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);

	// === Offense ===
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, Accuracy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, DamageIncrease, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, CrowdControlAccuracy, COND_None, REPNOTIFY_Always);

	// === Defense ===
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, Evasion, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, CrowdControlResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, CrowdControlRecovery, COND_None, REPNOTIFY_Always);

    //=== proc ====
    DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, EvasionProc, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UODAttributeSet, CriticalProc, COND_None, REPNOTIFY_Always);

    
}


void UODAttributeSet::OnDamageReceived(const FGameplayEffectModCallbackData& Data, const float& InDamage)
{
    const FGameplayEffectSpec& Spec = Data.EffectSpec;
    
    UAbilitySystemComponent* InstigatorASC = Spec.GetEffectContext().GetInstigatorAbilitySystemComponent();
    if (!InstigatorASC)
        return;

    AActor* InstigatorActor = InstigatorASC->GetAvatarActor();
    if (AODCombatCharacter* Instigator = Cast<AODCombatCharacter>(InstigatorActor))
    {
       AActor* AvatarActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
        if (AODCombatCharacter* Owner = Cast<AODCombatCharacter>(AvatarActor))
        {
             UODDamageReceivedComponent* ReceivedComponent = Owner->GetODDamageReceivedComponent();
             if (!ReceivedComponent)
                 return;
            
            ReceivedComponent->AddDamageReceivedArray_Internal(Instigator,InDamage);
        }
    }
}

void UODAttributeSet::MonsterReportDamageEvent(const FGameplayEffectModCallbackData& Data)
{
    if (!IsValid(Data.EffectSpec.GetContext().GetSourceObject()))
        return;

    if (AActor* Instigator = Cast<AActor>(Data.EffectSpec.GetContext().GetSourceObject()))
    {
        UAISense_Damage::ReportDamageEvent(
            GetWorld(),
             GetOwningActor(),
            Instigator,
            0.f,
            FVector(),
            FVector()
        );
    }
}

void UODAttributeSet::OnRep_Level(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, Level, OldValue);
}

// --------- Health ----------
void UODAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, Health, OldValue);
    
}

void UODAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, MaxHealth, OldValue);
}

void UODAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, HealthRegeneration, OldValue);
}

// --------- Mana ----------
void UODAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, Mana, OldValue);
}

void UODAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, MaxMana, OldValue);
}

void UODAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, ManaRegeneration, OldValue);
}

// --------- Movement ----------
void UODAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, MovementSpeed, OldValue);
}

// --------- Offense ----------
void UODAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, AttackPower, OldValue);
}

void UODAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, AttackSpeed, OldValue);
}

void UODAttributeSet::OnRep_Accuracy(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, Accuracy, OldValue);
}

void UODAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, CriticalChance, OldValue);
}

void UODAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, CriticalDamage, OldValue);
}

void UODAttributeSet::OnRep_DamageIncrease(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, DamageIncrease, OldValue);
}

void UODAttributeSet::OnRep_CrowdControlAccuracy(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, CrowdControlAccuracy, OldValue);
}

// --------- Defense ----------
void UODAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, Defense, OldValue);
}

void UODAttributeSet::OnRep_Evasion(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, Evasion, OldValue);
}

void UODAttributeSet::OnRep_DamageReduction(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, DamageReduction, OldValue);
}

void UODAttributeSet::OnRep_CrowdControlResistance(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, CrowdControlResistance, OldValue);
}

void UODAttributeSet::OnRep_CrowdControlRecovery(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, CrowdControlRecovery, OldValue);
}

void UODAttributeSet::OnRep_EvasionProc(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, EvasionProc, OldValue);
}

void UODAttributeSet::OnRep_CriticalProc(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UODAttributeSet, CriticalProc, OldValue);
}



