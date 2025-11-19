// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_DamageQuePlayer.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Helper/ODStaticFunction.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageText.h"


void UODWidget_DamageQuePlayer::AddDamage(int32 DamageValue, bool IsCritical)
{
	Super::AddDamage(DamageValue, IsCritical);
	
	UODWidget_DamageText* NewDamage = CreateWidget<UODWidget_DamageText>(GetWorld(), DamageTextClass);

	// Evasion
	if (DamageValue == 0)
	{
		NewDamage->SetDamage(FText::FromString(TEXT("빗맞음")), CreamMaterial); 
	}
	else if (IsCritical)
	{
		NewDamage->SetDamagePlayer(FText::AsNumber(DamageValue), RedMaterial);
		PlayAnimation(CriticalStringAnimation);
	}
	else
	{
		NewDamage->SetDamagePlayer(FText::AsNumber(DamageValue), RedMaterial);
	}


	
	ODUIStaticFunc::InsertAtTop(DamageBox, NewDamage, MaxCount, true);
	
}
