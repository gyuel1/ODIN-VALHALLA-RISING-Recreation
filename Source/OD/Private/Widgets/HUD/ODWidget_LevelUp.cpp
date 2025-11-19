// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/ODWidget_LevelUp.h"

#include "Components/TextBlock.h"


void UODWidget_LevelUp::LevelUp(const int32 NewLevel)
{
	TextBlock_Level->SetText(FText::AsNumber(NewLevel));
	PlayAnimation(Anim_LevelUp);
}
