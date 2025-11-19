// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Info/ODObjectInfo.h"
#include "ODMonsterInfo.generated.h"


UCLASS()
class OD_API UODMonsterInfo : public UODObjectInfo
{
	GENERATED_BODY()
public:
	UODMonsterInfo();
	
	//현재 몬스터 종류의 고유 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
	FName MonsterID;
	
	
	
	
};
