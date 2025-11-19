// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	OK, // One Button
	YesNo, // Two Button
	OKCancel, // Two Button
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed, // 확인
	Cancelled, // 취소
	Closed, // 닫기
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPopUpScreenType : uint8
{
	Confirm,
	Message,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};