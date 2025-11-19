// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ODObjectInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None,
	Player,
	Monster,
	Npc,
	FieldObject
};

UCLASS()
class OD_API UODObjectInfo : public UObject
{
	GENERATED_BODY()
public:
	UODObjectInfo();
	
	//표시 네임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
	FName DisplayName;

	//1차분류, 캐릭터 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterInfo")
	ECharacterType CharacterType;
	
	
	
	
};
