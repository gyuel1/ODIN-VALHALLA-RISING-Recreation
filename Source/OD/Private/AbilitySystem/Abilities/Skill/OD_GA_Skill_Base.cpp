// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Skill/OD_GA_Skill_Base.h"


UOD_GA_Skill_Base::UOD_GA_Skill_Base()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = true;
	bRetriggerInstancedAbility = false;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

