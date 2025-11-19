// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ODPartySubsystem.generated.h"




UCLASS()
class OD_API UODPartySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UODPartySubsystem();

	
};
