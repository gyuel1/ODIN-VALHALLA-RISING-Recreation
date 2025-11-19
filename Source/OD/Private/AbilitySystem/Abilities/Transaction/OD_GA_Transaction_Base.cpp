// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Transaction/OD_GA_Transaction_Base.h"


UOD_GA_Transaction_Base::UOD_GA_Transaction_Base()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
	bRetriggerInstancedAbility = false;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
}


