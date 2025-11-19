// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ODCharacter.h"


// Sets default values
AODCharacter::AODCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RepGraphCount = 0;
	
	//거리별 최적화
    bAlwaysRelevant = false;
    bOnlyRelevantToOwner = false;
    bNetUseOwnerRelevancy = false;
    NetCullDistanceSquared = FMath::Square(4000.f);
}

int32 AODCharacter::GetRepGraphCount()
{
	return RepGraphCount;
}

void AODCharacter::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}
