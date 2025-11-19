// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NoneStackWidget/ODWidget_Target.h"

#include "Components/HorizontalBox.h"


void UODWidget_Target::VisibilityTargetMain(ESlateVisibility Option)
{
	HorizontalBox_TargetMain->SetVisibility(Option);
}

void UODWidget_Target::VisibilityTargetSub(ESlateVisibility Option)
{
	HorizontalBox_TargetSub->SetVisibility(Option);

	if (Option == ESlateVisibility::Visible)
	{
		PlayAnimation(TargetSubActiveAnim,0,0);
	}

	else if (Option == ESlateVisibility::Hidden)
	{
		StopAnimation(TargetSubActiveAnim);
	}
}

void UODWidget_Target::TargetMainActiveAnimation()
{
	PlayAnimation(TargetMainActiveAnim,0,1,EUMGSequencePlayMode::Forward,2);
}
