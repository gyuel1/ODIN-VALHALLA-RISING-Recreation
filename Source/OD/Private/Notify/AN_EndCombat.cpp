// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AN_EndCombat.h"

#include "Character/ODPlayer.h"


void UAN_EndCombat::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AODPlayer* Player = Cast<AODPlayer>(MeshComp->GetOwner()))
	{
		Player->Server_EndCombat();
	}
}
