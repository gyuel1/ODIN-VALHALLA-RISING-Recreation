// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_DamageQueMonster.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Helper/ODStaticFunction.h"
#include "Widgets/NoneStackWidget/ODWidget_DamageText.h"


void UODWidget_DamageQueMonster::AddDamage(int32 DamageValue, bool IsCritical)
{
	Super::AddDamage(DamageValue, IsCritical);

	UODWidget_DamageText* NewDamage = CreateWidget<UODWidget_DamageText>(GetWorld(), DamageTextClass);
	
	// Evasiton
	if (DamageValue == 0)
	{
		NewDamage->SetDamage(FText::FromString(TEXT("빗맞음")), RedMaterial);
	}

	else if (IsCritical)
	{
		NewDamage->SetDamage(FText::AsNumber(DamageValue), OrangeMaterial);
		PlayAnimation(CriticalStringAnimation);
	}

	else if (!IsCritical)
	{
		NewDamage->SetDamage(FText::AsNumber(DamageValue), CreamMaterial);
	}
	

	ODUIStaticFunc::InsertAtTop(DamageBox, NewDamage, MaxCount);
	
}
