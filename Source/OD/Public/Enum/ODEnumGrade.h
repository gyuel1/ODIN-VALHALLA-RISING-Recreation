// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGradeType : uint8
{
	Common,    // 일반
	Advanced,  // 고급
	Rare,      // 희귀
	Hero,      // 영웅
	Legend,    // 전설
	Myth,      // 신화
	Ancient    // 고대
};