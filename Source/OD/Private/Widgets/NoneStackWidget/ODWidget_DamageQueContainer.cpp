// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_DamageQueContainer.h"

#include "Components/VerticalBox.h"
#include "Helper/ODDebugHelper.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageText.h"


void UODWidget_DamageQueContainer::AddDamage(int32 DamageValue, bool IsCritical)
{
	if (!DamageTextClass || !DamageBox)
	{
		Debug::Print(TEXT("UODWidget_DamageQueContainer 텍스트클래스나 데미지박스가 없습니다."));
		return;
	}
	
}

void UODWidget_DamageQueContainer::StartOrResetClearTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DamageRemoveTimerHandle);

		World->GetTimerManager().SetTimer(
		DamageRemoveTimerHandle,
		this,
		&ThisClass::PlayFadeOutAndRemove,
		MaxLiftTime,
		false);
	}
	else
	{
		Debug::Print(TEXT("타이머 실행실패"));
	}
}

void UODWidget_DamageQueContainer::PlayFadeOutAndRemove()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DamageRemoveTimerHandle);
	}

	DamageBox->ClearChildren();
}
