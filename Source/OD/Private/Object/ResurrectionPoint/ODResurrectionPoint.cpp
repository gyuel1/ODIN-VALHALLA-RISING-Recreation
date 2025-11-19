// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ResurrectionPoint/ODResurrectionPoint.h"

#include "Components/ArrowComponent.h"




AODResurrectionPoint::AODResurrectionPoint()
	:Super()
{

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	ResurrectionPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ResurrectionPoint"));
	ResurrectionPoint->SetupAttachment(GetRootComponent());
	
}
