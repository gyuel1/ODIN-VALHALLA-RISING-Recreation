// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Init/OD_GA_Init_Base.h"


UOD_GA_Init_Base::UOD_GA_Init_Base()
{
	
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = true;
	bRetriggerInstancedAbility = false;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
}
