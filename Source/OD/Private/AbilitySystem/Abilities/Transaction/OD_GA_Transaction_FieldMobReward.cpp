// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Transaction/OD_GA_Transaction_FieldMobReward.h"

#include "AbilitySystem/ODAttributeSet.h"
#include "AbilitySystem/Effect/Transaction/OD_GE_Transaction_FieldMobReward.h"
#include "Character/ODCombatCharacter.h"
#include "Character/ODMonsterBase.h"
#include "Character/ODPlayer.h"
#include "Components/ODCombatComponent.h"
#include "Components/ODDamageReceivedComponent.h"
#include "Components/ODPartyComponent.h"
#include "Components/ODRewardComponent.h"
#include "Party/ODPartyInfo.h"
#include "Structs/RewardRecipientsStruct.h"


UOD_GA_Transaction_FieldMobReward::UOD_GA_Transaction_FieldMobReward()
{
	AbilityTags.AddTag(OD_Ability_Transaction_FieldMobReward);

	FAbilityTriggerData Trig;
	Trig.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
	Trig.TriggerTag = FGameplayTag(OD_Transaction_FieldMobReward);
	AbilityTriggers.Add(Trig);
}

bool UOD_GA_Transaction_FieldMobReward::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	return true;
	
}

void UOD_GA_Transaction_FieldMobReward::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AODMonsterBase* CombatCharacter = Cast<AODMonsterBase>(ActorInfo->AvatarActor);
	UODDamageReceivedComponent* Received = nullptr;
	UODRewardComponent* RewardComponent= nullptr;
	
	//Ability Owner
	if (IsValid(CombatCharacter))
	{
		RewardComponent = CombatCharacter->GetODRewardComponent();
		Received = CombatCharacter->GetODDamageReceivedComponent();
	}
	
	//Owner ASC
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	
	
	//null check
	if (!CombatCharacter || !Received || !ASC || !RewardComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	

	//보상 목록 만들기
	const FDamageReceivedCharacterList& List = Received->GetDamageReceivedArray();
	if (List.Items.Num() <= 0)
		return;

	const FGameplayAttribute MaxHealthAttr = UODAttributeSet::GetMaxHealthAttribute();
	const float& MaxHealth = ASC->GetNumericAttribute(MaxHealthAttr);

	//개인
	TArray<FRewardRecipients> RewardRecipientArray;
	//파티
	TArray<FRewardRecipientParty> RewardRecipientPartyArray;
	
	for (const FDamageReceivedCharacterInfo& Info : List.Items)
	{
		if (!IsValid(Info.CombatCharacter))
			continue;

		const float& Weight = (MaxHealth > 0.f) ? (Info.AccumulatedDamage / MaxHealth) : 0.f;

		
		if (AODPlayer* Player = Cast<AODPlayer>(Info.CombatCharacter))
		{
			UODPartyComponent* PartyComponent = Player->GetODPartyComponent();
			if (PartyComponent && PartyComponent->HasParty())
			{
				bool bInsertOldParty = false;
				for (FRewardRecipientParty& RewardRecipientParty  : RewardRecipientPartyArray)
				{
					if (RewardRecipientParty.PartyCode == PartyComponent->GetParty()->GetPartyCode())
					{
						RewardRecipientParty.Weight += Weight;
						bInsertOldParty = true;
						break;
					}
				}
				if (!bInsertOldParty)
				{
					FRewardRecipientParty NewRewardRecipientParty = FRewardRecipientParty(PartyComponent->GetParty()->GetPartyCode());
					NewRewardRecipientParty.RewardRecipientActors = PartyComponent->GetParty();
					NewRewardRecipientParty.Weight += Weight;
					
					RewardRecipientPartyArray.Add(NewRewardRecipientParty);
				}
			}
			else
			{
				RewardRecipientArray.Add(FRewardRecipients(Info.CombatCharacter, Weight));
			}
		}
	}
	
	const FRewardGoldAndEXP& GoldAndEXP = RewardComponent->RewardGoldAndEXP;
	
	//파티대상들에게 Gold Exp
	for (const FRewardRecipientParty& RewardRecipientParty : RewardRecipientPartyArray)
	{
		int32 PartyNum = RewardRecipientParty.RewardRecipientActors->GetPartyMembers().Num();
		
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(CombatCharacter);
		
		FGameplayEffectSpecHandle Context = ASC->MakeOutgoingSpec(UOD_GE_Transaction_FieldMobReward::StaticClass(), 1,
																  EffectContext);
		
		FGameplayEffectSpec& Spec = *Context.Data.Get();
		Spec.AddDynamicAssetTag(OD_Effect_Transaction_FieldReward);
		
		//몬스터의 Gold EXP
		Spec.SetSetByCallerMagnitude(OD_Attribute_Transaction_Gold, GoldAndEXP.Gold);
		Spec.SetSetByCallerMagnitude(OD_Attribute_Transaction_Exp, GoldAndEXP.Exp);

		//Reward 대상자의 가중치
		Spec.SetSetByCallerMagnitude(OD_Magnitude_Reward_Weight, RewardRecipientParty.Weight / PartyNum);
		
		for (UODPartyMember* Target : RewardRecipientParty.RewardRecipientActors->GetPartyMembers())
		{
			UAbilitySystemComponent* TargetASC = Target->GetMember()->GetAbilitySystemComponent();
			if (!TargetASC)
				continue;

			ASC->ApplyGameplayEffectSpecToTarget(Spec,TargetASC);
		}
	}
	
	
	//개인대상에게 Gold Exp
	for (const FRewardRecipients& Target : RewardRecipientArray)
	{
		UAbilitySystemComponent* TargetASC = Target.RewardRecipientActor->GetAbilitySystemComponent();
		if (!TargetASC)
			continue;


		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(CombatCharacter);
		
		FGameplayEffectSpecHandle Context = ASC->MakeOutgoingSpec(UOD_GE_Transaction_FieldMobReward::StaticClass(), 1,
																  EffectContext);
		
		FGameplayEffectSpec& Spec = *Context.Data.Get();
		Spec.AddDynamicAssetTag(OD_Effect_Transaction_FieldReward);
		
		//몬스터의 Gold EXP
		Spec.SetSetByCallerMagnitude(OD_Attribute_Transaction_Gold, GoldAndEXP.Gold);
		Spec.SetSetByCallerMagnitude(OD_Attribute_Transaction_Exp, GoldAndEXP.Exp);

		//Reward 대상자의 가중치
		Spec.SetSetByCallerMagnitude(OD_Magnitude_Reward_Weight, Target.Weight);

		
		ASC->ApplyGameplayEffectSpecToTarget(Spec,TargetASC);
	}
	

	//아이템?
	
	
	EndAbility(Handle,ActorInfo,ActivationInfo,false,false);
}

void UOD_GA_Transaction_FieldMobReward::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
