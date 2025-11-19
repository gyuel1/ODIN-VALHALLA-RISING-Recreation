// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ODObjectBase.h"



AODObjectBase::AODObjectBase()
{
	PrimaryActorTick.bCanEverTick = false;
	RepGraphCount = 0;

	bReplicates = true;

	//거리별 최적화
	bAlwaysRelevant = false;
	bOnlyRelevantToOwner = false;
	bNetUseOwnerRelevancy = false;
	NetCullDistanceSquared = FMath::Square(4000.f);
}

int32 AODObjectBase::GetRepGraphCount()
{
	// ULevel* MakeLevel = GetLevel();
	// if (!MakeLevel)
	// 	return -1;
	//
	// if (IODRepGraphInterface* RGI = Cast<IODRepGraphInterface>(MakeLevel))
	// 	return RGI->GetRepGraphCount();


	return 0;
		
}

void AODObjectBase::SetRepGraphCount(const int32 InRepGraph)
{
	RepGraphCount = InRepGraph;
}

