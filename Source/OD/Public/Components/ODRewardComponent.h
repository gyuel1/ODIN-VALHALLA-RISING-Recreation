// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ODRewardComponent.generated.h"

USTRUCT(BlueprintType)
struct FRewardGoldAndEXP
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	float Gold =0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	float Exp =0.f;
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OD_API UODRewardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UODRewardComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FRewardGoldAndEXP RewardGoldAndEXP;

	//아이템 관련 리워드
	//배열로 아이템 태그를 들고있어야하나?
	
	
};