// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBuffAttribute : uint8
{
	MPCoast, 
	HPCoast,
	CoolDown,
	AttackRange,
	SkillRange,
	MaxTarget,
	SummonTime,

	// 도발
	Provocation, 
	Damage,
	ActivationTime, // 발동시간
	Fall, // 넘어짐
	HPDown,
	//무적
	Invincibility,
	//기절
	Fainting,
	//침묵
	Silence,
	//저주
	Curse,
	//중독
	Poisoning,
	//약화
	Weakening,
	//흡혈
	BloodSucking
	

	
};
