// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ODGameplayAbility.h"
#include "Tags/ODAbilityTags.h"
#include "OD_GA_Transaction_Base.generated.h"

using namespace GameplayAbilityTags;
using namespace TriggerTags;
using namespace GameplayEffectTags;
using namespace AttributeDataTags;
/**
 * 
 */
UCLASS()
class OD_API UOD_GA_Transaction_Base : public UODGameplayAbility
{
	GENERATED_BODY()

public:
	UOD_GA_Transaction_Base();

};
