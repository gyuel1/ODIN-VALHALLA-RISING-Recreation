// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_DamageText.h"

#include "Components/TextBlock.h"


void UODWidget_DamageText::SetDamage(const FText Damage, UMaterialInstance* FontMaterial) 
{
	
	DamageText->SetText(Damage);
	DamageText->SetFontMaterial(FontMaterial);
	PlayAnimation(CreatedWidgetAnim);
	PlayAnimation(TextNormalEffectAnim);
}

void UODWidget_DamageText::SetDamagePlayer(const FText Damage, UMaterialInstance* FontMaterial) 
{
	FText PlayerDamage = FText::Format(NSLOCTEXT("PlayerDamage", "DamageFormat", "-{0}"), Damage);
	DamageText->SetText(PlayerDamage);
	DamageText->SetFontMaterial(FontMaterial);
	PlayAnimation(TextNormalEffectAnim);
}
