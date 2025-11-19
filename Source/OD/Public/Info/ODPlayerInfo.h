// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/ODEnumCharacterJob.h"
#include "Info/ODObjectInfo.h"
#include "ODPlayerInfo.generated.h"


UCLASS()
class OD_API UODPlayerInfo : public UODObjectInfo
{
	GENERATED_BODY()

public:
	UODPlayerInfo();
	
	//플레이어 직업
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPlayerInfo")
	EPlayerJob PlayerJob;
	
	//길드 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPlayerInfo")
	FName Guild  = "";
	
};
