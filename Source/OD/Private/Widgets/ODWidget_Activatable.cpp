// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ODWidget_Activatable.h"
#include "Player/ODPlayerController.h"

AODPlayerController* UODWidget_Activatable::GetOwningPlayerController()
{
	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = TWeakObjectPtr<AODPlayerController>(GetOwningPlayer<AODPlayerController>());
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
