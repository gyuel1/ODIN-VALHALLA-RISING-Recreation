// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Update/OD_GA_Update_Base.h"


UOD_GA_Update_Base::UOD_GA_Update_Base()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = true;
	bRetriggerInstancedAbility = false;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}
